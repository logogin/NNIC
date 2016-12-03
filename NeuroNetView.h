// NeuroNetView.h : interface of the CNeuroNetView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEURONETVIEW_H__4CE1DF26_016C_4444_B715_B75DBF56E470__INCLUDED_)
#define AFX_NEURONETVIEW_H__4CE1DF26_016C_4444_B715_B75DBF56E470__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CNeuroNetView : public CEditView
{
protected: // create from serialization only
	CNeuroNetView();
	DECLARE_DYNCREATE(CNeuroNetView)

// Attributes
public:
	CNeuroNetDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNeuroNetView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNeuroNetView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//UINT StartBP(LPVOID pParam);
	//{{AFX_MSG(CNeuroNetView)
	afx_msg void OnStartNetcp();
	afx_msg void OnStartNetbp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in NeuroNetView.cpp
inline CNeuroNetDoc* CNeuroNetView::GetDocument()
   { return (CNeuroNetDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEURONETVIEW_H__4CE1DF26_016C_4444_B715_B75DBF56E470__INCLUDED_)
