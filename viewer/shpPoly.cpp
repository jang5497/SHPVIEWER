// shpPoly.cpp: implementation of the shpPoly class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "shpviewer.h"
#include "shpPoly.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

shpPolyLine::shpPolyLine()
{
}

shpPolyLine::~shpPolyLine()
{
	m_vSHPPoly.clear();
}

bool shpPolyLine::Open(CString _filename)
{
	if(!shpObject::Open(_filename))
		return false;

	// Temporary object..
	GPoint	tmpPoint;
	int i, j, tmp;

	// File pointer..
	FILE* fpSHP = fopen(m_strSHPPath, "rb");
	FILE* fpSHX = fopen(m_strSHXPath, "rb");

	// now real data reading..
	CProgressBar	bar("Open : ", 100, m_nRecords);
	for(i = 0; i < m_nRecords; ++i)
	{
		polyObject SHPPoly;

		fseek(fpSHX, 100 + i * 8, SEEK_SET);
		fread(&tmp, 4, 1, fpSHX);
		SwapWord(4, &tmp);
		tmp=tmp*2;

		fseek(fpSHP, tmp+12, SEEK_SET);
		fread((MBR*)&SHPPoly.m_MBR, 32, 1, fpSHP);	//get MBR
		fread((int*)&SHPPoly.m_nNumParts, 4, 1, fpSHP);	//get parts number
		fread((int*)&SHPPoly.m_nNumPoints, 4, 1, fpSHP);  //get points number

		// part reading
		for(j = 0; j < SHPPoly.m_nNumParts; ++j)
		{
			fread((int*)&tmp, 4, 1, fpSHP);
			SHPPoly.m_vParts.push_back(tmp);
		}

		// point reading..
		for(j = 0; j < SHPPoly.m_nNumPoints; ++j)
		{
			fread((double*)&tmpPoint.x, 8, 1, fpSHP);
			fread((double*)&tmpPoint.y, 8, 1, fpSHP);
			SHPPoly.m_vPoints.push_back(tmpPoint);
		}
		SHPPoly.m_nID = i;
		m_vSHPPoly.push_back(SHPPoly);
		bar.StepIt();
	}
	fclose(fpSHP);
	fclose(fpSHX);

	return true;
}

void shpPolyLine::Draw(CDC* _pDC, double _zoomFactor, GPoint _ptMapCenter, const CRect& _rctScreen, double _lfRatio)
{
	m_lfZoomFactor = _zoomFactor;
	m_geoCenterPoint = _ptMapCenter;
	m_scrCenterPoint.x = int(_rctScreen.Width() / 2);
	m_scrCenterPoint.y = int(_rctScreen.Height() / 2);
	m_lfRatio = _lfRatio;

	CPen	pen;
	CRect	mbrRect, rectBuffer;

	// ȭ�� ���̸� �׸��� �ʴ´�..
	mbrRect = WorldToDevice(m_MBR);
	if(!rectBuffer.IntersectRect(&mbrRect, &_rctScreen))
		return;
	
	// ȭ���� ������ ����..
	int savedDC = _pDC->SaveDC();
	int i,j;
	pen.CreatePen(m_nStyle, m_nWidth, m_objColor);
	_pDC->SelectObject(&pen);

	// ��ü ����ŭ �׸���..
	for(i = 0; i < m_nRecords; ++i)
	{
		mbrRect = WorldToDevice(m_vSHPPoly[i].m_MBR);
		if(!rectBuffer.IntersectRect(&mbrRect, &_rctScreen))
			continue;

		// memory allocation for screen point
		CPoint* pScrPoints = new CPoint[m_vSHPPoly[i].m_nNumPoints];
		for(j = 0; j < m_vSHPPoly[i].m_nNumPoints; ++j)
			pScrPoints[j] = WorldToDevice(m_vSHPPoly[i].m_vPoints[j]);

		int* pParts = new int[m_vSHPPoly[i].m_nNumParts];
		for(j = 0; j < m_vSHPPoly[i].m_nNumParts; ++j)
		{
			if(j == m_vSHPPoly[i].m_nNumParts - 1)
				pParts[j] = m_vSHPPoly[i].m_nNumPoints - m_vSHPPoly[i].m_vParts[j];
			else
				pParts[j] = m_vSHPPoly[i].m_vParts[j + 1] - m_vSHPPoly[i].m_vParts[j];
		}
		_pDC->PolyPolyline(pScrPoints, (DWORD*)pParts, m_vSHPPoly[i].m_nNumParts);

		delete []pParts;
		delete []pScrPoints;
	}

	// ���õǾ� ������ �׸���..
	if(m_eStatus == stateSelect)
		DrawSelected(_pDC, _rctScreen);

	if(m_bLabel)
		DrawLabel(_pDC);

	_pDC->RestoreDC(savedDC);
}

