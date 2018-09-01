// DataInfoDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SHPViewer.h"
#include "DataInfoDlg.h"


// DataInfo 대화 상자입니다.

IMPLEMENT_DYNAMIC(DataInfoDlg, CDialog)

DataInfoDlg::DataInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(DataInfoDlg::IDD, pParent)
{

}

DataInfoDlg::~DataInfoDlg()
{
}

void DataInfoDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_LINKID, m_linkid);
  DDX_Control(pDX, IDC_NODEID, m_nodeid);
  DDX_Control(pDX, IDC_LINKINFO, m_linkinfoList);
  DDX_Control(pDX, IDC_NODEINFO, m_nodeinfoList);
}


BEGIN_MESSAGE_MAP(DataInfoDlg, CDialog)
  ON_BN_CLICKED(IDC_LINKEXTRACT, &DataInfoDlg::OnBnClickedLinkextract)
  ON_BN_CLICKED(IDC_NODEEXTRACT, &DataInfoDlg::OnBnClickedNodeextract)
END_MESSAGE_MAP()

BOOL DataInfoDlg::Create(CWnd* pParent) 
{
  return CDialog::Create(IDD, pParent);
}

BOOL DataInfoDlg::OnInitDialog()
{
  CDialog::OnInitDialog();
  init_listinfo();
  m_linkid.SetWindowTextA(_T("0"));
  m_nodeid.SetWindowTextA(_T("1"));
  set_link_listinfo();
  set_node_listinfo();
  return TRUE;
}

void DataInfoDlg::OnBnClickedLinkextract()
{
  set_link_listinfo();
}

void DataInfoDlg::OnBnClickedNodeextract()
{
  set_node_listinfo();
}

void DataInfoDlg::init_listinfo()
{
  m_linkinfoList.InsertColumn( 0, _T("Field"),      LVCFMT_LEFT   | LVCF_SUBITEM, 75, 0);
  m_nodeinfoList.InsertColumn( 1, _T("value"),      LVCFMT_LEFT   | LVCF_SUBITEM, 75, 0);
  m_linkinfoList.InsertColumn( 0, _T("Field"),      LVCFMT_LEFT   | LVCF_SUBITEM, 75, 0);
  m_nodeinfoList.InsertColumn( 1, _T("value"),      LVCFMT_LEFT   | LVCF_SUBITEM, 75, 0);
  m_linkinfoList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
  m_nodeinfoList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
}

void DataInfoDlg::set_node_listinfo()
{
  m_nodeinfoList.DeleteAllItems();
  CString tmp;
  nodeRecord noderec;
  unsigned long nodeid;

  m_nodeid.GetWindowTextA(tmp); 
  nodeid = strtoul(tmp.GetBuffer(), NULL, 10);

  if (Reader::instance()->_nodeRecord_map.find(nodeid) != Reader::instance()->_nodeRecord_map.end()) {
    noderec = Reader::instance()->_nodeRecord_map[nodeid];
    for (int i = 0; i < noderec._connect_link.size(); ++i) {
      tmp.Format(_T("LINKID %d"), i);
      m_nodeinfoList.InsertItem(i, tmp);
      tmp.Format(_T("%lu"), noderec._connect_link[i]);
      m_nodeinfoList.SetItemText(i, 1, tmp);
    }
  } else {
    m_nodeinfoList.InsertItem(0, _T("LINKID"));
    m_nodeinfoList.SetItemText(1, 1, _T("0"));
  }
}

void DataInfoDlg::set_link_listinfo()
{
  m_linkinfoList.DeleteAllItems();
  CString tmp;
  linkRecord linkrec;
  unsigned long linkid;

  m_linkid.GetWindowTextA(tmp);
  linkid = strtoul(tmp.GetBuffer(), NULL, 10);

  m_linkinfoList.InsertItem(0, _T("F_NODEID"));
  m_linkinfoList.InsertItem(1, _T("T_NODEID"));
  m_linkinfoList.InsertItem(2, _T("LENGTH"));

  if (Reader::instance()->_linkRecord_map.find(linkid) != Reader::instance()->_linkRecord_map.end()) {
    linkrec = Reader::instance()->_linkRecord_map[linkid];
    tmp.Format(_T("%lu"), linkrec._f_node);
    m_linkinfoList.SetItemText(0, 1, tmp);
    tmp.Format(_T("%lu"), linkrec._t_node);
    m_linkinfoList.SetItemText(1, 1, tmp);
    tmp.Format(_T("%lf"), linkrec._length);
    m_linkinfoList.SetItemText(2, 1, tmp);
  } else {
    m_linkinfoList.SetItemText(0, 1, _T("0"));
    m_linkinfoList.SetItemText(1, 1, _T("0"));
    m_linkinfoList.SetItemText(2, 1, _T("0"));
  }
}