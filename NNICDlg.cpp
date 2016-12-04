// NNICDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NNIC.h"
#include "NNICDlg.h"

#include ".\NeuroNet\NetBP.h"
#include ".\NeuroNet\NetCP.h"
#include "ReportDlg.h"
#include "ResampleDlg.h"
#include "FileBmpDlg.h"
#include "FileNniDlg.h"
#include "GeneralDlg.h"
#include "CPDlg.h"
#include "BPDlg.h"
#include "DCTDlg.h"
#include "DCT.h"
#include ".\nnicdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

IMPLEMENT_DYNAMIC(CNNICDlg,CDialog)

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNNICDlg dialog

CNNICDlg::CNNICDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNNICDlg::IDD, pParent)
{
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	OPTIONSGENERAL optionsGENERAL(
		_T("UnTitled.nni"),
		FALSE,
		TRUE,
		TRUE,
		_T("Report.txt"),
		REPORT_APPEND,
		TRUE,
		2);
	OPTIONSBP optionsBP(0,0,1.0f,TRUE,TRUE,300,0.01f,80,8,4,FALSE,16,0.9f,0.01f,
		1000,0.05f,0.3f,0.1f,1000,0.01f);
	OPTIONSCP optionsCP(NET_TYPE_KOHONEN_GROSSBERG,256,10,0.035f,TRUE,0.3f,0.1f,0.01f,10000,3,0,1,50000);
	OPTIONSDCT optionsDCT(10,TRUE);
	m_optionsDefault=OPTIONS(optionsGENERAL,optionsBP,optionsCP,optionsDCT);
}

void CNNICDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNNICDlg)
	DDX_Control(pDX, IDC_STATIC_RIGHT, m_staticRight);
	DDX_Control(pDX, IDC_STATIC_LEFT, m_staticLeft);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNNICDlg, CDialog)
	//{{AFX_MSG_MAP(CNNICDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_FILE_LOADBITMAPFILE, OnFileLoadBitmapFile)
	ON_COMMAND(ID_FILE_LOADCOMPRFILE, OnFileLoadComprFile)
	ON_BN_CLICKED(IDC_BUTTON_COMPRESS, OnButtonCompress)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_OPTIONS, OnButtonOptions)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_OPTIONS_BP, OnOptionsBP)
	ON_COMMAND(ID_OPTIONS_CP, OnOptionsCP)
	ON_COMMAND(ID_OPTIONS_DCT, OnOptionsDCT)
	ON_COMMAND(ID_HELP_ABOUT, OnHelpAbout)
	ON_COMMAND(ID_FILE_SAVELEFTBITMAP, OnFileSaveLeftBitmap)
	ON_COMMAND(ID_OPTIONS_GENERAL, OnOptionsGENERAL)
	ON_COMMAND(ID_FILE_SAVERIGHTBITMAP, OnFileSaveRightBitmap)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNNICDlg message handlers

BOOL CNNICDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	CMenu menuMain;
	menuMain.LoadMenu(IDR_MAIN_MENU);
	SetMenu(&menuMain);

	CheckRadioButton(IDC_RADIO_BACK,IDC_RADIO_DCTBACK,IDC_RADIO_BACK);
	((CProgressCtrl *)GetDlgItem(IDC_PROGRESS_COMPR))->SetStep(1);
	EnableCompress(FALSE);
	EnableStop(FALSE);

	m_bFinished=FALSE;
	m_threadCompress=NULL;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CNNICDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CNNICDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CNNICDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CNNICDlg::OnFileLoadBitmapFile() 
{
	// TODO: Add your command handler code here
	CFileBmpDlg dlgFile(TRUE);
	dlgFile.SetTitle(_T("Open Bitmap File"));
	if (dlgFile.DoModal()==IDOK)
	{
		m_staticRight.SetBitmap(NULL);
		m_strLoadedFileName=dlgFile.GetPathName();
		m_bitmapLeft.LoadBMP(m_strLoadedFileName);
		DisplayLeftBitmap();
		EnableCompress(TRUE);
	}
}

void CNNICDlg::OnFileLoadComprFile()
{
	// TODO: Add your command handler code here
	CFileNniDlg dlgFile(TRUE);
	dlgFile.SetTitle(_T("Load Compressed File"));
	if (dlgFile.DoModal()==IDOK)
	{
		m_strLoadedFileName=dlgFile.GetPathName();
		m_staticLeft.SetBitmap(NULL);
		m_staticRight.SetBitmap(NULL);
		ClearStatus();
		m_threadDeCompress=AfxBeginThread(StartDeCompress,this);
	}
}

void CNNICDlg::OnFileSaveLeftBitmap()
{
	// TODO: Add your command handler code here
	if (!m_bitmapLeft.IsPresent())
	{
		AfxMessageBox(_T("Nothing to save."),MB_OK);
		return;
	}
	CFileBmpDlg dlgFile(FALSE,_T("UnTitled"));
	dlgFile.SetTitle(_T("Save Bitmap As"));
	if (dlgFile.DoModal()==IDOK)
	{
		CFile file(dlgFile.GetPathName(),CFile::modeCreate | CFile::modeWrite);
		m_bitmapLeft.SaveBMP(&file);
		file.Close();
	}
}

void CNNICDlg::OnFileSaveRightBitmap()
{
	// TODO: Add your command handler code here
	if (!m_bitmapRight.IsPresent())
	{
		AfxMessageBox(_T("Nothing to save."),MB_OK);
		return;
	}
	CFileBmpDlg dlgFile(FALSE,_T("UnTitled"));
	dlgFile.SetTitle(_T("Save Bitmap As"));
	if (dlgFile.DoModal()==IDOK)
	{
		CFile file(dlgFile.GetPathName(),CFile::modeCreate | CFile::modeWrite);
		m_bitmapRight.SaveBMP(&file);
		file.Close();
	}
}

void CNNICDlg::OnButtonCompress() 
{
	// TODO: Add your control notification handler code here
	m_staticRight.SetBitmap(NULL);
	UINT uiMethod=GetCheckedRadioButton(IDC_RADIO_BACK,IDC_RADIO_DCTBACK);
	ClearStatus();
	switch (uiMethod)
	{
	case IDC_RADIO_BACK:
		if (!m_bitmapLeft.CheckBitmap(8,0,0))
		{
			AfxMessageBox(_T("This compression method supports only 8-bit per pixel bitmaps"),MB_OK);
			return;
		}
		if (!m_bitmapLeft.CheckBitmap(0,
			m_optionsDefault.st_optionsBP.st_bInputBlock,
			m_optionsDefault.st_optionsBP.st_bInputBlock))
		{
			if (AfxMessageBox("A bitmap dimension does not fit for this compression method.\n"
				"Do you want to resample it?",MB_OKCANCEL | MB_ICONQUESTION)!=IDOK)
				return;
			CResampleDlg dlg(&m_bitmapLeft,m_optionsDefault.st_optionsBP.st_bInputBlock);
			if (dlg.DoModal()!=IDOK)
				return;
			DisplayLeftBitmap();
		}
		EnableOptions(FALSE);
		m_bFinished=FALSE;
		m_threadCompress=AfxBeginThread(StartBP,this);
		break;
	case IDC_RADIO_COUNTER:
		if (!m_bitmapLeft.CheckBitmap(24,0,0))
		{
			AfxMessageBox(_T("This compression method supports only 24-bit per pixel bitmaps"));
			return;
		}
		EnableOptions(FALSE);
		m_bFinished=FALSE;
		m_threadCompress=AfxBeginThread(StartCP,this);
		break;
	case IDC_RADIO_DCT:
		if (!m_bitmapLeft.CheckBitmap(8,0,0)&&!m_bitmapLeft.CheckBitmap(24,0,0))
		{
			AfxMessageBox(_T("This compression method supports only 8 or 24-bit per pixel bitmaps"));
			return;
		}
		if (!m_bitmapLeft.CheckBitmap(0,DCT_SIZE,DCT_SIZE))
		{
			if (AfxMessageBox("A bitmap dimension does not fit for this compression method.\n"
				"Do you want to resample it?",MB_OKCANCEL | MB_ICONQUESTION)!=IDOK)
				return;
			CResampleDlg dlg(&m_bitmapLeft,DCT_SIZE);
			if (dlg.DoModal()!=IDOK)
				return;
			DisplayLeftBitmap();
		}
		EnableOptions(FALSE);
		m_bFinished=FALSE;
		m_threadCompress=AfxBeginThread(StartDCT,this/*,THREAD_PRIORITY_NORMAL,10*1024*1024,0,NULL*/);
		break;
	case IDC_RADIO_DCTBACK:
		if (!m_bitmapLeft.CheckBitmap(8,0,0))
		{
			AfxMessageBox(_T("This compression method supports only 8-bit per pixel bitmaps"));
			return;
		}
		EnableOptions(FALSE);
		m_bFinished=FALSE;
		m_threadCompress=AfxBeginThread(StartDCTBack,this);
	}
}

void CNNICDlg::EnableCompress(const BOOLEAN bFlag)
{
	(GetDlgItem(IDC_BUTTON_COMPRESS))->EnableWindow(bFlag);
}

void CNNICDlg::EnableStop(const BOOLEAN bFlag)
{
	(GetDlgItem(IDC_BUTTON_STOP))->EnableWindow(bFlag);
}

void CNNICDlg::EnableOptions(const BOOLEAN bFlag)
{
	(GetDlgItem(IDC_BUTTON_OPTIONS))->EnableWindow(bFlag);
}

CBitmapKit * CNNICDlg::GetLeftBitmap()
{
	return &m_bitmapLeft;
}

CBitmapKit * CNNICDlg::GetRightBitmap()
{
	return &m_bitmapRight;
}

OPTIONSCP * CNNICDlg::GetOptionsCP()
{
	return &m_optionsDefault.st_optionsCP;
}

void CNNICDlg::SetProgress(const UINT uiPos)
{
	((CProgressCtrl *)GetDlgItem(IDC_PROGRESS_COMPR))->SetPos(uiPos);
}

INT CNNICDlg::StepProgress()
{
	return ((CProgressCtrl *)GetDlgItem(IDC_PROGRESS_COMPR))->StepIt();
}


void CNNICDlg::DisplayLeftBitmap()
{
	CRect rect;
	m_staticLeft.GetClientRect(&rect);
	FLOAT fRatio=(FLOAT)m_bitmapLeft.GetWidth()/m_bitmapLeft.GetHeight();
	DWORD dwNewWidth=rect.Width();
	DWORD dwNewHeight=(DWORD)(dwNewWidth/fRatio);
	if (dwNewHeight>rect.Height())
	{
		dwNewHeight=rect.Height();
		dwNewWidth=(DWORD)(dwNewHeight*fRatio);
	}
	m_staticLeft.SetBitmap(m_bitmapLeft.Scale(m_staticLeft.GetDC(),dwNewWidth,dwNewHeight));
//	m_staticLeft.SetBitmap(m_bitmapLeft);
}

void CNNICDlg::DisplayRightBitmap()
{
	CRect rect;
	m_staticRight.GetClientRect(&rect);
	FLOAT fRatio=(FLOAT)m_bitmapLeft.GetWidth()/m_bitmapLeft.GetHeight();
	DWORD dwNewWidth=rect.Width();
	DWORD dwNewHeight=(DWORD)(dwNewWidth/fRatio);
	if (dwNewHeight>rect.Height())
	{
		dwNewHeight=rect.Height();
		dwNewWidth=(DWORD)(dwNewHeight*fRatio);
	}
	m_staticRight.SetBitmap(m_bitmapRight.Scale(m_staticLeft.GetDC(),dwNewWidth,dwNewHeight));
}