void shpPolyLine::DrawSelected(CDC* _pDC, CRect _rctScreen)
{
	CPen	pen;

	// ȭ���� ������ ����..
	int savedDC = _pDC->SaveDC();
	pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	_pDC->SelectObject(&pen);
	int i, j;
	for(i = 0; i < m_vSelectedID.size(); ++i)
	{

		CPoint* pScrPoints;
		int*	pParts;

		// memory allocation for screen point
		pScrPoints = new CPoint[m_vSHPPoly[m_vSelectedID[i]].m_nNumPoints];
		for(j = 0; j < m_vSHPPoly[m_vSelectedID[i]].m_nNumPoints; ++j)
			pScrPoints[j] = WorldToDevice(m_vSHPPoly[m_vSelectedID[i]].m_vPoints[j]);

		pParts = new int[m_vSHPPoly[m_vSelectedID[i]].m_nNumParts];
		for(j = 0; j < m_vSHPPoly[m_vSelectedID[i]].m_nNumParts; ++j)
		{
			if(j == m_vSHPPoly[m_vSelectedID[i]].m_nNumParts - 1)
				pParts[j] = m_vSHPPoly[m_vSelectedID[i]].m_nNumPoints - m_vSHPPoly[m_vSelectedID[i]].m_vParts[j];
			else
				pParts[j] = m_vSHPPoly[m_vSelectedID[i]].m_vParts[j + 1] - m_vSHPPoly[m_vSelectedID[i]].m_vParts[j];
		}
		_pDC->PolyPolyline(pScrPoints, (DWORD*)pParts, m_vSHPPoly[m_vSelectedID[i]].m_nNumParts);

		delete []pParts;
		delete []pScrPoints;
	}
	_pDC->RestoreDC(savedDC);
}

void shpPolyLine::DrawRoute(CDC* _pDC, CRect _rctScreen, std::vector<std::vector<GPoint> >* _result_vec)
{
	CPen	pen;

	// ȭ���� ������ ����..
	int savedDC = _pDC->SaveDC();
	pen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	_pDC->SelectObject(&pen);
	for(int i = 0; i < _result_vec->size(); ++i)
	{
		CPoint* pScrPoints;
		// memory allocation for screen point
		pScrPoints = new CPoint[(*_result_vec)[i].size()];
		for(int j = 0; j < (*_result_vec)[i].size(); ++j)
			pScrPoints[j] = WorldToDevice((*_result_vec)[i][j]);

		_pDC->Polyline(pScrPoints, (int)(*_result_vec)[i].size());

		delete []pScrPoints;
	}
	_pDC->RestoreDC(savedDC);
}

