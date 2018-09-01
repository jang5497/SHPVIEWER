// SHPViewerDoc.h : interface of the CSHPViewerDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHPVIEWERDOC_H__07BFF746_602E_411A_8023_01BD640449A4__INCLUDED_)
#define AFX_SHPVIEWERDOC_H__07BFF746_602E_411A_8023_01BD640449A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSHPViewerDoc : public CDocument
{
protected: // create from serialization only
	CSHPViewerDoc();
	DECLARE_DYNCREATE(CSHPViewerDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSHPViewerDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSHPViewerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSHPViewerDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHPVIEWERDOC_H__07BFF746_602E_411A_8023_01BD640449A4__INCLUDED_)
