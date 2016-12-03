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
	DDX_Scroll(pDX, IDC_SCROLLBAR_QUALITY, m_iQuality);
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
	CScrollBar *pScroll=(CScrollBar *)GetDlgItem(IDC_SCROLLBAR_QUALITY);
	pScroll->SetScrollRange(0,25);
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
	m_strQuality.Format(_T("%d"),m_iQuality);
	UpdateData(FALSE);
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