void CNNICDlg::OnButtonStop() 
{
	// TODO: Add your control notification handler code here
	if (!m_bFinished)
	{
		TerminateThread(m_threadCompress->m_hThread,0);
		EnableCompress(TRUE);
		EnableOptions(TRUE);
	}
}

void CNNICDlg::SetFinished(const BOOLEAN bFlag)
{
	m_bFinished=bFlag;
}

void CNNICDlg::OnButtonOptions() 
{
	// TODO: Add your control notification handler code here
	BYTE bMethod=GetCheckedRadioButton(IDC_RADIO_BACK,IDC_RADIO_DCTBACK)-IDC_RADIO_BACK;
	ShowOptions(bMethod+1);
}

OPTIONSBP * CNNICDlg::GetOptionsBP()
{
	return &m_optionsDefault.st_optionsBP;
}

ULONGLONG CNNICDlg::GetLoadedFileSize()
{
	CFile file;
	CFileStatus fileStatus;
	file.GetStatus(m_strLoadedFileName,fileStatus);
	return fileStatus.m_size;
}

CString CNNICDlg::GetLoadedFileName(void)
{
	return m_strLoadedFileName;
}

OPTIONSDCT * CNNICDlg::GetOptionsDCT()
{
	return &m_optionsDefault.st_optionsDCT;
}

void CNNICDlg::ShowOptions(const BYTE bActivePage)
{
	CPropertySheet propOptions;
	propOptions.m_psh.dwFlags |=PSH_NOAPPLYNOW;
	propOptions.m_psh.dwFlags &= ~(PSH_HASHELP);

	CGeneralDlg pageGENERAL(m_optionsDefault.st_optionsGENERAL);
	pageGENERAL.m_psp.dwFlags &= ~(PSP_HASHELP);

	CBPDlg pageBP(m_optionsDefault.st_optionsBP);
	pageBP.m_psp.dwFlags &= ~(PSP_HASHELP);

	CCPDlg pageCP(m_optionsDefault.st_optionsCP);
	pageCP.m_psp.dwFlags &= ~(PSP_HASHELP);

	CDCTDlg pageDCT(m_optionsDefault.st_optionsDCT);
	pageDCT.m_psp.dwFlags &= ~(PSP_HASHELP);

	propOptions.SetTitle(_T("Options"));
	propOptions.AddPage(&pageGENERAL);
	propOptions.AddPage(&pageBP);
	propOptions.AddPage(&pageCP);
	propOptions.AddPage(&pageDCT);
	propOptions.SetActivePage(bActivePage);
	propOptions.DoModal();

	pageGENERAL.GetOptionsGENERAL(&m_optionsDefault.st_optionsGENERAL);
	pageBP.GetOptionsBP(&m_optionsDefault.st_optionsBP);
	pageCP.GetOptionsCP(&m_optionsDefault.st_optionsCP);
	pageDCT.GetOptionsDCT(&m_optionsDefault.st_optionsDCT);
}

void CNNICDlg::OnOptionsGENERAL()
{
	// TODO: Add your command handler code here
	ShowOptions(PAGE_GENERAL);
}

void CNNICDlg::OnOptionsBP()
{
	// TODO: Add your command handler code here
	ShowOptions(PAGE_BP);
}

void CNNICDlg::OnOptionsCP()
{
	// TODO: Add your command handler code here
	ShowOptions(PAGE_CP);
}

void CNNICDlg::OnOptionsDCT()
{
	// TODO: Add your command handler code here
	ShowOptions(PAGE_DCT);
}

void CNNICDlg::OnHelpAbout()
{
	// TODO: Add your command handler code here
	CAboutDlg dlg;
	dlg.DoModal();
}

void CNNICDlg::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
}

