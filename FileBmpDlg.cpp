// FileBmpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NNIC.h"
#include "FileBmpDlg.h"
#include ".\filebmpdlg.h"


// CFileBmpDlg

IMPLEMENT_DYNAMIC(CFileBmpDlg, CFileDialog)
CFileBmpDlg::CFileBmpDlg(BOOL bOpenFileDialog,LPCTSTR lpszFileName,CWnd* pParentWnd)
	: CFileDialog(bOpenFileDialog,_T("bmp"),lpszFileName,
		OFN_HIDEREADONLY | OFN_DONTADDTORECENT | OFN_EXPLORER | OFN_PATHMUSTEXIST,
		_T("BMP - Windows Bitmap (*.bmp)|*.bmp|All Files (*.*)|*.*||"),
		pParentWnd,OPENFILENAME_SIZE_VERSION_400)
{
	if (!bOpenFileDialog)
		m_ofn.Flags |= OFN_OVERWRITEPROMPT;
	else
		m_ofn.Flags |= OFN_FILEMUSTEXIST;
}

CFileBmpDlg::~CFileBmpDlg()
{
}


BEGIN_MESSAGE_MAP(CFileBmpDlg, CFileDialog)
END_MESSAGE_MAP()



// CFileBmpDlg message handlers


void CFileBmpDlg::SetTitle(const CString & strTitle)
{
	m_strTitle=strTitle;
	m_ofn.lpstrTitle=m_strTitle;
}
