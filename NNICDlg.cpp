// NNICDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NNIC.h"
#include "NNICDlg.h"

#include ".\NeuroNet\NetBP.h"
#include ".\NeuroNet\NetCP.h"
#include "ReportDlg.h"
#include "CPDlg.h"
#include "BPDlg.h"
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
	OPTIONSCP optionsCP(1024,10,0.035f,0.3f,0.1f,10000,0.01f,3,0,50000,1);
	OPTIONS options(optionsBP,optionsCP);
	CopyMemory(&m_optionsDefault,&options,sizeof(OPTIONS));
}

void CNNICDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNNICDlg)
	DDX_Control(pDX, IDC_STATIC_COMPRESSED, m_staticCompr);
	DDX_Control(pDX, IDC_STATIC_ORIGINAL, m_staticOriginal);
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
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
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

	CheckRadioButton(IDC_RADIO_BACK,IDC_RADIO_DCTBACK,IDC_RADIO_DCT);
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
		m_sOriginalFileName=dlgFile.GetPathName();
		m_OriginalImage.DecodeBMP(m_sOriginalFileName);
		//m_OriginalImage.DeletePadding();
		m_staticOriginal.SetBitmap(m_OriginalImage.GetHandle(m_staticOriginal.GetDC()));
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
		m_threadCompress=AfxBeginThread(StartDCT,this);
	}
}

void CNNICDlg::EnableCompress(const BOOL bFlag)
{
	(GetDlgItem(IDC_BUTTON_COMPRESS))->EnableWindow(bFlag);
}

void CNNICDlg::EnableStop(const BOOL bFlag)
{
	(GetDlgItem(IDC_BUTTON_STOP))->EnableWindow(bFlag);
}

void CNNICDlg::EnableOptions(const BOOL bFlag)
{
	(GetDlgItem(IDC_BUTTON_OPTIONS))->EnableWindow(bFlag);
}

