// BPDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NNIC.h"
#include "BPDlg.h"
#include ".\bpdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBPDlg property page

IMPLEMENT_DYNCREATE(CBPDlg, CPropertyPage)

CBPDlg::CBPDlg() : CPropertyPage(CBPDlg::IDD)
{

}

CBPDlg::CBPDlg(const OPTIONSBP &optionsBP) : CPropertyPage(CBPDlg::IDD)
{
	//{{AFX_DATA_INIT(CBPDlg)
	m_iInput=0;
	m_iHidden=0;
	m_fAlpha = 0.0f;
	m_fFinalLearnRate = 0.0f;
	m_fFinalMoment = 0.0f;
	m_fInitLearnRate = 0.0f;
	m_fInitMoment = 0.0f;
	m_fLearnChangeRate = 0.0f;
	m_dwLearnSteps = 0;
	m_fMinError = 0.0f;
	m_fMomentChangeRate = 0.0f;
	m_dwMomentSteps = 0;
	m_strHidden = _T("");
	m_strInput = _T("");
	m_strRatio = _T("");
	m_bBiases = FALSE;
	m_bMomentum = FALSE;
	m_bSecond = FALSE;
	m_dwNeurons = 0;
	m_dwPatterns = 0;
	m_dwCycles = 0;
	//}}AFX_DATA_INIT
	CopyMemory(&m_optionsBP,&optionsBP,sizeof(OPTIONSBP));
}

CBPDlg::~CBPDlg()
{
}

void CBPDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBPDlg)
	DDX_Text(pDX, IDC_EDIT_ALPHA, m_fAlpha);
	DDX_Text(pDX, IDC_EDIT_FINALLEARNRATE, m_fFinalLearnRate);
	DDV_MinMaxFloat(pDX, m_fFinalLearnRate, 0.f, 1.f);
	DDX_Text(pDX, IDC_EDIT_FINALMOMENT, m_fFinalMoment);
	DDV_MinMaxFloat(pDX, m_fFinalMoment, 0.f, 1.f);
	DDX_Text(pDX, IDC_EDIT_INITLEARNRATE, m_fInitLearnRate);
	DDV_MinMaxFloat(pDX, m_fInitLearnRate, 0.f, 1.f);
	DDX_Text(pDX, IDC_EDIT_INITMOMENT, m_fInitMoment);
	DDV_MinMaxFloat(pDX, m_fInitMoment, 0.f, 1.f);
	DDX_Text(pDX, IDC_EDIT_LEARNCHANGERATE, m_fLearnChangeRate);
	DDX_Text(pDX, IDC_EDIT_LEARNSTEPS, m_dwLearnSteps);
	DDX_Text(pDX, IDC_EDIT_MINERROR, m_fMinError);
	DDV_MinMaxFloat(pDX, m_fMinError, 0.f, 1.f);
	DDX_Text(pDX, IDC_EDIT_MOMENTCHANGERATE, m_fMomentChangeRate);
	DDV_MinMaxFloat(pDX, m_fMomentChangeRate, 0.f, 1.f);
	DDX_Text(pDX, IDC_EDIT_MOMENTSTEPS, m_dwMomentSteps);
	DDX_Text(pDX, IDC_STATIC_HIDDEN, m_strHidden);
	DDX_Text(pDX, IDC_STATIC_INPUT, m_strInput);
	DDX_Text(pDX, IDC_STATIC_RATIO, m_strRatio);
	DDX_Check(pDX, IDC_CHECK_BIASES, m_bBiases);
	DDX_Check(pDX, IDC_CHECK_MOMENTUM, m_bMomentum);
	DDX_Check(pDX, IDC_CHECK_SECOND, m_bSecond);
	DDX_Text(pDX, IDC_EDIT_NEURONS, m_dwNeurons);
	DDV_MinMaxDWord(pDX, m_dwNeurons, 1, 65535);
	DDX_Text(pDX, IDC_EDIT_PATTERNS, m_dwPatterns);
	DDV_MinMaxDWord(pDX, m_dwPatterns, 1, 65535);
	DDX_Text(pDX, IDC_EDIT_CYCLES, m_dwCycles);
	DDX_Scroll(pDX, IDC_SCROLLBAR_INPUT, m_iInput);
	DDX_Scroll(pDX, IDC_SCROLLBAR_HIDDEN, m_iHidden);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBPDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CBPDlg)
	ON_BN_CLICKED(IDC_RADIO_BATCH, OnRadioBatch)
	ON_BN_CLICKED(IDC_RADIO_SEQUENTIAL, OnRadioSequential)
	ON_BN_CLICKED(IDC_RADIO_SIGMOID, OnRadioSigmoid)
	ON_BN_CLICKED(IDC_RADIO_HYPERTAN, OnRadioHypertan)
	ON_BN_CLICKED(IDC_CHECK_SECOND, OnCheckSecond)
	ON_BN_CLICKED(IDC_CHECK_MOMENTUM, OnCheckMomentum)
