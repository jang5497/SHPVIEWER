// shpPoint.h: interface for the shpPoint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHPPOINT_H__36940BE1_615C_4C4A_8258_97B397438EB8__INCLUDED_)
#define AFX_SHPPOINT_H__36940BE1_615C_4C4A_8258_97B397438EB8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "shpObject.h"

class shpPoint : public shpObject  
{
public:
	// real data container..
	vector<GPoint>		m_vSHPPoint;

public:
	bool	Open(CString	_filename);
	void	Draw(CDC* _pDC, double _ZoomFactor, GPoint _ptMapCenter, const CRect& _rect, double _lfRatio);
	int		Track(CWnd* pWnd, GPoint point);

	shpPoint();
	virtual ~shpPoint();

protected:
	void	DrawSelected(CDC* _pDC);
	void	DrawLabel(CDC* _pDC);
	shpObject::hit_status HitTest(GPoint _GPoint, int& _nID);
};

#endif // !defined(AFX_SHPPOINT_H__36940BE1_615C_4C4A_8258_97B397438EB8__INCLUDED_)
