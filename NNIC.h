// NNIC.h : main header file for the NNIC application
//

#if !defined(AFX_NNIC_H__85B8E1FE_25E5_4386_A74D_B95C368ED0B2__INCLUDED_)
#define AFX_NNIC_H__85B8E1FE_25E5_4386_A74D_B95C368ED0B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CNNICApp:
// See NNIC.cpp for the implementation of this class
//

class CNNICApp : public CWinApp
{
public:
	CNNICApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNNICApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CNNICApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NNIC_H__85B8E1FE_25E5_4386_A74D_B95C368ED0B2__INCLUDED_)
