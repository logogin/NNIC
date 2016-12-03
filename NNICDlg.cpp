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
	OPTIONSCP optionsCP(1024,10,0.035f,0.3f,0.1f,10000,0.01f,3,0,50000,1);
	OPTIONSDCT optionsDCT(10,TRUE);
	OPTIONS options(optionsBP,optionsCP,optionsDCT);
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
		m_threadCompress=AfxBeginThread(StartDCT,this/*,THREAD_PRIORITY_NORMAL,10*1024*1024,0,NULL*/);
		break;
	case IDC_RADIO_DCTBACK:
		EnableOptions(FALSE);
		m_bFinished=FALSE;
		m_threadCompress=AfxBeginThread(StartDCTBack,this);
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
	ofstream out("image1.dat",ios::out | ios::binary);
	out<<pImage->GetImageSize();
	out.write(pBits,pImage->GetImageSize());

	out.close();
	ASSERT(pBits!=NULL);

	OPTIONSCP *optionsCP=pDlg->GetOptionsCP();
	ASSERT(optionsCP!=NULL);

	/*******!!!!!!!!!!!!!*********/
	//optionsCP->st_ulCycles=0;

	UINT uiLayers[3]={3,optionsCP->st_wClusters,3};
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
	BYTE *uiClusters=new BYTE[uiSize];
	//WORD *uiClusters=new WORD[uiSize];
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
	FLOAT fLearnRate=optionsCP->st_fInitLearnRate;
	WORD wNeighSize=optionsCP->st_wInitNeighSize;
	if (optionsCP->st_fInitLearnRate>=optionsCP->st_fFinalLearnRate)
		iLearnSign=-1;
	if (optionsCP->st_wInitNeighSize>=optionsCP->st_wFinalNeighSize)
		iNeighSign=-1;
	//QueryPerformanceCounter(&liCount1);
	do
	{
		FLOAT fTime=0.0;
		fWinDist=0.0f;
		pDlg->SetProgress(0);
		for (i=0; i<uiSize; i++)
		{
			
			ulLearnCount++;
			ulNeighCount++;
			if (ulLearnCount==optionsCP->st_ulLearnRateEpochs)
			{
				ulLearnCount=0;
				fLearnRate+=iLearnSign*optionsCP->st_fLearnChangeRate;
				if (iLearnSign==-1&&fLearnRate<optionsCP->st_fFinalLearnRate)
					fLearnRate=optionsCP->st_fFinalLearnRate;
				if (iLearnSign==1&&fLearnRate>optionsCP->st_fFinalLearnRate)
					fLearnRate=optionsCP->st_fFinalLearnRate;
			}

			if (ulNeighCount==optionsCP->st_ulNeighSizeEpochs)
			{
				ulNeighCount=0;
				switch (iNeighSign)
				{
				case -1:
					if (wNeighSize>optionsCP->st_wFinalNeighSize)
						wNeighSize-=optionsCP->st_wNeighChangeRate;
					else
						wNeighSize=optionsCP->st_wFinalNeighSize;
					break;
				case 1:
					if (wNeighSize<optionsCP->st_wFinalNeighSize)
						wNeighSize+=optionsCP->st_wNeighChangeRate;
					else
						wNeighSize=optionsCP->st_wFinalNeighSize;
					break;
				}
				
			}
		
			Net.SetNeighborhoodSize(wNeighSize);
			Net.SetAxons(INPUT_LAYER,vPatterns[i]);
			QueryPerformanceCounter(&liCount1);
			Net.PropagateKohonen();
			QueryPerformanceCounter(&liCount2);
			uiClusters[i]=Net.GetWinnerNeuron();
			if (!map.Lookup(uiClusters[i],pValue))
				map.SetAt(uiClusters[i],&uiClusters[i]);
			fWinDist+=Net.GetWinnerDistance();
			Net.LearnKohonen(fLearnRate);
			if (!((i+1)%uiInc))
				pDlg->StepProgress();
			
			fTime+=(FLOAT)(liCount2.QuadPart-liCount1.QuadPart)/liFreq.QuadPart;
		}
		
		fWinDist/=uiSize;
		uiCycles++;
		pDlg->SetDlgItemInt(IDC_STATIC_CYCLES,uiCycles);
		sString.Format(_T("%f"),fWinDist);
		pDlg->SetDlgItemText(IDC_STATIC_AVDIST,sString);
		sString.Format(_T("%f"),fTime);
		//QueryPerformanceCounter(&liCount2);
		//sString.Format(_T("%f"),(FLOAT)(liCount2.QuadPart-liCount1.QuadPart)/liFreq.QuadPart);
		pDlg->SetDlgItemText(IDC_STATIC_TIME,sString);
		pDlg->SetDlgItemInt(IDC_STATIC_UNUSED,optionsCP->st_wClusters-map.GetCount());
		map.RemoveAll();
	}
	while(fWinDist>optionsCP->st_fMinDist&&uiCycles<optionsCP->st_ulCycles);

	for (i=0; i<uiSize; i++)
		delete []vPatterns[i];
	delete []vPatterns;

	uiCycles=0;

	for (i=0; i<uiSize; i++)
	{
		Net.SetWinnerNeuron(uiClusters[i]);	
		Net.LearnGrossberg(&pBits[i*3]);
	}
	Net.FinilizeGrossberg();
	QueryPerformanceCounter(&liCount2);

	BYTE *pResult=new BYTE[uiSize*3];
	ASSERT(pResult!=NULL);
	ofstream f("debug.txt",ios::out);
	for (i=0; i<uiSize; i++)
	{
		f<<i<<" "<<(WORD)uiClusters[i]<<endl;
		for (UINT j=0; j<3; j++)
/******************/
			pResult[i*3+j]=(BYTE)floor(/*127.0+*/Net.GetWeight(GROSSBERG_LAYER,j,uiClusters[i])+0.5);
	}
	f.close();
