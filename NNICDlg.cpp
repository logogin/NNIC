// NNICDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NNIC.h"
#include "NNICDlg.h"

#include ".\NeuroNet\NetBP.h"
#include ".\NeuroNet\NetCP.h"
#include "ReportDlg.h"
#include "ResampleDlg.h"
#include "CPDlg.h"
#include "BPDlg.h"
#include "DCTDlg.h"
#include "DCT.h"

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
	//{{AFX_DATA_INIT(CNNICDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	OPTIONSBP optionsBP(0,0,1.0f,TRUE,TRUE,300,0.01f,80,8,4,FALSE,16,0.9f,0.01f,
		1000,0.05f,0.3f,0.1f,1000,0.01f);
	OPTIONSCP optionsCP(NET_TYPE_KOHONEN_GROSSBERG,256,10,0.035f,TRUE,0.3f,0.1f,0.01f,10000,3,0,1,50000);
	OPTIONSDCT optionsDCT(10,TRUE);
	OPTIONS options(optionsBP,optionsCP,optionsDCT);
	CopyMemory(&m_optionsDefault,&options,sizeof(OPTIONS));
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
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_BN_CLICKED(IDC_BUTTON_COMPRESS, OnButtonCompress)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_OPTIONS, OnButtonOptions)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNNICDlg message handlers

BOOL CNNICDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
/*	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}*/

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

void CNNICDlg::OnFileOpen() 
{
	// TODO: Add your command handler code here
	CFileDialog dlgFile(TRUE,_T("bmp"),_T(/*"mandrill256"*/"mandrillGray"));
	dlgFile.m_ofn.lpstrFilter=_T("BMP - Windows Bitmap\x0*.bmp");
	if (dlgFile.DoModal()==IDOK)
	{
		m_strLoadedFileName=dlgFile.GetPathName();
		m_leftBitmap.LoadBMP(m_strLoadedFileName);
//		CResampleDlg dlg(&m_leftBitmap);
//		dlg.DoModal();
		m_leftBitmap.Scale(m_staticLeft.GetDC(),100,100);
		//m_staticLeft.SetBitmap(m_leftBitmap.Scale(256,200)/*dlg.GetHandle()*/);
		//m_staticLeft.SetBitmap(m_leftBitmap.Scale(m_staticLeft.GetDC(),100,100));
		EnableCompress(TRUE);
	}

}

