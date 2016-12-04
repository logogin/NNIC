#pragma once


// CCompareDlg dialog

class CCompareDlg : public CDialog
{
	DECLARE_DYNAMIC(CCompareDlg)

public:
	CCompareDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCompareDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_COMPARE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonFirstLoad();
	afx_msg void OnBnClickedButtonSecondLoad();
protected:
	CString m_strFirstPath;
	CString m_strSecondPath;
	CString m_strRed;
	CString m_strGreen;
	CString m_strBlue;
	CString m_strAverage;
	CString m_strFull;
public:
	afx_msg void OnBnClickedButtonCompare();
};