//	ifstream f("result.dat",ios::in | ios::binary);
//	DWORD size;
//	f>>size;
//	f.read(pResult,size);
//	f.close();

	ImageKit *pComprImage=pDlg->GetComprImage();
	pComprImage->FromObject(*pImage,pResult);
	pDlg->DisplayComprBitmap();
	//	*pImage,pResult);
    	
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

	ULONG uiSrcLen=optionsCP->st_wClusters*3;
	ULONG uiDestLen=(ULONG)(uiSrcLen*100.1/100+0.5)+12;

	BYTE *pSrc=new BYTE[uiSrcLen];
	ASSERT(pSrc!=NULL);
	BYTE *pDest=new BYTE[uiDestLen];
	ASSERT(pDest!=NULL);
	ZeroMemory(pDest,uiDestLen);

	for (i=0; i<optionsCP->st_wClusters; i++)
		for (UINT j=0; j<3; j++)
			pSrc[i*3+j]=(BYTE)(127.0+Net.GetWeight(GROSSBERG_LAYER,j,i)+0.5);

	ASSERT(compress(pDest,&uiDestLen,pSrc,uiSrcLen)==Z_OK);

	file.Write(&uiSrcLen,sizeof(ULONG));
	file.Write(&uiDestLen,sizeof(ULONG));
	file.Write(pDest,uiDestLen);

	delete []pSrc;
	delete []pDest;
	uiSrcLen=uiSize*sizeof(BYTE);
	//uiSrcLen=uiSize*sizeof(WORD);
	uiDestLen=(ULONG)(uiSrcLen*100.1/100+0.5)+12;

	pDest=new BYTE[uiDestLen];
	ASSERT(pDest!=NULL);
	ZeroMemory(pDest,uiDestLen);

	ASSERT(compress(pDest,&uiDestLen,(BYTE *)&uiClusters[0],uiSrcLen)==Z_OK);

	file.Write(&uiSrcLen,sizeof(ULONG));
	file.Write(&uiDestLen,sizeof(ULONG));
	file.Write(pDest,uiDestLen);

	CReportDlg ReportDlg;
	ReportDlg.SetFileName(file.GetFileName());
	ReportDlg.SetDirectory(file.GetFilePath());
	ReportDlg.SetFileSize(pDlg->GetOriginalFileSize(),file.GetLength());
	file.Close();
	ReportDlg.SetImageSize(pImage->GetImageWidth(),pImage->GetImageHeight());
	ReportDlg.SetOriginalColors(pImage->GetBitsPerPixel(),pImage->GetColors());
	ReportDlg.SetCompressionMethod(_T("Counter Propagation"));
	DOUBLE fRed=pImage->GetPSNR(0,*pComprImage);
	DOUBLE fGreen=pImage->GetPSNR(1,*pComprImage);
	DOUBLE fBlue=pImage->GetPSNR(2,*pComprImage);
	ReportDlg.SetPSNR(fRed,fGreen,fBlue,(fRed+fGreen+fBlue)/3,pImage->GetPSNRFullChannel(*pComprImage));
	ReportDlg.SetCompressionTime((FLOAT)(liCount2.QuadPart-liCount1.QuadPart)/liFreq.QuadPart);

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
	//optionsBP->st_wLearnMode=1;
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
	
	Net.SetLearnType(uiMethod);
	Net.InitWeights();

	ImageKit *pImage=pDlg->GetOriginalImage();
	/********!!!!!!!!!!!*********/
