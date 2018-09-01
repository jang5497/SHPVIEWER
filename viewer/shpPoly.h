// shpPoly.h: interface for the shpPoly class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHPPOLY_H__42998652_634E_4D73_B257_2E3CBE5E6B8A__INCLUDED_)
#define AFX_SHPPOLY_H__42998652_634E_4D73_B257_2E3CBE5E6B8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "RoutePlan/RouteData.h"
#include "shpObject.h"

class shpPolyLine : public shpObject  
{
public:
	vector<polyObject>		m_vSHPPoly;

public:
	void	Draw(CDC* _pDC, double _zoomFactor, GPoint _ptMapCenter, const CRect& _rctScreen, double _lfRatio);
	bool	Open(CString _filename);
	int		Track(CWnd* pWnd, GPoint point);		

	shpPolyLine();
	virtual ~shpPolyLine();
	void  DrawRoute(CDC* _pDC, CRect _rctScreen, std::vector<std::vector<GPoint> >* _result_vec);
  
protected:
	shpObject::hit_status HitTest(GPoint _gPoint, CRect _rctScreen, int& _ID, int& _PointID);
	void	DrawSelected(CDC* _pDC, CRect _rctScreen);
	void	DrawLabel(CDC* _pDC);
};

#endif // !defined(AFX_SHPPOLY_H__42998652_634E_4D73_B257_2E3CBE5E6B8A__INCLUDED_)