//	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBPDlg message handlers

BOOL CBPDlg::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString sString;
	CheckRadioButton(IDC_RADIO_SEQUENTIAL,IDC_RADIO_BATCH,
		IDC_RADIO_SEQUENTIAL+m_optionsBP.st_bLearnMode);

	CheckRadioButton(IDC_RADIO_SIGMOID,IDC_RADIO_HYPERTAN,
		IDC_RADIO_SIGMOID+m_optionsBP.st_bSigmoidType);

	m_fAlpha=m_optionsBP.st_fSigmoidAlpha;
	m_bBiases=m_optionsBP.st_bUseBiases;
	m_bMomentum=m_optionsBP.st_bUseMomentum;

	CScrollBar *pScroll=(CScrollBar *)GetDlgItem(IDC_SCROLLBAR_INPUT);
	pScroll->SetScrollRange(4,8);
	m_iInput=m_optionsBP.st_wInputBlock/2;
	m_strInput.Format(_T("%dx%d"),m_optionsBP.st_wInputBlock,m_optionsBP.st_wInputBlock);

	pScroll=(CScrollBar *)GetDlgItem(IDC_SCROLLBAR_HIDDEN);
	pScroll->SetScrollRange(1,7);
	m_iHidden=m_optionsBP.st_wHiddenBlock/2;
	m_strHidden.Format(_T("%dx%d"),m_optionsBP.st_wHiddenBlock,m_optionsBP.st_wHiddenBlock);

	m_strRatio.Format(_T("%.2f"),(FLOAT)(m_iHidden*m_iHidden)/(m_iInput*m_iInput)*100);
	m_strRatio+=_T("%");

	m_bSecond=m_optionsBP.st_bSecondHidden;
	m_dwNeurons=m_optionsBP.st_wNeurons;

	m_fInitLearnRate=m_optionsBP.st_fInitLearnRate;
	m_fFinalLearnRate=m_optionsBP.st_fFinalLearnRate;
	m_dwLearnSteps=m_optionsBP.st_dwLearnSteps;
	m_fLearnChangeRate=m_optionsBP.st_fLearnChangeRate;
	m_fInitMoment=m_optionsBP.st_fInitMoment;
	m_fFinalMoment=m_optionsBP.st_fFinalMoment;
	m_dwMomentSteps=m_optionsBP.st_dwMomentSteps;
	m_fMomentChangeRate=m_optionsBP.st_fMomentChangeRate;
	
	m_dwCycles=m_optionsBP.st_dwLearnCycles;
	m_fMinError=m_optionsBP.st_fMinError;
	m_dwPatterns=m_optionsBP.st_wPatterns;

	UpdateData(FALSE);
	if (m_optionsBP.st_bLearnMode==0)
		OnRadioSequential();
	else
		OnRadioBatch();

	EnableSecondHidden(m_optionsBP.st_bSecondHidden);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBPDlg::OnRadioBatch() 
{
	// TODO: Add your control notification handler code here
	m_optionsBP.st_bLearnMode=1;
	GetDlgItem(IDC_CHECK_MOMENTUM)->EnableWindow(FALSE);
	EnableLearnRate(FALSE);
	EnableMomentum(FALSE);
}

void CBPDlg::OnRadioSequential() 
{
	// TODO: Add your control notification handler code here
	m_optionsBP.st_bLearnMode=0;
	GetDlgItem(IDC_CHECK_MOMENTUM)->EnableWindow(TRUE);
	EnableLearnRate(TRUE);
	EnableMomentum(m_bMomentum);
}

void CBPDlg::EnableLearnRate(const BOOLEAN bFlag)
{
	UpdateData(TRUE);
	GetDlgItem(IDC_EDIT_INITLEARNRATE)->EnableWindow(bFlag);
	GetDlgItem(IDC_EDIT_FINALLEARNRATE)->EnableWindow(bFlag);
	GetDlgItem(IDC_EDIT_LEARNSTEPS)->EnableWindow(bFlag);
	GetDlgItem(IDC_EDIT_LEARNCHANGERATE)->EnableWindow(bFlag);
}

