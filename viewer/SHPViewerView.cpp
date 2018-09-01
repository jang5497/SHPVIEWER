// SHPViewerView.cpp : implementation of the CSHPViewerView class
//

#include "stdafx.h"
#include "SHPViewer.h"
#include "RouteDlg.h"
#include "DataInfoDlg.h"
#include "SHPViewerDoc.h"
#include "SHPViewerView.h"
#include "MainFrm.h"
#include "time.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSHPViewerView

IMPLEMENT_DYNCREATE(CSHPViewerView, CView)

BEGIN_MESSAGE_MAP(CSHPViewerView, CView)
	//{{AFX_MSG_MAP(CSHPViewerView)
	ON_COMMAND(IDM_ZOOM_IN, OnZoomIn)
	ON_UPDATE_COMMAND_UI(IDM_ZOOM_IN, OnUpdateZoomIn)
	ON_COMMAND(IDM_ZOOM_OUT, OnZoomOut)
	ON_UPDATE_COMMAND_UI(IDM_ZOOM_OUT, OnUpdateZoomOut)
	ON_COMMAND(IDM_ZOOM_ALL, OnZoomAll)
	ON_COMMAND(IDM_CENTER_MOVE, OnCenterMove)
	ON_UPDATE_COMMAND_UI(IDM_CENTER_MOVE, OnUpdateCenterMove)
	ON_COMMAND(IDM_PANNING, OnPanning)
	ON_UPDATE_COMMAND_UI(IDM_PANNING, OnUpdatePanning)
	ON_COMMAND(IDM_ZOOM_WINDOW, OnZoomWindow)
	ON_UPDATE_COMMAND_UI(IDM_ZOOM_WINDOW, OnUpdateZoomWindow)
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(IDM_FILE_CLOSE, OnFileClose)
	ON_COMMAND(IDM_VECTOR_SELECT, OnVectorSelect)
	ON_UPDATE_COMMAND_UI(IDM_VECTOR_SELECT, OnUpdateVectorSelect)
	ON_COMMAND(IDM_SHAPE_INFO, OnShapeInfo)
	ON_UPDATE_COMMAND_UI(IDM_SHAPE_INFO, OnUpdateShapeInfo)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_MENU_COLOR_CHANGE, OnMenuColorChange)
	ON_COMMAND(IDM_SIZE_0, OnSize0)
	ON_COMMAND(IDM_SIZE_1, OnSize1)
	ON_COMMAND(IDM_SIZE_2, OnSize2)
	ON_COMMAND(IDM_SIZE_3, OnSize3)
	ON_COMMAND(IDM_SIZE_4, OnSize4)
	ON_COMMAND(IDM_SIZE_5, OnSize5)
	ON_COMMAND(IDM_PS_DASH, OnPsDash)
	ON_COMMAND(IDM_PS_DASHDOT, OnPsDashdot)
	ON_COMMAND(IDM_PS_DASHDOTDOT, OnPsDashdotdot)
	ON_COMMAND(IDM_PS_DOT, OnPsDot)
	ON_COMMAND(IDM_PS_SOLID, OnPsSolid)
	ON_COMMAND(ID_MENU_BRUSHCOLOR_CHANGE, OnMenuBrushcolorChange)
	ON_COMMAND(IDM_POLYGON_SOLIDBRUSH, OnPolygonSolidbrush)
	ON_COMMAND(IDM_POLYGON_LINE, OnPolygonLine)
	ON_COMMAND(IDM_SHOW_LABEL, OnShowLabel)
	ON_COMMAND(IDM_REMOVE_LABEL, OnRemoveLabel)
	ON_COMMAND(IDM_NGI_WEST, OnNgiWest)
	ON_UPDATE_COMMAND_UI(IDM_NGI_WEST, OnUpdateNgiWest)
	ON_COMMAND(IDM_NGI_EAST, OnNgiEast)
	ON_UPDATE_COMMAND_UI(IDM_NGI_EAST, OnUpdateNgiEast)
	ON_COMMAND(IDM_NGI_CENTER, OnNgiCenter)
	ON_UPDATE_COMMAND_UI(IDM_NGI_CENTER, OnUpdateNgiCenter)
	ON_COMMAND(IDM_KATEC_SYSTEM, OnKatecSystem)
	ON_UPDATE_COMMAND_UI(IDM_KATEC_SYSTEM, OnUpdateKatecSystem)
	ON_COMMAND(IDM_UTM_ZONE51, OnUtmZone51)
	ON_UPDATE_COMMAND_UI(IDM_UTM_ZONE51, OnUpdateUtmZone51)
	ON_COMMAND(IDM_UTM_ZONE52, OnUtmZone52)
	ON_UPDATE_COMMAND_UI(IDM_UTM_ZONE52, OnUpdateUtmZone52)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_COMMAND(ID_ROUTE, &CSHPViewerView::OnRouteDlg)
	ON_COMMAND(ID_DATA, &CSHPViewerView::OnDataDlg)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSHPViewerView construction/destruction
