// ResampleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NNIC.h"
#include "ResampleDlg.h"
#include ".\resampledlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResampleDlg dialog

IMPLEMENT_DYNCREATE(CResampleDlg, CDialog)

CResampleDlg::CResampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CResampleDlg::IDD, pParent)
	, m_threadResample(NULL)
	, m_dwOldWidth(0)
	, m_dwOldHeight(0)
	, m_dwNewWidth(0)
	, m_dwNewHeight(0)
{

}

CResampleDlg::CResampleDlg(CBitmapKit *pBitmap,const DWORD dwMultiple,CWnd* pParent /*=NULL*/)
	: CDialog(CResampleDlg::IDD, pParent)
	, m_threadResample(NULL)
	, m_dwOldWidth(0)
	, m_dwOldHeight(0)
	, m_dwNewWidth(0)
	, m_dwNewHeight(0)
{
	//{{AFX_DATA_INIT(CResampleDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pBitmapKit=pBitmap;
	m_dwMultiple=dwMultiple;
}


void CResampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CResampleDlg)
	DDX_Control(pDX, IDC_PROGRESS_RESAMPLE, m_progressResample);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_STATIC_OLDWIDTH, m_dwOldWidth);
	DDX_Text(pDX, IDC_STATIC_OLDHEIGHT, m_dwOldHeight);
	DDX_Text(pDX, IDC_STATIC_NEWWIDTH, m_dwNewWidth);
	DDX_Text(pDX, IDC_STATIC_NEWHEIGHT, m_dwNewHeight);
}


BEGIN_MESSAGE_MAP(CResampleDlg, CDialog)
	//{{AFX_MSG_MAP(CResampleDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_START, OnBnClickedButtonStart)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResampleDlg message handlers

BOOL CResampleDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_progressResample.SetRange(0,100);
	m_progressResample.SetPos(0);

	m_dwOldWidth=m_pBitmapKit->GetWidth();
	m_dwOldHeight=m_pBitmapKit->GetHeight();

	if (!(m_dwOldWidth/m_dwMultiple))
		m_dwNewWidth=m_dwMultiple;
	else
		m_dwNewWidth=(m_dwOldWidth/m_dwMultiple)*m_dwMultiple;

	if (!(m_dwOldHeight/m_dwMultiple))
		m_dwNewHeight=m_dwMultiple;
	else
		m_dwNewHeight=(m_dwOldHeight/m_dwMultiple)*m_dwMultiple;

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CResampleDlg::SetStep(const INT iStep)
{
	return m_progressResample.SetStep(iStep);	
}

void CResampleDlg::StepIt()
{
	m_progressResample.StepIt();
}

CBitmapKit * CResampleDlg::GetBitmapKit()
{
	return m_pBitmapKit;
}

UINT ResampleProgress(LPVOID pParam)
{
	//try
	//{
		CResampleDlg *pDlg=(CResampleDlg *)pParam;

		if (pDlg == NULL ||
			!pDlg->IsKindOf(RUNTIME_CLASS(CResampleDlg)))
		return TRUE;
	
		pDlg->GetBitmapKit()->Resample(pDlg->GetNewWidth(),pDlg->GetNewHeight(),pDlg,TRUE);
		pDlg->EndDialog(IDOK);
	//}
	//catch (CUserException *userException)
	//{
	//	AfxMessageBox(_T("Exception"),MB_OK);
	//	userException->Delete();
	//}
	return FALSE;
}

void CResampleDlg::OnBnClickedButtonStart()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	m_threadResample=AfxBeginThread(ResampleProgress,this);
}

void CResampleDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	if (m_threadResample==NULL)
	{
        CDialog::OnCancel();
		return;
	}
	m_threadResample->SuspendThread();
	if (AfxMessageBox("Do you want to cancel resampling?",MB_YESNO | MB_ICONQUESTION)!=IDYES)
	{
		m_threadResample->ResumeThread();
		return;
	}
	TerminateThread(m_threadResample,0);
	//m_threadResample->ResumeThread();
	//AfxThrowUserException();
	CDialog::OnCancel();
}

DWORD CResampleDlg::GetNewWidth(void)
{
	return m_dwNewWidth;
}

DWORD CResampleDlg::GetNewHeight(void)
{
	return m_dwNewHeight;
}
