#pragma once


// CFileNniDlg

class CFileNniDlg : public CFileDialog
{
	DECLARE_DYNAMIC(CFileNniDlg)

public:
	CFileNniDlg(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszFileName = NULL,
		CWnd* pParentWnd = NULL);
	virtual ~CFileNniDlg();

protected:
	DECLARE_MESSAGE_MAP()
public:
	void SetTitle(const CString & strTitle);
protected:
	CString m_strTitle;
};


