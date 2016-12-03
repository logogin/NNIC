#if !defined(AFX_CPDLG_H__2D2186EC_F233_4DDB_BE96_CF000B1F6B65__INCLUDED_)
#define AFX_CPDLG_H__2D2186EC_F233_4DDB_BE96_CF000B1F6B65__INCLUDED_

#include "Globals.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CPDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCPDlg dialog

class CCPDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CCPDlg)

// Construction
public:
	void GetOptionsCP(OPTIONSCP *optionsCP);
	CCPDlg(void);
	CCPDlg(const OPTIONSCP &optionsCP);
	~CCPDlg();

// Dialog Data
	//{{AFX_DATA(CCPDlg)
	enum { IDD = IDD_PROPPAGE_COUNTER };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCPDlg)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	OPTIONSCP m_optionsCP;
	// Generated message map functions
	//{{AFX_MSG(CCPDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CPDLG_H__2D2186EC_F233_4DDB_BE96_CF000B1F6B65__INCLUDED_)
