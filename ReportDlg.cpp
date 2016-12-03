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

	CString s;
	s.Format("%f dB",m_r);
	SetDlgItemText(IDC_EDIT_RED,s);
	s.Format("%f dB",m_g);
	SetDlgItemText(IDC_EDIT_GREEN,s);
	s.Format("%f dB",m_b);
	SetDlgItemText(IDC_EDIT_BLUE,s);
	s.Format("%f dB",m_a);
	SetDlgItemText(IDC_EDIT_AVERANGE,s);
	s.Format("%f dB",m_f);
	SetDlgItemText(IDC_EDIT_FULL,s);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CReportDlg::Set(double r, double g, double b, double a, double f)
{
	m_r=r;
	m_g=g;
	m_b=b;
	m_a=a;
	m_f=f;
}