UINT StartCP(LPVOID pParam)
{
	CNNICDlg* pDlg = (CNNICDlg *)pParam;

    if (pDlg == NULL ||
        !pDlg->IsKindOf(RUNTIME_CLASS(CNNICDlg)))
    return TRUE;

	pDlg->EnableCompress(FALSE);
	pDlg->EnableStop(TRUE);

	ImageKit *pImage=pDlg->GetOriginalImage();
	const BYTE *pBits=pImage->GetBits();
	ASSERT(pBits!=NULL);

	OPTIONSCP *pOptions=pDlg->GetOptionsCP();
	ASSERT(pOptions!=NULL);

	UINT uiLayers[3]={3,pOptions->st_wClusters,3};
	NetCP Net(uiLayers);
	//Net.SetNeighborhoodSize(pOptions->st_wInitNeighSize);
	Net.InitWeights(-1.0,2.0);

	const UINT uiSize=pImage->GetImageHeight()*pImage->GetImageWidth();
	FLOAT **vPatterns=new FLOAT *[uiSize];
	ASSERT(*vPatterns!=NULL);
	for (UINT i=0; i<uiSize; i++)
	{
		vPatterns[i]=new FLOAT[3];
		ASSERT(vPatterns[i]!=NULL);
		for (UINT j=0; j<3; j++)
			vPatterns[i][j]=pBits[i*3+j]-127.0f;
		Net.NormalizeVector(vPatterns[i],3);
	}

	FLOAT fWinDist;
	UINT uiCycles=0;
	WORD *uiClusters=new WORD[uiSize];
	ASSERT(uiClusters!=NULL);

	CString sString;
	LARGE_INTEGER liFreq;
	LARGE_INTEGER liCount1;
	LARGE_INTEGER liCount2;
	QueryPerformanceFrequency(&liFreq);
	UINT uiInc=uiSize/100;
	CMapWordToPtr map;
	LPVOID pValue;
	ULONG ulLearnCount=0;
	ULONG ulNeighCount=0;
	INT iLearnSign=1;
	INT iNeighSign=1;
	FLOAT fLearnRate=pOptions->st_fInitLearnRate;
	WORD wNeighSize=pOptions->st_wInitNeighSize;
	if (pOptions->st_fInitLearnRate>=pOptions->st_fFinalLearnRate)
		iLearnSign=-1;
	if (pOptions->st_wInitNeighSize>=pOptions->st_wFinalNeighSize)
		iNeighSign=-1;
	do
	{
		fWinDist=0.0f;
		pDlg->SetProgress(0);
		QueryPerformanceCounter(&liCount1);
		for (i=0; i<uiSize; i++)
		{
			ulLearnCount++;
			ulNeighCount++;
			if (ulLearnCount==pOptions->st_ulLearnRateEpochs)
			{
				ulLearnCount=0;
				fLearnRate+=iLearnSign*pOptions->st_fLearnChangeRate;
				if (iLearnSign==-1&&fLearnRate<pOptions->st_fFinalLearnRate)
					fLearnRate=pOptions->st_fFinalLearnRate;
				if (iLearnSign==1&&fLearnRate>pOptions->st_fFinalLearnRate)
					fLearnRate=pOptions->st_fFinalLearnRate;
			}

			if (ulNeighCount==pOptions->st_ulNeighSizeEpochs)
			{
				ulNeighCount=0;
				switch (iNeighSign)
				{
				case -1:
					if (wNeighSize>pOptions->st_wFinalNeighSize)
						wNeighSize-=pOptions->st_wNeighChangeRate;
					else
						wNeighSize=pOptions->st_wFinalNeighSize;
					break;
				case 1:
					if (wNeighSize<pOptions->st_wFinalNeighSize)
						wNeighSize+=pOptions->st_wNeighChangeRate;
					else
						wNeighSize=pOptions->st_wFinalNeighSize;
					break;
				}
				
			}

			Net.SetNeighborhoodSize(wNeighSize);
			Net.SetAxons(INPUT_LAYER,vPatterns[i]);
			Net.PropagateKohonen();
			uiClusters[i]=Net.GetWinnerNeuron();
			if (!map.Lookup(uiClusters[i],pValue))
				map.SetAt(uiClusters[i],&uiClusters[i]);
			fWinDist+=Net.GetWinnerDistance();
			Net.LearnKohonen(fLearnRate);
			if (!((i+1)%uiInc))
				pDlg->StepProgress();
		}
		QueryPerformanceCounter(&liCount2);
		fWinDist/=uiSize;
		uiCycles++;
		pDlg->SetDlgItemInt(IDC_STATIC_CYCLES,uiCycles);
		sString.Format(_T("%f"),fWinDist);
		pDlg->SetDlgItemText(IDC_STATIC_AVDIST,sString);
		sString.Format(_T("%f"),(FLOAT)(liCount2.QuadPart-liCount1.QuadPart)/liFreq.QuadPart);
		pDlg->SetDlgItemText(IDC_STATIC_TIME,sString);
		pDlg->SetDlgItemInt(IDC_STATIC_UNUSED,pOptions->st_wClusters-map.GetCount());
		map.RemoveAll();
	}
	while(fWinDist>pOptions->st_fMinDist&&uiCycles<=pOptions->st_ulCycles);

	for (i=0; i<uiSize; i++)
		delete []vPatterns[i];
	delete []vPatterns;

	uiCycles=0;
//	FLOAT fPixel[3];
//	do
//	{
//		fWinDist=0.0f;
//		pDlg->SetProgress(0);
//		QueryPerformanceCounter(&liCount1);
//		for (i=0; i<uiSize; i++)
//		{
//			//Net.SetAxons(INPUT_LAYER,vPatterns[i]);
//			//Net.PropagateKohonen();//
//			for (UINT j=0; j<3; j++)*/
//				fPixel[j]=pBits[3*i+j]/*-127.0f*/;
//			Net.SetWinnerNeuron(uiClusters[i]);
//			fWinDist+=Net.GetTargetDistance(fPixel);			
//			Net.LearnGrossberg(pOptions->st_fLRGrossberg,fPixel);
//			if (!((i+1)%uiInc))
//				pDlg->StepProgress();
//		}
//		QueryPerformanceCounter(&liCount2);
//		fWinDist/=uiSize;
//		uiCycles++;
//		pDlg->SetDlgItemInt(IDC_STATIC_CYCLES,uiCycles);
//		sString.Format(_T("%f"),fWinDist);
//		pDlg->SetDlgItemText(IDC_STATIC_AVDIST,sString);
//		sString.Format(_T("%f"),(FLOAT)(liCount2.QuadPart-liCount1.QuadPart)/liFreq.QuadPart);
//		pDlg->SetDlgItemText(IDC_STATIC_TIME,sString);
//	}
//	while(fWinDist>pOptions->st_fDistGrossberg&&uiCycles<=pOptions->st_uiCycles);

	for (i=0; i<uiSize; i++)
	{
		Net.SetWinnerNeuron(uiClusters[i]);	
		Net.LearnGrossberg(&pBits[i*3]);
	}
	Net.FinilizeGrossberg();

	BYTE *pResult=new BYTE[uiSize*3];
	ASSERT(pResult!=NULL);
	for (i=0; i<uiSize; i++)
		for (UINT j=0; j<3; j++)
/******************/
			pResult[i*3+j]=(BYTE)floor(/*127.0+*/Net.GetWeight(GROSSBERG_LAYER,j,uiClusters[i])+0.5);

	ImageKit *pComprImage=pDlg->GetComprImage();
	pComprImage->FromObject(*pImage,pResult);
	pDlg->DisplayComprBitmap();
	//	*pImage,pResult);
	DOUBLE r=pImage->GetPSNR(0,*pComprImage);
	DOUBLE b=pImage->GetPSNR(1,*pComprImage);
	DOUBLE g=pImage->GetPSNR(2,*pComprImage);
	DOUBLE av=(r+g+b)/3;
	DOUBLE all=pImage->GetPSNRFullChannel(*pComprImage);
	CReportDlg ReportDlg;

	
	
	delete []pResult;

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

	COMPRFILEINFO cfInfo(COMPRFILE_NNIC,COMPRTYPE_CP,pImage->GetImageWidth(),
		pImage->GetImageHeight(),uiSize);
	file.Write(&cfInfo,sizeof(COMPRFILEINFO));

	ULONG uiSrcLen=pOptions->st_wClusters*3;
	ULONG uiDestLen=(ULONG)(uiSrcLen*100.1/100+0.5)+12;

	BYTE *pSrc=new BYTE[uiSrcLen];
	ASSERT(pSrc!=NULL);
	BYTE *pDest=new BYTE[uiDestLen];
	ASSERT(pDest!=NULL);
	ZeroMemory(pDest,uiDestLen);

	for (i=0; i<pOptions->st_wClusters; i++)
		for (UINT j=0; j<3; j++)
			pSrc[i*3+j]=(BYTE)(127.0+Net.GetWeight(GROSSBERG_LAYER,j,i)+0.5);

	ASSERT(compress(pDest,&uiDestLen,pSrc,uiSrcLen)==Z_OK);

	file.Write(&uiSrcLen,sizeof(ULONG));
	file.Write(&uiDestLen,sizeof(ULONG));
	file.Write(pDest,uiDestLen);

	delete []pSrc;
	delete []pDest;
	uiSrcLen=uiSize*sizeof(WORD);
	uiDestLen=(ULONG)(uiSrcLen*100.1/100+0.5)+12;

	pDest=new BYTE[uiDestLen];
	ASSERT(pDest!=NULL);
	ZeroMemory(pDest,uiDestLen);

	ASSERT(compress(pDest,&uiDestLen,(BYTE *)&uiClusters[0],uiSrcLen)==Z_OK);

	file.Write(&uiSrcLen,sizeof(ULONG));
	file.Write(&uiDestLen,sizeof(ULONG));
	file.Write(pDest,uiDestLen);

	file.Close();

	ReportDlg.Set(r,g,b,av,all);
	ReportDlg.DoModal();

	delete []pDest;
	delete []uiClusters;
	pDlg->EnableOptions(TRUE);
	pDlg->EnableCompress(TRUE);
	pDlg->SetFinished(TRUE);
	return FALSE;
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
	UINT uiOutputLayer;
	if (optionsBP->st_bSecondHidden)
	{
		uiLayers[0]=uiInput;
		uiLayers[1]=optionsBP->st_wNeurons;
		uiLayers[2]=uiHidden;
		uiLayers[3]=uiInput;
		uiOutputLayer=3;
	}
	else
	{
		uiLayers[0]=uiInput;
		uiLayers[1]=uiHidden;
		uiLayers[2]=uiInput;
		uiOutputLayer=2;
	}
	
	NetBP Net(uiOutputLayer+1,uiLayers);
	Net.SetSignalBoundaries(-0.5,0.5);
	switch (optionsBP->st_wSigmoidType)
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
	switch (optionsBP->st_wLearnMode)
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
	
	Net.InitWeights();

	ImageKit *pImage=pDlg->GetOriginalImage();

	FLOAT Segments[32][32][64];
	for (UINT i=0 ;i<32; i++)
		for (UINT j=0; j<32; j++)
		{
			CPoint point(j*8,i*8);
			CSize size(8,8);
			CRect rect(point,size);
			pImage->GetSegment(rect,Segments[i][j],-0.5f,1.0f);
		}

	LARGE_INTEGER liFreq;
	LARGE_INTEGER liCount1;
	LARGE_INTEGER liCount2;
	QueryPerformanceFrequency(&liFreq);
	ULONG ulCycles=0;
	FLOAT fNetError;
	CString sString;
	BOOL bFlag;
	UINT uiPatterns;
	PVOID pValue;
	WORD uiIndex;
//	UINT uiIndex1;
//	UINT uiIndex2;
	CMapWordToPtr map;
//	POSITION pos;
	srand((unsigned int)(time(NULL)));
	map.RemoveAll();
	for (i=0; i<100; i++)
	{
		do
			uiIndex=rand()%1024;
		while (map.Lookup(uiIndex,pValue));
		map.SetAt(uiIndex,&uiIndex);
	}
////	ofstream file("debug.txt",ios::out);
	Net.SetLearnRate(0.1);
	QueryPerformanceCounter(&liCount1);
	do
	{
		//QueryPerformanceCounter(&liCount1);
		bFlag=TRUE;
		uiPatterns=0;
		for (i=0; i<32; i++)
			for (UINT j=0; j<32; j++)
			{
				Net.SetAxons(0,Segments[i][j]);
				Net.ForwardPass(0);
				if ((fNetError=Net.TargetFunction(2,Segments[i][j]))>0.32)
				{
					Net.BackwardPass(Segments[i][j]);
					bFlag=FALSE;
					uiPatterns++;
				}
			}
//		pos=map.GetStartPosition();
//		for (i=0; i<90; i++)
//		{
//			map.GetNextAssoc(pos,uiIndex,pValue);
//			uiIndex1=uiIndex/32;
//			uiIndex2=uiIndex%32;
//			Net.SetAxons(0,Segments[uiIndex1][uiIndex2]);
//			Net.ForwardPass(0);
//			if ((fNetError=Net.TargetFunction(2,Segments[uiIndex1][uiIndex2]))>0.1)
//			{
//				Net.BackwardPass(Segments[uiIndex1][uiIndex2]);
//				bFlag=FALSE;
//				uiPatterns++;
//			}
//			
//		}
//		file<<fNetError<<endl;
		QueryPerformanceCounter(&liCount2);
		ulCycles++;
		pDlg->SetDlgItemInt(IDC_STATIC_CYCLES,ulCycles);
		sString.Format(_T("%f %f"),fNetError,Net.GetLearnRate());
		pDlg->SetDlgItemText(IDC_STATIC_AVDIST,sString);
		sString.Format(_T("%f"),(FLOAT)(liCount2.QuadPart-liCount1.QuadPart)/liFreq.QuadPart);
		pDlg->SetDlgItemText(IDC_STATIC_TIME,sString);
		pDlg->SetDlgItemInt(IDC_STATIC_UNUSED,uiPatterns);
	}
	while (!bFlag&&uiPatterns>0);
//	FLOAT *fPatterns[1024];
//	FLOAT fError;
//	//Net.SetLearnRate(0.9f);
//	QueryPerformanceCounter(&liCount1);
//	do
//	{
//		//QueryPerformanceCounter(&liCount1);
//		fNetError=0.0;
//		uiPatterns=0;
//		bFlag=TRUE;
////		for (i=0; i<32; i++)
////			for (UINT j=0; j<32; j++)
////			{
////				Net.SetAxons(0,Segments[i][j]);
////				Net.ForwardPass(0);
////				if ((fError=Net.TargetFunction(2,Segments[i][j]))>0.01)
////				{
////					Net.UpdateGradients(Segments[i][j]);
////					fNetError+=fError;
////					fPatterns[uiPatterns++]=&Segments[i][j][0];
////					bFlag=FALSE;
////				}
////
////			}
//		pos=map.GetStartPosition();
//		for (i=0; i<100; i++)
//		{
//			map.GetNextAssoc(pos,uiIndex,pValue);
//			uiIndex1=uiIndex/32;
//			uiIndex2=uiIndex%32;
//			Net.SetAxons(0,Segments[uiIndex1][uiIndex2]);
//			Net.ForwardPass(0);
//			if ((fError=Net.TargetFunction(2,Segments[uiIndex1][uiIndex2]))>0.01)
//			{
//				Net.UpdateGradients(Segments[uiIndex1][uiIndex2]);
//				fNetError+=fError;
//				fPatterns[uiPatterns++]=&Segments[uiIndex1][uiIndex2][0];
//				bFlag=FALSE;
//			}
//		}
//		
//		if (!bFlag)
//		{
//			fNetError=Net.BackwardPass(uiPatterns,(const FLOAT **)&fPatterns[0],fNetError);
//		}
//		QueryPerformanceCounter(&liCount2);
//		ulCycles++;
//		pDlg->SetDlgItemInt(IDC_STATIC_CYCLES,ulCycles);
//		sString.Format(_T("%f %f"),fNetError,Net.GetLearnRate());
//		pDlg->SetDlgItemText(IDC_STATIC_AVDIST,sString);
//		sString.Format(_T("%f"),(FLOAT)(liCount2.QuadPart-liCount1.QuadPart)/liFreq.QuadPart);
//		pDlg->SetDlgItemText(IDC_STATIC_TIME,sString);
//		pDlg->SetDlgItemInt(IDC_STATIC_UNUSED,uiPatterns);
//	}
//	while (!bFlag&&uiPatterns>0);
//	file.close();

	FLOAT fOutput[64];
	ULONG ulSize=pImage->GetImageSize();
	BYTE *pResult=new BYTE[ulSize];
	for (i=0; i<32; i++)
		for (ULONG j=0; j<32; j++)
		{
			Net.SetAxons(0,Segments[i][j]);
			Net.ForwardPass(0);
			Net.GetAxons(2,fOutput);
			for (ULONG k=0; k<8; k++)
				for (ULONG m=0; m<8; m++)
					pResult[(i*8+k)*256+(j*8+m)]=
						(BYTE)floor((fOutput/*Segments[i][j]*/[8*k+m]+0.5f)/1.0f*255.0+0.5f);
		}

	ImageKit *pComprImage=pDlg->GetComprImage();
	pComprImage->FromObject(*pImage,pResult);
	pDlg->DisplayComprBitmap();
	//	*pImage,pResult);
	//DOUBLE r=pImage->GetPSNR(0,*pComprImage);
	//DOUBLE b=pImage->GetPSNR(1,*pComprImage);
	//DOUBLE g=pImage->GetPSNR(2,*pComprImage);
	//DOUBLE av=(r+g+b)/3;
	DOUBLE all=pImage->GetPSNRFullChannel(*pComprImage);
	CReportDlg ReportDlg;

	ReportDlg.Set(0,0,0,0,all);
	ReportDlg.DoModal();

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

	pDlg->EnableCompress(FALSE);
	pDlg->EnableStop(TRUE);

	DCT dct(8);
	dct.SetQuality(10);
	ImageKit *pImage=pDlg->GetOriginalImage();

	FLOAT Segments[32][32][64];
//		={140,144,147,140,140,155,179,175,
//		144,152,140,147,140,148,167,179,
//		152,155,136,167,163,162,152,172,
//		168,145,156,160,152,155,136,160,
//		162,148,156,148,140,136,147,162,
//		147,167,140,150,155,140,136,162,
//		136,156,123,167,162,144,140,147,
//		148,155,136,155,152,147,147,136};
//	FLOAT Res[64]={168,-18,15,-9,23,-9,-14,19,
//					21,-34,26,-9,-11,11,14,7,
//					-10,-24,-2,6,-18,3,-20,-1,
//					-8,-5,14,-15,-8,-3,-4,8,
//					-3,10,8,1,-11,18,18,15,
//					4,-2,-18,8,8,-4,1,-7,
//					9,1,-3,4,-1,-7,-1,-2,
//					0,-8,-2,2,1,4,-6,0};
//	CFile f;
//	f.Open("segments.dat",CFile::modeCreate | CFile::modeWrite);
//	for (UINT i=0 ;i<32; i++)
//		for (UINT j=0; j<32; j++)
//		{
//			CPoint point(j*8,i*8);
//			CSize size(8,8);
//			CRect rect(point,size);
//			pImage->GetSegment(rect,Segments[i][j],-1.0,2.0);
//			f.Write(Segments[i][j],sizeof(FLOAT)*64);
//		}
//
//	f.Close();
//	for (i=0; i<32; i++)
//		for (UINT j=0; j<32; j++)
//		{
//			dct.DCTransform(Segments[i][j]);
//			dct.GetBlock(Segments[i][j]);
//			dct.QuantizateBlock();
//			dct.GetBlock(Segments[i][j]);
//			dct.DequantizateBlock();
//			dct.IDCTransform();
//			dct.GetBlock(Segments[i][j]);
//		}
//
//	ULONG ulSize=pImage->GetImageSize();
//	BYTE *pResult=new BYTE[ulSize];
//	FLOAT fPixel;
//	for (i=0; i<32; i++)
//		for (ULONG j=0; j<32; j++)
//		{
//			for (ULONG k=0; k<8; k++)
//				for (ULONG m=0; m<8; m++)
//				{
//					fPixel=(FLOAT)floor(Segments[i][j][8*k+m]+128+0.5f);
//					if (fPixel<0.0)
//						fPixel=0.0;
//					if (fPixel>255.0)
//						fPixel=255.0;
//					pResult[(i*8+k)*256+(j*8+m)]=(BYTE)fPixel;
//						
//				}
//		}


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
//	file.Close();
	ImageKit *pComprImage=pDlg->GetComprImage();
//	pComprImage->FromObject(*pImage,pResult);
	pComprImage->DecodeBMP(_T("peppers1.bmp"));
	pDlg->DisplayComprBitmap();
	DOUBLE all=pImage->GetPSNRFullChannel(*pComprImage);
	CReportDlg ReportDlg;

	ReportDlg.Set(0,0,0,0,all);
	ReportDlg.DoModal();

	pDlg->EnableOptions(TRUE);
	pDlg->EnableCompress(TRUE);
	pDlg->SetFinished(TRUE);
	return FALSE;
}