UINT StartBP(LPVOID pParam)
{
	CNNICDlg* pDlg = (CNNICDlg *)pParam;

	if (pDlg == NULL ||
		!pDlg->IsKindOf(RUNTIME_CLASS(CNNICDlg)))
		return TRUE;

	pDlg->EnableCompress(FALSE);
	pDlg->EnableStop(TRUE);

	pDlg->SetStatusOperation(_T("Initialization"));

	OPTIONSBP *optionsBP=pDlg->GetOptionsBP();
	OPTIONSGENERAL *optionsGENERAL=pDlg->GetOptionsGENERAL();
	const WORD wInput=optionsBP->st_bInputBlock*optionsBP->st_bInputBlock;
	const WORD wHidden=optionsBP->st_bHiddenBlock*optionsBP->st_bHiddenBlock;
	BYTE BP_HIDDEN_LAYER;
	BYTE BP_OUTPUT_LAYER;
	WORD wLayers[4];
	if (optionsBP->st_bSecondHidden)
	{
		BP_HIDDEN_LAYER=2;
		BP_OUTPUT_LAYER=3;
		wLayers[BP_INPUT_LAYER]=wInput;
		wLayers[BP_INPUT_LAYER+1]=optionsBP->st_wNeurons;
		wLayers[BP_HIDDEN_LAYER]=wHidden;
		wLayers[BP_OUTPUT_LAYER]=wInput;
	}
	else
	{
		BP_HIDDEN_LAYER=1;
		BP_OUTPUT_LAYER=2;
		wLayers[BP_INPUT_LAYER]=wInput;
		wLayers[BP_HIDDEN_LAYER]=wHidden;
		wLayers[BP_OUTPUT_LAYER]=wInput;
	}
	
	const FLOAT fShift=-0.5;
	const FLOAT fSignalWidth=1.0;
	const FLOAT fScaleParam=0.5;
	NetBP Net(BP_OUTPUT_LAYER+1,wLayers);
	Net.SetSignalBoundaries(fShift,-fShift);

	switch (optionsBP->st_bSigmoidType)
	{
	case 0:
		Net.SetSigmoidType(SIGMOID_TYPE_ORIGINAL);
		break;
	case 1:
		Net.SetSigmoidType(SIGMOID_TYPE_HYPERTAN);
		break;
	}

	Net.SetSigmoidAlpha(optionsBP->st_fSigmoidAlpha);
	Net.SetScaleParam(fScaleParam);
	Net.UseBiases(optionsBP->st_bUseBiases);
	BYTE bMethod;
	switch (optionsBP->st_bLearnMode)
	{
	case 0:
		bMethod=LEARN_TYPE_SEQUENTIAL;
		break;
	case 1:
		bMethod=LEARN_TYPE_BATCH;
		break;
	}
	if (optionsBP->st_bUseMomentum)
		bMethod|=LEARN_TYPE_MOMENTUM;
	
	Net.SetLearnType(bMethod);
	Net.InitWeights();

	CBitmapKit *pBitmap=pDlg->GetLeftBitmap();
	BITMAP bm;
	pBitmap->GetBitmap(&bm);
	const DWORD dwSizePixels=bm.bmWidth*bm.bmHeight;
	const WORD wWidthBlocks=(WORD)bm.bmWidth/optionsBP->st_bInputBlock;
	const WORD wHeightBlocks=(WORD)bm.bmHeight/optionsBP->st_bInputBlock;
	const WORD wBlocks=wWidthBlocks*wHeightBlocks;

	DWORD i,j,k,l;
	DWORD dwIndex,dwXIndex,dwYIndex,dwInputIndex,dwHiddenIndex;
	FLOAT **pBlocks=AllocateFloatMatrix(wHeightBlocks,wWidthBlocks*wInput);
	ASSERT(pBlocks!=NULL);

	
	for (j=0; j<wWidthBlocks; j++)
	{
		dwIndex=j*wInput;
		for (i=0; i<wHeightBlocks; i++)
		{
			pBitmap->GetBlock(
				CRect(CPoint(j*optionsBP->st_bInputBlock,i*optionsBP->st_bInputBlock),
				CSize(optionsBP->st_bInputBlock,optionsBP->st_bInputBlock)),
				&(pBlocks[i][dwIndex]),0,fShift,fSignalWidth);
		}
	}
	LARGE_INTEGER liFreq;
	LARGE_INTEGER liCount1;
	LARGE_INTEGER liCount2;

	WORD wPatterns;
	if (optionsBP->st_wPatterns>wBlocks)
		optionsBP->st_wPatterns=wBlocks;
	FLOAT fError;
	FLOAT fNetError;
	DWORD dwCycles;
	BOOLEAN bFlag;
	CString strText;
	/*pDlg->SetProgressRange(0,wBlocks);*/
	QueryPerformanceFrequency(&liFreq);
	if (bMethod & LEARN_TYPE_SEQUENTIAL)
	{
		DWORD dwLearnCount=0;
		DWORD dwMomentCount=0;
		INT iLearnSign=1;
		INT iMomentSign=1;
		if (optionsBP->st_fInitLearnRate>=optionsBP->st_fFinalLearnRate)
			iLearnSign=-1;
		if (optionsBP->st_fInitMoment>=optionsBP->st_fFinalMoment)
			iMomentSign=-1;
		FLOAT fLearnRate=optionsBP->st_fInitLearnRate;
		FLOAT fMomentumParam=optionsBP->st_fInitMoment;
		Net.SetLearnRate(optionsBP->st_fInitLearnRate);
		Net.SetMomentumParam(optionsBP->st_fInitMoment);
		dwCycles=0;
		pDlg->SetStatusOperation(_T("Learning"));
		QueryPerformanceCounter(&liCount1);
		do
		{
			bFlag=TRUE;
			wPatterns=0;
			fNetError=0.0;
			for (i=0; i<dwSizePixels; i+=wInput)
			{
				Net.SetAxons(BP_INPUT_LAYER,&(pBlocks[0][i]));
				Net.ForwardPass(BP_INPUT_LAYER);
				if ((fError=Net.TargetFunction(BP_OUTPUT_LAYER,&(pBlocks[0][i])))>
					optionsBP->st_fMinError)
				{
					Net.BackwardPass(&(pBlocks[0][i]));
					bFlag=FALSE;
				}
				else
					wPatterns++;
				fNetError+=fError;
				dwLearnCount++;
				if (dwLearnCount==optionsBP->st_dwLearnSteps&&
					optionsBP->st_fFinalLearnRate!=optionsBP->st_fInitLearnRate)
				{
					dwLearnCount=0;
					fLearnRate+=iLearnSign*optionsBP->st_fLearnChangeRate;
					if (iLearnSign==-1&&fLearnRate<optionsBP->st_fFinalLearnRate)
						fLearnRate=optionsBP->st_fFinalLearnRate;
					if (iLearnSign==1&&fLearnRate>optionsBP->st_fFinalLearnRate)
						fLearnRate=optionsBP->st_fFinalLearnRate;
					Net.SetLearnRate(fLearnRate);
				}
				if (bMethod & LEARN_TYPE_MOMENTUM)
				{
					dwMomentCount++;
					if (dwMomentCount==optionsBP->st_dwMomentSteps&&
						optionsBP->st_fFinalMoment!=optionsBP->st_fInitMoment)
					{
						dwMomentCount=0;
						fMomentumParam+=iMomentSign*optionsBP->st_fMomentChangeRate;
						if (iMomentSign==-1&&fMomentumParam<optionsBP->st_fFinalMoment)
							fMomentumParam=optionsBP->st_fFinalMoment;
						if (iMomentSign==1&&fMomentumParam>optionsBP->st_fFinalMoment)
							fMomentumParam=optionsBP->st_fFinalMoment;
						Net.SetMomentumParam(fMomentumParam);
					}
				}
				/*pDlg->StepProgress();*/
			}
			QueryPerformanceCounter(&liCount2);
			dwCycles++;
			/*pDlg->SetProgress(0);*/
			strText.Format(_T("%d"),dwCycles);
			pDlg->SetStatus(1,_T("Learning cycle:"),strText);
			strText.Format(_T("%.5f"),fNetError/wBlocks);
			pDlg->SetStatus(2,_T("Average net error:"),strText);
			strText.Format(_T("%.5f"),Net.GetLearnRate());
			pDlg->SetStatus(3,_T("Learn rate:"),strText);
			strText.Format(_T("%d"),wPatterns);
			pDlg->SetStatus(4,_T("Learned patterns:"),strText);
			strText.Format(_T("%.5f"),(FLOAT)(liCount2.QuadPart-liCount1.QuadPart)/liFreq.QuadPart);
			pDlg->SetStatus(5,_T("Time:"),strText);
		}
		while (wPatterns<optionsBP->st_wPatterns&&dwCycles<optionsBP->st_dwLearnCycles&&!bFlag);
	}
	if (bMethod & LEARN_TYPE_BATCH)
	{
		FLOAT fTotalError;
		FLOAT **pPatterns=new FLOAT *[wWidthBlocks*wHeightBlocks];
		ASSERT(pPatterns!=NULL);
		dwCycles=0;
		QueryPerformanceCounter(&liCount1);
		do
		{
			fTotalError=0.0;
			fNetError=0.0;
			wPatterns=0;
			bFlag=TRUE;
			for (i=0; i<dwSizePixels; i+=wInput)
			{
				Net.SetAxons(BP_INPUT_LAYER,&(pBlocks[0][i]));
				Net.ForwardPass(BP_INPUT_LAYER);
				if ((fError=Net.TargetFunction(BP_OUTPUT_LAYER,&(pBlocks[0][i])))>
					optionsBP->st_fMinError)
				{
					Net.UpdateGradients(&(pBlocks[0][i]));
					fNetError+=fError;
					pPatterns[wPatterns++]=&(pBlocks[0][i]);
					bFlag=FALSE;
				}
				fTotalError+=fError;
				/*pDlg->StepProgress();*/
			}

			if (!bFlag)
				fNetError=Net.BackwardPass(wPatterns,(const FLOAT **)pPatterns,fNetError);
			QueryPerformanceCounter(&liCount2);
			dwCycles++;
			/*pDlg->SetProgress(0);*/
			strText.Format(_T("%d"),dwCycles);
			pDlg->SetStatus(1,_T("Learning cycle:"),strText);
			strText.Format(_T("%.5f"),fTotalError/wBlocks);
			pDlg->SetStatus(2,_T("Average net error:"),strText);
			strText.Format(_T("%.5f"),Net.GetLearnRate());
			pDlg->SetStatus(3,_T("Learn rate:"),strText);
			strText.Format(_T("%d"),wBlocks-wPatterns);
			pDlg->SetStatus(4,_T("Learned patterns:"),strText);
			strText.Format(_T("%.5f"),(FLOAT)(liCount2.QuadPart-liCount1.QuadPart)/liFreq.QuadPart);
			pDlg->SetStatus(5,_T("Time:"),strText);
		}
		while ((wBlocks-wPatterns)<optionsBP->st_wPatterns&&dwCycles<optionsBP->st_dwLearnCycles&&!bFlag);
		delete []pPatterns;
	}

	LPBYTE *lpHiddenBlocks=AllocateByteMatrix(wHeightBlocks,wWidthBlocks*wHidden);
	ASSERT(lpHiddenBlocks!=NULL);
	FLOAT *pHidden=new FLOAT[wHidden];
	ASSERT(pHidden!=NULL);

	pDlg->SetProgressRange(0,wBlocks);
	pDlg->SetProgress(0);
	pDlg->SetStatusOperation(_T("Compression"));
	for (j=0; j<wWidthBlocks; j++)
	{
		dwInputIndex=j*wInput;
		dwHiddenIndex=j*wHidden;
		for (i=0; i<wHeightBlocks; i++)
		{
			Net.SetAxons(BP_INPUT_LAYER,&(pBlocks[i][dwInputIndex]));
			Net.ForwardPass(BP_INPUT_LAYER);
			Net.GetAxons(BP_HIDDEN_LAYER,pHidden);
			for (k=0; k<wHidden; k++)
				lpHiddenBlocks[i][dwHiddenIndex+k]=
					(BYTE)((-fShift+pHidden[k])/fSignalWidth*255.0+0.5);
			pDlg->StepProgress();
		}
	}
	
	QueryPerformanceCounter(&liCount2);
	DestroyMatrix((LPVOID *)pBlocks);

	FLOAT **pOutput=AllocateFloatMatrix(optionsBP->st_bInputBlock,optionsBP->st_bInputBlock);
	ASSERT(pOutput!=NULL);

	LPBYTE *lpResultBits=AllocateByteMatrix(bm.bmHeight,bm.bmWidthBytes);
	ASSERT(lpResultBits!=NULL);
	
	//using namespace std;
	//ofstream f("debug_inp.txt",ios::out | ios::trunc);
	for (j=0; j<wWidthBlocks; j++)
	{
		dwHiddenIndex=j*wHidden;
		dwXIndex=j*optionsBP->st_bInputBlock;
		for (i=0; i<wHeightBlocks; i++)
		{
			for (k=0; k<wHidden; k++)
			{
				pHidden[k]=fShift+lpHiddenBlocks[i][dwHiddenIndex+k]/255.0f;
				//f<<pHidden[k]<<" "<<flush;
			}
			//f<<endl;
			Net.SetAxons(BP_HIDDEN_LAYER,pHidden);
			Net.ForwardPass(BP_HIDDEN_LAYER);
			Net.GetAxons(BP_OUTPUT_LAYER,pOutput[0]);
			dwYIndex=i*optionsBP->st_bInputBlock;
			for (k=0; k<optionsBP->st_bInputBlock; k++)
				for (l=0; l<optionsBP->st_bInputBlock; l++)
					lpResultBits[dwYIndex+k][dwXIndex+l]=
					(BYTE)((-fShift+pOutput[k][l])/fSignalWidth*255.0+0.5);
		}
	}
	//f.close();

	delete []pHidden;
	DestroyMatrix((LPVOID *)pOutput);

	CBitmapKit *pComprBitmap=pDlg->GetRightBitmap();
	pComprBitmap->CreateBitmap(bm,lpResultBits[0]);
	pDlg->DisplayRightBitmap();

	DestroyMatrix((LPVOID *)lpResultBits);

	pDlg->SetStatusOperation(_T("Writing out file"));
	if (!QueryZLibVersion())/*!!!!!!!!!!!!!!*/
		return FALSE;
	
	CFile file;
	file.Open(pDlg->GetComprFileName(),CFile::modeCreate | CFile::modeWrite);/*cur directory!!!*/
	
	COMPRFILEINFO cfInfo(COMPRFILE_NNIC,COMPRTYPE_BP,bm);
	file.Write(&cfInfo,sizeof(COMPRFILEINFO));

	COMPRDATABP cdBP(fShift,fSignalWidth,fScaleParam,
		optionsBP->st_bSigmoidType,optionsBP->st_fSigmoidAlpha,
		optionsBP->st_bUseBiases,wHidden,wInput);
	file.Write(&cdBP,sizeof(COMPRDATABP));

	DWORD dwSrcLen=Net.GetWeightsSize(BP_OUTPUT_LAYER);
	DWORD dwDestLen=GetBufferSize(dwSrcLen);/*+0.1%+12*/

	LPBYTE lpSrc=new BYTE[dwSrcLen];
	ASSERT(lpSrc!=NULL);
	LPBYTE lpDest=new BYTE[dwDestLen];
	ASSERT(lpDest!=NULL);

	Net.CopyWeights(lpSrc,BP_OUTPUT_LAYER);

	if (!Compress2(lpDest,&dwDestLen,lpSrc,dwSrcLen,Z_BEST_COMPRESSION))
		return FALSE;
	
	COMPRDATAINFO cdInfo(dwSrcLen,dwDestLen);
	file.Write(&cdInfo,sizeof(COMPRDATAINFO));
	file.Write(lpDest,dwDestLen);

	delete []lpSrc;
	delete []lpDest;

	dwSrcLen=wHeightBlocks*wWidthBlocks*wHidden*sizeof(BYTE);
	dwDestLen=GetBufferSize(dwSrcLen);/*+0.1%+12*/

	lpDest=new BYTE[dwDestLen];
	ASSERT(lpDest!=NULL);

	if (!Compress2(lpDest,&dwDestLen,lpHiddenBlocks[0],dwSrcLen,Z_BEST_COMPRESSION))
		return FALSE;

	cdInfo.Set(dwSrcLen,dwDestLen);
	file.Write(&cdInfo,sizeof(COMPRDATAINFO));
	file.Write(lpDest,dwDestLen);

	pDlg->SetStatusOperation(_T("Done"));

	CReportDlg dlgReport;
	dlgReport.SetFileDirectory(file.GetFileName(),file.GetFilePath());
	dlgReport.SetFileSize(pDlg->GetLoadedFileSize(),file.GetLength());
	dlgReport.SetImageSize(pBitmap->GetWidth(),pBitmap->GetHeight());
	dlgReport.SetOriginalColors(pBitmap->GetBitsPixel(),pBitmap->GetColors());
	dlgReport.SetComprMethod(_T("Back Propagation"));
	dlgReport.SetPSNR(pBitmap->GetPSNRFullChannel(pComprBitmap));
	dlgReport.SetComprTime((FLOAT)(liCount2.QuadPart-liCount1.QuadPart)/liFreq.QuadPart);
	file.Close();
	if (optionsGENERAL->st_bShowReport)
		dlgReport.DoModal();

	if (optionsGENERAL->st_bWriteReport)
		pDlg->WriteReportFile(dlgReport.GetReportString());

	pDlg->EnableOptions(TRUE);
	pDlg->EnableCompress(TRUE);
	pDlg->SetFinished(TRUE);
	return FALSE;
}