Reader* Reader::_instance = NULL;

CSHPViewerView::CSHPViewerView()
{
  m_witdth = 0;
	m_routeDlg = NULL;
  m_dataInfoDlg = NULL;
	m_bOpenSHP = false;
	m_lfZoomFactor = 1.0;
	m_geoCenterPoint.x = m_geoCenterPoint.y = 0.0;
	m_scrCenterPoint.x = m_scrCenterPoint.y = 0;
	m_ePrjMode = NGI_CENTER;
	m_PRJ.InitTM(6377397.155, 6356078.96325, 1., 2.21661859489632, 0.663225115757845, 200000, 500000, 1., 1.);
}

CSHPViewerView::~CSHPViewerView()
{

}

BOOL CSHPViewerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CSHPViewerView drawing

void CSHPViewerView::OnDraw(CDC* pDC)
{
  CSHPViewerDoc* pDoc = GetDocument();
  ASSERT_VALID(pDoc);

  // Double Buffering은 pDC와 compatible한 memDC를 하나 만들고 
  // Bitmap을 만들어 여기다가 그림을 그리고, 나중에 pDC에다가 BitBlt하는 것입니다.

  CRect rect;
  if(pDC->IsPrinting())
  {
    rect.top = 0;
    rect.left = 0;
    rect.right = pDC->GetDeviceCaps(HORZRES);
    rect.bottom = pDC->GetDeviceCaps(VERTRES);
  }
  else
  {
    GetClientRect(&rect);	
  }

  // 종횡비 계산..
  double	lfRatio_X = fabs((rect.Width()) / (m_MBR.xmax - m_MBR.xmin));
  double	lfRatio_Y = fabs((rect.Height()) / (m_MBR.ymax - m_MBR.ymin));

  // 비율 계산...
  if(lfRatio_X < lfRatio_Y)
    m_lfRatio = lfRatio_X;
  else
    m_lfRatio = lfRatio_Y;

  m_scrCenterPoint.x = (rect.right-rect.left)/2;
  m_scrCenterPoint.y = (rect.bottom-rect.top)/2;

  // Buffering
  CDC mem_dc;
  CDC* pDrawDC = pDC;
  CBitmap bitmap;
  CBitmap* pOldBitmap;

  if (mem_dc.CreateCompatibleDC(pDC))
  {
    if (bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height()))
    {
      pDrawDC = &mem_dc;
      pOldBitmap = mem_dc.SelectObject(&bitmap);
    }
  }

  pDrawDC->FillSolidRect(&rect, RGB(255, 255, 255));

  if(m_bOpenSHP)
    m_SHP.Draw(pDrawDC, m_lfZoomFactor, m_geoCenterPoint, rect, m_lfRatio);

  //Swap buffers
  if (pDrawDC != pDC)
  {
    pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(),
      &mem_dc,0 ,0 , SRCCOPY);
    mem_dc.SelectObject(pOldBitmap);
  }
  if (((m_startFlag._pt.x != 0) || (m_endFlag._pt.x != 0)) && m_routeDlg != NULL) {
    DrawFlag(pDC);
  }
}

