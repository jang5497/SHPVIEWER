// shpMultiPoint.h: interface for the shpMultiPoint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHPMULTIPOINT_H__81E6D6AC_F05F_4452_802D_2FB2C0916C5F__INCLUDED_)
#define AFX_SHPMULTIPOINT_H__81E6D6AC_F05F_4452_802D_2FB2C0916C5F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "shpObject.h"

class shpMultiPoint : public shpObject  
{
public:
	// real data container..
	vector<polyObject>		m_vSHPPoly;

protected:

public:
	bool	Open(CString _filename);
	void	Draw(CDC* _pDC, double _ZoomFactor, GPoint _ptMapCenter, const CRect& _rect, double _lfRatio);
	int		Track(CWnd* pWnd, GPoint point);		

	shpMultiPoint();
	virtual ~shpMultiPoint();

protected:
	shpObject::hit_status HitTest(GPoint _GPoint, int& _nID);
	void	DrawSelected(CDC* _pDC, CRect _rect);
	void	DrawLabel(CDC* _pDC);
};

#endif // !defined(AFX_SHPMULTIPOINT_H__81E6D6AC_F05F_4452_802D_2FB2C0916C5F__INCLUDED_)
