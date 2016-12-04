// CPDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NNIC.h"
#include "CPDlg.h"
#include ".\NeuroNet\NetCP.h"
#include ".\cpdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCPDlg property page

IMPLEMENT_DYNCREATE(CCPDlg, CPropertyPage)

CCPDlg::CCPDlg() : CPropertyPage(CCPDlg::IDD)
, m_bNetType(0)
{

}

CCPDlg::CCPDlg(OPTIONSCP *optionsCP) : CPropertyPage(CCPDlg::IDD)
{
	//{{AFX_DATA_INIT(CCPDlg)
	m_bTrainNeighbours = FALSE;
	m_dwCycles = 0;
	m_wClusters = 0;
	m_wNeighInit = 0;
	m_wNeighFinal = 0;
	m_wNeighChange = 0;
	m_fMinDist = 0.0f;
	m_fLearnInit = 0.0f;
	m_fLearnFinal = 0.0f;
	m_fLearnChange = 0.0f;
	m_dwLearnSteps = 0;
	m_dwNeighSteps = 0;
	//}}AFX_DATA_INIT
	m_optionsCP=optionsCP;
}

CCPDlg::~CCPDlg()
{
}

void CCPDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCPDlg)
	DDX_Check(pDX, IDC_CHECK_TRAINNEIGH, m_bTrainNeighbours);
	DDX_Text(pDX, IDC_EDIT_CYCLES, m_dwCycles);
	DDX_Text(pDX, IDC_EDIT_CLUSTERS, m_wClusters);
	DDV_MinMaxDWord(pDX, m_wClusters, 0, 65535);
	DDX_Text(pDX, IDC_EDIT_NEIGHINIT, m_wNeighInit);
	DDV_MinMaxDWord(pDX, m_wNeighInit, 0, 65535);
	DDX_Text(pDX, IDC_EDIT_NEIGHFINAL, m_wNeighFinal);
	DDV_MinMaxDWord(pDX, m_wNeighFinal, 0, 65535);
	DDX_Text(pDX, IDC_EDIT_NEIGHCHANGE, m_wNeighChange);
	DDV_MinMaxDWord(pDX, m_wNeighChange, 0, 65535);
	DDX_Text(pDX, IDC_EDIT_MINDIST, m_fMinDist);
	DDV_MinMaxFloat(pDX, m_fMinDist, 0.f, 1.f);
	DDX_Text(pDX, IDC_EDIT_LEARNINIT, m_fLearnInit);
	DDV_MinMaxFloat(pDX, m_fLearnInit, 0.f, 1.f);
	DDX_Text(pDX, IDC_EDIT_LEARNFINAL, m_fLearnFinal);
	DDV_MinMaxFloat(pDX, m_fLearnFinal, 0.f, 1.f);
	DDX_Text(pDX, IDC_EDIT_LEARNCHANGE, m_fLearnChange);
	DDV_MinMaxFloat(pDX, m_fLearnChange, 0.f, 1.f);
	DDX_Text(pDX, IDC_EDIT_LEARNSTEPS, m_dwLearnSteps);
	DDX_Text(pDX, IDC_EDIT_NEIGHSTEPS, m_dwNeighSteps);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCPDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CCPDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK_TRAINNEIGH, OnBnClickedCheckTrainNeigh)
	ON_BN_CLICKED(IDC_RADIO_KOHGROS, OnBnClickedRadioKohgros)
	ON_BN_CLICKED(IDC_RADIO_KOHONEN, OnBnClickedRadioKohonen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCPDlg message handlers

BOOL CCPDlg::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	switch (m_optionsCP->st_bNetType)
	{
	case NET_TYPE_KOHONEN_GROSSBERG:
		CheckRadioButton(IDC_RADIO_KOHGROS,IDC_RADIO_KOHONEN,IDC_RADIO_KOHGROS);
		EnableNeighRadiusChange(m_optionsCP->st_bTrainNeighbours);
		break;
	case NET_TYPE_KOHONEN:
		CheckRadioButton(IDC_RADIO_KOHGROS,IDC_RADIO_KOHONEN,IDC_RADIO_KOHONEN);
		EnableLearnRateChange(FALSE);
		EnableNeighRadiusChange(FALSE);
		break;
	}
	m_bNetType=m_optionsCP->st_bNetType;
	m_dwCycles=m_optionsCP->st_dwCycles;
	m_wClusters=m_optionsCP->st_wClusters;
	m_fMinDist=m_optionsCP->st_fMinDist;
	m_bTrainNeighbours=m_optionsCP->st_bTrainNeighbours;
	m_fLearnInit=m_optionsCP->st_fInitLearnRate;
	m_fLearnFinal=m_optionsCP->st_fFinalLearnRate;
	m_fLearnChange=m_optionsCP->st_fLearnChangeRate;
	m_dwLearnSteps=m_optionsCP->st_dwLearnRateSteps;
	m_wNeighInit=m_optionsCP->st_wInitNeighRadius;
	m_wNeighFinal=m_optionsCP->st_wFinalNeighRadius;
	m_wNeighChange=m_optionsCP->st_wNeighChangeRate;
	m_dwNeighSteps=m_optionsCP->st_dwNeighRadiusSteps;

	UpdateData(FALSE);
	EnableNeighRadius(m_bTrainNeighbours);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCPDlg::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);
	m_optionsCP->st_bNetType=m_bNetType;
	m_optionsCP->st_dwCycles=m_dwCycles;
	m_optionsCP->st_wClusters=(WORD)m_wClusters;
	m_optionsCP->st_fMinDist=m_fMinDist;
	m_optionsCP->st_bTrainNeighbours=m_bTrainNeighbours;
	m_optionsCP->st_fInitLearnRate=m_fLearnInit;
	m_optionsCP->st_fFinalLearnRate=m_fLearnFinal;
	m_optionsCP->st_fLearnChangeRate=m_fLearnChange;
	m_optionsCP->st_dwLearnRateSteps=m_dwLearnSteps;
	m_optionsCP->st_wInitNeighRadius=(WORD)m_wNeighInit;
	m_optionsCP->st_wFinalNeighRadius=(WORD)m_wNeighFinal;
	m_optionsCP->st_wNeighChangeRate=(WORD)m_wNeighChange;
	m_optionsCP->st_dwNeighRadiusSteps=m_dwNeighSteps;

	CPropertyPage::OnOK();
}