/////////////////////////////////////////////////////////////////////////////
// CSHPViewerView printing
void CSHPViewerView::DrawFlag(CDC* pDC)
{
  CPen	pen;
  CBrush	brush;
  CFont	font;
  //FONT
  int		nHeight;
  if(pDC->IsPrinting()) nHeight = 25 * pDC->GetDeviceCaps(HORZRES) / 1024;
  else nHeight = 25;
  font.CreateFont(nHeight, nHeight * 0.666,
		0, 0, 2,
    FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
    CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "돋움체");
  pDC->SelectObject(&font);
  pDC->SetBkMode(TRANSPARENT);
  pDC->SetTextAlign(0|0);
  if (m_startFlag._pt.x != 0) {
    pDC->SetTextColor(RGB(34, 116, 28));
    CPoint pt = WorldToDevice(m_startFlag._pt);
    pDC->TextOut(pt.x, pt.y, _T("S"));
  }
  if (m_endFlag._pt.x != 0) {
    pDC->SetTextColor(RGB(95, 0, 255));
    CPoint pt = WorldToDevice(m_endFlag._pt);
    pDC->TextOut(pt.x, pt.y, _T("E"));
  }
}
BOOL CSHPViewerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSHPViewerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CSHPViewerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CSHPViewerView diagnostics

#ifdef _DEBUG
void CSHPViewerView::AssertValid() const
{
	CView::AssertValid();
}

void CSHPViewerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSHPViewerDoc* CSHPViewerView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSHPViewerDoc)));
	return (CSHPViewerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSHPViewerView message handlers
void CSHPViewerView::OnZoomIn() 
{
	(m_eManipulationMode==ZoomIn) ? m_eManipulationMode = None : m_eManipulationMode = ZoomIn;
}

void CSHPViewerView::OnUpdateZoomIn(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck((m_eManipulationMode==ZoomIn));
}

void CSHPViewerView::OnZoomOut() 
{
	(m_eManipulationMode==ZoomOut) ? m_eManipulationMode = None : m_eManipulationMode = ZoomOut;
}

void CSHPViewerView::OnUpdateZoomOut(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck((m_eManipulationMode==ZoomOut));
}

void CSHPViewerView::OnZoomAll() 
{
	//지도중심좌표 구하기
	m_geoCenterPoint.x = (m_MBR.xmin + m_MBR.xmax)/2;
	m_geoCenterPoint.y = (m_MBR.ymin + m_MBR.ymax)/2;
	
	m_lfZoomFactor = 1.0;

	Invalidate();
}

void CSHPViewerView::OnShapeInfo() 
{
	(m_eManipulationMode == Infotool) ? m_eManipulationMode = None : m_eManipulationMode = Infotool;		
}

void CSHPViewerView::OnUpdateShapeInfo(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck((m_eManipulationMode == Infotool));	
}

void CSHPViewerView::OnVectorSelect() 
{
	(m_eManipulationMode==Selector) ? m_eManipulationMode = None : m_eManipulationMode = Selector;	
}


void CSHPViewerView::OnUpdateVectorSelect(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck((m_eManipulationMode == Selector));
}

void CSHPViewerView::OnCenterMove() 
{
	(m_eManipulationMode==CenterMove) ? m_eManipulationMode = None : m_eManipulationMode = CenterMove;
}

void CSHPViewerView::OnUpdateCenterMove(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck((m_eManipulationMode==CenterMove));
}

void CSHPViewerView::OnPanning() 
{
	(m_eManipulationMode==Panning) ? m_eManipulationMode = None : m_eManipulationMode = Panning;
}

void CSHPViewerView::OnUpdatePanning(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck((m_eManipulationMode==Panning));
}

void CSHPViewerView::OnZoomWindow() 
{
	(m_eManipulationMode==ZoomWindow) ? m_eManipulationMode = None : m_eManipulationMode = ZoomWindow;
}

void CSHPViewerView::OnUpdateZoomWindow(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck((m_eManipulationMode==ZoomWindow));
}

void CSHPViewerView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);

	m_scrCenterPoint.x = cx/2;
	m_scrCenterPoint.y = cy/2;
}

