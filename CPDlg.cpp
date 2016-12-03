// CPDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NNIC.h"
#include "CPDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCPDlg property page

IMPLEMENT_DYNCREATE(CCPDlg, CPropertyPage)

CCPDlg::CCPDlg() : CPropertyPage(CCPDlg::IDD)
{

}

CCPDlg::CCPDlg(const OPTIONSCP &optionsCP) : CPropertyPage(CCPDlg::IDD)
{
	//{{AFX_DATA_INIT(CCPDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	CopyMemory(&m_optionsCP,&optionsCP,sizeof(OPTIONSCP));
}

CCPDlg::~CCPDlg()
{
}

void CCPDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCPDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCPDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CCPDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCPDlg message handlers

BOOL CCPDlg::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetDlgItemInt(IDC_EDIT_CLUSTERS,m_optionsCP.st_wClusters);
	SetDlgItemInt(IDC_EDIT_CYCLES,m_optionsCP.st_ulCycles);

	CString sFloat;
	sFloat.Format(_T("%.4f"),m_optionsCP.st_fMinDist);
	SetDlgItemText(IDC_EDIT_MINDIST,sFloat);

	sFloat.Format(_T("%.4f"),m_optionsCP.st_fInitLearnRate);
	SetDlgItemText(IDC_EDIT_INITLEARNRATE,sFloat);
	sFloat.Format(_T("%.4f"),m_optionsCP.st_fFinalLearnRate);
	SetDlgItemText(IDC_EDIT_FINALLEARNRATE,sFloat);
	SetDlgItemInt(IDC_EDIT_LEARNEPOCHS,m_optionsCP.st_ulLearnRateEpochs);
	sFloat.Format(_T("%.4f"),m_optionsCP.st_fLearnChangeRate);
	SetDlgItemText(IDC_EDIT_LEARNCHANGERATE,sFloat);

	SetDlgItemInt(IDC_EDIT_INITNEIGHSIZE,m_optionsCP.st_wInitNeighSize);
	SetDlgItemInt(IDC_EDIT_FINALNEIGHSIZE,m_optionsCP.st_wFinalNeighSize);
	SetDlgItemInt(IDC_EDIT_NEIGHEPOCHS,m_optionsCP.st_ulNeighSizeEpochs);
	SetDlgItemInt(IDC_EDIT_NEIGHCHANGERATE,m_optionsCP.st_wNeighChangeRate);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCPDlg::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_optionsCP.st_wClusters=GetDlgItemInt(IDC_EDIT_CLUSTERS);
	m_optionsCP.st_ulCycles=GetDlgItemInt(IDC_EDIT_CYCLES);

	CString sFloat;
	GetDlgItemText(IDC_EDIT_MINDIST,sFloat);
	m_optionsCP.st_fMinDist=(FLOAT)atof(sFloat);

	GetDlgItemText(IDC_EDIT_INITLEARNRATE,sFloat);
	m_optionsCP.st_fInitLearnRate=(FLOAT)atof(sFloat);
	GetDlgItemText(IDC_EDIT_FINALLEARNRATE,sFloat);
	m_optionsCP.st_fFinalLearnRate=(FLOAT)atof(sFloat);
	m_optionsCP.st_ulLearnRateEpochs=GetDlgItemInt(IDC_EDIT_LEARNEPOCHS);
	GetDlgItemText(IDC_EDIT_LEARNCHANGERATE,sFloat);
	m_optionsCP.st_fLearnChangeRate=(FLOAT)atof(sFloat);

	m_optionsCP.st_wInitNeighSize=GetDlgItemInt(IDC_EDIT_INITNEIGHSIZE);
	m_optionsCP.st_wFinalNeighSize=GetDlgItemInt(IDC_EDIT_FINALNEIGHSIZE);
	m_optionsCP.st_ulNeighSizeEpochs=GetDlgItemInt(IDC_EDIT_NEIGHEPOCHS);
	m_optionsCP.st_wNeighChangeRate=GetDlgItemInt(IDC_EDIT_NEIGHCHANGERATE);

	CPropertyPage::OnOK();
}

void CCPDlg::GetOptionsCP(OPTIONSCP *optionsCP)
{
	CopyMemory(optionsCP,&m_optionsCP,sizeof(OPTIONSCP));
}
