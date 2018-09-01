// shpPoint.cpp: implementation of the shpPoint class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "shpPoint.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

shpPoint::shpPoint()
{
}

shpPoint::~shpPoint()
{
	m_vSHPPoint.clear();
}

bool shpPoint::Open(CString _filename)
{
	if(!shpObject::Open(_filename))
		return false;

	GPoint	tmpPoint;
	int tmp;

	// File pointer..
	FILE* fpSHP = fopen(m_strSHPPath, "rb");
	FILE* fpSHX = fopen(m_strSHXPath, "rb");

	// prepare adequate space..
	m_vSHPPoint.reserve(m_nRecords);

	CProgressBar	bar("Open : ", 100, m_nRecords);
	// now real data reading..
	for(int i = 0; i < m_nRecords; ++i)
	{
		fseek(fpSHX, 100 + i * 8, SEEK_SET);
		fread(&tmp, 4, 1, fpSHX);
		SwapWord(4, &tmp);
		tmp=tmp * 2;

		fseek(fpSHP, tmp + 12, SEEK_SET);
		fread(&tmpPoint.x, 8, 1, fpSHP);
		fread(&tmpPoint.y, 8, 1, fpSHP);

		m_vSHPPoint.push_back(tmpPoint);
		bar.StepIt();
	}
	fclose(fpSHP);
	fclose(fpSHX);

	return true;
}

void shpPoint::Draw(CDC* _pDC, double _ZoomFactor, GPoint _ptMapCenter, const CRect& _rect, double _lfRatio)
{
	m_lfZoomFactor = _ZoomFactor;
	m_geoCenterPoint = _ptMapCenter;
	m_scrCenterPoint.x = int(_rect.Width() / 2);
	m_scrCenterPoint.y = int(_rect.Height() / 2);
	m_lfRatio = _lfRatio;

	CPen pen;
	CBrush	brush;

	// for intersect calculation..
	CRect	mbrRect, rectBuffer;
	CPoint	tmpPoint;

	mbrRect = WorldToDevice(m_MBR);
	if(!rectBuffer.IntersectRect(&mbrRect, &_rect))
		return;

	// DC saving..
	int savedDC = _pDC->SaveDC();

	pen.CreatePen(m_nStyle, m_nWidth, m_objColor);
	brush.CreateSolidBrush(m_objColor);

	_pDC->SelectObject(&pen);
	_pDC->SelectObject(&brush);

	int nSize = m_vSHPPoint.size();
	int nOffset = (2 + m_nWidth);

	// now drawing..
	for(int i=0; i < nSize; ++i)
	{
		tmpPoint = WorldToDevice(m_vSHPPoint[i]);
		_pDC->Ellipse(tmpPoint.x - nOffset, tmpPoint.y - nOffset, tmpPoint.x + nOffset, tmpPoint.y + nOffset);
	}

	// if selected, selected point should be drawn
	DrawSelected(_pDC);

	// labeling...
	if(m_bLabel)
		DrawLabel(_pDC);

	// DC restoring..
	_pDC->RestoreDC(savedDC);
}

void shpPoint::DrawSelected(CDC* _pDC)
{
	CBrush	brush;
	CPen	pen;
	CPoint	tmpPoint;

	int savedDC = _pDC->SaveDC();

	pen.CreatePen(PS_SOLID, 0, m_objColor);
	brush.CreateSolidBrush(RGB(255, 0, 0));

	_pDC->SelectObject(&pen);
	_pDC->SelectObject(&brush);

	int nOffset = 3 + m_nWidth;

	for(int i = 0; i < m_vSelectedID.size(); ++i)
	{
		tmpPoint = WorldToDevice(m_vSHPPoint[m_vSelectedID[i]]);
		_pDC->Rectangle(tmpPoint.x - nOffset, tmpPoint.y - nOffset, tmpPoint.x + nOffset, tmpPoint.y + nOffset);
	}

	_pDC->RestoreDC(savedDC);
}

void shpPoint::DrawLabel(CDC* _pDC)
{
	CFont	m_font;
	CPoint	tmpPoint;
	int		nHeight;

	if(_pDC->IsPrinting())
		nHeight = 12 * _pDC->GetDeviceCaps(HORZRES) / 1024;
	else
		nHeight = 12;
	
	int savedDC = _pDC->SaveDC();
	
	m_font.CreateFont(nHeight, nHeight * 0.666,
		0, 0, 0,
		FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "돋움체");
		
	_pDC->SelectObject(&m_font);
	_pDC->SetBkMode(TRANSPARENT);
	_pDC->SetTextColor(RGB(0, 0, 0));
	_pDC->SetTextAlign(0|0);

	int nSize = m_vSHPPoint.size();
	for(int i=0; i < nSize; ++i)
	{
		CString tmpString = m_DBF.ReadStringAttribute(i, m_nLabelField);
		tmpPoint = WorldToDevice(m_vSHPPoint[i]);
		_pDC->TextOut(tmpPoint.x, tmpPoint.y, tmpString);
	}
	_pDC->RestoreDC(savedDC);
}

int	shpPoint::Track(CWnd* pWnd, GPoint point)
{
	// 일단 기존 selected point를 제거한다..
	m_vSelectedID.clear();

	// 점을 찍었는지 선을 찍었는지 hit test 한다.
	vector<GPoint>::iterator iter;
	int ID;

	CRect rect;
	CPoint node;

	hit_status hit_test = HitTest(point, ID);
	iter = m_vSHPPoint.begin() + ID;

	if(hit_test != hitPoint)
	// 아무데도 아니다.
	{
		if (m_eStatus == stateSelect)
		{
			m_eStatus = stateNone;

			// 다시 그리라고 한다.
			pWnd->InvalidateRect(NULL);
		}
		return -1;
	}
	m_vSelectedID.push_back(ID);

	// 원래 선택이 되어있지 않았다면, 선택된 것을 표시하기 위해 한번 그려준다.
	if (m_eStatus == stateNone)
	{
		m_eStatus = stateSelect;
		pWnd->InvalidateRect(NULL);				

		// 선택되어 있지 않았을 때는 점을 옮기는 것은 지원 안한다.
		return ID;
	}
	m_eStatus = stateSelect;	

	pWnd->InvalidateRect(NULL);

	return ID;
}

shpObject::hit_status shpPoint::HitTest(GPoint _GPoint, int& _nID)
{
	vector<GPoint>::iterator itr;
	// 점 위를 일단 먼저 본다.
	itr = m_vSHPPoint.begin();
	for (int i = 0; i < m_vSHPPoint.size(); ++i)
	{
		// mouse pointer가 점 근처에 있나 본다.
		if (PtInRect(*itr, _GPoint))
		{
			_nID = i;
			return hitPoint;
		}
		++itr;
	}
	return hitNone;
}