void CSHPViewerView::OnLButtonDown(UINT nFlags, CPoint point) 
{
  if (m_routeDlg != NULL) {
    char string[40];
    int index = m_SHP.Track(this,DeviceToWorld(point)) ;
    if (index != -1) {
      if (m_startFlag._is_select) {
        m_startFlag._is_select = false;
        m_startFlag._pt = DeviceToWorld(point);
        strcpy(string, m_SHP.m_pSHPObject->ReadStringAttribute(index, 0));
        m_startFlag._linkid = strtoul(string, NULL, 10);
        m_routeDlg->set_listinfo();
      } else if (m_endFlag._is_select)  {
        m_endFlag._is_select = false;
        m_endFlag._pt = DeviceToWorld(point);
        strcpy(string, m_SHP.m_pSHPObject->ReadStringAttribute(index, 0));
        m_endFlag._linkid = strtoul(string, NULL, 10);
        m_routeDlg->set_listinfo();
      }
    }
    Invalidate();
  }
  switch(m_eManipulationMode)
  {
  case ZoomIn:
    m_geoCenterPoint = DeviceToWorld(point);		// 그냥 전체 확대를 위해..
    m_lfZoomFactor *= 1.7;
    Invalidate();
    break;

  case ZoomOut:
    m_geoCenterPoint = DeviceToWorld(point);		// 그냥 전체 축소를 위해..
    m_lfZoomFactor *= 0.65;
    Invalidate();
    break;

  case CenterMove:
    m_geoCenterPoint = DeviceToWorld(point);
    Invalidate();
    break;

  case ZoomWindow:
  case Panning:
    m_ptZoomWindowLT = m_ptZoomWindowBR = point;
    SetCapture();
    break;

  case Selector:
    m_SHP.Track(this, DeviceToWorld(point));
    break;

  case Infotool:
    m_SHP.InfoShow(this, DeviceToWorld(point));
    break;
  }
  CView::OnLButtonDown(nFlags, point);
}

void CSHPViewerView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CRect	rect;			// 화면크기를 받아오는 변수
	GPoint	tmpPoint1, tmpPoint2;
	double ZoomWidth, ZoomHeight;
	double ZoomRatio;

	m_ptZoomWindowBR = point;
	tmpPoint1 = DeviceToWorld(m_ptZoomWindowLT);
	tmpPoint2 = DeviceToWorld(m_ptZoomWindowBR);
		
	switch(m_eManipulationMode)
	{
	case ZoomWindow:  // 영역확대인 경우

		ReleaseCapture();  // 마우스 버튼이 놓이는 순간 마우스 이벤트 관할권을 Windows에 돌려준다. 
						// 쌍을 이루는 함수는 SetCapture()임.. OnLButtonDown에서 찾을 수 있음

		// 실좌표계에서의 확대창의 크기를 계산한다.
		ZoomWidth  = fabs(tmpPoint1.x - tmpPoint2.x);
		ZoomHeight = fabs(tmpPoint1.y - tmpPoint2.y);

		GetClientRect(&rect);  // 화면의 크기를 받아온다.
		
		if(ZoomWidth >= ZoomHeight)
			ZoomRatio = rect.Width()/ZoomWidth;	//Zoom의 크기를 계산한다.
		else
			ZoomRatio = rect.Height()/ZoomHeight;
		
		m_lfZoomFactor = ZoomRatio/m_lfRatio;  // ZoomIn, ZoomOut을 위해 m_fZoomRatio로 계산해 처리한다. 
		
		if(m_lfRatio >500)	
			break;
		
		m_geoCenterPoint.x = (tmpPoint1.x + tmpPoint2.x) / 2;  //지도 중심 좌표를 계산해 준다.
		m_geoCenterPoint.y = (tmpPoint1.y + tmpPoint2.y) / 2;  //지도 중심 좌표를 계산한다.

		Invalidate(NULL);

		break;
	
	case Panning:  // 패닝 명령인 경우...
	
		ReleaseCapture();  // 마우스 이벤트 관할을 윈도우로 돌려주고..
		
		break;
	}

	CView::OnLButtonUp(nFlags, point);
}