UINT StartCP(LPVOID pParam)
{
	CNNICDlg* pDlg = (CNNICDlg *)pParam;

	if (pDlg == NULL ||
		!pDlg->IsKindOf(RUNTIME_CLASS(CNNICDlg)))
		return TRUE;

	pDlg->EnableCompress(FALSE);
	pDlg->EnableStop(TRUE);

	pDlg->SetStatusOperation(_T("Initialization"));

	CBitmapKit *pBitmap=pDlg->GetLeftBitmap();
	BITMAP bm;
	pBitmap->GetBitmap(&bm);
	const DWORD dwSizePixels=pBitmap->GetSizePixels();
	const BYTE bBytesPixel=pBitmap->GetBytesPixel();
	const DWORD dwSizeClear=pBitmap->GetSizeClear();
	
	const LPBYTE *lpSrcBits=pBitmap->GetBits();
	ASSERT(lpSrcBits!=NULL);

	OPTIONSCP *optionsCP=pDlg->GetOptionsCP();
	OPTIONSGENERAL *optionsGENERAL=pDlg->GetOptionsGENERAL();
	ASSERT(optionsCP!=NULL);

	const FLOAT fColorShift=-128.0;
	DWORD i,j,k;
	DWORD dwIndex;

	WORD wLayers[CP_MAX_NETSIZE];;
	switch (optionsCP->st_bNetType)
	{
	case NET_TYPE_KOHONEN_GROSSBERG:
		wLayers[CP_INPUT_LAYER]=bBytesPixel;
		wLayers[KOHONEN_LAYER]=optionsCP->st_wClusters;
		wLayers[GROSSBERG_LAYER]=bBytesPixel;
		break;

	case NET_TYPE_KOHONEN:
		wLayers[CP_INPUT_LAYER]=bBytesPixel;
		wLayers[KOHONEN_LAYER]=optionsCP->st_wClusters;
		break;
	}

	NetCP Net(wLayers,optionsCP->st_bNetType);

	FLOAT *pLearnRate=NULL;
	FLOAT fLearnRate=optionsCP->st_fInitLearnRate;
	WORD wNeighRadius=optionsCP->st_wInitNeighRadius;
	if (optionsCP->st_bTrainNeighbours)
	{
		pLearnRate=new FLOAT[__max(optionsCP->st_wInitNeighRadius,
			optionsCP->st_wFinalNeighRadius)+1];
		wNeighRadius=optionsCP->st_wInitNeighRadius;
	}
	else
	{
		pLearnRate=new FLOAT;
		wNeighRadius=0;
	}
	ASSERT(pLearnRate!=NULL);

	Net.SetLearnRate(fLearnRate);
	Net.SetNeighbouringRadius(wNeighRadius);
	Net.CalculateLearnRate(pLearnRate);
	
	WORD **wClusters=AllocateWordMatrix(bm.bmHeight,bm.bmWidth);
	ASSERT(wClusters!=NULL);
	FLOAT *pWeights=new FLOAT[bBytesPixel];

	BYTE *pMap=new BYTE[optionsCP->st_wClusters];
		ASSERT(pMap!=NULL);

	DWORD dwSrcLen=optionsCP->st_wClusters*bBytesPixel;
	DWORD dwDestLen=GetBufferSize(dwSrcLen);
	LPBYTE lpSrc=NULL;

	DWORD dwCycles;
	
	CString strText;
	BOOLEAN bFlag;
	WORD wUsed;

	LARGE_INTEGER liFreq;
	LARGE_INTEGER liCount1;
	LARGE_INTEGER liCount2;
	QueryPerformanceFrequency(&liFreq);
	if (optionsCP->st_bNetType==NET_TYPE_KOHONEN_GROSSBERG)
	{
		Net.InitWeights(-1.0,2.0);

		FLOAT **pVectors=AllocateFloatMatrix(bm.bmHeight,bm.bmWidthBytes);
		ASSERT(pVectors!=NULL);

		for (j=0; j<bm.bmWidth; j++)
		{
			dwIndex=j*bBytesPixel;
			for (i=0; i<bm.bmHeight; i++)
			{
				for (k=0; k<bBytesPixel; k++)
					pVectors[i][dwIndex+k]=fColorShift+lpSrcBits[i][dwIndex+k];
				Net.NormalizeVector(&(pVectors[i][dwIndex]),bBytesPixel);
			}
		}

		INT iLearnSign=1;
		INT iNeighSign=1;
		if (optionsCP->st_fInitLearnRate>=optionsCP->st_fFinalLearnRate)
			iLearnSign=-1;
		if (optionsCP->st_wInitNeighRadius>=optionsCP->st_wFinalNeighRadius)
			iNeighSign=-1;

		DWORD dwLearnCount;
		DWORD dwNeighCount;

		FLOAT fWinDist;
		dwCycles=0;
		dwLearnCount=0;
		dwNeighCount=0;
		pDlg->SetProgressRange(0,bm.bmWidth);
		pDlg->SetStatusOperation(_T("Learning"));
		QueryPerformanceCounter(&liCount1);
		do
		{
			fWinDist=0.0;
			pDlg->SetProgress(0);
			FLOAT fTime=0.0;
			wUsed=0;
			ZeroMemory(pMap,optionsCP->st_wClusters);
			for (j=0; j<bm.bmWidth; j++)
			{
				dwIndex=j*bBytesPixel;
				for (i=0; i<bm.bmHeight; i++)
				{
					Net.SetAxons(CP_INPUT_LAYER,&(pVectors[i][dwIndex]));
					Net.PropagateKohonenMax();
					wClusters[i][j]=Net.GetWinnerNeuron();
					/*if (!map.Lookup(wClusters[i][j],bValue))
						map.SetAt(wClusters[i][j],FALSE);*/
					if (!pMap[wClusters[i][j]])
					{
						pMap[wClusters[i][j]]=TRUE;
						wUsed++;
					}
					fWinDist+=Net.GetWinnerDistance();
					Net.LearnKohonen(pLearnRate);
					dwLearnCount++;
					dwNeighCount++;
					bFlag=FALSE;
					if (dwLearnCount==optionsCP->st_dwLearnRateSteps)
					{
						dwLearnCount=0;
						fLearnRate+=iLearnSign*optionsCP->st_fLearnChangeRate;
						if (iLearnSign==-1&&fLearnRate<optionsCP->st_fFinalLearnRate)
							fLearnRate=optionsCP->st_fFinalLearnRate;
						if (iLearnSign==1&&fLearnRate>optionsCP->st_fFinalLearnRate)
							fLearnRate=optionsCP->st_fFinalLearnRate;
						Net.SetLearnRate(fLearnRate);
						bFlag=TRUE;
					}
					if (wNeighRadius&&dwNeighCount==optionsCP->st_dwNeighRadiusSteps)
					{

						dwNeighCount=0;
						wNeighRadius+=iNeighSign*optionsCP->st_wNeighChangeRate;
						if (iNeighSign==-1&&wNeighRadius<optionsCP->st_wFinalNeighRadius)
							wNeighRadius=optionsCP->st_wFinalNeighRadius;
						if (iNeighSign==1&&wNeighRadius>optionsCP->st_wFinalNeighRadius)
							wNeighRadius=optionsCP->st_wFinalNeighRadius;
						Net.SetNeighbouringRadius(wNeighRadius);
						bFlag=TRUE;
					}
					if (bFlag)
						Net.CalculateLearnRate(pLearnRate);	
				}
				pDlg->StepProgress();
			}
			fWinDist/=dwSizePixels;
			dwCycles++;
			QueryPerformanceCounter(&liCount2);
			pDlg->SetProgress(0);
			strText.Format(_T("%d"),dwCycles);
			pDlg->SetStatus(1,_T("Learning cycle:"),strText);
			strText.Format(_T("%.5f"),fWinDist);
			pDlg->SetStatus(2,_T("Average net error:"),strText);
			strText.Format(_T("%d"),optionsCP->st_wClusters-wUsed);
			pDlg->SetStatus(3,_T("Unused clusters:"),strText);
			strText.Format(_T("%.5f"),(FLOAT)(liCount2.QuadPart-liCount1.QuadPart)/liFreq.QuadPart);
			pDlg->SetStatus(4,_T("Time:"),strText);
		}
		while (fWinDist>optionsCP->st_fMinDist&&dwCycles<optionsCP->st_dwCycles);
		DestroyMatrix((LPVOID *)pVectors);

		pDlg->SetStatusOperation(_T("Compression"));
		pDlg->SetProgress(0);
		for (j=0; j<bm.bmWidth; j++)
		{
			dwIndex=j*bBytesPixel;
			for (i=0; i<bm.bmHeight; i++)
			{
				Net.SetWinnerNeuron(wClusters[i][j]);	
				Net.LearnGrossberg(&(lpSrcBits[i][dwIndex]));
			}
			pDlg->StepProgress();
		}
		Net.FinalizeGrossberg();	
		QueryPerformanceCounter(&liCount2);

		lpSrc=new BYTE[dwSrcLen];
		ASSERT(lpSrc!=NULL);

		for (i=0; i<optionsCP->st_wClusters; i++)
		{
			Net.GetWeightsToNeuron(GROSSBERG_LAYER,(WORD)i,pWeights);
			dwIndex=i*bBytesPixel;
			for (j=0; j<bBytesPixel; j++)
				lpSrc[dwIndex+j]=(BYTE)(pWeights[j]+0.5);
		}
	}
	if (optionsCP->st_bNetType==NET_TYPE_KOHONEN)
	{
		Net.InitWeights();

		const WORD wPrime[4]={487,491,499,503};
		FLOAT fLearnPower=(FLOAT)(1.0f/(optionsCP->st_dwCycles)
			*log(optionsCP->st_fFinalLearnRate/optionsCP->st_fInitLearnRate));
		FLOAT fNeighPower=FLOAT(1.0f/(optionsCP->st_dwCycles)
			*log(1.0f/optionsCP->st_wInitNeighRadius));

		bFlag=FALSE;
		DWORD dwCount;
//		WORD wPixel;
		const DWORD dwPixelsPerCycle=(DWORD)((FLOAT)dwSizePixels/optionsCP->st_dwCycles+0.5f);
		DWORD dwStep=wPrime[3];
		for (i=0; i<3&&!bFlag; i++)
			if (dwSizePixels%wPrime[i])
			{
				bFlag=TRUE;
				dwStep=wPrime[i];
			}

		i=0;
		pDlg->SetProgress(0);
		dwCycles=0;
		dwCount=0;
		wUsed=0;
		pDlg->SetProgressRange(0,optionsCP->st_dwCycles);
		ZeroMemory(pMap,optionsCP->st_wClusters);
		pDlg->SetStatusOperation(_T("Learning"));
		QueryPerformanceCounter(&liCount1);
		do
		{
			i+=dwStep;
			if (i>=dwSizePixels)
				i-=dwSizePixels;
//			dwPixel=i/3;
			Net.SetAxons(CP_INPUT_LAYER,&(lpSrcBits[i/bm.bmWidth][i%bm.bmWidth*bBytesPixel]));
			Net.PropagateKohonenMin();
			wClusters[0][i]=Net.GetWinnerNeuron();
			if (!pMap[wClusters[0][i]])
			{
				pMap[wClusters[0][i]]=TRUE;
				wUsed++;
			}
			Net.LearnKohonen(pLearnRate);

			dwCount++;
			if (dwCount==dwPixelsPerCycle)
			{
				dwCount=0;
				dwCycles++;
				fLearnRate=(FLOAT)(optionsCP->st_fInitLearnRate*exp(fLearnPower*dwCycles));
				Net.SetLearnRate(fLearnRate);
				if (optionsCP->st_bTrainNeighbours)
				{
					wNeighRadius=(WORD)(optionsCP->st_wInitNeighRadius*exp(fNeighPower*dwCycles));
					Net.SetNeighbouringRadius(wNeighRadius);
				}
				Net.CalculateLearnRate(pLearnRate);
				QueryPerformanceCounter(&liCount2);
				pDlg->StepProgress();
				strText.Format(_T("%d"),dwCycles);
				pDlg->SetStatus(1,_T("Learning cycle:"),strText);
				strText.Format(_T("%d"),optionsCP->st_wClusters-wUsed);
				pDlg->SetStatus(2,_T("Unused clusters:"),strText);
				strText.Format(_T("%.5f"),(FLOAT)(liCount2.QuadPart-liCount1.QuadPart)/liFreq.QuadPart);
				pDlg->SetStatus(3,_T("Time:"),strText);
			}
		}
		while (dwCycles<optionsCP->st_dwCycles);

		pDlg->SetStatusOperation(_T("Compression"));
		wUsed=0;
		pDlg->SetProgressRange(0,bm.bmWidth);
		pDlg->SetProgress(0);
		ZeroMemory(pMap,optionsCP->st_wClusters);
		for (j=0; j<bm.bmWidth; j++)
		{
			dwIndex=j*bBytesPixel;
			for (i=0; i<bm.bmHeight; i++)
			{
				Net.SetAxons(CP_INPUT_LAYER,&(lpSrcBits[i][dwIndex]));
				Net.PropagateKohonenMin();
				wClusters[i][j]=Net.GetWinnerNeuron();
				if (!pMap[wClusters[i][j]])
				{
					pMap[wClusters[i][j]]=TRUE;
					wUsed++;
				}
			}
			pDlg->StepProgress();
		}
		QueryPerformanceCounter(&liCount2);
		strText.Format(_T("%d"),optionsCP->st_wClusters-wUsed);
		pDlg->SetStatus(2,_T("Unused clusters:"),strText);
		strText.Format(_T("%.5f"),(FLOAT)(liCount2.QuadPart-liCount1.QuadPart)/liFreq.QuadPart);
		pDlg->SetStatus(3,_T("Time:"),strText);
		
		lpSrc=new BYTE[dwSrcLen];
		ASSERT(lpSrc!=NULL);
		
		for (i=0; i<optionsCP->st_wClusters; i++)
		{
			Net.GetWeightsFromNeuron(KOHONEN_LAYER,(WORD)i,pWeights);
			dwIndex=i*bBytesPixel;
			for (j=0; j<bBytesPixel; j++)
				lpSrc[dwIndex+j]=(BYTE)(pWeights[j]+0.5);
		}
	}
	
	delete []pMap;
	delete []pLearnRate;

	LPBYTE *lpResultBits=AllocateByteMatrix(bm.bmHeight,bm.bmWidthBytes);
	ASSERT(lpResultBits!=NULL);
	for (j=0; j<bm.bmWidth; j++)
	{
		dwIndex=j*bBytesPixel;
		for (i=0; i<bm.bmHeight; i++)	
		{
			for (k=0; k<bBytesPixel; k++)
				lpResultBits[i][dwIndex+k]=lpSrc[wClusters[i][j]*bBytesPixel+k];
		}
	}

	CBitmapKit *pComprBitmap=pDlg->GetRightBitmap();
	pComprBitmap->CreateBitmap(bm,lpResultBits[0]);
	pDlg->DisplayRightBitmap();

	DestroyMatrix((LPVOID *)lpResultBits);

	pDlg->SetStatusOperation(_T("Writing out file"));
	if (!QueryZLibVersion())
		return FALSE;

	CFile file;
	file.Open(pDlg->GetComprFileName(),CFile::modeCreate | CFile::modeWrite);/*cur directory!!!*/
	COMPRFILEINFO cfInfo(COMPRFILE_NNIC,COMPRTYPE_CP,bm);
	file.Write(&cfInfo,sizeof(COMPRFILEINFO));

	BYTE *lpDest=new BYTE[dwDestLen];
	ASSERT(lpDest!=NULL);

	if (!Compress2(lpDest,&dwDestLen,lpSrc,dwSrcLen,Z_BEST_COMPRESSION))
		return FALSE;

	COMPRDATAINFO cdInfo(dwSrcLen,dwDestLen);
	file.Write(&cdInfo,sizeof(COMPRDATAINFO));
	
	file.Write(lpDest,dwDestLen);

	delete []lpSrc;
	delete []lpDest;

	LPBYTE lpClusters=(LPBYTE)(wClusters[0]);
	if (optionsCP->st_wClusters<=256)
	{
		dwSrcLen=dwSizePixels*sizeof(BYTE);
		/*Packing*/
		for (i=1; i<dwSizePixels; i++)
			lpClusters[i]=LOBYTE(wClusters[0][i]);
	}
	else
		dwSrcLen=dwSizePixels*sizeof(WORD);
		
	dwDestLen=GetBufferSize(dwSrcLen);

	lpDest=new BYTE[dwDestLen];
	ASSERT(lpDest!=NULL);

	if (!Compress2(lpDest,&dwDestLen,lpClusters,dwSrcLen,Z_BEST_COMPRESSION))
		return FALSE;

	DestroyMatrix((LPVOID *)wClusters);

	cdInfo.Set(dwSrcLen,dwDestLen);
	file.Write(&cdInfo,sizeof(COMPRDATAINFO));

	file.Write(lpDest,dwDestLen);
	delete []lpDest;

	pDlg->SetStatusOperation(_T("Done"));
	CReportDlg dlgReport;
	dlgReport.SetFileDirectory(file.GetFileName(),file.GetFilePath());
	dlgReport.SetFileSize(pDlg->GetLoadedFileSize(),file.GetLength());
	dlgReport.SetImageSize(bm.bmWidth,bm.bmHeight);
	dlgReport.SetOriginalColors((BYTE)bm.bmBitsPixel,pBitmap->GetColors());
	dlgReport.SetComprMethod(_T("Counter Propagation"));
	DOUBLE fRed=pBitmap->GetPSNR(pComprBitmap,0);
	DOUBLE fGreen=pBitmap->GetPSNR(pComprBitmap,1);
	DOUBLE fBlue=pBitmap->GetPSNR(pComprBitmap,2);
	dlgReport.SetPSNR(fRed,fGreen,fBlue,(fRed+fGreen+fBlue)/bBytesPixel,
		pBitmap->GetPSNRFullChannel(pComprBitmap));
	dlgReport.SetComprTime((FLOAT)(liCount2.QuadPart-liCount1.QuadPart)/liFreq.QuadPart);
	file.Close();
	if (optionsGENERAL->st_bShowReport)
		dlgReport.DoModal();

	if (optionsGENERAL->st_bWriteReport)
		pDlg->WriteReportFile(dlgReport.GetReportString());

	pDlg->EnableOptions(TRUE);
	pDlg->EnableCompress(TRUE);
	pDlg->SetFinished(TRUE);
	return FALSE;
}

