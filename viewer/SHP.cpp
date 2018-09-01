// SHP.cpp: implementation of the CSHP class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SHP.h"

#include "MainFrm.h"
#include "FieldDLG.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSHP::CSHP()
{
	m_bOpen = false;
}

CSHP::~CSHP()
{
	Close();
}

bool CSHP::Open(CString _filename)
{
	FILE* fpSHP = fopen(_filename, "rb");

	// get shp type
	fseek(fpSHP, 32L, SEEK_SET);
	fread(&m_nShapeType, 4, 1, fpSHP);
	if (m_nShapeType != SHPT_ARC) return false;
	// data opening..
	switch(m_nShapeType)
	{
	case SHPT_POINT:
		m_pSHPObject = new shpPoint;
		if(!((shpPoint*)m_pSHPObject)->Open(_filename))
			return false;
		break;

	case SHPT_ARC:
		m_pSHPObject = new shpPolyLine;
		if(!((shpPolyLine*)m_pSHPObject)->Open(_filename))
			return false;
		break;

	case SHPT_POLYGON:
		m_pSHPObject = new shpPolygon;
		if(!((shpPolygon*)m_pSHPObject)->Open(_filename))
			return false;
		break;

	case SHPT_MULTIPOINT:
		m_pSHPObject = new shpMultiPoint;
		if(!((shpMultiPoint*)m_pSHPObject)->Open(_filename))
			return false;
		break;
	
	default:
		return false;
	}
	fclose(fpSHP);
	m_bOpen = true;
	init_record();
	return true;
}

bool CSHP::Close()
{
	if(m_bOpen)
	{
		switch(m_nShapeType)
		{
		case SHPT_POINT:
			delete (shpPoint*)m_pSHPObject;
			break;

		case SHPT_ARC:
			delete (shpPolyLine*)m_pSHPObject;
			break;
		
		case SHPT_POLYGON:
			delete (shpPolygon*)m_pSHPObject;
			break;

		case SHPT_MULTIPOINT:
			delete (shpMultiPoint*)m_pSHPObject;
			break;

		default:
			return false;
		}
	}
	m_bOpen = false;

	return true;
}
void CSHP::init_record()
{
  Reader::instance();
  for(int i = 0; i < (int)((shpPolyLine*)m_pSHPObject)->m_vSHPPoly.size(); ++i)
  {
    char str_buff[128] = {0,};
    unsigned long linkid = 0;
    unsigned long f_nodeid = 0, t_nodeid = 0;
    int size = 0;
    linkRecord linkrec;
    nodeRecord f_noderec, t_noderec;

    strcpy(str_buff, m_pSHPObject->ReadStringAttribute(i, 0));
    linkid = strtoul(str_buff, NULL, 10);
    strcpy(str_buff, m_pSHPObject->ReadStringAttribute(i, 1));
    f_nodeid = strtoul(str_buff, NULL, 10);
    strcpy(str_buff, m_pSHPObject->ReadStringAttribute(i, 2));
    t_nodeid = strtoul(str_buff, NULL, 10);
    linkrec._f_node = f_nodeid;
    linkrec._t_node = t_nodeid;
    linkrec._shppoly_idx = i;
    vector<GPoint> * gpoint = &((shpPolyLine*)m_pSHPObject)->m_vSHPPoly[i].m_vPoints;
    linkrec._point_vec.reserve(gpoint->size());
    for (int j = 0; j < gpoint->size(); ++j) {
      linkrec._point_vec.push_back(Point((*gpoint)[j].x, (*gpoint)[j].y));
    }
    linkrec.calculate_length();

    Reader::instance()->_linkRecord_map.insert(linkRecord_map::value_type(linkid, linkrec));
    f_noderec._f_pt._x = ((shpPolyLine*)m_pSHPObject)->m_vSHPPoly[i].m_vPoints[0].x;
    f_noderec._f_pt._y = ((shpPolyLine*)m_pSHPObject)->m_vSHPPoly[i].m_vPoints[0].y;
    f_noderec.push_link(linkid);

    if (Reader::instance()->_nodeRecord_map.end() != Reader::instance()->_nodeRecord_map.find(f_nodeid))
    {
      f_noderec._t_pt = Reader::instance()->_nodeRecord_map[f_nodeid]._t_pt;
      for (int idx = 0; idx < Reader::instance()->_nodeRecord_map[f_nodeid]._connect_link.size(); ++idx) {
        f_noderec.push_link(Reader::instance()->_nodeRecord_map[f_nodeid]._connect_link[idx]);
      }
      Reader::instance()->_nodeRecord_map[f_nodeid] = f_noderec;
    } else {
      Reader::instance()->_nodeRecord_map.insert(nodeRecord_map::value_type(f_nodeid, f_noderec));
    }

    size = ((shpPolyLine*)m_pSHPObject)->m_vSHPPoly[i].m_vPoints.size()-1;
    t_noderec._t_pt._x =  ((shpPolyLine*)m_pSHPObject)->m_vSHPPoly[i].m_vPoints[size].x;
    t_noderec._t_pt._y =  ((shpPolyLine*)m_pSHPObject)->m_vSHPPoly[i].m_vPoints[size].y;
    t_noderec.push_link(linkid);

    if (Reader::instance()->_nodeRecord_map.end() != Reader::instance()->_nodeRecord_map.find(t_nodeid))
    {
      t_noderec._f_pt = Reader::instance()->_nodeRecord_map[t_nodeid]._f_pt;
      for (int idx = 0; idx < Reader::instance()->_nodeRecord_map[t_nodeid]._connect_link.size(); ++idx) {
        t_noderec.push_link(Reader::instance()->_nodeRecord_map[t_nodeid]._connect_link[idx]);
      }
      Reader::instance()->_nodeRecord_map[t_nodeid] = t_noderec;
    } else {
      Reader::instance()->_nodeRecord_map.insert(nodeRecord_map::value_type(t_nodeid, t_noderec));
    }
  }
}
void CSHP::Draw(CDC* _pDC, double _ZoomFactor, GPoint _ptMapCenter, const CRect& _rect, double _lfRatio)
{
  if(m_bOpen)
    m_pSHPObject->Draw(_pDC, _ZoomFactor, _ptMapCenter, _rect, _lfRatio);
  FILE *fp = fopen("./result.bin", "rb");
  if (fp != NULL) {
    int size = 0;
    Point pt;
    GPoint g_pt;
    std::vector<std::vector<GPoint> > result_vec;
    while (feof(fp) == 0) {
      std::vector<GPoint> point_vec;
      fread((char *)&size, sizeof(int), 1, fp);
      point_vec.reserve(size);
      for (int i = 0; i < size; ++i) {
        fread((char *)&pt, sizeof(Point), 1, fp);
        g_pt = pt;
        point_vec.push_back(g_pt);
      }
      result_vec.push_back(point_vec);
      point_vec.clear();
      std::vector<GPoint>().swap(point_vec);
    }
    fclose(fp);
    shpPolyLine* polyLine = (shpPolyLine*)m_pSHPObject;
    polyLine->DrawRoute(_pDC, _rect, &result_vec);
    result_vec.clear();
    std::vector<std::vector<GPoint> >().swap(result_vec);
  }
}

