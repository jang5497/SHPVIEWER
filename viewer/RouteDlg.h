#pragma once
#include "SHPViewerView.h"
#include "afxcmn.h"

// RouteDlg 대화 상자입니다.

class RouteDlg : public CDialog
{
	DECLARE_DYNAMIC(RouteDlg)

public:
	RouteDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~RouteDlg();
	BOOL RouteDlg::Create(CWnd* pParent);
	enum { IDD = IDD_ROUTE_DLG };

private :
	CSHPViewerView* m_view;
// 대화 상자 데이터입니다.

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
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
