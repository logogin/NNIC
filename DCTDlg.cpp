// DCTDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NNIC.h"
#include "DCTDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDCTDlg property page

IMPLEMENT_DYNCREATE(CDCTDlg, CPropertyPage)

CDCTDlg::CDCTDlg() : CPropertyPage(CDCTDlg::IDD)
{

}

CDCTDlg::CDCTDlg(const OPTIONSDCT &optionsDCT) : CPropertyPage(CDCTDlg::IDD)
{
	//{{AFX_DATA_INIT(CDCTDlg)
	m_bShift = FALSE;
	m_iQuality = 0;
	m_strQuality = _T("");
	//}}AFX_DATA_INIT
	CopyMemory(&m_optionsDCT,&optionsDCT,sizeof(OPTIONSDCT));
}

CDCTDlg::~CDCTDlg()
{
}

void CDCTDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDCTDlg)
	DDX_Check(pDX, IDC_CHECK_SHIFT, m_bShift);
	DDX_Slider(pDX, IDC_SLIDER_QUALITY, m_iQuality);
	DDX_Text(pDX, IDC_STATIC_QUALITY, m_strQuality);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDCTDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CDCTDlg)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDCTDlg message handlers

BOOL CDCTDlg::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	CSliderCtrl *pSlider=(CSliderCtrl *)GetDlgItem(IDC_SLIDER_QUALITY);
	pSlider->SetRange(0,25);
	m_iQuality=m_optionsDCT.st_bQuality;
	m_strQuality.Format(_T("%d"),m_optionsDCT.st_bQuality);
	m_bShift=m_optionsDCT.st_bShift;

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDCTDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	UpdateData(TRUE);

	m_optionsDCT.st_bQuality=m_iQuality;
	m_strQuality.Format(_T("%d"),m_optionsDCT.st_bQuality);

	UpdateData(FALSE);

	CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDCTDlg::GetOptionsDCT(OPTIONSDCT *optionsDCT)
{
	CopyMemory(optionsDCT,&m_optionsDCT,sizeof(OPTIONSDCT));
}

void CDCTDlg::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);

	m_optionsDCT.st_bQuality=m_iQuality;
	m_optionsDCT.st_bShift=m_bShift;

	CPropertyPage::OnOK();
}
