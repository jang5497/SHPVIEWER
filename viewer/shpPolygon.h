// shpPolygon.h: interface for the shpPolygon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHPPOLYGON_H__CCE87B33_CDC4_11D5_BA26_00104B1701D2__INCLUDED_)
#define AFX_SHPPOLYGON_H__CCE87B33_CDC4_11D5_BA26_00104B1701D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "shpObject.h"

class shpPolygon : public shpObject  
{
public:
	// real data container..
	vector<polyObject>		m_vSHPPoly;

protected:

public:
	void	Draw(CDC* _pDC, double _ZoomFactor, GPoint _ptMapCenter, const CRect& _rect, double _lfRatio);
	bool	Open(CString _filename);
	int		Track(CWnd* pWnd, GPoint point);		

	shpPolygon();
	virtual ~shpPolygon();

protected:
	shpObject::hit_status HitTest(GPoint _gPoint, CRect _rctScreen, int& _ID, int& _PointID);
	void	DrawPolygon(CDC* _pDC, CRect _rect);
	void	DrawPolyLine(CDC* _pDC, CRect _rect);
	void	DrawSelected(CDC* _pDC, CRect _rctScreen);
	void	DrawLabel(CDC* _pDC);
	double	Area2(GPoint a, GPoint b, GPoint c);
	GPoint	FindCG(vector<GPoint>& vPoint);
	GPoint	Centroid3(GPoint a, GPoint b, GPoint c);
};

#endif // !defined(AFX_SHPPOLYGON_H__CCE87B33_CDC4_11D5_BA26_00104B1701D2__INCLUDED_)