UINT StartDCT(LPVOID pParam)
{
	CNNICDlg* pDlg = (CNNICDlg *)pParam;

	if (pDlg == NULL ||
		!pDlg->IsKindOf(RUNTIME_CLASS(CNNICDlg)))
		return TRUE;

	pDlg->SetStatusOperation(_T("Initialization"));
	OPTIONSDCT *optionsDCT=pDlg->GetOptionsDCT();
	OPTIONSGENERAL *optionsGENERAL=pDlg->GetOptionsGENERAL();
	ASSERT(optionsDCT!=NULL);

	pDlg->EnableCompress(FALSE);
	pDlg->EnableStop(TRUE);

	DCT Dct(DCT_SIZE);
	Dct.SetQuality(optionsDCT->st_bQuality);
	CBitmapKit *pBitmap=pDlg->GetLeftBitmap();
	BITMAP bm;
    pBitmap->GetBitmap(&bm);
	const BYTE bBytesPixel=pBitmap->GetBytesPixel();
	const DWORD dwBlockSize=DCT_BLOCK_SIZE*sizeof(SHORT);

	SHORT sZigZag[DCT_SIZE*DCT_SIZE];
	FLOAT fShift;
	if (optionsDCT->st_bShift)
		fShift=-128.0;
	else
		fShift=0.0;
	const FLOAT fSignalWidth=256.0;

	FLOAT fPixel;
	INT i,j,k,m,l;

	LPBYTE *lpResultBits=AllocateByteMatrix(bm.bmHeight,bm.bmWidthBytes);
	ASSERT(lpResultBits!=NULL);

	FLOAT **pBlock=AllocateFloatMatrix(DCT_SIZE,DCT_SIZE);
	ASSERT(pBlock!=NULL);

	SHORT *lpSrc=new SHORT[bm.bmWidth*bm.bmHeight*bBytesPixel];
	ASSERT(lpSrc!=NULL);

	LARGE_INTEGER liFreq;
	LARGE_INTEGER liCount1;
	LARGE_INTEGER liCount2;
	
	DWORD  dwCount=0;
	CString strText;
	pDlg->SetProgressRange(0,bm.bmHeight/DCT_SIZE);
	pDlg->SetProgress(0);
	pDlg->SetStatusOperation(_T("Compression"));
	QueryPerformanceFrequency(&liFreq);
	QueryPerformanceCounter(&liCount1);
	for (i=0; i<bm.bmHeight; i+=DCT_SIZE)
	{
		for (j=0; j<bm.bmWidth; j+=DCT_SIZE)
			for (k=0; k<bBytesPixel; k++)
			{
				pBitmap->GetBlock(CRect(j,i,j+DCT_SIZE,i+DCT_SIZE),
					pBlock[0],k,fShift,fSignalWidth);
				Dct.DCTransform(pBlock[0]);
				Dct.QuantizateBlock();
				Dct.ZigZagSequence(sZigZag);
				Dct.DequantizateBlock();
				Dct.IDCTransform();
				Dct.GetBlock(pBlock[0]);
				for (l=0; l<DCT_SIZE; l++)
					for (m=0; m<DCT_SIZE; m++)
					{
						fPixel=-fShift+pBlock[l][m]+0.5f;
						if (fPixel<0.0)
							fPixel=0.0;
						if (fPixel>255.0)
							fPixel=255.0;
						lpResultBits[i+l][(j+m)*bBytesPixel+k]=(BYTE)fPixel;
					}
				CopyMemory(&(lpSrc[dwCount]),sZigZag,DCT_BLOCK_SIZE*sizeof(SHORT));
				dwCount+=DCT_BLOCK_SIZE;
			}
		QueryPerformanceCounter(&liCount2);
		pDlg->StepProgress();
		strText.Format(_T("%.5f"),(FLOAT)(liCount2.QuadPart-liCount1.QuadPart)/liFreq.QuadPart);
		pDlg->SetStatus(1,_T("Time:"),strText);	
	}

	DestroyMatrix((LPVOID *)pBlock);
	CBitmapKit *pComprBitmap=pDlg->GetRightBitmap();
	pComprBitmap->CreateBitmap(bm,lpResultBits[0]);
	pDlg->DisplayRightBitmap();

	DestroyMatrix((LPVOID *)lpResultBits);

	pDlg->SetStatusOperation(_T("Writing out file"));
	if (!QueryZLibVersion())
		return FALSE;

	CFile file;
	file.Open(pDlg->GetComprFileName(),CFile::modeCreate | CFile::modeWrite);/*cur directory!!!*/

	COMPRFILEINFO cfInfo(COMPRFILE_NNIC,COMPRTYPE_DCT,bm);
	file.Write(&cfInfo,sizeof(COMPRFILEINFO));

	COMPRDATADCT cdDCT(DCT_SIZE,optionsDCT->st_bQuality,fShift,fSignalWidth);
	file.Write(&cdDCT,sizeof(COMPRDATADCT));

	DWORD dwSrcLen=bm.bmWidth*bm.bmHeight*bBytesPixel*sizeof(SHORT); 
	DWORD dwDestLen=GetBufferSize(dwSrcLen);
	LPBYTE lpDest=new BYTE[dwDestLen];
	ASSERT(lpDest!=NULL);
	if (!Compress2(lpDest,&dwDestLen,(LPBYTE)lpSrc,dwSrcLen,Z_BEST_COMPRESSION))
		return FALSE;

	delete []lpSrc;

	COMPRDATAINFO cdInfo(dwSrcLen,dwDestLen);
	file.Write(&cdInfo,sizeof(COMPRDATAINFO));
	
	file.Write(lpDest,dwDestLen);
	delete []lpDest;
		
	pDlg->SetStatusOperation(_T("Done"));
	CReportDlg dlgReport;
	dlgReport.SetFileDirectory(file.GetFileName(),file.GetFilePath());
	dlgReport.SetFileSize(pDlg->GetLoadedFileSize(),file.GetLength());
	
	dlgReport.SetImageSize(bm.bmWidth,bm.bmHeight);
	dlgReport.SetOriginalColors((BYTE)bm.bmBitsPixel,pBitmap->GetColors());
	dlgReport.SetComprMethod(_T("DC Transform"));
	if (bBytesPixel>1)
	{
		DOUBLE fRed=pBitmap->GetPSNR(pComprBitmap,0);
		DOUBLE fGreen=pBitmap->GetPSNR(pComprBitmap,1);
		DOUBLE fBlue=pBitmap->GetPSNR(pComprBitmap,2);
		dlgReport.SetPSNR(fRed,fGreen,fBlue,(fRed+fGreen+fBlue)/3,
		pBitmap->GetPSNRFullChannel(pComprBitmap));
	}
	else
		dlgReport.SetPSNR(pBitmap->GetPSNRFullChannel(pComprBitmap));
	dlgReport.SetComprTime((FLOAT)(liCount2.QuadPart-liCount1.QuadPart)/liFreq.QuadPart);
	file.Close();
	if (optionsGENERAL->st_bShowReport)
		dlgReport.DoModal();

	if (optionsGENERAL->st_bWriteReport)
		pDlg->WriteReportFile(dlgReport.GetReportString());

	pDlg->EnableOptions(TRUE);
	pDlg->EnableCompress(TRUE);
	pDlg->SetFinished(TRUE);
	return FALSE;
}