void CNNICDlg::OnButtonCompress() 
{
	// TODO: Add your control notification handler code here
	UINT uiMethod=GetCheckedRadioButton(IDC_RADIO_BACK,IDC_RADIO_DCTBACK);

	switch (uiMethod)
	{
	case IDC_RADIO_BACK:
		EnableOptions(FALSE);
		m_bFinished=FALSE;
		m_threadCompress=AfxBeginThread(StartBP,this);
		break;
	case IDC_RADIO_COUNTER:
		EnableOptions(FALSE);
		m_bFinished=FALSE;
		m_threadCompress=AfxBeginThread(StartCP,this);
		break;
	case IDC_RADIO_DCT:
		EnableOptions(FALSE);
		m_bFinished=FALSE;
		m_threadCompress=AfxBeginThread(StartDCT,this/*,THREAD_PRIORITY_NORMAL,10*1024*1024,0,NULL*/);
		break;
	case IDC_RADIO_DCTBACK:
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

UINT StartBP(LPVOID pParam)
{
	CNNICDlg* pDlg = (CNNICDlg *)pParam;

    if (pDlg == NULL ||
        !pDlg->IsKindOf(RUNTIME_CLASS(CNNICDlg)))
    return TRUE;

	pDlg->EnableCompress(FALSE);
	pDlg->EnableStop(TRUE);

	OPTIONSBP *optionsBP=pDlg->GetOptionsBP();
	UINT uiInput=optionsBP->st_wInputBlock*optionsBP->st_wInputBlock;
	UINT uiHidden=optionsBP->st_wHiddenBlock*optionsBP->st_wHiddenBlock;
	UINT uiLayers[4];
	//BYTE bInputLayer=0;
	BYTE bHiddenLayer;
	BYTE bOutputLayer;
	if (optionsBP->st_bSecondHidden)
	{
		uiLayers[0]=uiInput;
		uiLayers[1]=optionsBP->st_wNeurons;
		uiLayers[2]=uiHidden;
		uiLayers[3]=uiInput;
		bHiddenLayer=2;
		bOutputLayer=3;
	}
	else
	{
		uiLayers[0]=uiInput;
		uiLayers[1]=uiHidden;
		uiLayers[2]=uiInput;
		bHiddenLayer=1;
		bOutputLayer=2;
	}
	
	NetBP Net(bOutputLayer+1,uiLayers);
	Net.SetSignalBoundaries(-0.5,0.5);

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
	Net.SetScaleParam(0.5);
	Net.UseBiases(optionsBP->st_bUseBiases);
	UINT uiMethod;
	//optionsBP->st_wLearnMode=1;
	switch (optionsBP->st_bLearnMode)
	{
	case 0:
		uiMethod=LEARN_TYPE_SEQUENTIAL;
		break;
	case 1:
		uiMethod=LEARN_TYPE_BATCH;
		break;
	}
	if (optionsBP->st_bUseMomentum)
		uiMethod|=LEARN_TYPE_MOMENTUM;
	
	Net.SetLearnType(uiMethod);
	Net.InitWeights();

	CBitmapKit *pBitmap=pDlg->GetLeftBitmap();
	/********!!!!!!!!!!!*********/
//	optionsBP->st_wPatterns=1024;

	FLOAT Segments[32][32][64];
	for (UINT i=0 ;i<32; i++)
		for (UINT j=0; j<32; j++)
		{
			CPoint point(j*8,i*8);
			CSize size(8,8);
			CRect rect(point,size);
			pBitmap->GetSegment(rect,Segments[i][j],0,-0.5f,1.0f);
		}

	LARGE_INTEGER liFreq;
	LARGE_INTEGER liCount1;
	LARGE_INTEGER liCount2;

	UINT uiPatterns;
	FLOAT fNetError;
	ULONG ulCycles;
	BOOLEAN bFlag;
	CString sString;

	QueryPerformanceFrequency(&liFreq);
	if (uiMethod & LEARN_TYPE_SEQUENTIAL)
	{
		ULONG ulLearnCount=0;
		ULONG ulMomentCount=0;
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
		ulCycles=0;
		QueryPerformanceCounter(&liCount1);
		do
		{
			bFlag=TRUE;
			uiPatterns=0;
			for (i=0; i<32; i++)
				for (UINT j=0; j<32; j++)
				{
					Net.SetAxons(0,Segments[i][j]);
					Net.ForwardPass(0);
					if ((fNetError=Net.TargetFunction(bOutputLayer,Segments[i][j]))>
							optionsBP->st_fMinError)
					{
						Net.BackwardPass(Segments[i][j]);
						bFlag=FALSE;
					}
					else
						uiPatterns++;
					ulLearnCount++;
					if (ulLearnCount==optionsBP->st_dwLearnSteps
							&&optionsBP->st_fFinalLearnRate!=optionsBP->st_fInitLearnRate)
					{
						ulLearnCount=0;
						fLearnRate+=iLearnSign*optionsBP->st_fLearnChangeRate;
						if (iLearnSign==-1&&fLearnRate<optionsBP->st_fFinalLearnRate)
							fLearnRate=optionsBP->st_fFinalLearnRate;
						if (iLearnSign==1&&fLearnRate>optionsBP->st_fFinalLearnRate)
							fLearnRate=optionsBP->st_fFinalLearnRate;
						Net.SetLearnRate(fLearnRate);
					}
					if (uiMethod & LEARN_TYPE_MOMENTUM)
					{
						ulMomentCount++;
						if (ulMomentCount==optionsBP->st_dwMomentSteps
							&&optionsBP->st_fFinalMoment!=optionsBP->st_fInitMoment)
						{
							ulMomentCount=0;
							fMomentumParam+=iMomentSign*optionsBP->st_fMomentChangeRate;
							if (iMomentSign==-1&&fMomentumParam<optionsBP->st_fFinalMoment)
								fMomentumParam=optionsBP->st_fFinalMoment;
							if (iMomentSign==1&&fMomentumParam>optionsBP->st_fFinalMoment)
								fMomentumParam=optionsBP->st_fFinalMoment;
							Net.SetMomentumParam(fMomentumParam);
						}
					}
				}
			QueryPerformanceCounter(&liCount2);
			ulCycles++;
			pDlg->SetDlgItemInt(IDC_STATIC_CYCLES,ulCycles);
			sString.Format(_T("%f %f"),fNetError,Net.GetLearnRate());
			pDlg->SetDlgItemText(IDC_STATIC_AVDIST,sString);
			sString.Format(_T("%f"),(FLOAT)(liCount2.QuadPart-liCount1.QuadPart)/liFreq.QuadPart);
			pDlg->SetDlgItemText(IDC_STATIC_TIME,sString);
			pDlg->SetDlgItemInt(IDC_STATIC_UNUSED,uiPatterns);
		}
		while (uiPatterns<optionsBP->st_wPatterns&&ulCycles<optionsBP->st_dwLearnCycles&&!bFlag);
	}
	if (uiMethod & LEARN_TYPE_BATCH)
	{
		FLOAT fError;
		FLOAT *fPatterns[1024];
		ulCycles=0;
		QueryPerformanceCounter(&liCount1);
		do
		{
			fNetError=0.0;
			uiPatterns=0;
			bFlag=TRUE;
			for (i=0; i<32; i++)
				for (UINT j=0; j<32; j++)
				{
					Net.SetAxons(0,Segments[i][j]);
					Net.ForwardPass(0);
					if ((fError=Net.TargetFunction(bOutputLayer,Segments[i][j]))>
							optionsBP->st_fMinError)
					{
						Net.UpdateGradients(Segments[i][j]);
						fNetError+=fError;
						fPatterns[uiPatterns++]=&Segments[i][j][0];
						bFlag=FALSE;
					}
				}

			if (!bFlag)
				fNetError=Net.BackwardPass(uiPatterns,(const FLOAT **)&fPatterns[0],fNetError);
			QueryPerformanceCounter(&liCount2);
			ulCycles++;
			pDlg->SetDlgItemInt(IDC_STATIC_CYCLES,ulCycles);
			sString.Format(_T("%f %f"),fNetError,Net.GetLearnRate());
			pDlg->SetDlgItemText(IDC_STATIC_AVDIST,sString);
			sString.Format(_T("%f"),(FLOAT)(liCount2.QuadPart-liCount1.QuadPart)/liFreq.QuadPart);
			pDlg->SetDlgItemText(IDC_STATIC_TIME,sString);
			pDlg->SetDlgItemInt(IDC_STATIC_UNUSED,1024-uiPatterns);
		}
		while ((1024-uiPatterns)<optionsBP->st_wPatterns&&ulCycles<optionsBP->st_dwLearnCycles&&!bFlag);
	}

	BYTE bHiddenPatterns[32][32][16];
	FLOAT fHiddenOutput[16];

	for (i=0; i<32; i++)
		for (ULONG j=0; j<32; j++)
		{
			Net.SetAxons(0,Segments[i][j]);
			Net.ForwardPass(0);
			Net.GetAxons(bHiddenLayer,fHiddenOutput);
			for (ULONG k=0; k<16; k++)
				bHiddenPatterns[i][j][k]=(BYTE)floor((fHiddenOutput[k]+0.5f)/1.0f*255.0f+0.5f);
		}

	FLOAT fOutput[64];
	FLOAT fHiddenInput[16];
	ULONG ulSize=pBitmap->GetSizeBytes();
	DWORD dwWidthBytes=pBitmap->GetWidthBytes();
	BYTE *pResult=new BYTE[ulSize];
	for (i=0; i<32; i++)
		for (ULONG j=0; j<32; j++)
		{
			for (ULONG k=0; k<16; k++)
				fHiddenInput[k]=-0.5f+bHiddenPatterns[i][j][k]/255.0f;
			Net.SetAxons(bHiddenLayer,fHiddenInput);
			Net.ForwardPass(bHiddenLayer);
			Net.GetAxons(bOutputLayer,fOutput);
			for (k=0; k<8; k++)
				for (ULONG m=0; m<8; m++)
					pResult[(i*8+k)*dwWidthBytes+(j*8+m)]=
						(BYTE)floor((fOutput/*Segments[i][j]*/[8*k+m]+0.5f)/1.0f*255.0+0.5f);
		}

//	FLOAT fOutput[64];
//	ULONG ulSize=pImage->GetImageSize();
//	BYTE *pResult=new BYTE[ulSize];
//	for (i=0; i<32; i++)
//		for (ULONG j=0; j<32; j++)
//		{
//			Net.SetAxons(0,Segments[i][j]);
//			Net.ForwardPass(0);
//			Net.GetAxons(bOutputLayer,fOutput);
//			for (ULONG k=0; k<8; k++)
//				for (ULONG m=0; m<8; m++)
//					pResult[(i*8+k)*256+(j*8+m)]=
//						(BYTE)floor((fOutput/*Segments[i][j]*/[8*k+m]+0.5f)/1.0f*255.0+0.5f);
//		}

	CBitmapKit *pComprBitmap=pDlg->GetRightBitmap();
	BITMAP bm;
	pBitmap->GetBitmap(&bm);
	pComprBitmap->CreateBitmap(bm,pResult);
	pDlg->DisplayRightBitmap();

	if (zlibVersion()[0] != ZLIB_VERSION[0]) 
	{
		AfxMessageBox(_T("Incompatible ZLib version"),MB_OK);
        return TRUE;

    } 
	else 
		if (strcmp(zlibVersion(), ZLIB_VERSION) != 0) 
			AfxMessageBox(_T("WARNING: different ZLib version"),MB_OK);

	CFile file;
	file.Open(_T("image.tmp"),CFile::modeCreate | CFile::modeWrite);/*cur directory!!!*/

	COMPRFILEINFO cfInfo(COMPRFILE_NNIC,COMPRTYPE_BP,pComprBitmap->GetWidth(),
		pComprBitmap->GetHeight(),pComprBitmap->GetSizeClear());
	file.Write(&cfInfo,sizeof(COMPRFILEINFO));

	ULONG uiSrcLen=8*8*4*4*sizeof(FLOAT);
	ULONG uiDestLen=(ULONG)(uiSrcLen*100.1/100+0.5)+12;

	BYTE *pSrc=new BYTE[uiSrcLen];
	ASSERT(pSrc!=NULL);
	BYTE *pDest=new BYTE[uiDestLen];
	ASSERT(pDest!=NULL);
	ZeroMemory(pDest,uiDestLen);

	Net.CopyWeights(pSrc,bHiddenLayer);

	ASSERT(compress(pDest,&uiDestLen,pSrc,uiSrcLen)==Z_OK);

	file.Write(&uiSrcLen,sizeof(ULONG));
	file.Write(&uiDestLen,sizeof(ULONG));
	file.Write(pDest,uiDestLen);

	delete []pSrc;
	delete []pDest;
	uiSrcLen=32*32*16*sizeof(BYTE);
	uiDestLen=(ULONG)(uiSrcLen*100.1/100+0.5)+12;

	pDest=new BYTE[uiDestLen];
	ASSERT(pDest!=NULL);
	ZeroMemory(pDest,uiDestLen);

	ASSERT(compress(pDest,&uiDestLen,(BYTE *)bHiddenPatterns,uiSrcLen)==Z_OK);

	file.Write(&uiSrcLen,sizeof(ULONG));
	file.Write(&uiDestLen,sizeof(ULONG));
	file.Write(pDest,uiDestLen);
	
	CReportDlg ReportDlg;
	ReportDlg.SetFileName(file.GetFileName());
	ReportDlg.SetDirectory(file.GetFilePath());
	ReportDlg.SetFileSize(pDlg->GetLoadedFileSize(),file.GetLength());
	file.Close();
	ReportDlg.SetImageSize(pBitmap->GetWidth(),pBitmap->GetHeight());
	ReportDlg.SetOriginalColors(pBitmap->GetBitsPixel(),pBitmap->GetColors());
	ReportDlg.SetCompressionMethod(_T("Back Propagation"));
	ReportDlg.SetPSNR(pBitmap->GetPSNRFullChannel(pComprBitmap));
	ReportDlg.SetCompressionTime((FLOAT)(liCount2.QuadPart-liCount1.QuadPart)/liFreq.QuadPart);
	ReportDlg.DoModal();

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

	CBitmapKit *pBitmap=pDlg->GetLeftBitmap();
	DWORD dwSize=pBitmap->GetSizePixel();
	DWORD dwWidth=pBitmap->GetWidth();
	DWORD dwHeight=pBitmap->GetHeight();
	DWORD dwWidthBytes=pBitmap->GetWidthBytes();
	DWORD dwBitmapSize=pBitmap->GetSizeClear();
	
	const LPBYTE *lpBits=pBitmap->GetBits();
	ASSERT(lpBits!=NULL);

	OPTIONSCP *optionsCP=pDlg->GetOptionsCP();
	ASSERT(optionsCP!=NULL);

	const BYTE bVectorSize=3;
	const FLOAT fColorShift=128.0f;
	DWORD i,j,k;

	WORD *wLayers=NULL;
	switch (optionsCP->st_bNetType)
	{
	case NET_TYPE_KOHONEN_GROSSBERG:
		wLayers=new WORD[3];
		ASSERT(wLayers!=NULL);
		wLayers[INPUT_LAYER]=bVectorSize;
		wLayers[KOHONEN_LAYER]=optionsCP->st_wClusters;
		wLayers[GROSSBERG_LAYER]=bVectorSize;
		break;

	case NET_TYPE_KOHONEN:
		wLayers=new WORD[2];
		wLayers[INPUT_LAYER]=bVectorSize;
		wLayers[KOHONEN_LAYER]=optionsCP->st_wClusters;
		break;
	}

	NetCP Net(wLayers,optionsCP->st_bNetType);

	delete []wLayers;

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

	LPBYTE *pResultBitmap=NULL;
	WORD *wClusters=new WORD[dwSize];
	ASSERT(wClusters!=NULL);

	DWORD dwSrcLen=optionsCP->st_wClusters*bVectorSize;
	DWORD dwDestLen=(DWORD)(dwSrcLen*100.1/100+0.5)+12;
	BYTE *pSrc=NULL;

	DWORD dwCycles;
	CMap<WORD,WORD,BYTE,BYTE> map;
	BYTE bValue;
	DWORD dwFrom=(DWORD)(optionsCP->st_wClusters*1.2);
	map.InitHashTable(FindPrimeNumber(dwFrom));

	CString strText;
	DWORD dwInc;
	BOOLEAN bFlag;

	LARGE_INTEGER liFreq;
	LARGE_INTEGER liCount1;
	LARGE_INTEGER liCount2;
	QueryPerformanceFrequency(&liFreq);
	switch (optionsCP->st_bNetType)
	{
	case NET_TYPE_KOHONEN_GROSSBERG:
		{
			Net.InitWeights(-1.0f,2.0f);

			FLOAT **pVectors=NULL;
			ASSERT((pVectors=AllocateFloatMatrix(dwSize,bVectorSize))!=NULL);

			for (i=0; i<dwSize; i++)
			{
				for (j=0; j<bVectorSize; j++)
					pVectors[i][j]=lpBits[i/dwWidth][i%dwWidth*bVectorSize+j]-fColorShift;
				Net.NormalizeVector(pVectors[i],bVectorSize);
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

			dwInc=dwSize/100;
			dwCycles=0;
			dwLearnCount=0;
			dwNeighCount=0;
			//QueryPerformanceCounter(&liCount1);
			do
			{
				
				fWinDist=0.0f;
				pDlg->SetProgress(0);
				FLOAT fTime=0.0;
				for (i=0; i<dwSize; i++)
				{
					
					Net.SetAxons(INPUT_LAYER,pVectors[i]);
				
					QueryPerformanceCounter(&liCount1);
					Net.PropagateKohonenMax();
					QueryPerformanceCounter(&liCount2);
					
					wClusters[i]=Net.GetWinnerNeuron();
					if (!map.Lookup(wClusters[i],bValue))
						map.SetAt(wClusters[i],FALSE);
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

					if (!(i%(dwInc+1)))
						pDlg->StepProgress();
					fTime+=(FLOAT)(liCount2.QuadPart-liCount1.QuadPart)/liFreq.QuadPart;
				}
				fWinDist/=dwSize;
				dwCycles++;
				//QueryPerformanceCounter(&liCount2);
				pDlg->SetDlgItemInt(IDC_STATIC_CYCLES,dwCycles);
				strText.Format(_T("%f"),fWinDist);
				pDlg->SetDlgItemText(IDC_STATIC_AVDIST,strText);
				strText.Format(_T("%f"),fTime);
				//strText.Format(_T("%f"),(FLOAT)(liCount2.QuadPart-liCount1.QuadPart)/liFreq.QuadPart);
				pDlg->SetDlgItemText(IDC_STATIC_TIME,strText);
				pDlg->SetDlgItemInt(IDC_STATIC_UNUSED,optionsCP->st_wClusters-map.GetCount());
				map.RemoveAll();
			}
			while (fWinDist>optionsCP->st_fMinDist&&dwCycles<optionsCP->st_dwCycles);
			DestroyMatrix((LPVOID *)pVectors);

			for (i=0; i<dwSize; i++)
			{
				Net.SetWinnerNeuron(wClusters[i]);	
				Net.LearnGrossberg(&(lpBits[i/dwWidth][i%dwWidth*bVectorSize]));
			}
			Net.FinalizeGrossberg();	

			pResultBitmap=AllocateByteMatrix(dwHeight,dwWidthBytes);
			ASSERT(pResultBitmap!=NULL);

			//ofstream f("debug.txt",ios::out);
			DWORD dwIndex;
			for (i=0; i<dwHeight; i++)
				for (j=0; j<dwWidth; j++)
				{
					dwIndex=i*dwWidth+j;
					for (k=0; k<bVectorSize; k++)
						pResultBitmap[i][j*bVectorSize+k]=
							(BYTE)(Net.GetWeight(GROSSBERG_LAYER,(WORD)k,
								wClusters[dwIndex])+0.5f);
				}			
			pSrc=new BYTE[dwSrcLen];
			ASSERT(pSrc!=NULL);
			
			for (i=0; i<optionsCP->st_wClusters; i++)
				for (j=0; j<bVectorSize; j++)
					pSrc[i*bVectorSize+j]=(BYTE)(Net.GetWeight(GROSSBERG_LAYER,
						(WORD)j,(WORD)i)+0.5f);
		}

		break;

	case NET_TYPE_KOHONEN:
		Net.InitWeights();

		const WORD wPrime[4]={487,491,499,503};
		FLOAT fLearnPower=(FLOAT)(1.0f/(optionsCP->st_dwCycles-1)
			*log(optionsCP->st_fFinalLearnRate/optionsCP->st_fInitLearnRate));
		FLOAT fNeighPower=FLOAT(1.0f/(optionsCP->st_dwCycles-10)
			*log(1.0f/optionsCP->st_wInitNeighRadius));

		bFlag=FALSE;
		DWORD dwCount;
//		WORD wPixel;
		const DWORD dwPixelsPerCycle=(DWORD)((FLOAT)dwSize/optionsCP->st_dwCycles+0.5f);
		DWORD dwStep=wPrime[3];
		for (i=0; i<3&&!bFlag; i++)
			if (dwSize%wPrime[i])
			{
				bFlag=TRUE;
				dwStep=wPrime[i];
			}

		i=0;
		dwInc=optionsCP->st_dwCycles/100;
		pDlg->SetProgress(0);
		dwCycles=0;
		dwCount=0;
		QueryPerformanceCounter(&liCount1);
		do
		{
			i+=dwStep;
			if (i>=dwSize)
				i-=dwSize;
//			dwPixel=i/3;
			Net.SetAxons(INPUT_LAYER,&(lpBits[i/dwWidth][i%dwWidth*bVectorSize]));
			Net.PropagateKohonenMin();
			wClusters[i]=Net.GetWinnerNeuron();
//			if (!map.Lookup(wClusters[wPixel],bValue))
//				map.SetAt(wClusters[wPixel],FALSE);
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
				if (!(dwCycles%(dwInc+1)))
					pDlg->StepProgress();
				QueryPerformanceCounter(&liCount2);
				pDlg->SetDlgItemInt(IDC_STATIC_CYCLES,dwCycles);
				strText.Format(_T("%f"),(FLOAT)(liCount2.QuadPart-liCount1.QuadPart)/liFreq.QuadPart);
				pDlg->SetDlgItemText(IDC_STATIC_TIME,strText);
				pDlg->SetDlgItemInt(IDC_STATIC_UNUSED,optionsCP->st_wClusters-map.GetCount());
//				map.RemoveAll();
			}
		}
		while (dwCycles<optionsCP->st_dwCycles);

		pResultBitmap=AllocateByteMatrix(dwHeight,dwWidthBytes);
		ASSERT(pResultBitmap!=NULL);

		for (i=0; i<dwHeight; i++)
			for (j=0; j<dwWidth; j++)
			{
				Net.SetAxons(INPUT_LAYER,&(lpBits[i][j*bVectorSize]));
				Net.PropagateKohonenMin();
				wClusters[i*dwWidth+j]=Net.GetWinnerNeuron();
				for (k=0; k<bVectorSize; k++)
					pResultBitmap[i][j*bVectorSize+k]=
						(BYTE)(Net.GetWeight(KOHONEN_LAYER,
							wClusters[i*dwWidth+j],(WORD)k)+0.5f);
		}
		pSrc=new BYTE[dwSrcLen];
		ASSERT(pSrc!=NULL);
		
		for (i=0; i<optionsCP->st_wClusters; i++)
			for (j=0; j<bVectorSize; j++)
				pSrc[i*bVectorSize+j]=(BYTE)(Net.GetWeight(KOHONEN_LAYER,
					(WORD)i,(WORD)j)+0.5f);
		break;
	}
	
	delete []pLearnRate;

	CBitmapKit *pComprBitmap=pDlg->GetRightBitmap();
	BITMAP bm;
	pBitmap->GetBitmap(&bm);
	pComprBitmap->CreateBitmap(bm,pResultBitmap[0]);
	pDlg->DisplayRightBitmap();

	DestroyMatrix((LPVOID *)pResultBitmap);

	if (zlibVersion()[0] != ZLIB_VERSION[0]) 
	{
		AfxMessageBox(_T("Incompatible ZLib version"),MB_OK);
        return TRUE;

    } 
	else 
		if (strcmp(zlibVersion(), ZLIB_VERSION) != 0) 
			AfxMessageBox(_T("WARNING: different ZLib version"),MB_OK);

	CFile file;
	file.Open(_T("image.tmp"),CFile::modeCreate | CFile::modeWrite);/*cur directory!!!*/

	COMPRFILEINFO cfInfo(COMPRFILE_NNIC,COMPRTYPE_CP,
		dwWidth,dwHeight,pComprBitmap->GetSizeClear());
	file.Write(&cfInfo,sizeof(COMPRFILEINFO));

	BYTE *pDest=new BYTE[dwDestLen];
	ASSERT(pDest!=NULL);
	ZeroMemory(pDest,dwDestLen);


	ASSERT(compress(pDest,&dwDestLen,pSrc,dwSrcLen)==Z_OK);

	COMPRDATAINFO cdInfo(dwSrcLen,dwDestLen);
	file.Write(&cdInfo,sizeof(COMPRDATAINFO));
	
	file.Write(pDest,dwDestLen);

	delete []pSrc;
	delete []pDest;

	BYTE *bClusters=(BYTE *)wClusters;
	if (optionsCP->st_wClusters<=256)
	{
		dwSrcLen=dwSize*sizeof(BYTE);

		/*Packing*/
		for (i=1; i<dwSize; i++)
			bClusters[i]=LOBYTE(wClusters[i]);
	}
	else
		dwSrcLen=dwSize*sizeof(WORD);
		
	dwDestLen=(DWORD)(dwSrcLen*100.1/100+0.5)+12;

	pDest=new BYTE[dwDestLen];
	ASSERT(pDest!=NULL);
	ZeroMemory(pDest,dwDestLen);

	ASSERT(compress(pDest,&dwDestLen,bClusters,dwSrcLen)==Z_OK);

	delete []wClusters;

	cdInfo.Set(dwSrcLen,dwDestLen);
	file.Write(&cdInfo,sizeof(COMPRDATAINFO));

	file.Write(pDest,dwDestLen);

	CReportDlg ReportDlg;
	ReportDlg.SetFileName(file.GetFileName());
	ReportDlg.SetDirectory(file.GetFilePath());
	ReportDlg.SetFileSize(pDlg->GetLoadedFileSize(),file.GetLength());
	file.Close();
	ReportDlg.SetImageSize(dwWidth,dwHeight);
	ReportDlg.SetOriginalColors(pBitmap->GetBitsPixel(),pBitmap->GetColors());
	ReportDlg.SetCompressionMethod(_T("Counter Propagation"));
	DOUBLE fRed=pBitmap->GetPSNR(pComprBitmap,0);
	DOUBLE fGreen=pBitmap->GetPSNR(pComprBitmap,1);
	DOUBLE fBlue=pBitmap->GetPSNR(pComprBitmap,2);
	ReportDlg.SetPSNR(fRed,fGreen,fBlue,(fRed+fGreen+fBlue)/bVectorSize,
		pBitmap->GetPSNRFullChannel(pComprBitmap));
	ReportDlg.SetCompressionTime((FLOAT)(liCount2.QuadPart-liCount1.QuadPart)/liFreq.QuadPart);

	ReportDlg.DoModal();

	delete []pDest;

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

	OPTIONSDCT *optionsDCT=pDlg->GetOptionsDCT();
	ASSERT(optionsDCT!=NULL);

	pDlg->EnableCompress(FALSE);
	pDlg->EnableStop(TRUE);

	DCT Dct(DCT_SIZE);
	Dct.SetQuality(optionsDCT->st_bQuality);
	CBitmapKit *pBitmap=pDlg->GetLeftBitmap();

	FLOAT fSegment[DCT_SIZE][DCT_SIZE];
	SHORT sZigZag[DCT_SIZE*DCT_SIZE];
	FLOAT fShift;
	if (optionsDCT->st_bShift)
		fShift=-128.0f;
	else
		fShift=0.0;

	const BYTE bBytesPixel=pBitmap->GetBytesPixel();
	const DWORD dwWidth=pBitmap->GetWidth();
	const DWORD dwHeight=pBitmap->GetHeight();
	const DWORD dwWidthBytes=pBitmap->GetWidthBytes();
	const DWORD dwImageSize=pBitmap->GetSizeClear();
	//const ULONG ulHeight=pImage->GetImageHeight();
	//const ULONG ulWidth=pImage->GetImageWidth();
	//const ULONG ulPixels=ulHeight*ulWidth;

	FLOAT fPixel;
	INT i,j,k,m,l;

	LPBYTE *lpBits=AllocateByteMatrix(dwHeight,dwWidthBytes);
	ASSERT(lpBits!=NULL);

	PWORD pResult=new WORD[dwImageSize];

	LARGE_INTEGER liFreq;
	LARGE_INTEGER liCount1;
	LARGE_INTEGER liCount2;
	QueryPerformanceFrequency(&liFreq);
	QueryPerformanceCounter(&liCount1);

	ULONG ulCounter=0;
	for (i=0; i<dwHeight; i+=8)
		for (j=0; j<dwWidth; j+=8)
			for (k=0; k<bBytesPixel; k++)
			{
				CPoint point(j,i);
				CSize size(DCT_SIZE,DCT_SIZE);
				CRect rect(point,size);
				pBitmap->GetSegment(rect,&(fSegment[0][0]),k,fShift,256.0f);
				Dct.DCTransform(&(fSegment[0][0]));
				Dct.QuantizateBlock();
				Dct.ZigZagSequence(sZigZag);
				Dct.DequantizateBlock();
				Dct.IDCTransform();
				Dct.GetBlock(&(fSegment[0][0]));
				for (l=0; l<DCT_SIZE; l++)
					for (m=0; m<DCT_SIZE; m++)
					{
						fPixel=fSegment[l][m]-fShift+0.5f;
						if (fPixel<0.0)
							fPixel=0.0;
						if (fPixel>255.0)
							fPixel=255.0;
						lpBits[i+l][(j+m)*bBytesPixel+k]=(BYTE)fPixel;
						pResult[ulCounter++]=sZigZag[l*DCT_SIZE+m];
						//pResult[k*32*32*DCT_SIZE*DCT_SIZE+(i*DCT_SIZE+l)*32*DCT_SIZE+(j*DCT_SIZE+m)]=sZigZag[l*DCT_SIZE+m];
					}

			}
	QueryPerformanceCounter(&liCount2);

	CBitmapKit *pComprBitmap=pDlg->GetRightBitmap();
	BITMAP bm;
	pBitmap->GetBitmap(&bm);
	pComprBitmap->CreateBitmap(bm,lpBits[0]);

	pDlg->DisplayRightBitmap();

	DestroyMatrix((LPVOID *)lpBits);

	if (zlibVersion()[0] != ZLIB_VERSION[0]) 
	{
		AfxMessageBox(_T("Incompatible ZLib version"),MB_OK);
        return TRUE;

    } 
	else 
		if (strcmp(zlibVersion(), ZLIB_VERSION) != 0) 
			AfxMessageBox(_T("WARNING: different ZLib version"),MB_OK);

	CFile file;
	file.Open(_T("image.tmp"),CFile::modeCreate | CFile::modeWrite);/*cur directory!!!*/

	COMPRFILEINFO cfInfo(COMPRFILE_NNIC,COMPRTYPE_DCT,dwWidth,dwHeight,dwImageSize);
	file.Write(&cfInfo,sizeof(COMPRFILEINFO));

	ULONG uiSrcLen=32*32*DCT_SIZE*DCT_SIZE*sizeof(SHORT)*bBytesPixel;
	ULONG uiDestLen=(ULONG)(uiSrcLen*100.1/100+0.5)+12;

	BYTE *pDest=new BYTE[uiDestLen];
	ASSERT(pDest!=NULL);
	ZeroMemory(pDest,uiDestLen);

	ASSERT(compress(pDest,&uiDestLen,(BYTE *)pResult,uiSrcLen)==Z_OK);

	delete []pResult;

	file.Write(&uiSrcLen,sizeof(ULONG));
	file.Write(&uiDestLen,sizeof(ULONG));
	file.Write(pDest,uiDestLen);
		
	CReportDlg ReportDlg;
	ReportDlg.SetFileName(file.GetFileName());
	ReportDlg.SetDirectory(file.GetFilePath());
	ReportDlg.SetFileSize(pDlg->GetLoadedFileSize(),file.GetLength());
	file.Close();
	ReportDlg.SetImageSize(dwWidth,dwHeight);
	ReportDlg.SetOriginalColors(pBitmap->GetBitsPixel(),pBitmap->GetColors());
	ReportDlg.SetCompressionMethod(_T("DC Transform"));
	if (bBytesPixel>1)
	{
		DOUBLE fRed=pBitmap->GetPSNR(pComprBitmap,0);
		DOUBLE fGreen=pBitmap->GetPSNR(pComprBitmap,1);
		DOUBLE fBlue=pBitmap->GetPSNR(pComprBitmap,2);
		ReportDlg.SetPSNR(fRed,fGreen,fBlue,(fRed+fGreen+fBlue)/3,
		pBitmap->GetPSNRFullChannel(pComprBitmap));
	}
	else
		ReportDlg.SetPSNR(pBitmap->GetPSNRFullChannel(pComprBitmap));
	ReportDlg.SetCompressionTime((FLOAT)(liCount2.QuadPart-liCount1.QuadPart)/liFreq.QuadPart);
	ReportDlg.DoModal();

	delete []pDest;
	
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

	pDlg->EnableCompress(FALSE);
	pDlg->EnableStop(TRUE);

	DCT dct(DCT_SIZE);
	CBitmapKit *pBitmap=pDlg->GetLeftBitmap();

	FLOAT Segments[32][32][64];

	for (UINT i=0; i<32; i++)
		for (UINT j=0; j<32; j++)
		{
			CPoint point(j*8,i*8);
			CSize size(8,8);
			CRect rect(point,size);
			pBitmap->GetSegment(rect,Segments[i][j],0,-127.0/*0.0*/,256.0);
		}

	/******* DCT ******/
	for (i=0; i<32; i++)
		for (UINT j=0; j<32; j++)
		{
			dct.DCTransform(Segments[i][j]);
			dct.GetBlock(Segments[i][j]);
			for (UINT k=0; k<64; k++)
				Segments[i][j][k]=-0.5f+(1016.0f+Segments[i][j][k])/2040.0f;
		}

	/***** Back ********/
	OPTIONSBP *optionsBP=pDlg->GetOptionsBP();
	UINT uiInput=optionsBP->st_wInputBlock*optionsBP->st_wInputBlock;
	UINT uiHidden=optionsBP->st_wHiddenBlock*optionsBP->st_wHiddenBlock;
	UINT uiLayers[4];
	BYTE bInputLayer=0;
	BYTE bHiddenLayer;
	BYTE bOutputLayer;
	if (optionsBP->st_bSecondHidden)
	{
		uiLayers[0]=uiInput;
		uiLayers[1]=optionsBP->st_wNeurons;
		uiLayers[2]=uiHidden;
		uiLayers[3]=uiInput;
		bHiddenLayer=2;
		bOutputLayer=3;
	}
	else
	{
		uiLayers[0]=uiInput;
		uiLayers[1]=uiHidden;
		uiLayers[2]=uiInput;
		bHiddenLayer=1;
		bOutputLayer=2;
	}
	
	NetBP Net(bOutputLayer+1,uiLayers);
	Net.SetSignalBoundaries(-0.5,0.5);

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
	Net.SetScaleParam(0.5);
	Net.UseBiases(optionsBP->st_bUseBiases);
	UINT uiMethod;
	switch (optionsBP->st_bLearnMode)
	{
	case 0:
		uiMethod=LEARN_TYPE_SEQUENTIAL;
		break;
	case 1:
		uiMethod=LEARN_TYPE_BATCH;
		break;
	}
	if (optionsBP->st_bUseMomentum)
		uiMethod|=LEARN_TYPE_MOMENTUM;
	
	Net.SetLearnType(uiMethod);
	Net.InitWeights();

	LARGE_INTEGER liFreq;
	LARGE_INTEGER liCount1;
	LARGE_INTEGER liCount2;

	UINT uiPatterns;
	FLOAT fNetError;
	ULONG ulCycles;
	BOOLEAN bFlag;
	CString sString;

	QueryPerformanceFrequency(&liFreq);
	if (uiMethod & LEARN_TYPE_SEQUENTIAL)
	{
		ULONG ulLearnCount=0;
		ULONG ulMomentCount=0;
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
		ulCycles=0;
		QueryPerformanceCounter(&liCount1);
		do
		{
			bFlag=TRUE;
			uiPatterns=0;
			for (i=0; i<32; i++)
				for (UINT j=0; j<32; j++)
				{
					Net.SetAxons(0,Segments[i][j]);
					Net.ForwardPass(0);
					if ((fNetError=Net.TargetFunction(bOutputLayer,Segments[i][j]))>
							optionsBP->st_fMinError)
					{
						Net.BackwardPass(Segments[i][j]);
						bFlag=FALSE;
					}
					else
						uiPatterns++;
					ulLearnCount++;
					if (ulLearnCount==optionsBP->st_dwLearnSteps
							&&optionsBP->st_fFinalLearnRate!=optionsBP->st_fInitLearnRate)
					{
						ulLearnCount=0;
						fLearnRate+=iLearnSign*optionsBP->st_fLearnChangeRate;
						if (iLearnSign==-1&&fLearnRate<optionsBP->st_fFinalLearnRate)
							fLearnRate=optionsBP->st_fFinalLearnRate;
						if (iLearnSign==1&&fLearnRate>optionsBP->st_fFinalLearnRate)
							fLearnRate=optionsBP->st_fFinalLearnRate;
						Net.SetLearnRate(fLearnRate);
					}
					if (uiMethod & LEARN_TYPE_MOMENTUM)
					{
						ulMomentCount++;
						if (ulMomentCount==optionsBP->st_dwMomentSteps
							&&optionsBP->st_fFinalMoment!=optionsBP->st_fInitMoment)
						{
							ulMomentCount=0;
							fMomentumParam+=iMomentSign*optionsBP->st_fMomentChangeRate;
							if (iMomentSign==-1&&fMomentumParam<optionsBP->st_fFinalMoment)
								fMomentumParam=optionsBP->st_fFinalMoment;
							if (iMomentSign==1&&fMomentumParam>optionsBP->st_fFinalMoment)
								fMomentumParam=optionsBP->st_fFinalMoment;
							Net.SetMomentumParam(fMomentumParam);
						}
					}
				}
			QueryPerformanceCounter(&liCount2);
			ulCycles++;
			pDlg->SetDlgItemInt(IDC_STATIC_CYCLES,ulCycles);
			sString.Format(_T("%f %f"),fNetError,Net.GetLearnRate());
			pDlg->SetDlgItemText(IDC_STATIC_AVDIST,sString);
			sString.Format(_T("%f"),(FLOAT)(liCount2.QuadPart-liCount1.QuadPart)/liFreq.QuadPart);
			pDlg->SetDlgItemText(IDC_STATIC_TIME,sString);
			pDlg->SetDlgItemInt(IDC_STATIC_UNUSED,uiPatterns);
		}
		while (uiPatterns<optionsBP->st_wPatterns&&ulCycles<optionsBP->st_dwLearnCycles&&!bFlag);
	}
	if (uiMethod & LEARN_TYPE_BATCH)
	{
		FLOAT fError;
		FLOAT *fPatterns[1024];
		ulCycles=0;
		Net.SetLearnRate(0.9f);
		QueryPerformanceCounter(&liCount1);
		do
		{
			fNetError=0.0;
			uiPatterns=0;
			bFlag=TRUE;
			for (i=0; i<32; i++)
				for (UINT j=0; j<32; j++)
				{
					Net.SetAxons(0,Segments[i][j]);
					Net.ForwardPass(0);
					if ((fError=Net.TargetFunction(bOutputLayer,Segments[i][j]))>
							optionsBP->st_fMinError)
					{
						Net.UpdateGradients(Segments[i][j]);
						fNetError+=fError;
						fPatterns[uiPatterns++]=&Segments[i][j][0];
						bFlag=FALSE;
					}
				}

			if (!bFlag)
				fNetError=Net.BackwardPass(uiPatterns,(const FLOAT **)&fPatterns[0],fNetError);
			QueryPerformanceCounter(&liCount2);
			ulCycles++;
			pDlg->SetDlgItemInt(IDC_STATIC_CYCLES,ulCycles);
			sString.Format(_T("%f %f"),fNetError,Net.GetLearnRate());
			pDlg->SetDlgItemText(IDC_STATIC_AVDIST,sString);
			sString.Format(_T("%f"),(FLOAT)(liCount2.QuadPart-liCount1.QuadPart)/liFreq.QuadPart);
			pDlg->SetDlgItemText(IDC_STATIC_TIME,sString);
			pDlg->SetDlgItemInt(IDC_STATIC_UNUSED,1024-uiPatterns);
		}
		while ((1024-uiPatterns)<optionsBP->st_wPatterns&&ulCycles<optionsBP->st_dwLearnCycles&&!bFlag);
	}

	/******* Back ********/
	BYTE bHiddenPatterns[32][32][16];
	FLOAT fHiddenOutput[16];

	for (i=0; i<32; i++)
		for (ULONG j=0; j<32; j++)
		{
			Net.SetAxons(bInputLayer,Segments[i][j]);
			Net.ForwardPass(bInputLayer);
			Net.GetAxons(bHiddenLayer,fHiddenOutput);
			for (ULONG k=0; k<16; k++)
				bHiddenPatterns[i][j][k]=(BYTE)floor((fHiddenOutput[k]+0.5f)/1.0f*255.0f+0.5f);
		}

//	FLOAT fOutput[64];
	FLOAT fHiddenInput[16];
//	ULONG ulSize=pImage->GetImageSize();
//	BYTE *pResult=new BYTE[ulSize];
	for (i=0; i<32; i++)
		for (ULONG j=0; j<32; j++)
		{
			for (ULONG k=0; k<16; k++)
				fHiddenInput[k]=-0.5f+bHiddenPatterns[i][j][k]/255.0f;
			Net.SetAxons(bHiddenLayer,fHiddenInput);
			Net.ForwardPass(bHiddenLayer);
			Net.GetAxons(bOutputLayer,Segments[i][j]);
			for (k=0; k<64; k++)
				Segments[i][j][k]=-1016.0f+(Segments[i][j][k]+0.5f)/1.0f*2040.0f;
			dct.IDCTransform(Segments[i][j]);
			dct.GetBlock(Segments[i][j]);
		}


	ULONG ulSize=pBitmap->GetSizeClear();
	BYTE *pResult=new BYTE[ulSize];
	FLOAT fPixel;
	for (i=0; i<32; i++)
		for (ULONG j=0; j<32; j++)
		{
			for (ULONG k=0; k<8; k++)
				for (ULONG m=0; m<8; m++)
				{
					fPixel=(FLOAT)floor(Segments[i][j][8*k+m]+127.0+0.5f);
					if (fPixel<0.0)
						fPixel=0.0;
					if (fPixel>255.0)
						fPixel=255.0;
					pResult[(i*8+k)*256+(j*8+m)]=(BYTE)fPixel;
				}
		}


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
//	CReportDlg ReportDlg;
//	ReportDlg.SetFileName(file.GetFileName());
//	ReportDlg.SetDirectory(file.GetFilePath());
//	ReportDlg.SetFileSize(pDlg->GetOriginalFileSize(),file.GetLength());
//	file.Close();
//	ReportDlg.SetImageSize(pImage->GetImageWidth(),pImage->GetImageHeight());
//	ReportDlg.SetOriginalColors(pImage->GetBitsPerPixel(),pImage->GetColors());
//	ReportDlg.SetCompressionMethod(_T("DCT with Back Propagation"));
//	ReportDlg.SetPSNR(pImage->GetPSNRFullChannel(*pComprImage));
//	ReportDlg.SetCompressionTime((FLOAT)(liCount2.QuadPart-liCount1.QuadPart)/liFreq.QuadPart);
//	ReportDlg.DoModal();

	pDlg->EnableOptions(TRUE);
	pDlg->EnableCompress(TRUE);
	pDlg->SetFinished(TRUE);
	return FALSE;
}

CBitmapKit * CNNICDlg::GetLeftBitmap()
{
	return &m_leftBitmap;
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

CBitmapKit * CNNICDlg::GetRightBitmap()
{
	return &m_rightBitmap;
}

void CNNICDlg::DisplayLeftBitmap()
{
	m_staticLeft.SetBitmap(m_leftBitmap);
//	UpdateData(FALSE);
}

void CNNICDlg::DisplayRightBitmap()
{
	m_staticRight.SetBitmap(m_rightBitmap);
//	UpdateData(FALSE);
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
	CPropertySheet propOptions;
	propOptions.m_psh.dwFlags &= ~(PSH_HASHELP);
	CBPDlg pageBP(m_optionsDefault.st_optionsBP);
	pageBP.m_psp.dwFlags &= ~(PSP_HASHELP);
	CCPDlg pageCP(m_optionsDefault.st_optionsCP);
	pageCP.m_psp.dwFlags &= ~(PSP_HASHELP);
	CDCTDlg pageDCT(m_optionsDefault.st_optionsDCT);
	pageDCT.m_psp.dwFlags &= ~(PSP_HASHELP);
	propOptions.SetTitle(_T("Compression Options"));
	propOptions.AddPage(&pageBP);
	propOptions.AddPage(&pageCP);
	propOptions.AddPage(&pageDCT);

	UINT uiMethod=GetCheckedRadioButton(IDC_RADIO_BACK,IDC_RADIO_DCTBACK)-IDC_RADIO_BACK;
	propOptions.SetActivePage(uiMethod);
	propOptions.DoModal();
	pageBP.GetOptionsBP(&m_optionsDefault.st_optionsBP);
	pageCP.GetOptionsCP(&m_optionsDefault.st_optionsCP);
	pageDCT.GetOptionsDCT(&m_optionsDefault.st_optionsDCT);
}

OPTIONSBP * CNNICDlg::GetOptionsBP()
{
	return &m_optionsDefault.st_optionsBP;
}

DWORD CNNICDlg::GetLoadedFileSize()
{
	CFile file;
	CFileStatus fileStatus;
	file.GetStatus(m_strLoadedFileName,fileStatus);
	return fileStatus.m_size;
}

OPTIONSDCT * CNNICDlg::GetOptionsDCT()
{
	return &m_optionsDefault.st_optionsDCT;
}

void CNNICDlg::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
}