#define PI 3.141592653589793238462643383279502884197169399375105820974944592308
void CSHPViewerView::OnMouseMove(UINT nFlags, CPoint point) 
{
	CRect	tmpRect;
	GetClientRect(&tmpRect);

	m_scrCenterPoint.x = tmpRect.Width() / 2;
	m_scrCenterPoint.y = tmpRect.Height() / 2;

	char buffer[100];
	int i;
	GPoint	tmp;

	CStatusBar *pStatusBar = GetStatusBar(); // 상태바 포인터를 얻고..
    if (!pStatusBar) return;

    // Draw a Geo-Coordinate of mouse point
	double lat, lon;
	int lonD, lonM, latD, latM;
	double lonS, latS;

	tmp=DeviceToWorld(point);
	m_PRJ.Inv(tmp.x, tmp.y, &lon, &lat);
	m_PRJ.D2DMS(180 * lon / PI, 180 * lat / PI, lonD, lonM, lonS, latD, latM, latS);

	i = sprintf(buffer, "E=%.2lf   ", tmp.x);
	i+= sprintf(buffer+i, "N=%.2lf   ", tmp.y);
	i+= sprintf(buffer+i, "Lon=%d:%d:%.5lf   ", lonD, lonM, lonS);
	i+= sprintf(buffer+i, "Lat=%d:%d:%.5lf", latD, latM, latS);

    if (::IsWindow(m_hWnd) && IsWindowVisible()) 
	{
        pStatusBar->SetWindowText(buffer);
        pStatusBar->UpdateWindow();
	}

	// Mouse manipulation mode
	switch(m_eManipulationMode)
	{
	case ZoomWindow:
		if(nFlags & MK_LBUTTON)
		{
			CClientDC	dc(this);
			CPen	cPen, *oldPen;
			
			cPen.CreatePen(PS_DOT, 1, RGB(0,0,0));
			oldPen = (CPen*)dc.SelectObject(&cPen);
			dc.SetROP2(R2_XORPEN);
			
			//기존의 Window을 지운다.
			dc.MoveTo(m_ptZoomWindowLT);
			dc.LineTo(m_ptZoomWindowLT.x, m_ptZoomWindowBR.y);
			dc.LineTo(m_ptZoomWindowBR);
			dc.LineTo(m_ptZoomWindowBR.x, m_ptZoomWindowLT.y);
			dc.LineTo(m_ptZoomWindowLT);
			// 새로운 window를 그린다.
			dc.MoveTo(m_ptZoomWindowLT);
			dc.LineTo(m_ptZoomWindowLT.x, point.y);
			dc.LineTo(point);
			dc.LineTo(point.x, m_ptZoomWindowLT.y);
			dc.LineTo(m_ptZoomWindowLT);

			m_ptZoomWindowBR = point;
			dc.SelectObject(oldPen);
		}
		break;

	case Panning:
		if(nFlags & MK_LBUTTON)
		{
			double dX = (m_ptZoomWindowBR.x - point.x) ;
			double dY = (m_ptZoomWindowBR.y - point.y) ;

			m_geoCenterPoint.x += dX / (m_lfRatio * m_lfZoomFactor );
			m_geoCenterPoint.y -= dY / (m_lfRatio * m_lfZoomFactor );
			
			m_ptZoomWindowBR = point;

			InvalidateRect(NULL);
		}
	}
	CView::OnMouseMove(nFlags, point);
}