int	CSHP::Track(CWnd* pWnd, GPoint point)
{
	return m_pSHPObject->Track(pWnd, point);
}

bool CSHP::InfoShow(CWnd* pWnd, GPoint point)
{
	int ID = Track(pWnd, point);
	
	if(ID == -1)
		return false;

	vector<CString> vstrFieldName;	
	vector<CString>	vstrRecord;
 
	// MainFrm에 있는 녀석을 가져오고...
	CInfoDLG*& pInfoDLG = ((CMainFrame*)AfxGetMainWnd())->m_pInfoDLG;
				
	for(int j = 0; j < m_pSHPObject->GetFieldNumber(); ++j)
	{
		char			field[80];
		int				width, decimal;
		CString			tmpString;

		tmpString = m_pSHPObject->ReadStringAttribute(ID, j);
		m_pSHPObject->GetFieldInfo(j, field, &width, &decimal);
		CString tmp(field);

		vstrRecord.push_back(tmpString);
		vstrFieldName.push_back(tmp);
	}

	// 모드리스 다이얼로그가 생성되어 있으면 가장 상위 윈도우로 올린다
	if (pInfoDLG->GetSafeHwnd() != NULL)
	{
		pInfoDLG->InsertItem(vstrFieldName, vstrRecord);
		pInfoDLG->ShowWindow(SW_SHOW);
		return true;
	}

	pInfoDLG = new CInfoDLG;
	pInfoDLG->Create(134 /* IDD_INFO_DLG */);
	pInfoDLG->InsertItem(vstrFieldName, vstrRecord);
	pInfoDLG->ShowWindow(SW_SHOW);

	return true;
}

void CSHP::SetColor(COLORREF _color)
{
	m_pSHPObject->SetColor(_color);
}

void CSHP::SetBrushColor(COLORREF _color)
{
	m_pSHPObject->SetBrushColor(_color);
}

void CSHP::SetWidth(int _nWidth)
{
	m_pSHPObject->SetWidth(_nWidth);
}

void CSHP::SetStyle(int _nStyle)
{
	m_pSHPObject->SetStyle(_nStyle);
}

void CSHP::SetSolid(bool _bSolid)
{
	m_pSHPObject->SetSolid(_bSolid);
}

void CSHP::ShowLabel()
{
	CFieldDLG	dlg;

	for(int j = 0;j < m_pSHPObject->GetFieldNumber(); ++j)
	{
		char			field[80];
		int				width, decimal;

		m_pSHPObject->GetFieldInfo(j, field, &width, &decimal);
		CString tmp(field);

		dlg.m_vField.push_back(tmp);
	}
	if(dlg.DoModal() != IDOK)
		return;

	if(dlg.m_nSelected >= 0 && dlg.m_nSelected < m_pSHPObject->GetFieldNumber())
		m_pSHPObject->SetLabel(dlg.m_nSelected);
}

void CSHP::RemoveLabel()
{
	m_pSHPObject->RemoveLabel();
}

MBR CSHP::GetMBR()
{
	if(m_bOpen)
		return m_pSHPObject->GetMBR();
	else
	{	
		MBR tmpMBR;

		tmpMBR.xmax = tmpMBR.xmin = tmpMBR.ymax = tmpMBR.ymin = 0.;
		return tmpMBR;
	}
}
