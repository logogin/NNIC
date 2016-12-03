// ReportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NNIC.h"
#include "ReportDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReportDlg dialog


CReportDlg::CReportDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReportDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReportDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CReportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReportDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReportDlg, CDialog)
	//{{AFX_MSG_MAP(CReportDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReportDlg message handlers

BOOL CReportDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here

	SetDlgItemText(IDC_EDIT_FILENAME,m_sFileName);
	SetDlgItemText(IDC_EDIT_DIRECTORY,m_sDirectory);
	SetDlgItemText(IDC_EDIT_IMAGESIZE,m_sImageSize);
	SetDlgItemText(IDC_EDIT_COLORS,m_sOriginalColors);
	SetDlgItemText(IDC_EDIT_METHOD,m_sCompressionMethod);
	SetDlgItemText(IDC_EDIT_ORSIZE,m_sOriginalSize);
	SetDlgItemText(IDC_EDIT_COMPRSIZE,m_sCompressedSize);
	SetDlgItemText(IDC_EDIT_RATIO,m_sRatio);
	SetDlgItemText(IDC_EDIT_RED,m_sRed);
	SetDlgItemText(IDC_EDIT_GREEN,m_sGreen);
	SetDlgItemText(IDC_EDIT_BLUE,m_sBlue);
	SetDlgItemText(IDC_EDIT_AVERANGE,m_sAverange);
	SetDlgItemText(IDC_EDIT_FULL,m_sFull);
	SetDlgItemText(IDC_EDIT_TIME,m_sCompressionTime);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CReportDlg::SetFileName(const CString &sFileName)
{
	m_sFileName=sFileName;
}

void CReportDlg::SetDirectory(const CString &sPath)
{
	m_sDirectory=sPath.Left(sPath.GetLength()-m_sFileName.GetLength());
}

void CReportDlg::SetFileSize(const ULONG ulOriginalSize,const ULONG ulCompressedSize)
{
	m_sRatio.Format(_T("%.2f"),(FLOAT)ulCompressedSize/ulOriginalSize*100.0);
	m_sRatio+=_T("%");
	m_sOriginalSize.Format(_T("%.2f KB (%d bytes)"),ulOriginalSize/1024.0,ulOriginalSize);
	m_sCompressedSize.Format(_T("%.2f KB (%d bytes)"),ulCompressedSize/1024.0,ulCompressedSize);
}

void CReportDlg::SetImageSize(const ULONG ulWidth, const ULONG ulHeight)
{
	m_sImageSize.Format(_T("%dx%d pixels"),ulWidth,ulHeight);
}

void CReportDlg::SetOriginalColors(const BYTE bBitsPerPixel,const ULONG ulColors)
{
	m_sOriginalColors.Format(_T("%d (%d BitsPerPixel)"),ulColors,bBitsPerPixel);
}

void CReportDlg::SetPSNR(const DOUBLE fRed, const DOUBLE fGreen, const DOUBLE fBlue, const DOUBLE fAverange, const DOUBLE fFull)
{
	m_sRed.Format(_T("%.2f dB"),fRed);
	m_sGreen.Format(_T("%.2f dB"),fGreen);
	m_sBlue.Format(_T("%.2f dB"),fBlue);
	m_sAverange.Format(_T("%.2f dB"),fAverange);
	m_sFull.Format(_T("%.2f dB"),fFull);
}

void CReportDlg::SetCompressionTime(const FLOAT fTime)
{
	m_sCompressionTime.Format(_T("%.2f sec."),fTime);
}

void CReportDlg::SetCompressionMethod(const CString &sMethod)
{
	m_sCompressionMethod=sMethod;
}

void CReportDlg::SetPSNR(const DOUBLE fFull)
{
	m_sRed=m_sGreen=m_sBlue=m_sAverange=_T("N/A");
	m_sFull.Format(_T("%.2f dB"),fFull);
}
