#pragma once
#include "SHPViewerView.h"
#include "afxcmn.h"

// RouteDlg ��ȭ �����Դϴ�.

class RouteDlg : public CDialog
{
	DECLARE_DYNAMIC(RouteDlg)

public:
	RouteDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~RouteDlg();
	BOOL RouteDlg::Create(CWnd* pParent);
	enum { IDD = IDD_ROUTE_DLG };

private :
	CSHPViewerView* m_view;
// ��ȭ ���� �������Դϴ�.

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
  virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStartflag();
	afx_msg void OnBnClickedEndflag();
	afx_msg void OnBnClickedRoute();
  void init_listinfo();
  void set_listinfo();

public :
  CListCtrl m_startList;
  CListCtrl m_endList;
};
