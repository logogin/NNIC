// NeuroNetDoc.h : interface of the CNeuroNetDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEURONETDOC_H__24EE2EEF_C19A_42A6_9458_D551C0F75EC1__INCLUDED_)
#define AFX_NEURONETDOC_H__24EE2EEF_C19A_42A6_9458_D551C0F75EC1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CNeuroNetDoc : public CDocument
{
protected: // create from serialization only
	CNeuroNetDoc();
	DECLARE_DYNCREATE(CNeuroNetDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNeuroNetDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNeuroNetDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CNeuroNetDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEURONETDOC_H__24EE2EEF_C19A_42A6_9458_D551C0F75EC1__INCLUDED_)
