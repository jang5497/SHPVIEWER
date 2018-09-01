// SHPViewerView.h : interface of the CSHPViewerView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHPVIEWERVIEW_H__1C9FAEE1_74F4_4A6B_B559_44294D74DCBC__INCLUDED_)
#define AFX_SHPVIEWERVIEW_H__1C9FAEE1_74F4_4A6B_B559_44294D74DCBC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SHP.h"
#include "InfoDLG.h"
#include "Projection_UTM.h"
#include "SHPViewerDoc.h"

class RouteDlg;
class DataInfoDlg;
class CSHPViewerView : public CView
{
public: // create from serialization only
	CSHPViewerView();
	DECLARE_DYNCREATE(CSHPViewerView)

// Attributes
public:
	enum ProjectionMode {NGI_WEST = 0, NGI_CENTER, NGI_EAST, KATEC, ZONE51, ZONE52} m_ePrjMode;
	CProjection_UTM	m_PRJ;
	
	CSHP		m_SHP;

	MBR			m_MBR;

	bool		m_bOpenSHP;

	GPoint		m_geoCenterPoint;	// 지도 중심 좌표
	CPoint		m_scrCenterPoint; // 화면 중심 좌표
	double		m_lfZoomFactor;	// ZoomIn/out을 위한 변수
	double		m_lfRatio;		// 스크린 -> Geo 좌표..
	CPoint		m_ptZoomWindowLT, m_ptZoomWindowBR; //좌상단 점과 우하단 점..
	CRect		m_rctScreen;
	Flag         m_startFlag, m_endFlag;
	RouteDlg*    m_routeDlg;
  DataInfoDlg* m_dataInfoDlg;
  int          m_witdth;
	// 화면 제어 관련..
	enum ManipulMode {None=0, ZoomIn, ZoomOut, ZoomWindow, Panning, CenterMove, Selector, Infotool} m_eManipulationMode;


// Operations
public:
	void			ReMBR(const MBR& _mbr);
	CStatusBar*		GetStatusBar();
	GPoint			DeviceToWorld(const CPoint& _scrPoint);
	CPoint			WorldToDevice(const GPoint& _geoPoint);
	CSHPViewerDoc*	GetDocument();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSHPViewerView)
public:
	void Route();
  void DrawFlag(CDC* pDC);
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSHPViewerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void SetWidth(int _width);
// Generated message map functions
protected:
	//{{AFX_MSG(CSHPViewerView)
	afx_msg void OnZoomIn();
	afx_msg void OnUpdateZoomIn(CCmdUI* pCmdUI);
	afx_msg void OnZoomOut();
	afx_msg void OnUpdateZoomOut(CCmdUI* pCmdUI);
	afx_msg void OnZoomAll();
	afx_msg void OnCenterMove();
	afx_msg void OnUpdateCenterMove(CCmdUI* pCmdUI);
	afx_msg void OnPanning();
	afx_msg void OnUpdatePanning(CCmdUI* pCmdUI);
	afx_msg void OnZoomWindow();
	afx_msg void OnUpdateZoomWindow(CCmdUI* pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnFileOpen();
	afx_msg void OnFileClose();
	afx_msg void OnVectorSelect();
	afx_msg void OnUpdateVectorSelect(CCmdUI* pCmdUI);
	afx_msg void OnShapeInfo();
	afx_msg void OnUpdateShapeInfo(CCmdUI* pCmdUI);
	afx_msg void OnFileSave();
	afx_msg void OnMenuColorChange();
	afx_msg void OnSize0();
	afx_msg void OnSize1();
	afx_msg void OnSize2();
	afx_msg void OnSize3();
	afx_msg void OnSize4();
	afx_msg void OnSize5();
	afx_msg void OnPsDash();
	afx_msg void OnPsDashdot();
	afx_msg void OnPsDashdotdot();
	afx_msg void OnPsDot();
	afx_msg void OnPsSolid();
	afx_msg void OnMenuBrushcolorChange();
	afx_msg void OnPolygonSolidbrush();
	afx_msg void OnPolygonLine();
	afx_msg void OnShowLabel();
	afx_msg void OnRemoveLabel();
	afx_msg void OnNgiWest();
	afx_msg void OnUpdateNgiWest(CCmdUI* pCmdUI);
	afx_msg void OnNgiEast();
	afx_msg void OnUpdateNgiEast(CCmdUI* pCmdUI);
	afx_msg void OnNgiCenter();
	afx_msg void OnUpdateNgiCenter(CCmdUI* pCmdUI);
	afx_msg void OnKatecSystem();
	afx_msg void OnUpdateKatecSystem(CCmdUI* pCmdUI);
	afx_msg void OnUtmZone51();
	afx_msg void OnUpdateUtmZone51(CCmdUI* pCmdUI);
	afx_msg void OnUtmZone52();
	afx_msg void OnUpdateUtmZone52(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnRouteDlg();
	afx_msg void OnDataDlg();
};

#ifndef _DEBUG  // debug version in SHPViewerView.cpp
inline CSHPViewerDoc* CSHPViewerView::GetDocument()
   { return (CSHPViewerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHPVIEWERVIEW_H__1C9FAEE1_74F4_4A6B_B559_44294D74DCBC__INCLUDED_)
