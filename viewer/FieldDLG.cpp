// FieldDLG.cpp : implementation file
//

#include "stdafx.h"
#include "FieldDLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFieldDLG dialog


CFieldDLG::CFieldDLG(CWnd* pParent /*=NULL*/)
	: CDialog(CFieldDLG::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFieldDLG)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFieldDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFieldDLG)
	DDX_Control(pDX, 1020 /* IDC_LIST1 */, m_ctlList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFieldDLG, CDialog)
	//{{AFX_MSG_MAP(CFieldDLG)
	ON_LBN_SELCHANGE(1020 /* IDC_LIST1 */, OnSelchangeList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFieldDLG message handlers

BOOL CFieldDLG::OnInitDialog() 
{
	CDialog::OnInitDialog();

	for(int i = 0; i < m_vField.size(); ++i)
		m_ctlList.AddString(m_vField[i]);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFieldDLG::OnSelchangeList1() 
{
	m_nSelected = m_ctlList.GetCurSel();
}
