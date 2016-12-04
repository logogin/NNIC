#pragma once


// CGeneralDlg dialog

class CGeneralDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CGeneralDlg)

public:
	CGeneralDlg();
	CGeneralDlg(const OPTIONSGENERAL &optionsGENERAL);
	virtual ~CGeneralDlg();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_GENERAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
	CString m_strDefComprFileName;
	BOOL m_bPromtOverwrite;
	BOOL m_bShowReport;
	BOOL m_bWriteReport;
	CString m_strReportFileName;
	BOOL m_bLimitFileSize;
	DWORD m_wReportFileSize;
	BYTE m_bStoringMethod;
public:
	afx_msg void OnBnClickedRadioAppend();
	afx_msg void OnBnClickedRadioOverwrite();
	afx_msg void OnBnClickedButtonLoadDeafults();
	afx_msg void OnBnClickedButtonSaveDefaults();
	afx_msg void OnBnClickedCheckWriteReport();
	afx_msg void OnBnClickedCheckLimitFileSize();
protected:
	// General Options Structure
	OPTIONSGENERAL m_optionsGENERAL;
	void EnableWriteReport(const BOOLEAN bFlag);
	void EnableLimitFileSize(const BOOLEAN bFlag);
public:
	void GetOptionsGENERAL(OPTIONSGENERAL * optionsGENERAL);
};