void CCPDlg::EnableLearnRateChange(const BOOLEAN bFlag)
{
	GetDlgItem(IDC_EDIT_MINDIST)->EnableWindow(bFlag);
	GetDlgItem(IDC_EDIT_LEARNCHANGE)->EnableWindow(bFlag);
	GetDlgItem(IDC_EDIT_LEARNSTEPS)->EnableWindow(bFlag);
}

void CCPDlg::EnableNeighRadiusChange(const BOOLEAN bFlag)
{
	GetDlgItem(IDC_EDIT_NEIGHCHANGE)->EnableWindow(bFlag);
	GetDlgItem(IDC_EDIT_NEIGHSTEPS)->EnableWindow(bFlag);
}

void CCPDlg::EnableNeighRadius(const BOOLEAN bFlag)
{
	GetDlgItem(IDC_EDIT_NEIGHINIT)->EnableWindow(bFlag);
	GetDlgItem(IDC_EDIT_NEIGHFINAL)->EnableWindow(bFlag);
	BOOLEAN bState=((CButton *)GetDlgItem(IDC_RADIO_KOHGROS))->GetCheck();
	EnableNeighRadiusChange(bState & bFlag);
}

void CCPDlg::OnBnClickedCheckTrainNeigh()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	EnableNeighRadius(m_bTrainNeighbours);
}

void CCPDlg::OnBnClickedRadioKohgros()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_bNetType=NET_TYPE_KOHONEN_GROSSBERG;
	EnableLearnRateChange(TRUE);
	EnableNeighRadiusChange(m_bTrainNeighbours);
}

void CCPDlg::OnBnClickedRadioKohonen()
{
	// TODO: Add your control notification handler code here
	m_bNetType=NET_TYPE_KOHONEN;
	EnableLearnRateChange(FALSE);
	EnableNeighRadiusChange(FALSE);
}
