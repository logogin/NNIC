// NNICDlg.h : header file
//

#if !defined(AFX_NNICDLG_H__DD2B284F_84B4_4F8B_B6DB_342539641EB4__INCLUDED_)
#define AFX_NNICDLG_H__DD2B284F_84B4_4F8B_B6DB_342539641EB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CNNICDlg dialog
UINT StartBP(LPVOID pParam);
UINT StartCP(LPVOID pParam);
UINT StartDCT(LPVOID pParam);
UINT StartDCTBack(LPVOID pParam);


class CNNICDlg : public CDialog
{
	DECLARE_DYNAMIC(CNNICDlg);
// Construction
public:
	OPTIONSDCT * GetOptionsDCT(void);
	ULONGLONG GetLoadedFileSize(void);
	OPTIONSBP * GetOptionsBP(void);
	void SetFinished(const BOOLEAN bFlag);
	void DisplayRightBitmap(void);
	void DisplayLeftBitmap(void);
	CBitmapKit * GetLeftBitmap(void);
	CBitmapKit * GetRightBitmap(void);
	INT StepProgress(void);
	void SetProgress(const UINT uiPos);
	OPTIONSCP * GetOptionsCP(void);
	
	void EnableOptions(const BOOLEAN bFlag);
	void EnableStop(const BOOLEAN bFlag);
	void EnableCompress(const BOOLEAN bFlag);
	CNNICDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CNNICDlg)
	enum { IDD = IDD_NNIC_DIALOG };
	CStatic	m_staticRight;
	CStatic	m_staticLeft;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNNICDlg)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CWinThread * m_threadCompress;
	BOOLEAN m_bFinished;
	OPTIONS m_optionsDefault;
	CBitmapKit m_leftBitmap;
	CBitmapKit m_rightBitmap;
	CString m_strLoadedFileName;
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
public:
//	afx_msg void OnSize(UINT nType, int cx, int cy);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NNICDLG_H__DD2B284F_84B4_4F8B_B6DB_342539641EB4__INCLUDED_)
