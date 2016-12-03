// ComprDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NNIC.h"
#include "ComprDlg.h"
#include "BPDlg.h"
#include "CPDlg.h"
#include "NNICDlg.h"
#include "ReportDlg.h"

#include "./NeuroNet/NetCP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CComprDlg dialog
IMPLEMENT_DYNAMIC(CComprDlg,CDialog);

CComprDlg::CComprDlg(const OPTIONS &options,ImageKit *pImageKit,CWnd* pParent /*=NULL*/)
	: CDialog(CComprDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CComprDlg)
	//}}AFX_DATA_INIT
	CopyMemory(&m_Options,&options,sizeof(OPTIONS));
	ASSERT(pImageKit!=NULL);
	m_pImageKit=pImageKit;
}

void CComprDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CComprDlg)
	DDX_Control(pDX, IDC_BUTTON_OPTIONS, m_btnOptions);
	DDX_Control(pDX, IDC_PROGRESS_COMPRESSION, m_prgCompress);
	DDX_Control(pDX, IDC_BUTTON_START, m_btnStart);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CComprDlg, CDialog)
	//{{AFX_MSG_MAP(CComprDlg)
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_OPTIONS, OnButtonOptions)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComprDlg message handlers

void CComprDlg::OnButtonStart() 
{
	// TODO: Add your control notification handler code here
	/*UINT (*ptrMethods[4])(LPVOID);*/
	UINT uiMethod=GetCheckedRadioButton(IDC_RADIO_BACK,IDC_RADIO_DSTBACK);

	switch (uiMethod)
	{
	case IDC_RADIO_COUNTER:
		EnableStart(FALSE);
		EnableOptions(FALSE);
		m_bFinished=FALSE;
		m_trdCompress=AfxBeginThread(StartCP,this);
		break;
	}
}

void CComprDlg::OnButtonOptions() 
{
	// TODO: Add your control notification handler code here

	CPropertySheet propOptions;
	CBPDlg pageBP;
	CCPDlg pageCP(m_Options.st_optionsCP);
	propOptions.SetTitle(_T("Compression Options"));
	propOptions.AddPage(&pageBP);
	propOptions.AddPage(&pageCP);

	propOptions.SetActivePage(&pageCP);
	propOptions.DoModal();
	pageCP.GetOptionsCP(&m_Options.st_optionsCP);
}

BOOL CComprDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CheckRadioButton(IDC_RADIO_BACK,IDC_RADIO_DSTBACK,IDC_RADIO_COUNTER);
	m_prgCompress.SetStep(1);
	m_trdCompress=NULL;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

