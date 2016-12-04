// CompareDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NNIC.h"
#include "FileBmpDlg.h"
#include "CompareDlg.h"

// CCompareDlg dialog

IMPLEMENT_DYNAMIC(CCompareDlg, CDialog)
CCompareDlg::CCompareDlg(CWnd* pParent /*=NULL*/)
: CDialog(CCompareDlg::IDD, pParent)
, m_strFirstPath(_T(""))
, m_strSecondPath(_T(""))
, m_strRed(_T(""))
, m_strGreen(_T(""))
, m_strAverage(_T(""))
, m_strFull(_T(""))
{
}

CCompareDlg::~CCompareDlg()
{
}

void CCompareDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FIRSTPATH, m_strFirstPath);
	DDX_Text(pDX, IDC_EDIT_SECONDPATH, m_strSecondPath);
	DDX_Text(pDX, IDC_EDIT_RED, m_strRed);
	DDX_Text(pDX, IDC_EDIT_GREEN, m_strGreen);
	DDX_Text(pDX, IDC_EDIT_BLUE, m_strBlue);
	DDX_Text(pDX, IDC_EDIT_AVERAGE, m_strAverage);
	DDX_Text(pDX, IDC_EDIT_FULL, m_strFull);
}


BEGIN_MESSAGE_MAP(CCompareDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_FIRSTLOAD, OnBnClickedButtonFirstLoad)
	ON_BN_CLICKED(IDC_BUTTON_SECONDLOAD, OnBnClickedButtonSecondLoad)
	ON_BN_CLICKED(IDC_BUTTON_COMPARE, OnBnClickedButtonCompare)
END_MESSAGE_MAP()


// CCompareDlg message handlers

void CCompareDlg::OnBnClickedButtonFirstLoad()
{
	// TODO: Add your control notification handler code here
	CFileBmpDlg dlgFile(TRUE);
	dlgFile.SetTitle(_T("Load Bitmap File"));
	if (dlgFile.DoModal()==IDOK)
	{
		m_strFirstPath=dlgFile.GetPathName();
		UpdateData(FALSE);
	}
}

void CCompareDlg::OnBnClickedButtonSecondLoad()
{
	// TODO: Add your control notification handler code here
	CFileBmpDlg dlgFile(TRUE);
	dlgFile.SetTitle(_T("Load Bitmap File"));
	if (dlgFile.DoModal()==IDOK)
	{
		m_strSecondPath=dlgFile.GetPathName();
		UpdateData(FALSE);
	}
}

void CCompareDlg::OnBnClickedButtonCompare()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_strFirstPath.IsEmpty()||m_strSecondPath.IsEmpty())
	{
		AfxMessageBox(_T("Please, select bitmaps for comparison."),MB_OK);
		return;
	}
	CBitmapKit bitmapFirst;
	bitmapFirst.LoadBMP(m_strFirstPath);
	CBitmapKit bitmapSecond;
	bitmapSecond.LoadBMP(m_strSecondPath);
	if (bitmapFirst.GetBitsPixel()!=bitmapSecond.GetBitsPixel())
	{
		AfxMessageBox(_T("Bitmaps have different color depth!"),MB_OK);
		return;
	}
	if (bitmapFirst.GetHeight()!=bitmapSecond.GetHeight()||
		bitmapFirst.GetWidth()!=bitmapSecond.GetWidth())
	{
		AfxMessageBox(_T("Bitmaps have different dimensions!"),MB_OK);
		return;
	}
	DOUBLE fPSNR;
	if (bitmapFirst.GetBitsPixel()<24)
	{
		m_strRed=_T("N/A");
		m_strGreen=_T("N/A");
		m_strBlue=_T("N/A");
		m_strAverage=_T("N/A");
		fPSNR=bitmapFirst.GetPSNRFullChannel(&bitmapSecond);
		if (_finite(fPSNR))
			m_strFull.Format(_T("%.2f dB"),fPSNR);
		else
			m_strFull=_T("Inf.");
	}
	else
	{
		DOUBLE fSum=0.0;
		fPSNR=bitmapFirst.GetPSNR(&bitmapSecond,0);
		fSum+=fPSNR;
		if (_finite(fPSNR))
			m_strRed.Format(_T("%.2f dB"),fPSNR);
		else
			m_strRed=_T("Inf.");
		fPSNR=bitmapFirst.GetPSNR(&bitmapSecond,1);
		fSum+=fPSNR;
		if (_finite(fPSNR))
			m_strGreen.Format(_T("%.2f dB"),fPSNR);
		else
			m_strGreen=_T("Inf.");
		fPSNR=bitmapFirst.GetPSNR(&bitmapSecond,2);
		fSum+=fPSNR;
		if (_finite(fPSNR))
			m_strBlue.Format(_T("%.2f dB"),fPSNR);
		else
			m_strBlue=_T("Inf.");
		fPSNR=fSum/3;
		if (_finite(fPSNR))
			m_strAverage.Format(_T("%.2f dB"),fPSNR);
		else
			m_strAverage=_T("Inf.");
		fPSNR=bitmapFirst.GetPSNRFullChannel(&bitmapSecond);
		if (_finite(fPSNR))
			m_strFull.Format(_T("%.2f dB"),fPSNR);
		else
			m_strFull=_T("Inf.");
	}
	UpdateData(FALSE);
}
