// ReportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NNIC.h"
#include "ReportDlg.h"
#include ".\reportdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReportDlg dialog


CReportDlg::CReportDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReportDlg::IDD, pParent)
	, m_strReportString(_T(""))
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

	SetDlgItemText(IDC_EDIT_FILENAME,m_strFileName);
	SetDlgItemText(IDC_EDIT_DIRECTORY,m_strDirectory);
	SetDlgItemText(IDC_EDIT_IMAGESIZE,m_strImageSize);
	SetDlgItemText(IDC_EDIT_COLORS,m_strOriginalColors);
	SetDlgItemText(IDC_EDIT_METHOD,m_strComprMethod);
	SetDlgItemText(IDC_EDIT_ORIGINALSIZE,m_strOriginalSize);
	SetDlgItemText(IDC_EDIT_COMPRSIZE,m_strComprSize);
	SetDlgItemText(IDC_EDIT_RATIO,m_strRatio);
	SetDlgItemText(IDC_EDIT_RED,m_strRed);
	SetDlgItemText(IDC_EDIT_GREEN,m_strGreen);
	SetDlgItemText(IDC_EDIT_BLUE,m_strBlue);
	SetDlgItemText(IDC_EDIT_AVERAGE,m_strAverage);
	SetDlgItemText(IDC_EDIT_FULL,m_strFull);
	SetDlgItemText(IDC_EDIT_TIME,m_strComprTime);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CReportDlg::SetFileDirectory(const CString &strFileName,
								  const CString &strPath)
{
	m_strFileName=strFileName;
	m_strDirectory=strPath.Left(strPath.GetLength()-m_strFileName.GetLength());
}

void CReportDlg::SetFileSize(const ULONGLONG ulOriginalSize,const ULONGLONG ulComprSize)
{
	m_strRatio.Format(_T("%.2f"),(FLOAT)ulComprSize/ulOriginalSize*100.0);
	m_strRatio+=_T("%");
	m_strOriginalSize.Format(_T("%.2f KB (%d bytes)"),ulOriginalSize/1024.0,ulOriginalSize);
	m_strComprSize.Format(_T("%.2f KB (%d bytes)"),ulComprSize/1024.0,ulComprSize);
}

void CReportDlg::SetImageSize(const DWORD dwWidth, const DWORD dwHeight)
{
	m_strImageSize.Format(_T("%dx%d pixels"),dwWidth,dwHeight);
}

void CReportDlg::SetOriginalColors(const BYTE bBitsPixel,const DWORD dwColors)
{
	m_strOriginalColors.Format(_T("%d (%d bpp)"),dwColors,bBitsPixel);
}

void CReportDlg::SetPSNR(const DOUBLE fRed,
						 const DOUBLE fGreen,
						 const DOUBLE fBlue,
						 const DOUBLE fAverage,
						 const DOUBLE fFull)
{
	m_strRed.Format(_T("%.2f dB"),fRed);
	m_strGreen.Format(_T("%.2f dB"),fGreen);
	m_strBlue.Format(_T("%.2f dB"),fBlue);
	m_strAverage.Format(_T("%.2f dB"),fAverage);
	m_strFull.Format(_T("%.2f dB"),fFull);
}

void CReportDlg::SetComprTime(const FLOAT fTime)
{
	m_strComprTime.Format(_T("%.2f sec."),fTime);
}

void CReportDlg::SetComprMethod(const CString &strMethod)
{
	m_strComprMethod=strMethod;
}

void CReportDlg::SetPSNR(const DOUBLE fFull)
{
	m_strRed=m_strGreen=m_strBlue=m_strAverage=_T("N/A");
	m_strFull.Format(_T("%.2f dB"),fFull);
}

CString CReportDlg::GetReportString(void)
{
	m_strReportString=_T("");
	CString strString;
	strString.Format(_T("%-25s %s\n"),_T("File name: "),m_strFileName);
	m_strReportString+=strString;
	strString.Format(_T("%-25s %s\n"),_T("Directory: "),m_strDirectory);
	m_strReportString+=strString;
	strString.Format(_T("%-25s %s\n"),_T("Image size: "),m_strImageSize);
	m_strReportString+=strString;
	strString.Format(_T("%-25s %s\n"),_T("Original colors: "),m_strOriginalColors);
	m_strReportString+=strString;
	strString.Format(_T("%-25s %s\n"),_T("Compression method: "),m_strComprMethod);
	m_strReportString+=strString;
	strString.Format(_T("%-25s %s\n"),_T("Original file size: "),m_strOriginalSize);
	m_strReportString+=strString;
	strString.Format(_T("%-25s %s\n"),_T("Compressed file size: "),m_strComprSize);
	m_strReportString+=strString;
	strString.Format(_T("%-25s %s\n"),_T("Ratio: "),m_strRatio);
	m_strReportString+=strString;
	strString.Format(_T("%-25s %s\n"),_T("Red channel PSNR: "),m_strRed);
	m_strReportString+=strString;
	strString.Format(_T("%-25s %s\n"),_T("Green channel PSNR: "),m_strGreen);
	m_strReportString+=strString;
	strString.Format(_T("%-25s %s\n"),_T("Blue channel PSNR: "),m_strBlue);
	m_strReportString+=strString;
	strString.Format(_T("%-25s %s\n"),_T("Average PSNR: "),m_strAverage);
	m_strReportString+=strString;
	strString.Format(_T("%-25s %s\n"),_T("Full channel PSNR: "),m_strFull);
	m_strReportString+=strString;
	strString.Format(_T("%-25s %s\n"),_T("Compressed in: "),m_strComprTime);
	m_strReportString+=strString;
	return m_strReportString;
}