ImageKit * CNNICDlg::GetOriginalImage()
{
	return &m_OriginalImage;
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

ImageKit * CNNICDlg::GetComprImage()
{
	return &m_ComprImage;
}

void CNNICDlg::DisplayComprBitmap()
{
	m_staticCompr.SetBitmap(m_ComprImage.GetHandle(m_staticCompr.GetDC()));
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

void CNNICDlg::SetFinished(const BOOL bFlag)
{
	m_bFinished=bFlag;
}

void CNNICDlg::OnButtonOptions() 
{
	// TODO: Add your control notification handler code here
	CPropertySheet propOptions;
	CBPDlg pageBP(m_optionsDefault.st_optionsBP);
	CCPDlg pageCP(m_optionsDefault.st_optionsCP);
	propOptions.SetTitle(_T("Compression Options"));
	propOptions.AddPage(&pageBP);
	propOptions.AddPage(&pageCP);

	//UINT uiMethod=GetCheckedRadioButton(IDC_RADIO_BACK,IDC_RADIO_DCTBACK)-IDC_RADIO_BACK;
	//propOptions.SetActivePage(/*&pageCP*/uiMethod);
	propOptions.DoModal();
	pageBP.GetOptionsBP(&m_optionsDefault.st_optionsBP);
	pageCP.GetOptionsCP(&m_optionsDefault.st_optionsCP);
}

OPTIONSBP * CNNICDlg::GetOptionsBP()
{
	return &m_optionsDefault.st_optionsBP;
}

void CNNICDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	OnFileOpen();
}
