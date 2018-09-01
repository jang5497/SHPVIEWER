// SHP.h: interface for the CSHP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHP_H__6FB857FC_8104_4FD2_ADD3_C437C9146730__INCLUDED_)
#define AFX_SHP_H__6FB857FC_8104_4FD2_ADD3_C437C9146730__INCLUDED_

#include "shpObject.h"
#include "shpPoint.h"
#include "shpPoly.h"
#include "shpPolygon.h"
#include "shpMultiPoint.h"
#include "RoutePlan/RoutePlan.h"
#include "RoutePlan/Reader.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class Flag
{
public :
	Flag() : _is_select(false), _linkid(0) {};
	~Flag() {};
  void clear() {
    _is_select = false;
    _pt.x = 0;
    _pt.y = 0;
    _linkid = 0;
  }
public :
	bool           _is_select;
	GPoint         _pt;
	unsigned long  _linkid;
};

class CSHP  
{
public:	// for variables
	// Real Data container...
	shpObject*			m_pSHPObject;
	bool				m_bOpen;					// To check whether SHP is Opened or not

public: // for functions
	CSHP();
	virtual ~CSHP();
	
	bool		Open(CString	_filename);				// SHP file open
	bool		Close();
	void    init_record();
	void		Draw(CDC* _pDC, double _ZoomFactor, GPoint _ptMapCenter, const CRect& _rect, double _lfRatio);
	int			Track(CWnd* pWnd, GPoint point);
	bool		InfoShow(CWnd* pWnd, GPoint point);
	void		SetColor(COLORREF _color);
	void		SetBrushColor(COLORREF _color);
	void		SetWidth(int _nWidth);
	void		SetStyle(int _nStyle);
	void		SetSolid(bool _bSolid);
	void		ShowLabel();
	void		RemoveLabel();
	MBR			GetMBR();

protected:
	int			m_nShapeType;
};

#endif // !defined(AFX_SHP_H__6FB857FC_8104_4FD2_ADD3_C437C9146730__INCLUDED_)
