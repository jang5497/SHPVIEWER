// RouteDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SHPViewer.h"
#include "RouteDlg.h"


// RouteDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(RouteDlg, CDialog)

RouteDlg::RouteDlg(CWnd* pParent /*=NULL*/)
	: CDialog(RouteDlg::IDD, pParent)
{
}

RouteDlg::~RouteDlg()
{
  m_view = NULL;
}

void RouteDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_LIST_START, m_startList);
  DDX_Control(pDX, IDC_LIST_END, m_endList);
}


BEGIN_MESSAGE_MAP(RouteDlg, CDialog)
	ON_BN_CLICKED(IDC_STARTFLAG, &RouteDlg::OnBnClickedStartflag)
	ON_BN_CLICKED(IDC_ENDFLAG, &RouteDlg::OnBnClickedEndflag)
	ON_BN_CLICKED(IDC_ROUTE, &RouteDlg::OnBnClickedRoute)
END_MESSAGE_MAP()

BOOL RouteDlg::OnInitDialog()
{
  CDialog::OnInitDialog();
  init_listinfo();
  set_listinfo();
  return TRUE;
}

BOOL RouteDlg::Create(CWnd* pParent) 
{
  m_view = (CSHPViewerView*)pParent;
  return CDialog::Create(IDD, pParent);
}
// RouteDlg 메시지 처리기입니다.

void RouteDlg::OnBnClickedStartflag()
{
  m_view->m_startFlag.clear();
	m_view->m_startFlag._is_select = true;
	if (m_view->m_endFlag._is_select) m_view->m_endFlag._is_select = false; 
}

void RouteDlg::OnBnClickedEndflag()
{
  m_view->m_endFlag.clear();
	m_view->m_endFlag._is_select = true;
	if (m_view->m_startFlag._is_select) m_view->m_startFlag._is_select = false;
}

void RouteDlg::OnBnClickedRoute()
{
	if (m_view->m_startFlag._linkid != 0 && m_view->m_endFlag._linkid != 0) {
		m_view->Route();
	}
}

void RouteDlg::init_listinfo()
{
  m_startList.InsertColumn( 0, _T("Field"),      LVCFMT_LEFT   | LVCF_SUBITEM, 40, 0);
  m_startList.InsertColumn( 1, _T("value"),      LVCFMT_LEFT   | LVCF_SUBITEM, 75, 0);
  m_endList.InsertColumn( 0, _T("Field"),      LVCFMT_LEFT   | LVCF_SUBITEM, 40, 0);
  m_endList.InsertColumn( 1, _T("value"),      LVCFMT_LEFT   | LVCF_SUBITEM, 75, 0);
  m_startList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
  m_endList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
}

void RouteDlg::set_listinfo()
{
  m_startList.DeleteAllItems();
  m_endList.DeleteAllItems();
  CString tmp;
  m_startList.InsertItem(0, _T("Linkid"));
  m_startList.InsertItem(1, _T("X"));
  m_startList.InsertItem(2, _T("Y"));

  tmp.Format(_T("%lu"), m_view->m_startFlag._linkid);
  m_startList.SetItemText(0, 1, tmp);
  tmp.Format(_T("%lf"), m_view->m_startFlag._pt.x);
  m_startList.SetItemText(1, 1, tmp);
  tmp.Format(_T("%lf"), m_view->m_startFlag._pt.y);
  m_startList.SetItemText(2, 1, tmp);

  m_startList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
  
  m_endList.InsertItem(0, _T("Linkid"));
  m_endList.InsertItem(1, _T("X"));
  m_endList.InsertItem(2, _T("Y"));

  tmp.Format(_T("%lu"), m_view->m_endFlag._linkid);
  m_endList.SetItemText(0, 1, tmp);
  tmp.Format(_T("%lf"), m_view->m_endFlag._pt.x);
  m_endList.SetItemText(1, 1, tmp);
  tmp.Format(_T("%lf"), m_view->m_endFlag._pt.y);
  m_endList.SetItemText(2, 1, tmp);

  m_endList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
  
  m_startList.SetRedraw(TRUE);
  m_endList.SetRedraw(TRUE);
}
