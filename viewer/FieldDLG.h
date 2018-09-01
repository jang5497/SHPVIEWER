#if !defined(AFX_FIELDDLG_H__807653F7_40AC_4ED5_8988_3E825AB4D702__INCLUDED_)
#define AFX_FIELDDLG_H__807653F7_40AC_4ED5_8988_3E825AB4D702__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FieldDLG.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFieldDLG dialog

class CFieldDLG : public CDialog
{
// Construction
public:
	CFieldDLG(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFieldDLG)
	enum { IDD = 169 /* IDD_FIELD_DLG */ };
	CListBox	m_ctlList;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFieldDLG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	vector<CString>	m_vField;
	int m_nSelected;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFieldDLG)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeList1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FIELDDLG_H__807653F7_40AC_4ED5_8988_3E825AB4D702__INCLUDED_)