CPoint CSHPViewerView::WorldToDevice(const GPoint& _geoPoint)
{
	CPoint	tmpPoint;

	tmpPoint.x = long(m_lfRatio * (_geoPoint.x - m_geoCenterPoint.x) * m_lfZoomFactor + m_scrCenterPoint.x);
	tmpPoint.y = long(m_lfRatio * (m_geoCenterPoint.y - _geoPoint.y) * m_lfZoomFactor + m_scrCenterPoint.y);

	return tmpPoint;
}

GPoint CSHPViewerView::DeviceToWorld(const CPoint& _scrPoint)
{
	GPoint	tmpGeoPoint;

	tmpGeoPoint.x = m_geoCenterPoint.x + (_scrPoint.x - m_scrCenterPoint.x) / (m_lfZoomFactor * m_lfRatio);
	tmpGeoPoint.y = m_geoCenterPoint.y - (_scrPoint.y - m_scrCenterPoint.y) / (m_lfZoomFactor * m_lfRatio);

	return tmpGeoPoint;
}

BOOL CSHPViewerView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	HCURSOR	hCursor;

	switch(m_eManipulationMode)
	{
	case ZoomIn:		// ZoomIn, ZoomWindow는 같은 커서 사용
	case ZoomWindow:
		hCursor = AfxGetApp()->LoadCursor(IDC_ZOOM_IN);
		SetCursor(hCursor);
		return true;

	case ZoomOut:		// ZoomOut 커서..
		hCursor = AfxGetApp()->LoadCursor(IDC_ZOOM_OUT);
		SetCursor(hCursor);
		return true;

	case Panning:		// 패닝 커서..
		hCursor = AfxGetApp()->LoadCursor(IDC_PANNING);
		SetCursor(hCursor);
		return true;

	case CenterMove:	// 중심이동 커서..
		hCursor = AfxGetApp()->LoadCursor(IDC_ZOOM_CENTER);
		SetCursor(hCursor);
		return true;

	case Infotool:
		hCursor = AfxGetApp()->LoadCursor(IDC_SELECTOR);
		SetCursor(hCursor);
		return true;
	}

	return CView::OnSetCursor(pWnd, nHitTest, message);
}

BOOL CSHPViewerView::OnEraseBkgnd(CDC* pDC) 
{
//	return CView::OnEraseBkgnd(pDC);
	return false;
}

CStatusBar* CSHPViewerView::GetStatusBar()
{
    CFrameWnd *pFrame = (CFrameWnd*)AfxGetMainWnd();
    if (!pFrame || !pFrame->IsKindOf(RUNTIME_CLASS(CFrameWnd))) 
        return NULL;

    CStatusBar* pBar = (CStatusBar*) pFrame->GetMessageBar();
    if (!pBar || !pBar->IsKindOf(RUNTIME_CLASS(CStatusBar))) 
        return NULL;

    return pBar;
}

void CSHPViewerView::OnFileOpen() 
{
	CRect	tmpRect;
	GetClientRect(tmpRect);

	CString tmpString;

	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, 
				"SHP Files (*.shp)|*.shp|All Files (*.*)|*.*|", NULL);

	// Real routine of SHP & DIB loading
	if(dlg.DoModal() == IDOK)
	{
		tmpString  = dlg.GetFileExt();
		tmpString.MakeLower();

		if(tmpString == "shp")
		{
			if(m_bOpenSHP)
				m_SHP.Close();

			if(!m_SHP.Open(dlg.GetPathName()))
			{
				AfxMessageBox("An Error Occured!!", IDOK | MB_ICONSTOP);
				return;
			}
			ReMBR(m_SHP.GetMBR());
			m_bOpenSHP = true;
		}
		else
			return;
		
		m_geoCenterPoint.x = (m_MBR.xmax + m_MBR.xmin) / 2;
		m_geoCenterPoint.y = (m_MBR.ymax + m_MBR.ymin) / 2;
		m_lfZoomFactor = 1.;
		m_lfRatio = tmpRect.Height() / m_MBR.ymax;

		Invalidate();
	}
	else
		return;
	
}

