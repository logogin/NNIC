#pragma once


// CFileBmpDlg

class CFileBmpDlg : public CFileDialog
{
	DECLARE_DYNAMIC(CFileBmpDlg)

public:
	CFileBmpDlg(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszFileName = NULL,
		CWnd* pParentWnd = NULL);
	virtual ~CFileBmpDlg();

protected:
	DECLARE_MESSAGE_MAP()
public:
	void SetTitle(const CString & strTitle);
protected:
	CString m_strTitle;
};


