// NNICDlg.h : header file
//

#if !defined(AFX_NNICDLG_H__DD2B284F_84B4_4F8B_B6DB_342539641EB4__INCLUDED_)
#define AFX_NNICDLG_H__DD2B284F_84B4_4F8B_B6DB_342539641EB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CNNICDlg dialog

const DWORD StatusID[5]={IDC_STATIC_STATUS1,
	IDC_STATIC_STATUS2,
	IDC_STATIC_STATUS3,
	IDC_STATIC_STATUS4,
	IDC_STATIC_STATUS5};

const DWORD StatusTextID[5]={IDC_STATIC_STATUSTEXT1,
	IDC_STATIC_STATUSTEXT2,
	IDC_STATIC_STATUSTEXT3,
	IDC_STATIC_STATUSTEXT4,
	IDC_STATIC_STATUSTEXT5};

UINT StartBP(LPVOID pParam);
UINT StartCP(LPVOID pParam);
UINT StartDCT(LPVOID pParam);
UINT StartDCTBack(LPVOID pParam);
UINT StartDeCompress(LPVOID pParam);

#define PAGE_GENERAL 0
#define PAGE_BP 1
#define PAGE_CP 2
#define PAGE_DCT 3

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
	CWinThread * m_threadDeCompress;
	BOOLEAN m_bFinished;
	OPTIONS m_optionsDefault;
	CBitmapKit m_bitmapRight;
	CBitmapKit m_bitmapLeft;
	CString m_strLoadedFileName;
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CNNICDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnFileLoadBitmapFile();
	afx_msg void OnFileLoadComprFile();
	afx_msg void OnButtonCompress();
	afx_msg void OnButtonStop();
	afx_msg void OnButtonOptions();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CString GetLoadedFileName(void);
	afx_msg void OnOptionsBP();
	void ShowOptions(const BYTE bActivePage);
	afx_msg void OnOptionsCP();
	afx_msg void OnOptionsDCT();
	afx_msg void OnHelpAbout();
	afx_msg void OnFileSaveLeftBitmap();
	afx_msg void OnOptionsGENERAL();
	OPTIONSGENERAL * GetOptionsGENERAL(void);
	afx_msg void OnFileSaveRightBitmap();
	CString GetComprFileName(void);
	void WriteReportFile(const CString & strReportString);
	void SetStatusOperation(const CString & strOperation);
	void SetStatus(const DWORD dwID, const CString & strName, const CString & strValue);
	void ClearStatus(void);
	void SetProgressRange(const INT iLower, const INT iUpper);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NNICDLG_H__DD2B284F_84B4_4F8B_B6DB_342539641EB4__INCLUDED_)