void CBPDlg::EnableMomentum(const BOOLEAN bFlag)
{
	GetDlgItem(IDC_EDIT_INITMOMENT)->EnableWindow(bFlag);
	GetDlgItem(IDC_EDIT_FINALMOMENT)->EnableWindow(bFlag);
	GetDlgItem(IDC_EDIT_MOMENTSTEPS)->EnableWindow(bFlag);
	GetDlgItem(IDC_EDIT_MOMENTCHANGERATE)->EnableWindow(bFlag);
}

void CBPDlg::OnRadioSigmoid() 
{
	// TODO: Add your control notification handler code here
	m_optionsBP.st_bSigmoidType=0;
}

void CBPDlg::OnRadioHypertan() 
{
	// TODO: Add your control notification handler code here
	m_optionsBP.st_bSigmoidType=1;
}

void CBPDlg::EnableSecondHidden(const BOOLEAN bFlag)
{
	GetDlgItem(IDC_STATIC_WITH)->EnableWindow(bFlag);
	GetDlgItem(IDC_EDIT_NEURONS)->EnableWindow(bFlag);
	GetDlgItem(IDC_STATIC_NEURONS)->EnableWindow(bFlag);
}

void CBPDlg::OnCheckSecond() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_optionsBP.st_bSecondHidden=m_bSecond;
	EnableSecondHidden(m_optionsBP.st_bSecondHidden);
}

void CBPDlg::OnCheckMomentum() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_optionsBP.st_bUseMomentum=m_bMomentum;
	EnableMomentum(m_optionsBP.st_bUseMomentum);
}

void CBPDlg::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_optionsBP.st_fSigmoidAlpha=m_fAlpha;
	m_optionsBP.st_bUseBiases=m_bBiases;
	m_optionsBP.st_dwLearnCycles=m_dwCycles;
	m_optionsBP.st_fMinError=m_fMinError;
	m_optionsBP.st_wPatterns=(WORD)m_dwPatterns;
	m_optionsBP.st_wInputBlock=m_iInput*2;
	m_optionsBP.st_wHiddenBlock=m_iHidden*2;
	m_optionsBP.st_wNeurons=(WORD)m_dwNeurons;
	m_optionsBP.st_fInitLearnRate=m_fInitLearnRate;
	m_optionsBP.st_fFinalLearnRate=m_fFinalLearnRate;
	m_optionsBP.st_dwLearnSteps=m_dwLearnSteps;
	m_optionsBP.st_fLearnChangeRate=m_fLearnChangeRate;
	m_optionsBP.st_fInitMoment=m_fInitMoment;
	m_optionsBP.st_fFinalMoment=m_fFinalMoment;
	m_optionsBP.st_dwMomentSteps=m_dwMomentSteps;
	m_optionsBP.st_fMomentChangeRate=m_fMomentChangeRate;
	CPropertyPage::OnOK();
}

void CBPDlg::GetOptionsBP(OPTIONSBP *optionsBP)
{
	CopyMemory(optionsBP,&m_optionsBP,sizeof(OPTIONSBP));
}
void CBPDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	int iMinPos;
	int iMaxPos;
	pScrollBar->GetScrollRange(&iMinPos, &iMaxPos); 
	iMaxPos = pScrollBar->GetScrollLimit();

	// Get the current position of scroll box.
	int iCurPos = pScrollBar->GetScrollPos();

	// Determine the new position of scroll box.
	switch (nSBCode)
	{
	case SB_LEFT:      // Scroll to far left.
		iCurPos = iMinPos;
		break;
	case SB_RIGHT:      // Scroll to far right.
		iCurPos = iMaxPos;
		break;
	case SB_ENDSCROLL:   // End scroll.
		break;
	case SB_LINELEFT:      // Scroll left.
		if (iCurPos > iMinPos)
			iCurPos--;
		break;
	case SB_LINERIGHT:   // Scroll right.
		if (iCurPos < iMaxPos)
			iCurPos++;
		break;
	case SB_THUMBPOSITION: // Scroll to absolute position. nPos is the position
	   iCurPos = nPos;      // of the scroll box at the end of the drag operation.
	   break;
	case SB_THUMBTRACK:   // Drag scroll box to specified position. nPos is the
		iCurPos = nPos;     // position that the scroll box has been dragged to.
		break;
	}
	// Set the new position of the thumb (scroll box).
	pScrollBar->SetScrollPos(iCurPos);
	CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);

	UpdateData(TRUE);
	m_strInput.Format(_T("%dx%d"),m_iInput*2,m_iInput*2);
	m_strHidden.Format(_T("%dx%d"),m_iHidden*2,m_iHidden*2);
	
	m_strRatio.Format(_T("%.2f"),(FLOAT)(m_iHidden*m_iHidden)/(m_iInput*m_iInput)*100);
	m_strRatio+=_T("%");
	UpdateData(FALSE);
}
