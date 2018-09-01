// shpObject.cpp: implementation of the shpObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "shpObject.h"

#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

shpObject::shpObject()
{
	m_objColor = RGB(0, 0, 0);
	m_nStyle = 0;
	m_nWidth = 0;

	m_nDetectPixelSize = 4;

	m_eStatus = stateNone;

	m_nShapeType = 0;

	m_brushColor = RGB(0, 200, 200);

	m_bSolidPolygon = false;
	m_bLabel = false;
}

shpObject::~shpObject()
{
	m_DBF.Close();
}

CPoint shpObject::WorldToDevice(const GPoint& _GPoint)
{
	CPoint	tmpPoint;

	tmpPoint.x = long(m_lfRatio * (_GPoint.x - m_geoCenterPoint.x) * m_lfZoomFactor + (double)m_scrCenterPoint.x);
	tmpPoint.y = long(m_lfRatio * (m_geoCenterPoint.y - _GPoint.y) * m_lfZoomFactor + (double)m_scrCenterPoint.y);

	return tmpPoint;
}

GPoint shpObject::DeviceToWorld(const CPoint& _point)
{
	GPoint	tmpGPoint;

	tmpGPoint.x = m_geoCenterPoint.x + (_point.x - m_scrCenterPoint.x) / (m_lfZoomFactor * m_lfRatio);
	tmpGPoint.y = m_geoCenterPoint.y - (_point.y - m_scrCenterPoint.y) / (m_lfZoomFactor * m_lfRatio);

	return tmpGPoint;
}

CRect shpObject::WorldToDevice(const MBR& _mbr)
{
	CRect	tmpRect;
	CPoint	tmpPT1, tmpPT2;
	GPoint pt1, pt2;

	pt1.x = _mbr.xmin;	pt1.y = _mbr.ymin;
	pt2.x = _mbr.xmax;  pt2.y = _mbr.ymax;

	tmpPT1 = WorldToDevice(pt1);
	tmpPT2 = WorldToDevice(pt2);

	tmpRect.bottom	= tmpPT1.y;
	tmpRect.left	= tmpPT1.x;
	tmpRect.top		= tmpPT2.y;
	tmpRect.right	= tmpPT2.x;

	return tmpRect;
}

MBR	shpObject::DeviceToWorld(const CRect& _rect)
{
	MBR	tmpMBR;
	GPoint	tmpPT1, tmpPT2;

	CPoint	pt1(_rect.left,	 _rect.bottom);
	CPoint	pt2(_rect.right, _rect.top);

	tmpPT1 = DeviceToWorld(pt1);
	tmpPT2 = DeviceToWorld(pt2);

	tmpMBR.xmax = tmpPT2.x;
	tmpMBR.xmin = tmpPT1.x;
	tmpMBR.ymax = tmpPT2.y;
	tmpMBR.ymin = tmpPT1.y;

	return tmpMBR;
}

bool shpObject::PtInRect(const GPoint& _geoInputPoint, const GPoint& _geoRectCenter)
{
	if(_geoInputPoint.x < (_geoRectCenter.x - (double)m_nDetectPixelSize / m_lfZoomFactor / m_lfRatio)) 
		return false;
	else if(_geoInputPoint.x > (_geoRectCenter.x + (double)m_nDetectPixelSize / m_lfZoomFactor / m_lfRatio)) 
		return false;
	else if(_geoInputPoint.y < ( _geoRectCenter.y - (double)m_nDetectPixelSize / m_lfZoomFactor/ m_lfRatio)) 
		return false;
	else if(_geoInputPoint.y > ( _geoRectCenter.y + (double)m_nDetectPixelSize / m_lfZoomFactor / m_lfRatio)) 
		return false;
	else 
		return true;
}

bool shpObject::PtInPolygon(vector<GPoint>& _vPoint, GPoint _GPoint)
{
	int i, j;
	bool c = false;
	int size = _vPoint.size();

    for (i = 0, j = (size - 1); i < size; j = i++) 
	{
		if ((((_vPoint[i].y <= _GPoint.y) && (_GPoint.y < _vPoint[j].y)) || ((_vPoint[j].y <= _GPoint.y) && ( _GPoint.y < _vPoint[i].y))) && (_GPoint.x < (_vPoint[j].x - _vPoint[i].x) * (_GPoint.y - _vPoint[i].y) / (_vPoint[j].y - _vPoint[i].y) + _vPoint[i].x))
			c = !c;
	}
	return c;
}

