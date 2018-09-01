#pragma once
#include "SHPViewerView.h"
#include "afxwin.h"
#include "afxcmn.h"

// DataInfoDlg 대화 상자입니다.

class DataInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(DataInfoDlg)

public:
	DataInfoDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~DataInfoDlg();
	BOOL DataInfoDlg::Create(CWnd* pParent);
	enum { IDD = IDD_DATAINFO_DLG };

private :

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
  virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedLinkextract();
  afx_msg void OnBnClickedNodeextract();
  void init_listinfo();
  void set_link_listinfo();
  void set_node_listinfo();


public :
  CEdit m_linkid;
  CEdit m_nodeid;
  CListCtrl m_linkinfoList;
  CListCtrl m_nodeinfoList;
};