shpObject::hit_status shpPolyLine::HitTest(GPoint _gPoint, CRect _rctScreen, int& _ID, int& _PointID)
{
	POINT_LIST::iterator iter;

	CRect mbrRect, rectBuffer;
	mbrRect = WorldToDevice(m_MBR);
	int i, j;
	if(!rectBuffer.IntersectRect(&mbrRect, &_rctScreen))
		return hitNone;;
	// �� ���� ���� ����.
	for(i = 0; i < m_vSHPPoly.size(); ++i)
	{
		mbrRect = WorldToDevice(m_vSHPPoly[i].m_MBR);
		if(!rectBuffer.IntersectRect(&mbrRect, &_rctScreen))
			continue;

		iter = m_vSHPPoly[i].m_vPoints.begin();
		for (int j = 0; j < m_vSHPPoly[i].m_vPoints.size(); ++j)
		{
			// mouse pointer�� �� ��ó�� �ֳ� ����.
			if (PtInRect(*iter, _gPoint))
			{
				_ID = m_vSHPPoly[i].m_nID;
				_PointID = j;
				return hitPoint;
			}
			++iter;
		}
	}

	// ������ ����.
	for(i =0; i < m_vSHPPoly.size(); ++i)
	{		
		// ȭ�� ���̸� ó������ �ʴ´�.
		mbrRect = WorldToDevice(m_vSHPPoly[i].m_MBR);
		if(!rectBuffer.IntersectRect(&mbrRect, &_rctScreen))
			continue;

		// ��Ʈ�� ����Ʈ�� ������ ���..
		int* pParts = (int*)malloc(sizeof(int) * m_vSHPPoly[i].m_nNumParts);
		for(j = 0; j < m_vSHPPoly[i].m_nNumParts; ++j)
		{
			if(j == m_vSHPPoly[i].m_nNumParts - 1)
				pParts[j] = m_vSHPPoly[i].m_nNumPoints - m_vSHPPoly[i].m_vParts[j];
			else
				pParts[j] = m_vSHPPoly[i].m_vParts[j + 1] - m_vSHPPoly[i].m_vParts[j];
		}

		// ��Ʈ���� ȸ���ϸ� ������ ������..
		for(j = 0; j < m_vSHPPoly[i].m_nNumParts; ++j)
		{
			iter = m_vSHPPoly[i].m_vPoints.begin() + m_vSHPPoly[i].m_vParts[j];
			GPoint first = *iter++;
			GPoint second;

			for(int k = 0; k < (pParts[j] - 1); ++k)
			{
				second = *iter;
				if (IsPointNearLine(first, second, _gPoint,(double)m_nDetectPixelSize/m_lfZoomFactor/m_lfRatio))
				{
					_ID = m_vSHPPoly[i].m_nID;
					_PointID = -1;
					return hitCurve;
				}
				first = second;
				++iter;
			}	// for k
		} // for j
		free(pParts);
	} // for i
    return hitNone;
}

int shpPolyLine::Track(CWnd* pWnd, GPoint point)
{
	m_vSelectedID.clear();

	// ���� ������� ���� ������� hit test �Ѵ�.
	int ID;				// ������Ʈ ���̵�...
	int PointID;		// ����� ����Ʈ�ΰ�..
	
	CRect rect;
	CPoint node;

	CDC* dc=pWnd->GetDC();
	pWnd->GetClientRect(rect);

	hit_status hit_test = HitTest(point, rect, ID, PointID);

	if ((hit_test != hitPoint) && (hit_test != hitCurve))
	// �ƹ����� �ƴϴ�.
	{
		if (m_eStatus == stateSelect)
		{
			m_eStatus = stateNone;

			// �ٽ� �׸���� �Ѵ�.
			pWnd->InvalidateRect(NULL);
		}
		return -1;
	}

	m_vSelectedID.push_back(ID);
	// ���� ������ �Ǿ����� �ʾҴٸ�, ���õ� ���� ǥ���ϱ� ���� �ѹ� �׷��ش�.
	if (m_eStatus == stateNone)
	{
		m_eStatus = stateSelect;
		pWnd->InvalidateRect(NULL);				

		// ���õǾ� ���� �ʾ��� ���� ���� �ű�� ���� ���� ���Ѵ�.
		hit_test = hitCurve;
		return ID;
	}

	// ���¸� ������ ������ ��ȭ��Ų��.
	m_eStatus = stateSelect;
	pWnd->ReleaseDC(dc);
	// ���� �׸���� �Ѵ�.
	pWnd->InvalidateRect(NULL);

	return ID;
}

void shpPolyLine::DrawLabel(CDC* _pDC)
{
	int savedDC = _pDC->SaveDC();

	CFont	m_font;
	CPoint	tmpPoint;	
	
	int		nHeight;
	if(_pDC->IsPrinting())
		nHeight = 12 * _pDC->GetDeviceCaps(HORZRES) / 1024;
	else
		nHeight = 12;

	m_font.CreateFont(nHeight, nHeight * 0.666,
		0, 0, 0,
		FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "����ü");
		
	_pDC->SelectObject(&m_font);
	_pDC->SetBkMode(TRANSPARENT);
	_pDC->SetTextColor(RGB(0, 0, 0));
	_pDC->SetTextAlign(0|0);

	m_nField = m_DBF.GetFieldCount();
	int nSize = m_vSHPPoly.size();
	for(int i=0; i < nSize; ++i)
	{
		CString tmpString = m_DBF.ReadStringAttribute(i, m_nLabelField);
		tmpPoint = WorldToDevice(m_vSHPPoly[i].m_vPoints[0]);
		_pDC->TextOut(tmpPoint.x, tmpPoint.y, tmpString);
	}
	_pDC->RestoreDC(savedDC);
}