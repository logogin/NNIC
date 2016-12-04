#if !defined(AFX_REPORTDLG_H__AF9E34A5_A945_4D3F_9668_9C89ED93E4EE__INCLUDED_)
#define AFX_REPORTDLG_H__AF9E34A5_A945_4D3F_9668_9C89ED93E4EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReportDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CReportDlg dialog

class CReportDlg : public CDialog
{
// Construction
public:
	void SetPSNR(const DOUBLE fFull);
	void SetComprMethod(const CString &strMethod);
	void SetComprTime(const FLOAT fTime);
	void SetPSNR(
		const DOUBLE fRed,
		const DOUBLE fGreen,
		const DOUBLE fBlue,
		const DOUBLE fAverage,
		const DOUBLE fFull);
	void SetFileSize(const ULONGLONG ulOriginalSize,const ULONGLONG ulComprSize);
	void SetOriginalColors(const BYTE bBitsPixel,const DWORD dwColors);
	void SetImageSize(const DWORD dwWidth,const DWORD dwHeight);
	void SetFileDirectory(const CString &strFileName,const CString &strPath);
	CReportDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CReportDlg)
	enum { IDD = IDD_DIALOG_REPORT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReportDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString m_strFileName;
	CString m_strDirectory;
	CString m_strImageSize;
	CString m_strOriginalColors;
	CString m_strOriginalSize;
	CString m_strComprSize;
	CString m_strRatio;
	CString m_strComprMethod;
	CString m_strRed;
	CString m_strGreen;
	CString m_strBlue;
	CString m_strAverage;
	CString m_strFull;
	CString m_strComprTime;
	CString m_strReportString;

	// Generated message map functions
	//{{AFX_MSG(CReportDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CString GetReportString(void);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPORTDLG_H__AF9E34A5_A945_4D3F_9668_9C89ED93E4EE__INCLUDED_)