bool shpObject::IsPointNearLine(const GPoint& _a, const GPoint& _b, const GPoint& _p, double _d)
{
	// check distance from the point to the line by x and by y
	double dx = _a.x - _b.x;
	double dy = _a.y - _b.y;
	double r = sqrt(dx*dx + dy*dy);

	// compute coordinates relative to the origin of the line
	double x1 = _p.x - _b.x;
	double y1 = _p.y - _b.y;

	// compute coordinates relative to the line
	double x2 = (x1*dx + y1*dy)/r;
	double y2 = (-x1*dy + y1*dx)/r;

	return (x2>=0) && (x2<=r) && (y2<=_d) && (y2 >= -_d);
}

void shpObject::ReMBR(vector<GPoint>& _vPoint)
{
	m_MBR.xmax = m_MBR.ymax = -99999999.;
	m_MBR.xmin = m_MBR.ymin = +99999999.;

	for(int i = 0; i < _vPoint.size(); ++i)
	{
		m_MBR.xmax = (m_MBR.xmax < _vPoint[i].x) ? _vPoint[i].x : m_MBR.xmax; 
		m_MBR.xmin = (m_MBR.xmin > _vPoint[i].x) ? _vPoint[i].x : m_MBR.xmin;
		m_MBR.ymax = (m_MBR.ymax < _vPoint[i].y) ? _vPoint[i].y : m_MBR.ymax; 
		m_MBR.ymin = (m_MBR.ymin > _vPoint[i].y) ? _vPoint[i].y : m_MBR.ymin;
	}
}

bool shpObject::Open(CString _filename)
{
	FILE		*fpSHP;						//SHP File Pointer
    FILE		*fpSHX;						//SHX File Pointer

	int tmp;
	
	m_strSHPPath = _filename;	//SHP file path

	tmp = _filename.GetLength()-4;
	m_strSHXPath = _filename.Mid(0, tmp) + ".shx";	//SHX file path
	m_strDBFPath = _filename.Mid(0, tmp) + ".dbf";	// dbf file path

	// Get Field number..
	m_DBF.Open((LPCTSTR)m_strDBFPath, "rb");
	m_nField = m_DBF.GetFieldCount();

	//open SHP/SHX file
	fpSHP = fopen(m_strSHPPath, "rb");
	fpSHX = fopen(m_strSHXPath, "rb");

	if(fpSHP == NULL || fpSHX == NULL)
	{
		AfxMessageBox("SHP File Error!!", IDOK | MB_ICONSTOP);
		return false;
	}

	// Now read a Header of SHP/SHX 
	// Get File size & Number of records
	fseek(fpSHX, 24L, SEEK_SET);
	fread(&tmp, 4, 1, fpSHX);
	SwapWord(4, &tmp);	// Byte Order Changing
	int nFileSize = tmp * 2;	// get file size
	m_nRecords = (nFileSize - 100) / 8;	//get a number of records

	// get shp type
	fseek(fpSHP, 32L, SEEK_SET);
	fread(&m_nShapeType, 4, 1, fpSHP);
	
	// Check a invalid shp type
	if(!(m_nShapeType == 1 || m_nShapeType == 3 || m_nShapeType == 5 || m_nShapeType == 8)) 
	{
		AfxMessageBox("Invalid SHP Type.\nThis program only support Point, Arc, Polygon & MultiPoint SHP Type.", IDOK | MB_ICONSTOP);
		return false;
	}
	// get a MBR
	fread(&m_MBR, sizeof(MBR), 1, fpSHP);

	fclose(fpSHP);
	fclose(fpSHX);

	srand((unsigned)time(NULL));	// 랜덤함수 초기화..
	//m_objColor = RGB(rand()%255, rand()%255, rand()%255); //SHP 색..
	m_objColor = RGB(0, 0, 0); //SHP 색..
	
	return true;
}

void shpObject::Draw(CDC* _pDC, double _ZoomFactor, GPoint _ptMapCenter, const CRect& _rect, double _lfRatio)
{
	// virtual functions..
}

int shpObject::Track(CWnd* pWnd, GPoint point)
{
	return 1;
}

DBFFieldType shpObject::GetFieldInfo(int iField, char* pszFieldName, int* pnWidth, int* pnDecimals)
{
	return m_DBF.GetFieldInfo(iField, pszFieldName, pnWidth, pnDecimals);
}

void shpObject::SwapWord(int length, void* wordP)   
{
	int		i;
    unsigned char	temp;

    for( i=0; i<length/2; i++ )
    {
		temp = ((unsigned char *) wordP)[i];
		((unsigned char *)wordP)[i] = ((unsigned char *) wordP)[length-i-1];
		((unsigned char *)wordP)[length-i-1] = temp;
    }
}