UINT StartDCTBack(LPVOID pParam)
{
	CNNICDlg* pDlg = (CNNICDlg *)pParam;

    if (pDlg == NULL ||
        !pDlg->IsKindOf(RUNTIME_CLASS(CNNICDlg)))
    return TRUE;

//	pDlg->EnableCompress(FALSE);
//	pDlg->EnableStop(TRUE);
//
//	DCT dct(DCT_SIZE);
//	CBitmapKit *pBitmap=pDlg->GetLeftBitmap();
//
//	FLOAT Segments[32][32][64];
//
//	for (UINT i=0; i<32; i++)
//		for (UINT j=0; j<32; j++)
//		{
//			CPoint point(j*8,i*8);
//			CSize size(8,8);
//			CRect rect(point,size);
//			pBitmap->GetSegment(rect,Segments[i][j],0,-127.0/*0.0*/,256.0);
//		}
//
//	/******* DCT ******/
//	for (i=0; i<32; i++)
//		for (UINT j=0; j<32; j++)
//		{
//			dct.DCTransform(Segments[i][j]);
//			dct.GetBlock(Segments[i][j]);
//			for (UINT k=0; k<64; k++)
//				Segments[i][j][k]=-0.5f+(1016.0f+Segments[i][j][k])/2040.0f;
//		}
//
//	/***** Back ********/
//	OPTIONSBP *optionsBP=pDlg->GetOptionsBP();
//	WORD wInput=optionsBP->st_wInputBlock*optionsBP->st_wInputBlock;
//	WORD wHidden=optionsBP->st_wHiddenBlock*optionsBP->st_wHiddenBlock;
//	WORD wLayers[4];
//	BYTE bInputLayer=0;
//	BYTE bHiddenLayer;
//	BYTE bOutputLayer;
//	if (optionsBP->st_bSecondHidden)
//	{
//		wLayers[0]=wInput;
//		wLayers[1]=optionsBP->st_wNeurons;
//		wLayers[2]=wHidden;
//		wLayers[3]=wInput;
//		bHiddenLayer=2;
//		bOutputLayer=3;
//	}
//	else
//	{
//		wLayers[0]=wInput;
//		wLayers[1]=wHidden;
//		wLayers[2]=wInput;
//		bHiddenLayer=1;
//		bOutputLayer=2;
//	}
//	
//	NetBP Net(bOutputLayer+1,wLayers);
//	Net.SetSignalBoundaries(-0.5,0.5);
//
//	switch (optionsBP->st_bSigmoidType)
//	{
//	case 0:
//		Net.SetSigmoidType(SIGMOID_TYPE_ORIGINAL);
//		break;
//	case 1:
//		Net.SetSigmoidType(SIGMOID_TYPE_HYPERTAN);
//		break;
//	}
//
//	Net.SetSigmoidAlpha(optionsBP->st_fSigmoidAlpha);
//	Net.SetScaleParam(0.5);
//	Net.UseBiases(optionsBP->st_bUseBiases);
//	UINT uiMethod;
//	switch (optionsBP->st_bLearnMode)
//	{
//	case 0:
//		uiMethod=LEARN_TYPE_SEQUENTIAL;
//		break;
//	case 1:
//		uiMethod=LEARN_TYPE_BATCH;
//		break;
//	}
//	if (optionsBP->st_bUseMomentum)
//		uiMethod|=LEARN_TYPE_MOMENTUM;
//	
//	Net.SetLearnType(uiMethod);
//	Net.InitWeights();
//
//	LARGE_INTEGER liFreq;
//	LARGE_INTEGER liCount1;
//	LARGE_INTEGER liCount2;
//
//	UINT uiPatterns;
//	FLOAT fNetError;
//	ULONG ulCycles;
//	BOOLEAN bFlag;
//	CString strText;
//
//	QueryPerformanceFrequency(&liFreq);
//	if (uiMethod & LEARN_TYPE_SEQUENTIAL)
//	{
//		ULONG ulLearnCount=0;
//		ULONG ulMomentCount=0;
//		INT iLearnSign=1;
//		INT iMomentSign=1;
//		if (optionsBP->st_fInitLearnRate>=optionsBP->st_fFinalLearnRate)
//			iLearnSign=-1;
//		if (optionsBP->st_fInitMoment>=optionsBP->st_fFinalMoment)
//			iMomentSign=-1;
//		FLOAT fLearnRate=optionsBP->st_fInitLearnRate;
//		FLOAT fMomentumParam=optionsBP->st_fInitMoment;
//		Net.SetLearnRate(optionsBP->st_fInitLearnRate);
//		Net.SetMomentumParam(optionsBP->st_fInitMoment);
//		ulCycles=0;
//		QueryPerformanceCounter(&liCount1);
//		do
//		{
//			bFlag=TRUE;
//			uiPatterns=0;
//			for (i=0; i<32; i++)
//				for (UINT j=0; j<32; j++)
//				{
//					Net.SetAxons(0,Segments[i][j]);
//					Net.ForwardPass(0);
//					if ((fNetError=Net.TargetFunction(bOutputLayer,Segments[i][j]))>
//							optionsBP->st_fMinError)
//					{
//						Net.BackwardPass(Segments[i][j]);
//						bFlag=FALSE;
//					}
//					else
//						uiPatterns++;
//					ulLearnCount++;
//					if (ulLearnCount==optionsBP->st_dwLearnSteps
//							&&optionsBP->st_fFinalLearnRate!=optionsBP->st_fInitLearnRate)
//					{
//						ulLearnCount=0;
//						fLearnRate+=iLearnSign*optionsBP->st_fLearnChangeRate;
//						if (iLearnSign==-1&&fLearnRate<optionsBP->st_fFinalLearnRate)
//							fLearnRate=optionsBP->st_fFinalLearnRate;
//						if (iLearnSign==1&&fLearnRate>optionsBP->st_fFinalLearnRate)
//							fLearnRate=optionsBP->st_fFinalLearnRate;
//						Net.SetLearnRate(fLearnRate);
//					}
//					if (uiMethod & LEARN_TYPE_MOMENTUM)
//					{
//						ulMomentCount++;
//						if (ulMomentCount==optionsBP->st_dwMomentSteps
//							&&optionsBP->st_fFinalMoment!=optionsBP->st_fInitMoment)
//						{
//							ulMomentCount=0;
//							fMomentumParam+=iMomentSign*optionsBP->st_fMomentChangeRate;
//							if (iMomentSign==-1&&fMomentumParam<optionsBP->st_fFinalMoment)
//								fMomentumParam=optionsBP->st_fFinalMoment;
//							if (iMomentSign==1&&fMomentumParam>optionsBP->st_fFinalMoment)
//								fMomentumParam=optionsBP->st_fFinalMoment;
//							Net.SetMomentumParam(fMomentumParam);
//						}
//					}
//				}
//			QueryPerformanceCounter(&liCount2);
//			ulCycles++;
//			pDlg->SetDlgItemInt(IDC_STATIC_CYCLES,ulCycles);
//			strText.Format(_T("%f %f"),fNetError,Net.GetLearnRate());
//			pDlg->SetDlgItemText(IDC_STATIC_AVDIST,strText);
//			strText.Format(_T("%f"),(FLOAT)(liCount2.QuadPart-liCount1.QuadPart)/liFreq.QuadPart);
//			pDlg->SetDlgItemText(IDC_STATIC_TIME,strText);
//			pDlg->SetDlgItemInt(IDC_STATIC_UNUSED,uiPatterns);
//		}
//		while (uiPatterns<optionsBP->st_wPatterns&&ulCycles<optionsBP->st_dwLearnCycles&&!bFlag);
//	}
//	if (uiMethod & LEARN_TYPE_BATCH)
//	{
//		FLOAT fError;
//		FLOAT *fPatterns[1024];
//		ulCycles=0;
//		Net.SetLearnRate(0.9f);
//		QueryPerformanceCounter(&liCount1);
//		do
//		{
//			fNetError=0.0;
//			uiPatterns=0;
//			bFlag=TRUE;
//			for (i=0; i<32; i++)
//				for (UINT j=0; j<32; j++)
//				{
//					Net.SetAxons(0,Segments[i][j]);
//					Net.ForwardPass(0);
//					if ((fError=Net.TargetFunction(bOutputLayer,Segments[i][j]))>
//							optionsBP->st_fMinError)
//					{
//						Net.UpdateGradients(Segments[i][j]);
//						fNetError+=fError;
//						fPatterns[uiPatterns++]=&Segments[i][j][0];
//						bFlag=FALSE;
//					}
//				}
//
//			if (!bFlag)
//				fNetError=Net.BackwardPass(uiPatterns,(const FLOAT **)&fPatterns[0],fNetError);
//			QueryPerformanceCounter(&liCount2);
//			ulCycles++;
//			pDlg->SetDlgItemInt(IDC_STATIC_CYCLES,ulCycles);
//			strText.Format(_T("%f %f"),fNetError,Net.GetLearnRate());
//			pDlg->SetDlgItemText(IDC_STATIC_AVDIST,strText);
//			strText.Format(_T("%f"),(FLOAT)(liCount2.QuadPart-liCount1.QuadPart)/liFreq.QuadPart);
//			pDlg->SetDlgItemText(IDC_STATIC_TIME,strText);
//			pDlg->SetDlgItemInt(IDC_STATIC_UNUSED,1024-uiPatterns);
//		}
//		while ((1024-uiPatterns)<optionsBP->st_wPatterns&&ulCycles<optionsBP->st_dwLearnCycles&&!bFlag);
//	}
//
//	/******* Back ********/
//	BYTE bHiddenPatterns[32][32][16];
//	FLOAT fHiddenOutput[16];
//
//	for (i=0; i<32; i++)
//		for (ULONG j=0; j<32; j++)
//		{
//			Net.SetAxons(bInputLayer,Segments[i][j]);
//			Net.ForwardPass(bInputLayer);
//			Net.GetAxons(bHiddenLayer,fHiddenOutput);
//			for (ULONG k=0; k<16; k++)
//				bHiddenPatterns[i][j][k]=(BYTE)floor((fHiddenOutput[k]+0.5f)/1.0f*255.0f+0.5f);
//		}
//
////	FLOAT fOutput[64];
//	FLOAT fHiddenInput[16];
////	ULONG ulSize=pImage->GetImageSize();
////	BYTE *pResult=new BYTE[ulSize];
//	for (i=0; i<32; i++)
//		for (ULONG j=0; j<32; j++)
//		{
//			for (ULONG k=0; k<16; k++)
//				fHiddenInput[k]=-0.5f+bHiddenPatterns[i][j][k]/255.0f;
//			Net.SetAxons(bHiddenLayer,fHiddenInput);
//			Net.ForwardPass(bHiddenLayer);
//			Net.GetAxons(bOutputLayer,Segments[i][j]);
//			for (k=0; k<64; k++)
//				Segments[i][j][k]=-1016.0f+(Segments[i][j][k]+0.5f)/1.0f*2040.0f;
//			dct.IDCTransform(Segments[i][j]);
//			dct.GetBlock(Segments[i][j]);
//		}
//
//
//	ULONG ulSize=pBitmap->GetSizeClear();
//	BYTE *pResult=new BYTE[ulSize];
//	FLOAT fPixel;
//	for (i=0; i<32; i++)
//		for (ULONG j=0; j<32; j++)
//		{
//			for (ULONG k=0; k<8; k++)
//				for (ULONG m=0; m<8; m++)
//				{
//					fPixel=(FLOAT)floor(Segments[i][j][8*k+m]+127.0+0.5f);
//					if (fPixel<0.0)
//						fPixel=0.0;
//					if (fPixel>255.0)
//						fPixel=255.0;
//					pResult[(i*8+k)*256+(j*8+m)]=(BYTE)fPixel;
//				}
//		}
//

//	ULONG ulSize=pImage->GetImageSize();
//	BYTE *pResult=new BYTE[ulSize];
//	FLOAT fPixel;
//	FLOAT fOutput[64];
//	CFile file;
//	file.Open("result.dat",CFile::modeRead);
//	for (i=0; i<32; i++)
//		for (UINT j=0; j<32; j++)
//		{
//			file.Read(fOutput,sizeof(FLOAT)*64);
//			for (ULONG k=0; k<8; k++)
//				for (ULONG m=0; m<8; m++)
//				{
//					fPixel=(FLOAT)floor(fOutput[8*k+m]*255+0.5);
//					pResult[(i*8+k)*256+(j*8+m)]=(BYTE)fPixel;
//				}
//		}

//	CBitmapKit *pComprBitmap=pDlg->GetRightBitmap();
//	pComprBitmap->CreateIndirect(pBitmap);
//	p
//	pDlg->DisplayComprBitmap();
//
//	if (zlibVersion()[0] != ZLIB_VERSION[0]) 
//	{
//		AfxMessageBox(_T("Incompatible ZLib version"),MB_OK);
//        return TRUE;
//
//    } 
//	else 
//		if (strcmp(zlibVersion(), ZLIB_VERSION) != 0) 
//			AfxMessageBox(_T("WARNING: different ZLib version"),MB_OK);
//
//	CFile file;
//	file.Open(_T("image.tmp"),CFile::modeCreate | CFile::modeWrite);/*cur directory!!!*/
//
//	COMPRFILEINFO cfInfo(COMPRFILE_NNIC,COMPRTYPE_BP,pImage->GetImageWidth(),
//		pImage->GetImageHeight(),ulSize);
//	file.Write(&cfInfo,sizeof(COMPRFILEINFO));
//
//	ULONG uiSrcLen=8*8*4*4*sizeof(FLOAT);
//	ULONG uiDestLen=(ULONG)(uiSrcLen*100.1/100+0.5)+12;
//
//	BYTE *pSrc=new BYTE[uiSrcLen];
//	ASSERT(pSrc!=NULL);
//	BYTE *pDest=new BYTE[uiDestLen];
//	ASSERT(pDest!=NULL);
//	ZeroMemory(pDest,uiDestLen);
//
//	Net.CopyWeights(pSrc,bHiddenLayer);
//
//	ASSERT(compress(pDest,&uiDestLen,pSrc,uiSrcLen)==Z_OK);
//
//	file.Write(&uiSrcLen,sizeof(ULONG));
//	file.Write(&uiDestLen,sizeof(ULONG));
//	file.Write(pDest,uiDestLen);
//
//	delete []pSrc;
//	delete []pDest;
//	uiSrcLen=32*32*16*sizeof(BYTE);
//	uiDestLen=(ULONG)(uiSrcLen*100.1/100+0.5)+12;
//
//	pDest=new BYTE[uiDestLen];
//	ASSERT(pDest!=NULL);
//	ZeroMemory(pDest,uiDestLen);
//
//	ASSERT(compress(pDest,&uiDestLen,(BYTE *)bHiddenPatterns,uiSrcLen)==Z_OK);
//
//	file.Write(&uiSrcLen,sizeof(ULONG));
//	file.Write(&uiDestLen,sizeof(ULONG));
//	file.Write(pDest,uiDestLen);
//	
//	CReportDlg dlgReport;
//	dlgReport.SetFileName(file.GetFileName());
//	dlgReport.SetDirectory(file.GetFilePath());
//	dlgReport.SetFileSize(pDlg->GetOriginalFileSize(),file.GetLength());
//	file.Close();
//	dlgReport.SetImageSize(pImage->GetImageWidth(),pImage->GetImageHeight());
//	dlgReport.SetOriginalColors(pImage->GetBitsPerPixel(),pImage->GetColors());
//	dlgReport.SetComprMethod(_T("DCT with Back Propagation"));
//	dlgReport.SetPSNR(pImage->GetPSNRFullChannel(*pComprImage));
//	dlgReport.SetComprTime((FLOAT)(liCount2.QuadPart-liCount1.QuadPart)/liFreq.QuadPart);
//	dlgReport.DoModal();

	pDlg->EnableOptions(TRUE);
	pDlg->EnableCompress(TRUE);
	pDlg->SetFinished(TRUE);
	return FALSE;
}

