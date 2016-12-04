// FileNniDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NNIC.h"
#include "FileNniDlg.h"
#include ".\filennidlg.h"


// CFileNniDlg

IMPLEMENT_DYNAMIC(CFileNniDlg, CFileDialog)
CFileNniDlg::CFileNniDlg(BOOL bOpenFileDialog, LPCTSTR lpszFileName, CWnd* pParentWnd)
	: CFileDialog(bOpenFileDialog, _T("nni"), lpszFileName,
		OFN_HIDEREADONLY | OFN_DONTADDTORECENT | OFN_EXPLORER | OFN_PATHMUSTEXIST,
		_T("NNI - NNIC Compressed Image (*.nni)|*.nni|All Files (*.*)|*.*||"),
		pParentWnd, OPENFILENAME_SIZE_VERSION_400)
{
	if (!bOpenFileDialog)
		m_ofn.Flags |= OFN_OVERWRITEPROMPT;
	else
		m_ofn.Flags |= OFN_FILEMUSTEXIST;
}

CFileNniDlg::~CFileNniDlg()
{
}


BEGIN_MESSAGE_MAP(CFileNniDlg, CFileDialog)
END_MESSAGE_MAP()



// CFileNniDlg message handlers


void CFileNniDlg::SetTitle(const CString & strTitle)
{
	m_strTitle=strTitle;
	m_ofn.lpstrTitle=m_strTitle;
}
