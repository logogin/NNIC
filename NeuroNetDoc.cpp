// NeuroNetDoc.cpp : implementation of the CNeuroNetDoc class
//

#include "stdafx.h"
#include "NeuroNet.h"

#include "NeuroNetDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNeuroNetDoc

IMPLEMENT_DYNCREATE(CNeuroNetDoc, CDocument)

BEGIN_MESSAGE_MAP(CNeuroNetDoc, CDocument)
	//{{AFX_MSG_MAP(CNeuroNetDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNeuroNetDoc construction/destruction

CNeuroNetDoc::CNeuroNetDoc()
{
	// TODO: add one-time construction code here

}

CNeuroNetDoc::~CNeuroNetDoc()
{
}

BOOL CNeuroNetDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	((CEditView*)m_viewList.GetHead())->SetWindowText(NULL);

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CNeuroNetDoc serialization

void CNeuroNetDoc::Serialize(CArchive& ar)
{
	// CEditView contains an edit control which handles all serialization
	((CEditView*)m_viewList.GetHead())->SerializeRaw(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CNeuroNetDoc diagnostics

#ifdef _DEBUG
void CNeuroNetDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CNeuroNetDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CNeuroNetDoc commands
