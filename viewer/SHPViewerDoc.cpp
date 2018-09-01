// SHPViewerDoc.cpp : implementation of the CSHPViewerDoc class
//

#include "stdafx.h"
#include "SHPViewer.h"
#include "SHPViewerDoc.h"
#include "SHPViewerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSHPViewerDoc

IMPLEMENT_DYNCREATE(CSHPViewerDoc, CDocument)

BEGIN_MESSAGE_MAP(CSHPViewerDoc, CDocument)
	//{{AFX_MSG_MAP(CSHPViewerDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSHPViewerDoc construction/destruction

CSHPViewerDoc::CSHPViewerDoc()
{

}

CSHPViewerDoc::~CSHPViewerDoc()
{

}

BOOL CSHPViewerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CSHPViewerDoc serialization
void CSHPViewerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CSHPViewerDoc diagnostics
#ifdef _DEBUG
void CSHPViewerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSHPViewerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSHPViewerDoc commands
