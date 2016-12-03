// NNICDlg.h : header file
//

#if !defined(AFX_NNICDLG_H__DD2B284F_84B4_4F8B_B6DB_342539641EB4__INCLUDED_)
#define AFX_NNICDLG_H__DD2B284F_84B4_4F8B_B6DB_342539641EB4__INCLUDED_

#include "IMAGEKIT\ImageKit.h"	// Added by ClassView
#include "Globals.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include ".\ImageKit\ImageKit.h"
/////////////////////////////////////////////////////////////////////////////
// CNNICDlg dialog
UINT StartBP(LPVOID pParam);
UINT StartCP(LPVOID pParam);
UINT StartDCT(LPVOID pParam);
UINT StartDCTBack(LPVOID pParam);

class ImageKit;

class CNNICDlg : public CDialog
{
	DECLARE_DYNAMIC(CNNICDlg);
// Construction
public:
	OPTIONSDCT * GetOptionsDCT(void);
	ULONG GetOriginalFileSize(void);
	OPTIONSBP * GetOptionsBP(void);
	void SetFinished(const BOOL bFlag);
	void DisplayComprBitmap(void);
	ImageKit * GetComprImage(void);
	INT StepProgress(void);
	void SetProgress(const UINT uiPos);
	OPTIONSCP * GetOptionsCP(void);
	ImageKit * GetOriginalImage(void);
	void EnableOptions(const BOOL bFlag);
	void EnableStop(const BOOL bFlag);
	void EnableCompress(const BOOL bFlag);
	CNNICDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CNNICDlg)
	enum { IDD = IDD_NNIC_DIALOG };
	CStatic	m_staticCompr;
	CStatic	m_staticOriginal;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNNICDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CWinThread * m_threadCompress;
	BOOL m_bFinished;
	OPTIONS m_optionsDefault;
	ImageKit m_OriginalImage;
	ImageKit m_ComprImage;
//	ImageKit m_Image;
	CString m_sOriginalFileName;
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CNNICDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnFileOpen();
	afx_msg void OnButtonCompress();
	afx_msg void OnButtonStop();
	afx_msg void OnButtonOptions();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NNICDLG_H__DD2B284F_84B4_4F8B_B6DB_342539641EB4__INCLUDED_)