UINT StartCP(LPVOID pParam)
{
	CComprDlg* pDlg = (CComprDlg *)pParam;

    if (pDlg == NULL ||
        !pDlg->IsKindOf(RUNTIME_CLASS(CComprDlg)))
    return TRUE;

	ImageKit *pImage=pDlg->GetImageKit();
	const BYTE *pBits=pImage->GetBits();
	ASSERT(pBits!=NULL);

	OPTIONSCP *pOptions=pDlg->GetOptionsCP();
	ASSERT(pOptions!=NULL);

	UINT uiLayers[3]={3,pOptions->st_uiClusters,3};
	NetCP Net(uiLayers);
	Net.SetNeighborhoodSize(pOptions->st_uiNeighSize);
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
	FLOAT fStartLearn=0.9f;
	FLOAT fPrevDist;
	BOOL bFirst=TRUE;
	do
	{
		
		fWinDist=0.0f;
		pDlg->SetProgress(0);
		QueryPerformanceCounter(&liCount1);
		for (i=0; i<uiSize; i++)
		{
			Net.SetAxons(INPUT_LAYER,vPatterns[i]);
			Net.PropagateKohonen();
			uiClusters[i]=Net.GetWinnerNeuron();
			fWinDist+=Net.GetWinnerDistance();			
			Net.LearnKohonen(/*pOptions->st_fLRKohonen*/fStartLearn);
			if (!((i+1)%uiInc))
				pDlg->StepProgress();
		}
		QueryPerformanceCounter(&liCount2);
		fWinDist/=uiSize;
		if (bFirst)
		{
			fPrevDist=fWinDist;
			bFirst=FALSE;
		}

		if (fWinDist>fPrevDist)
			fStartLearn/=2;
		fPrevDist=fWinDist;
		/*else
			fStartLearn*=2;*/
		uiCycles++;
		pDlg->SetDlgItemInt(IDC_STATIC_CYCLES,uiCycles);
		sString.Format(_T("%f"),fWinDist);
		pDlg->SetDlgItemText(IDC_STATIC_AVDIST,sString);
		sString.Format(_T("%f"),(FLOAT)(liCount2.QuadPart-liCount1.QuadPart)/liFreq.QuadPart);
		pDlg->SetDlgItemText(IDC_STATIC_TIME,sString);
	}
	while(fWinDist>pOptions->st_fDistKohonen&&uiCycles<=pOptions->st_uiCycles);

	for (i=0; i<uiSize; i++)
		delete []vPatterns[i];
	delete []vPatterns;

	uiCycles=0;
	FLOAT fPixel[3];
	do
	{
		fWinDist=0.0f;
		pDlg->SetProgress(0);
		QueryPerformanceCounter(&liCount1);
		for (i=0; i<uiSize; i++)
		{
			//Net.SetAxons(INPUT_LAYER,vPatterns[i]);
			//Net.PropagateKohonen();
			for (UINT j=0; j<3; j++)
				fPixel[j]=pBits[3*i+j]/*-127.0f*/;
			Net.SetWinnerNeuron(uiClusters[i]);
			fWinDist+=Net.GetTargetDistance(fPixel);			
			Net.LearnGrossberg(pOptions->st_fLRGrossberg,fPixel);
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
	}
	while(fWinDist>pOptions->st_fDistGrossberg&&uiCycles<=pOptions->st_uiCycles);

	BYTE *pResult=new BYTE[uiSize*3];
	ASSERT(pResult!=NULL);
	for (i=0; i<uiSize; i++)
		for (UINT j=0; j<3; j++)
			pResult[i*3+j]=(BYTE)(/*127.0+*/Net.GetWeight(GROSSBERG_LAYER,j,uiClusters[i])+0.5);

	ImageKit ComprImage(*pImage,pResult);
	FLOAT r=pImage->GetPSNR(0,ComprImage);
	FLOAT b=pImage->GetPSNR(1,ComprImage);
	FLOAT g=pImage->GetPSNR(2,ComprImage);
	FLOAT av=(r+g+b)/3;
	FLOAT all=(FLOAT)pImage->GetPSNRFullChannel(ComprImage);
	CReportDlg ReportDlg;

	pDlg->DisplayComprBitmap(pResult);
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

	ULONG uiSrcLen=pOptions->st_uiClusters*3;
	ULONG uiDestLen=(ULONG)(uiSrcLen*100.1/100+0.5)+12;

	BYTE *pSrc=new BYTE[uiSrcLen];
	ASSERT(pSrc!=NULL);
	BYTE *pDest=new BYTE[uiDestLen];
	ASSERT(pDest!=NULL);
	ZeroMemory(pDest,uiDestLen);

	for (i=0; i<pOptions->st_uiClusters; i++)
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
	pDlg->EnableStart(TRUE);
	pDlg->SetFinished(TRUE);
	return FALSE;
}

ImageKit * CComprDlg::GetImageKit()
{
	return m_pImageKit;
}

OPTIONSCP * CComprDlg::GetOptionsCP()
{
	return &m_Options.st_optionsCP;
}

void CComprDlg::EnableStart(const BOOL bFlag)
{
	m_btnStart.EnableWindow(bFlag);
}

void CComprDlg::SetProgress(const UINT uiPos)
{
	m_prgCompress.SetPos(uiPos);
}

INT CComprDlg::StepProgress()
{
	return m_prgCompress.StepIt();
}

void CComprDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	if (m_trdCompress!=NULL&&!m_bFinished)
	{
		m_btnOptions.EnableWindow(TRUE);
		m_btnStart.EnableWindow(TRUE);
		TerminateThread(m_trdCompress->m_hThread,0);
		delete m_trdCompress;
		m_trdCompress=NULL;
	}

	else
		CDialog::OnCancel();
}

void CComprDlg::EnableOptions(const BOOL bFlag)
{
	m_btnOptions.EnableWindow(bFlag);
}

void CComprDlg::SetFinished(const BOOL fFlag)
{
	m_bFinished=fFlag;
}

void CComprDlg::DisplayComprBitmap(const BYTE *pData)
{
	CNNICDlg *pDlg=(CNNICDlg *)GetParent();

	BOOL b=IsWindow(pDlg->m_hWnd);

	ImageKit ComprImage(*m_pImageKit,pData);
	CStatic *pStatic=(CStatic *)pDlg->GetDlgItem(IDC_STATIC_COMPRESSED);
	pStatic->SetBitmap(ComprImage.GetHandle(pStatic->GetDC()));
}