UINT StartDeCompress(LPVOID pParam)
{
	CNNICDlg* pDlg = (CNNICDlg *)pParam;

    if (pDlg == NULL ||
        !pDlg->IsKindOf(RUNTIME_CLASS(CNNICDlg)))
    return FALSE;

	pDlg->SetStatusOperation(_T("Initialization"));
	CString strFileName=pDlg->GetLoadedFileName();

	CFile file(strFileName,CFile::modeRead);
	COMPRFILEINFO cfInfo;
	file.Read(&cfInfo,sizeof(COMPRFILEINFO));

	DWORD i,j,k,l,m;
	DWORD dwIndex,dwXIndex,dwYIndex;
	COMPRDATAINFO cdInfo;

	CString strText;
	LARGE_INTEGER liFreq;
	LARGE_INTEGER liCount1;
	LARGE_INTEGER liCount2;
		
	const bBytesPixel=BITS2BYTES(cfInfo.bmBitsPixel);

	LPBYTE *lpBits=NULL;

	if (cfInfo.st_dwFileType!=COMPRFILE_NNIC)
	{
		file.Close();
		AfxMessageBox(_T("Unknown file format!"),MB_OK);
		return FALSE;
	}

	if (!QueryZLibVersion())
		return FALSE;

	BOOLEAN bFlag=FALSE;

	if (cfInfo.st_bComprMethod==COMPRTYPE_BP)
	{
		bFlag=TRUE;
		COMPRDATABP cdBP;
		file.Read(&cdBP,sizeof(COMPRDATABP));
		WORD wLayerRank[2]={cdBP.st_wInputLayer,cdBP.st_wOutputLayer};
		NetBP Net(2,wLayerRank);
		switch (cdBP.st_bSigmoidType)
		{
		case 0:
			Net.SetSigmoidType(SIGMOID_TYPE_ORIGINAL);
			break;
		case 1:
			Net.SetSigmoidType(SIGMOID_TYPE_HYPERTAN);
			break;
		}

		Net.SetSigmoidAlpha(cdBP.st_fSigmoidAlpha);
		Net.SetScaleParam(cdBP.st_fScaleParam);
		Net.UseBiases(cdBP.st_bUseBiases);
		Net.SetSignalBoundaries(cdBP.st_fShift,-cdBP.st_fShift);

		file.Read(&cdInfo,sizeof(COMPRDATAINFO));

		LPBYTE *lpSrc=new LPBYTE;
		lpSrc[0]=new BYTE[cdInfo.st_dwSrcLen];
		ASSERT(lpSrc!=NULL);
		LPBYTE lpDest=new BYTE[cdInfo.st_dwDestLen];
		ASSERT(lpDest!=NULL);

		pDlg->SetStatusOperation(_T("Decompression"));
		file.Read(lpDest,cdInfo.st_dwDestLen);
		if (!UnCompress(lpSrc[0],&cdInfo.st_dwSrcLen,lpDest,cdInfo.st_dwDestLen))
			return FALSE;
		delete []lpDest;

		const BYTE BP_OUTPUT_LAYER=1;
		Net.InitWeights(BP_OUTPUT_LAYER,lpSrc[0]);
	
		delete []lpSrc[0];
		delete lpSrc;
		
		file.Read(&cdInfo,sizeof(COMPRDATAINFO));

		const BYTE bBlockSize=(BYTE)sqrtf(cdBP.st_wOutputLayer);
		const DWORD dwHeightBlocks=cfInfo.bmHeight/bBlockSize;
		const DWORD dwWidthBlocks=cfInfo.bmWidth/bBlockSize;

		lpSrc=AllocateByteMatrix(dwHeightBlocks,dwWidthBlocks*cdBP.st_wInputLayer);
		ASSERT(lpSrc!=NULL);
		lpDest=new BYTE[cdInfo.st_dwDestLen];
		ASSERT(lpDest!=NULL);

		file.Read(lpDest,cdInfo.st_dwDestLen);
		file.Close();
		if (!UnCompress(lpSrc[0],&cdInfo.st_dwSrcLen,lpDest,cdInfo.st_dwDestLen))
			return FALSE;
		delete []lpDest;

		lpBits=AllocateByteMatrix(cfInfo.bmHeight,cfInfo.bmWidthBytes);
		ASSERT(lpBits!=NULL);

		FLOAT *pInput=new FLOAT[cdBP.st_wInputLayer];
		ASSERT(pInput!=NULL);

		FLOAT **pOutput=AllocateFloatMatrix(bBlockSize,bBlockSize);
		ASSERT(pOutput!=NULL);

		pDlg->SetProgressRange(0,dwWidthBlocks);
		pDlg->SetProgress(0);
		QueryPerformanceFrequency(&liFreq);
		QueryPerformanceCounter(&liCount1);
		for (j=0; j<dwWidthBlocks; j++)
		{
			dwIndex=j*cdBP.st_wInputLayer;
			dwXIndex=j*bBlockSize;
			for (i=0; i<dwHeightBlocks; i++)
			{
				for (k=0; k<cdBP.st_wInputLayer; k++)
					pInput[k]=cdBP.st_fShift+lpSrc[i][dwIndex+k]/255.0f;
			
				Net.SetAxons(BP_INPUT_LAYER,pInput);
				Net.ForwardPass(BP_INPUT_LAYER);
				Net.GetAxons(BP_OUTPUT_LAYER,pOutput[0]);
				dwYIndex=i*bBlockSize;
				for (k=0; k<bBlockSize; k++)
					for (l=0; l<bBlockSize; l++)
						lpBits[dwYIndex+k][dwXIndex+l]=
							(BYTE)((-(cdBP.st_fShift)+pOutput[k][l])/
							cdBP.st_fSignalWidth*255.0+0.5);
			}
			QueryPerformanceCounter(&liCount2);
			pDlg->StepProgress();
			strText.Format(_T("%.5f"),(FLOAT)(liCount2.QuadPart-liCount1.QuadPart)/liFreq.QuadPart);
			pDlg->SetStatus(1,_T("Time:"),strText);	
		}

		delete []pInput;
		DestroyMatrix((LPVOID *)pOutput);
		DestroyMatrix((LPVOID *)lpSrc);
	}

	if (cfInfo.st_bComprMethod==COMPRTYPE_CP)
	{
		bFlag=TRUE;
		file.Read(&cdInfo,sizeof(COMPRDATAINFO));

		BYTE bStep;
		const bBytesPixel=BITS2BYTES(cfInfo.bmBitsPixel);
		if (cdInfo.st_dwSrcLen/bBytesPixel>256)
			bStep=sizeof(WORD);
		else
			bStep=sizeof(BYTE);

		LPBYTE *lpSrc=new LPBYTE;
		lpSrc[0]=new BYTE[cdInfo.st_dwSrcLen];
		ASSERT(lpSrc!=NULL);
		LPBYTE lpDest=new BYTE[cdInfo.st_dwDestLen];
		ASSERT(lpDest!=NULL);

		pDlg->SetStatusOperation(_T("Decompression"));
		file.Read(lpDest,cdInfo.st_dwDestLen);
		if (!UnCompress(lpSrc[0],&cdInfo.st_dwSrcLen,lpDest,cdInfo.st_dwDestLen))
			return FALSE;
		delete []lpDest;

		LPBYTE lpColorMap=new BYTE[cdInfo.st_dwSrcLen];
		ASSERT(lpColorMap!=NULL);
		CopyMemory(lpColorMap,lpSrc[0],cdInfo.st_dwSrcLen);

		delete []lpSrc[0];
		delete lpSrc;
				
		file.Read(&cdInfo,sizeof(COMPRDATAINFO));

		lpSrc=AllocateByteMatrix(cfInfo.bmHeight,cfInfo.bmWidth*bStep);
		ASSERT(lpSrc!=NULL);
		lpDest=new BYTE[cdInfo.st_dwDestLen];
		ASSERT(lpDest!=NULL);

		file.Read(lpDest,cdInfo.st_dwDestLen);
		file.Close();
		if (!UnCompress(lpSrc[0],&cdInfo.st_dwSrcLen,lpDest,cdInfo.st_dwDestLen))
			return FALSE;
		delete []lpDest;

		lpBits=AllocateByteMatrix(cfInfo.bmHeight,cfInfo.bmWidthBytes);
		ASSERT(lpBits!=NULL);

		DWORD dwColorIndex;
		pDlg->SetProgressRange(0,cfInfo.bmWidth);
		pDlg->SetProgress(0);
		QueryPerformanceFrequency(&liFreq);
		QueryPerformanceCounter(&liCount1);
		for (j=0; j<cfInfo.bmWidth; j++)
		{
			dwIndex=j*bStep;
			dwXIndex=j*bBytesPixel;
			for (i=0; i<cfInfo.bmHeight; i++)
			{
				dwColorIndex=0;
				CopyMemory(&dwColorIndex,&(lpSrc[i][dwIndex]),bStep);
				dwColorIndex*=bBytesPixel;
				for (k=0; k<bBytesPixel; k++)
					lpBits[i][dwXIndex+k]=lpColorMap[dwColorIndex+k];
			}
			QueryPerformanceCounter(&liCount2);
			pDlg->StepProgress();
			strText.Format(_T("%.5f"),(FLOAT)(liCount2.QuadPart-liCount1.QuadPart)/liFreq.QuadPart);
			pDlg->SetStatus(1,_T("Time:"),strText);
		}
		DestroyMatrix((LPVOID *)lpSrc);
		delete []lpColorMap;
	}

	if (cfInfo.st_bComprMethod==COMPRTYPE_DCT)
	{
		bFlag=TRUE;
		COMPRDATADCT cdDCT;
		file.Read(&cdDCT,sizeof(COMPRDATADCT));

		const WORD wBlockSize=cdDCT.st_bDCTSize*cdDCT.st_bDCTSize;
		DWORD dwBlockIndex;
		FLOAT fPixel;

		DCT dct(cdDCT.st_bDCTSize);
		dct.SetQuality(cdDCT.st_bQuality);

		file.Read(&cdInfo,sizeof(COMPRDATAINFO));

		LPBYTE *lpSrc=AllocateByteMatrix(cfInfo.bmHeight/cdDCT.st_bDCTSize,
			cdInfo.st_dwSrcLen/(cfInfo.bmHeight/cdDCT.st_bDCTSize));
		ASSERT(lpSrc!=NULL);
		LPBYTE lpDest=new BYTE[cdInfo.st_dwDestLen];
		ASSERT(lpDest!=NULL);

		pDlg->SetStatusOperation(_T("Decompression"));
		file.Read(lpDest,cdInfo.st_dwDestLen);
		if (!UnCompress(lpSrc[0],&cdInfo.st_dwSrcLen,lpDest,cdInfo.st_dwDestLen))
			return FALSE;
		delete []lpDest;
		
		lpBits=AllocateByteMatrix(cfInfo.bmHeight,cfInfo.bmWidthBytes);
		ASSERT(lpBits!=NULL);
		FLOAT **pBlock=AllocateFloatMatrix(cdDCT.st_bDCTSize,cdDCT.st_bDCTSize);
		ASSERT(pBlock!=NULL);
		pDlg->SetProgressRange(0,cfInfo.bmWidth/cdDCT.st_bDCTSize);
		pDlg->SetProgress(0);
		QueryPerformanceFrequency(&liFreq);
		QueryPerformanceCounter(&liCount1);
		for (j=0; j<cfInfo.bmWidth; j+=cdDCT.st_bDCTSize)
		{
			dwIndex=j/cdDCT.st_bDCTSize*bBytesPixel;
			for (i=0; i<cfInfo.bmHeight; i+=cdDCT.st_bDCTSize)
				for (k=0; k<bBytesPixel; k++)
				{
					dwBlockIndex=(dwIndex+k)*wBlockSize;
					dwBlockIndex*=sizeof(SHORT);
					dct.DeZigZagSequence((SHORT *)&(lpSrc[i/cdDCT.st_bDCTSize][dwBlockIndex]));
					dct.DequantizateBlock();
					dct.IDCTransform();
					dct.GetBlock(pBlock[0]);
					for (l=0; l<cdDCT.st_bDCTSize; l++)
						for (m=0; m<cdDCT.st_bDCTSize; m++)
						{
							fPixel=-cdDCT.st_fShift+pBlock[l][m]+0.5f;
							if (fPixel<0.0)
								fPixel=0.0;
							if (fPixel>255.0)
								fPixel=255.0;
							lpBits[i+l][(j+m)*bBytesPixel+k]=(BYTE)fPixel;
						}
				}
			QueryPerformanceCounter(&liCount2);
			pDlg->StepProgress();
			strText.Format(_T("%.5f"),(FLOAT)(liCount2.QuadPart-liCount1.QuadPart)/liFreq.QuadPart);
			pDlg->SetStatus(1,_T("Time:"),strText);
		}
		delete []pBlock;
		DestroyMatrix((LPVOID *)lpSrc);
	}
	if (!bFlag)
	{
		file.Close();
		AfxMessageBox(_T("Unknown compression method!"),MB_OK);
		return FALSE;
	}

	pDlg->SetStatusOperation(_T("Done"));
	CBitmapKit *pBitmap=pDlg->GetLeftBitmap();
	pBitmap->CreateBitmap(cfInfo.st_bmBitmap,lpBits[0]);
	pDlg->DisplayLeftBitmap();
    DestroyMatrix((LPVOID *)lpBits);
	return FALSE;
}
OPTIONSGENERAL * CNNICDlg::GetOptionsGENERAL(void)
{
	return &m_optionsDefault.st_optionsGENERAL;
}

