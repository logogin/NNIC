// NeuroNet.h : main header file for the NEURONET application
//

#if !defined(AFX_NEURONET_H__38D6C6E2_F73E_4521_AE9D_58E9632A123D__INCLUDED_)
#define AFX_NEURONET_H__38D6C6E2_F73E_4521_AE9D_58E9632A123D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CNeuroNetApp:
// See NeuroNet.cpp for the implementation of this class
//

class CNeuroNetApp : public CWinApp
{
public:
	CNeuroNetApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNeuroNetApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CNeuroNetApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEURONET_H__38D6C6E2_F73E_4521_AE9D_58E9632A123D__INCLUDED_)