void CSHPViewerView::OnFileClose() 
{
	m_bOpenSHP = !m_SHP.Close();

	Invalidate();
}

void CSHPViewerView::ReMBR(const MBR& _mbr)
{
	// only for multiple SHP
	/*	
		if(_mbr.xmax>=m_MBR.xmax) m_MBR.xmax=_mbr.xmax;
		if(_mbr.xmin<=m_MBR.xmin) m_MBR.xmin=_mbr.xmin;
		if(_mbr.ymax>=m_MBR.ymax) m_MBR.ymax=_mbr.ymax;
		if(_mbr.ymin<=m_MBR.ymin) m_MBR.ymin=_mbr.ymin;
	*/
	m_MBR = _mbr;

	m_geoCenterPoint.x = (m_MBR.xmax + m_MBR.xmin) / 2;
	m_geoCenterPoint.y = (m_MBR.ymax + m_MBR.ymin) / 2;
}

void CSHPViewerView::OnFileSave() 
{
//	if(m_bOpenSHP)
//		m_SHP.Save();
}

void CSHPViewerView::OnMenuColorChange() 
{
	CColorDialog	dlg;
	
	if(dlg.DoModal()==IDOK)
	{
		m_SHP.SetColor(dlg.GetColor());
		Invalidate();
	}
	else
		return;
}

void CSHPViewerView::OnSize0() 
{
	SetWidth(0);
}

void CSHPViewerView::OnSize1() 
{
	SetWidth(1);
}

void CSHPViewerView::OnSize2() 
{
	SetWidth(0);
}

void CSHPViewerView::OnSize3() 
{
	SetWidth(3);
}

void CSHPViewerView::OnSize4() 
{
	SetWidth(4);
}

void CSHPViewerView::OnSize5() 
{
	SetWidth(5);
}

void CSHPViewerView::SetWidth(int _width)
{
  m_witdth = _width;
	m_SHP.SetWidth(_width);
	Invalidate();
}

void CSHPViewerView::OnPsDash() 
{
	m_SHP.SetStyle(PS_DASH);
	Invalidate();
}

void CSHPViewerView::OnPsDashdot() 
{
	m_SHP.SetStyle(PS_DOT);
	Invalidate();
}

void CSHPViewerView::OnPsDashdotdot() 
{	
	m_SHP.SetStyle(PS_DASHDOTDOT);
	Invalidate();
}

void CSHPViewerView::OnPsDot() 
{
	m_SHP.SetStyle(PS_DOT);
	Invalidate();
}

void CSHPViewerView::OnPsSolid() 
{
	m_SHP.SetStyle(PS_SOLID);
	Invalidate();	
}

void CSHPViewerView::OnMenuBrushcolorChange() 
{
	CColorDialog	dlg;
	
	if(dlg.DoModal()!= IDOK)
		return;
	
	m_SHP.SetBrushColor(dlg.GetColor());
		Invalidate();
}

void CSHPViewerView::OnPolygonSolidbrush() 
{
	m_SHP.SetSolid(true);
	Invalidate();
}

void CSHPViewerView::OnPolygonLine() 
{
	m_SHP.SetSolid(false);
	Invalidate();
}

void CSHPViewerView::OnShowLabel() 
{
	m_SHP.ShowLabel();
	Invalidate();
}

void CSHPViewerView::OnRemoveLabel() 
{
	m_SHP.RemoveLabel();
	Invalidate();
}

