// SHPViewer.h : main header file for the SHPVIEWER application
//

#if !defined(AFX_SHPVIEWER_H__FE8A1436_D197_4ECA_A331_74805D4833EB__INCLUDED_)
#define AFX_SHPVIEWER_H__FE8A1436_D197_4ECA_A331_74805D4833EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CSHPViewerApp:
// See SHPViewer.cpp for the implementation of this class
//

class CSHPViewerApp : public CWinApp
{
public:
	CSHPViewerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSHPViewerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CSHPViewerApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHPVIEWER_H__FE8A1436_D197_4ECA_A331_74805D4833EB__INCLUDED_)
