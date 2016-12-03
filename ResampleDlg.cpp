// ResampleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NNIC.h"
#include "ResampleDlg.h"

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
{

}

CResampleDlg::CResampleDlg(CBitmapKit *pBitmap,CWnd* pParent /*=NULL*/)
	: CDialog(CResampleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CResampleDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pBitmapKit=pBitmap;
}


void CResampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CResampleDlg)
	DDX_Control(pDX, IDC_PROGRESS_RESAMPLE, m_progressResample);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CResampleDlg, CDialog)
	//{{AFX_MSG_MAP(CResampleDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResampleDlg message handlers

BOOL CResampleDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_progressResample.SetRange(0,100);
	m_progressResample.SetPos(0);
	UpdateData(FALSE);
	AfxBeginThread(ResampleProgress,this);
//	HANDLE hEvent=OpenEvent(EVENT_MODIFY_STATE,FALSE,_T("RESAMPLE_PROGRESS_EVENT"));
//	SetEvent(hEvent);
//	CloseHandle(hEvent);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CResampleDlg::SetStep(const INT iStep)
{
//	UpdateData(TRUE);
	return m_progressResample.SetStep(iStep);	
}

void CResampleDlg::StepIt()
{
	m_progressResample.StepIt();
//	UpdateData(FALSE);
}

CBitmapKit * CResampleDlg::GetBitmapKit()
{
	return m_pBitmapKit;
}

HBITMAP CResampleDlg::GetHandle()
{
	return m_hBitmap;
}

void CResampleDlg::SetHandle(HBITMAP hBitmap)
{
	m_hBitmap=hBitmap;
}

UINT ResampleProgress(LPVOID pParam)
{
	CResampleDlg *pDlg=(CResampleDlg *)pParam;

	if (pDlg == NULL ||
        !pDlg->IsKindOf(RUNTIME_CLASS(CResampleDlg)))
    return TRUE;
	
	pDlg->SetHandle((pDlg->GetBitmapKit())->Resample(256,256,pDlg));
	pDlg->EndDialog(IDOK);

	return FALSE;
}