void CSHPViewerView::OnNgiWest() 
{
	m_PRJ.InitTM(6377397.155, 6356078.96325, 1., 2.18171200985643, 0.663225115757845, 200000, 500000, 1., 1.);
	m_ePrjMode = NGI_WEST;
}

void CSHPViewerView::OnUpdateNgiWest(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck((m_ePrjMode==NGI_WEST));
}

void CSHPViewerView::OnNgiEast() 
{
	m_PRJ.InitTM(6377397.155, 6356078.96325, 1., 2.2515251799362, 0.663225115757845, 200000, 500000, 1., 1.);
	m_ePrjMode = NGI_EAST;
}

void CSHPViewerView::OnUpdateNgiEast(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck((m_ePrjMode==NGI_EAST));
}

void CSHPViewerView::OnNgiCenter() 
{
	m_PRJ.InitTM(6377397.155, 6356078.96325, 1., 2.21661859489632, 0.663225115757845, 200000, 500000, 1., 1.);	
	m_ePrjMode = NGI_CENTER;
}

void CSHPViewerView::OnUpdateNgiCenter(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck((m_ePrjMode==NGI_CENTER));	
}

void CSHPViewerView::OnKatecSystem() 
{
	m_PRJ.InitTM(6377397.155, 6356078.96325, 0.9999, 2.23402144255274, 0.663225115757845, 400000, 600000, 1., 1.);	
	m_ePrjMode = KATEC;
}

void CSHPViewerView::OnUpdateKatecSystem(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck((m_ePrjMode==KATEC));			
}

void CSHPViewerView::OnUtmZone51() 
{
	m_PRJ.Init(6378137., 6356752.3142, 0.9996, 51);
	m_ePrjMode = ZONE51;
}

void CSHPViewerView::OnUpdateUtmZone51(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck((m_ePrjMode==ZONE51));			
}

void CSHPViewerView::OnUtmZone52() 
{
	m_PRJ.Init(6378137., 6356752.3142, 0.9996, 52);	
	m_ePrjMode = ZONE52;
}

void CSHPViewerView::OnUpdateUtmZone52(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck((m_ePrjMode==ZONE52));				
}

void CSHPViewerView::OnRouteDlg()
{
	if (m_bOpenSHP) {
		if (m_routeDlg != NULL) {
			m_routeDlg->ShowWindow(SW_SHOW);
			m_routeDlg->SetFocus();
			return;
		}
		m_routeDlg = new RouteDlg;
		m_routeDlg->Create(this);
		m_routeDlg->ShowWindow(SW_SHOW);
	}
}

void CSHPViewerView::OnDataDlg()
{
  if (m_bOpenSHP) {
		if (m_dataInfoDlg != NULL) {
			m_dataInfoDlg->ShowWindow(SW_SHOW);
			m_dataInfoDlg->SetFocus();
			return;
		}
    m_dataInfoDlg = new DataInfoDlg;
    m_dataInfoDlg->Create(this);
		m_dataInfoDlg->ShowWindow(SW_SHOW);
	}
}

void CSHPViewerView::Route()
{
	int ret = 0;
	FlagPoint s_point, e_point;
	s_point._linkid = m_startFlag._linkid;
	s_point._pt._x = m_startFlag._pt.x;
	s_point._pt._y = m_startFlag._pt.y;
	
	e_point._linkid = m_endFlag._linkid;
	e_point._pt._x = m_endFlag._pt.x;
	e_point._pt._y = m_endFlag._pt.y;
	
	RoutePlan* route = new RoutePlan;
	route->init_route(s_point, e_point);
	ret = route->Route();
	if (ret == ROUTE_SUCCESS) route->make_result();
	delete route;
  if (ret != ROUTE_SUCCESS) {
    ::AfxMessageBox(_T("error code %d"), ret);
  }
	Invalidate();
}