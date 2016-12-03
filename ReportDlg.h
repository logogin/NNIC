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
	void SetCompressionMethod(const CString &sMethod);
	void SetCompressionTime(const FLOAT fTime);
	void SetPSNR(const DOUBLE fRed,const DOUBLE fGreen, const DOUBLE fBlue, const DOUBLE fAverange, const DOUBLE fFull);
	void SetFileSize(const ULONGLONG ulOriginalSize,const ULONGLONG ulCompressedSize);
	void SetOriginalColors(const BYTE bBitsPerPixel,const ULONG ulColors);
	void SetImageSize(const ULONG ulWidth,const ULONG ulHeight);
	void SetDirectory(const CString &sPath);
	void SetFileName(const CString &sFileName);
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
	CString m_sFileName;
	CString m_sDirectory;
	CString m_sImageSize;
	CString m_sOriginalColors;
	CString m_sOriginalSize;
	CString m_sCompressedSize;
	CString m_sRatio;
	CString m_sCompressionMethod;
	CString m_sRed;
	CString m_sGreen;
	CString m_sBlue;
	CString m_sAverange;
	CString m_sFull;
	CString m_sCompressionTime;

	// Generated message map functions
	//{{AFX_MSG(CReportDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPORTDLG_H__AF9E34A5_A945_4D3F_9668_9C89ED93E4EE__INCLUDED_)