//	optionsBP->st_wPatterns=1024;

	FLOAT Segments[32][32][64];
	for (UINT i=0 ;i<32; i++)
		for (UINT j=0; j<32; j++)
		{
			CPoint point(j*8,i*8);
			CSize size(8,8);
			CRect rect(point,size);
			pImage->GetSegment(rect,Segments[i][j],0,-0.5f,1.0f);
		}

	LARGE_INTEGER liFreq;
	LARGE_INTEGER liCount1;
	LARGE_INTEGER liCount2;

	UINT uiPatterns;
	FLOAT fNetError;
	ULONG ulCycles;
	BOOL bFlag;
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
	ULONG ulSize=pImage->GetImageSize();
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
					pResult[(i*8+k)*256+(j*8+m)]=
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

	ImageKit *pComprImage=pDlg->GetComprImage();
	pComprImage->FromObject(*pImage,pResult);
	pDlg->DisplayComprBitmap();

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

	COMPRFILEINFO cfInfo(COMPRFILE_NNIC,COMPRTYPE_BP,pImage->GetImageWidth(),
		pImage->GetImageHeight(),ulSize);
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
	ReportDlg.SetFileSize(pDlg->GetOriginalFileSize(),file.GetLength());
	file.Close();
	ReportDlg.SetImageSize(pImage->GetImageWidth(),pImage->GetImageHeight());
	ReportDlg.SetOriginalColors(pImage->GetBitsPerPixel(),pImage->GetColors());
	ReportDlg.SetCompressionMethod(_T("Back Propagation"));
	ReportDlg.SetPSNR(pImage->GetPSNRFullChannel(*pComprImage));
	ReportDlg.SetCompressionTime((FLOAT)(liCount2.QuadPart-liCount1.QuadPart)/liFreq.QuadPart);
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

	OPTIONSDCT *optionsDCT=pDlg->GetOptionsDCT();
	ASSERT(optionsDCT!=NULL);

	pDlg->EnableCompress(FALSE);
	pDlg->EnableStop(TRUE);

	DCT Dct(DCT_SIZE);
	Dct.SetQuality(optionsDCT->st_bQuality);
	ImageKit *pImage=pDlg->GetOriginalImage();

	FLOAT fSegment[DCT_SIZE][DCT_SIZE];
	SHORT sZigZag[DCT_SIZE*DCT_SIZE];
	FLOAT fShift;
	if (optionsDCT->st_bShift)
		fShift=-128.0f;
	else
		fShift=0.0;

	const BYTE bBytesPerPixel=pImage->GetBytesPerPixel();
	const ULONG ulHeight=pImage->GetImageHeight();
	const ULONG ulWidth=pImage->GetImageWidth();
	const ULONG ulPixels=ulHeight*ulWidth;

	FLOAT fPixel;
	INT i,j,k,m,l;

	BYTE **pBitmap=NULL;
	ASSERT((pBitmap=AllocateByteMatrix(ulHeight,ulWidth*bBytesPerPixel))!=NULL);

	USHORT *pResult=new USHORT[ulPixels*bBytesPerPixel];

	LARGE_INTEGER liFreq;
	LARGE_INTEGER liCount1;
	LARGE_INTEGER liCount2;
	QueryPerformanceFrequency(&liFreq);
	QueryPerformanceCounter(&liCount1);

	ULONG ulCounter=0;
	for (i=0; i<32; i++)
		for (j=0; j<32; j++)
			for (k=0; k<bBytesPerPixel; k++)
			{
				CPoint point(j*DCT_SIZE,i*DCT_SIZE);
				CSize size(DCT_SIZE,DCT_SIZE);
				CRect rect(point,size);
				pImage->GetSegment(rect,&(fSegment[0][0]),k,fShift,256.0f);
				Dct.DCTransform(&(fSegment[0][0]));
				Dct.QuantizateBlock();
				Dct.ZigZagSequence(sZigZag);
				Dct.DequantizateBlock();
				Dct.IDCTransform();
				Dct.GetBlock(&(fSegment[0][0]));
				for (l=0; l<DCT_SIZE; l++)
					for (m=0; m<DCT_SIZE; m++)
					{
						fPixel=(FLOAT)floor(fSegment[l][m]-fShift+0.5f);
						if (fPixel<0.0)
							fPixel=0.0;
						if (fPixel>255.0)
							fPixel=255.0;
						pBitmap[i*DCT_SIZE+l][(j*DCT_SIZE+m)*bBytesPerPixel+k]=(BYTE)fPixel;
						pResult[ulCounter++]=sZigZag[l*DCT_SIZE+m];
						//pResult[k*32*32*DCT_SIZE*DCT_SIZE+(i*DCT_SIZE+l)*32*DCT_SIZE+(j*DCT_SIZE+m)]=sZigZag[l*DCT_SIZE+m];
					}

			}
	QueryPerformanceCounter(&liCount2);

	ImageKit *pComprImage=pDlg->GetComprImage();
	pComprImage->FromObject(*pImage,pBitmap[0]);
	pDlg->DisplayComprBitmap();

	DestroyMatrix((LPVOID *)pBitmap);

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

	COMPRFILEINFO cfInfo(COMPRFILE_NNIC,COMPRTYPE_DCT,pImage->GetImageWidth(),
		pImage->GetImageHeight(),ulPixels*bBytesPerPixel);
	file.Write(&cfInfo,sizeof(COMPRFILEINFO));

	ULONG uiSrcLen=32*32*DCT_SIZE*DCT_SIZE*sizeof(SHORT)*bBytesPerPixel;
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
	ReportDlg.SetFileSize(pDlg->GetOriginalFileSize(),file.GetLength());
	file.Close();
	ReportDlg.SetImageSize(pImage->GetImageWidth(),pImage->GetImageHeight());
	ReportDlg.SetOriginalColors(pImage->GetBitsPerPixel(),pImage->GetColors());
	ReportDlg.SetCompressionMethod(_T("DC Transform"));
	if (bBytesPerPixel>1)
	{
		DOUBLE fRed=pImage->GetPSNR(0,*pComprImage);
		DOUBLE fGreen=pImage->GetPSNR(1,*pComprImage);
		DOUBLE fBlue=pImage->GetPSNR(2,*pComprImage);
		ReportDlg.SetPSNR(fRed,fGreen,fBlue,(fRed+fGreen+fBlue)/3,
			pImage->GetPSNRFullChannel(*pComprImage));
	}
	else
		ReportDlg.SetPSNR(pImage->GetPSNRFullChannel(*pComprImage));
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
	ImageKit *pImage=pDlg->GetOriginalImage();

	FLOAT Segments[32][32][64];

	for (UINT i=0; i<32; i++)
		for (UINT j=0; j<32; j++)
		{
			CPoint point(j*8,i*8);
			CSize size(8,8);
			CRect rect(point,size);
			pImage->GetSegment(rect,Segments[i][j],0,-127.0/*0.0*/,256.0);
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
	
	Net.SetLearnType(uiMethod);
	Net.InitWeights();

	LARGE_INTEGER liFreq;
	LARGE_INTEGER liCount1;
	LARGE_INTEGER liCount2;

	UINT uiPatterns;
	FLOAT fNetError;
	ULONG ulCycles;
	BOOL bFlag;
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


	ULONG ulSize=pImage->GetImageSize();
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

	ImageKit *pComprImage=pDlg->GetComprImage();
	pComprImage->FromObject(*pImage,pResult);
	pDlg->DisplayComprBitmap();

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

	COMPRFILEINFO cfInfo(COMPRFILE_NNIC,COMPRTYPE_BP,pImage->GetImageWidth(),
		pImage->GetImageHeight(),ulSize);
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
	ReportDlg.SetFileSize(pDlg->GetOriginalFileSize(),file.GetLength());
	file.Close();
	ReportDlg.SetImageSize(pImage->GetImageWidth(),pImage->GetImageHeight());
	ReportDlg.SetOriginalColors(pImage->GetBitsPerPixel(),pImage->GetColors());
	ReportDlg.SetCompressionMethod(_T("Back Propagation"));
	ReportDlg.SetPSNR(pImage->GetPSNRFullChannel(*pComprImage));
	ReportDlg.SetCompressionTime((FLOAT)(liCount2.QuadPart-liCount1.QuadPart)/liFreq.QuadPart);
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
	propOptions.SetActivePage(/*&pageCP*/uiMethod);
	propOptions.DoModal();
	pageBP.GetOptionsBP(&m_optionsDefault.st_optionsBP);
	pageCP.GetOptionsCP(&m_optionsDefault.st_optionsCP);
	pageDCT.GetOptionsDCT(&m_optionsDefault.st_optionsDCT);
}

OPTIONSBP * CNNICDlg::GetOptionsBP()
{
	return &m_optionsDefault.st_optionsBP;
}

ULONG CNNICDlg::GetOriginalFileSize()
{
	CFile file;
	CFileStatus rStatus;
	file.GetStatus(m_sOriginalFileName,rStatus);
	return rStatus.m_size;
}

OPTIONSDCT * CNNICDlg::GetOptionsDCT()
{
	return &m_optionsDefault.st_optionsDCT;
}