CString CNNICDlg::GetComprFileName(void)
{
	BOOLEAN bFlag=FALSE;
	CString strFileName=m_optionsDefault.st_optionsGENERAL.st_strDefComprFileName;
	if (m_optionsDefault.st_optionsGENERAL.st_bPromtOverwrite&&
		FileExists(strFileName))
		do
			if (AfxMessageBox(_T("File ")+strFileName+
				_T(" already exists.\nDo you want to replace it?"),MB_YESNO)==IDYES)
				bFlag=TRUE;
			else
			{
				CFileNniDlg dlgFile(FALSE,strFileName);
				dlgFile.SetTitle(_T("Save Compressed File As"));
				if (dlgFile.DoModal()==IDOK)
				{
					strFileName=dlgFile.GetPathName();
					bFlag=TRUE;
				}
			}
		while (!bFlag);
	return strFileName;
}

void CNNICDlg::WriteReportFile(const CString & strReportString)
{
	CStdioFile fileReport;
	switch (m_optionsDefault.st_optionsGENERAL.st_bStoringMethod)
	{
	case REPORT_OVERWRITE:
		fileReport.Open(m_optionsDefault.st_optionsGENERAL.st_strReportFileName,
			CFile::modeCreate | CFile::modeWrite);
		break;
	case REPORT_APPEND:
		fileReport.Open(m_optionsDefault.st_optionsGENERAL.st_strReportFileName,
			CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite);
		if (m_optionsDefault.st_optionsGENERAL.st_bLimitFileSize&&
			fileReport.GetLength()/1024>=m_optionsDefault.st_optionsGENERAL.st_wReportFileSize)
			fileReport.SetLength(0);
		fileReport.SeekToEnd();
		break;
	}

	CString strText;
	strText.Format(_T("------------- %s -------------\n"),
		COleDateTime::GetCurrentTime().Format());
	fileReport.WriteString(strText);
	fileReport.WriteString(strReportString);
	fileReport.WriteString(_T("\n\n"));
	fileReport.Close();
}

void CNNICDlg::SetStatusOperation(const CString & strOperation)
{
	SetDlgItemText(IDC_STATIC_OPERATION,strOperation);
}

void CNNICDlg::SetStatus(const DWORD dwID, const CString & strName, const CString & strValue)
{
	SetDlgItemText(StatusID[dwID-1],strName);
	SetDlgItemText(StatusTextID[dwID-1],strValue);
}

void CNNICDlg::ClearStatus(void)
{
	SetProgress(0);
	SetDlgItemText(IDC_STATIC_OPERATION,_T(""));
	DWORD i;
	for (i=0; i<5; i++)
	{
		SetDlgItemText(StatusID[i],_T(""));
		SetDlgItemText(StatusTextID[i],_T(""));
	}
}

void CNNICDlg::SetProgressRange(const INT iLower, const INT iUpper)
{
	((CProgressCtrl *)GetDlgItem(IDC_PROGRESS_COMPR))->SetRange32(iLower,iUpper);
}
