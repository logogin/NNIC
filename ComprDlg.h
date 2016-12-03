#if !defined(AFX_COMPRDLG_H__ED665F02_745E_43E0_9FD1_D0B9D304E7A5__INCLUDED_)
#define AFX_COMPRDLG_H__ED665F02_745E_43E0_9FD1_D0B9D304E7A5__INCLUDED_

#include "Globals.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ComprDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CComprDlg dialog

class CComprDlg : public CDialog
{
	DECLARE_DYNAMIC(CComprDlg);
// Construction
public:
	void DisplayComprBitmap(const BYTE *pData);
	void SetFinished(const BOOL fFlag);
	void EnableOptions(const BOOL bFlag);
	INT StepProgress(void);
	void SetProgress(const UINT uiPos);
	void EnableStart(const BOOL bFlag);
	OPTIONSCP * GetOptionsCP(void);
	ImageKit * GetImageKit(void);
	CComprDlg(const OPTIONS &options,ImageKit *pImageKit,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CComprDlg)
	enum { IDD = IDD_DIALOG_COMPRESS };
	CButton	m_btnOptions;
	CProgressCtrl	m_prgCompress;
	CButton	m_btnStart;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComprDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL m_bFinished;
	CWinThread * m_trdCompress;
	ImageKit * m_pImageKit;
	OPTIONS m_Options;
	// Generated message map functions
	//{{AFX_MSG(CComprDlg)
	afx_msg void OnButtonStart();
	afx_msg void OnButtonOptions();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMPRDLG_H__ED665F02_745E_43E0_9FD1_D0B9D304E7A5__INCLUDED_)
