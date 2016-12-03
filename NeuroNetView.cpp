// NeuroNetView.cpp : implementation of the CNeuroNetView class
//

#include "stdafx.h"
#include "NeuroNet.h"

#include "NeuroNetDoc.h"
#include "NeuroNetView.h"
#include "MainFrm.h"

#include "Net.h"
#include "NetCP.h"
#include "ImageKit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNeuroNetView

IMPLEMENT_DYNCREATE(CNeuroNetView, CEditView)

BEGIN_MESSAGE_MAP(CNeuroNetView, CEditView)
	//{{AFX_MSG_MAP(CNeuroNetView)
	ON_COMMAND(ID_START_NETCP, OnStartNetcp)
	ON_COMMAND(ID_START_NETBP, OnStartNetbp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNeuroNetView construction/destruction

CNeuroNetView::CNeuroNetView()
{
	// TODO: add construction code here

}

CNeuroNetView::~CNeuroNetView()
{
}

BOOL CNeuroNetView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	BOOL bPreCreated = CEditView::PreCreateWindow(cs);
	cs.style &= ~(ES_AUTOHSCROLL|WS_HSCROLL);	// Enable word-wrapping

	return bPreCreated;
}

/////////////////////////////////////////////////////////////////////////////
// CNeuroNetView drawing

void CNeuroNetView::OnDraw(CDC* pDC)
{
	CNeuroNetDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CNeuroNetView diagnostics

#ifdef _DEBUG
void CNeuroNetView::AssertValid() const
{
	CEditView::AssertValid();
}

void CNeuroNetView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CNeuroNetDoc* CNeuroNetView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNeuroNetDoc)));
	return (CNeuroNetDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CNeuroNetView message handlers
UINT StartNetCP(LPVOID pParam)
{
	CNeuroNetView* pView = (CNeuroNetView *)pParam;

    if (pView == NULL ||
        !pView->IsKindOf(RUNTIME_CLASS(CNeuroNetView)))
    return 1;

	LARGE_INTEGER freq;
	LARGE_INTEGER count1;
	LARGE_INTEGER count2;
	QueryPerformanceFrequency(&freq);

#define SIZE 65536

	ImageKit Image;
	Image.DecodeBMP(_T("mandrill256.bmp"));

	BYTE *pData=Image.GetBits();
	UINT uiClusters[SIZE];

//	FLOAT vectors[2][4]={{0.4f,0.98f,0.1f,0.2f},{0.5f,0.22f,0.8f,0.9f}};
	UINT uiLayers[3]={3,1024,3};
	NetCP Net(uiLayers);

	Net.SetNeighborhoodSize(0);
	Net.InitWeights(-1.0f,2.0f);

	CString sMessage;

	CMainFrame *pMainFrame = (CMainFrame *)pView->GetParent();

	FLOAT fDist;
	UINT uiCycles=0;
	//ofstream out("dist.txt",ios::out);
	do
	{
		fDist=0.0f;
		QueryPerformanceCounter(&count1);
		for (UINT i=0; i<SIZE; i++)
		{
			FLOAT fPixel[3]={pData[i*3]-127.0f,pData[i*3+1]-127.0f,pData[i*3+2]-127.f};
			Net.SetAxons(INPUT_LAYER,fPixel);
			Net.PropagateKohonen();
			float d=Net.GetWinnerDistance();
			fDist+=d;
			uiClusters[i]=Net.GetWinnerNeuron();
			Net.LearnKohonen(0.3f);
			//Net.LearnGrossberg(0.2f,fPixel);
			//out<<uiCycles<<"\t"<<uiClusters[i]<<"\t"<<d<<endl;
		}
		QueryPerformanceCounter(&count2);

		UINT uiUnused=0;
		for (i=0; i<1024; i++)
		{
			BOOL flag=FALSE;
			for (UINT j=0; j<SIZE&&!flag; j++)
				if (uiClusters[j]==i)
					flag=TRUE;
			if (!flag)
				uiUnused++;
		}

		uiCycles++;
		fDist/=SIZE;
		sMessage.Format(_T("Avg. Dist: %f Time: %f sec. Cycle: %d Unused: %d"),
			fDist,(float)(count2.QuadPart-count1.QuadPart)/freq.QuadPart,uiCycles,uiUnused);
		pMainFrame->SetMessageText(sMessage);
	}
	while (fDist>0.034f);

	FLOAT fLearnRate=0.1f;
	uiCycles=0;
	do
	{
		fDist=0.0f;
		QueryPerformanceCounter(&count1);
		for (UINT i=0; i<SIZE; i++)
		{
			FLOAT fPixel[3]={pData[i*3]-127.0f,pData[i*3+1]-127.0f,pData[i*3+2]-127.f};
			FLOAT fP[3]={pData[i*3],pData[i*3+1],pData[i*3+2]};
			Net.SetAxons(INPUT_LAYER,fPixel);
			Net.PropagateKohonen();
			uiClusters[i]=Net.GetWinnerNeuron();
			fDist+=Net.GetTargetDistance(fP);
			Net.LearnGrossberg(fLearnRate,fP);
		}
		QueryPerformanceCounter(&count2);

		//fLearnRate=-0.01;
		/*UINT uiUnused=0;
		for (i=0; i<512; i++)
		{
			BOOL flag=FALSE;
			for (UINT j=0; j<SIZE&&!flag; j++)
				if (uiClusters[j]==i)
					flag=TRUE;
			if (!flag)
				uiUnused++;
		}*/

		uiCycles++;
		fDist/=SIZE;
		sMessage.Format(_T("Avg. Dist: %f Time: %f sec. Cycle: %d "),
			fDist,(float)(count2.QuadPart-count1.QuadPart)/freq.QuadPart,uiCycles);
		pMainFrame->SetMessageText(sMessage);
	}
	while (fDist>14.1f);

	//BYTE bColors[1024][3];
//	UINT uiPixels;
//	FLOAT fColors[3];
	BYTE bColors[3];
	
/*	for (UINT i=0; i<1024; i++)
	{
		uiPixels=0;
		ZeroMemory(fColors,sizeof(FLOAT)*3);
		for (UINT j=0; j<SIZE; j++)
			if (i==uiClusters[j])
			{
				for (UINT k=0; k<3; k++)
					fColors[k]+=pData[j*3+k]-127;
				uiPixels++;
			}
		if (uiPixels)
		{
			for (j=0; j<3; j++)
				bColors[j]=127+(fColors[j]/uiPixels+0.5);
			for (j=0; j<SIZE; j++)
				if (i==uiClusters[j])
					CopyMemory(&pData[j*3],bColors,sizeof(BYTE)*3);
		}
	}*/
	for (UINT i=0; i<SIZE; i++)
	{
		for (UINT j=0; j<3; j++)
			bColors[j]=/*127+*/Net.GetWeight(GROSSBERG_LAYER,j,uiClusters[i])+0.5;
		CopyMemory(&pData[i*3],bColors,sizeof(BYTE)*3);
	}
	//out.close();
	CFile BMPFile;
	BMPFile.Open(_T("mandrill256.bmp"),CFile::modeRead);

	BITMAPFILEHEADER bmfHeader;
	BMPFile.Read(&bmfHeader,sizeof(BITMAPFILEHEADER));

	BITMAPINFOHEADER bmiHeader;
	BMPFile.Read(&bmiHeader,sizeof(BITMAPINFOHEADER));

	BMPFile.Close();
	BMPFile.Open(_T("image.bmp"),CFile::modeCreate | CFile.modeWrite);
	BMPFile.Write(&bmfHeader,sizeof(BITMAPFILEHEADER));
	BMPFile.Write(&bmiHeader,sizeof(BITMAPINFOHEADER));
	BMPFile.Write(pData,sizeof(BYTE)*(bmfHeader.bfSize - bmfHeader.bfOffBits));
	BMPFile.Close();
	return 0;
}

void CNeuroNetView::OnStartNetcp() 
{
	// TODO: Add your command handler code here
	AfxBeginThread(StartNetCP,this);
/*	LARGE_INTEGER freq;
	LARGE_INTEGER count1;
	LARGE_INTEGER count2;
	QueryPerformanceFrequency(&freq);

	ImageKit Image;
	Image.DecodeBMP(_T("Mandrill256.bmp"));

	BYTE *pData=Image.GetBits();

	UINT uiLayers[3]={3,1024,3};
	NetCP Net(uiLayers);
//	Net.InitWeights(-127.0f,255.0f);
#define SIZE 65536*/
//#define SIZE 4096

/*	UINT uiClusters[SIZE];

	CString sMessage;
	CNeuroNetDoc* pDoc = GetDocument();
	CMainFrame *pMainFrame = (CMainFrame *)GetParentFrame();

	LARGE_INTEGER liFreq;
	QueryPerformanceFrequency(&liFreq);
	LARGE_INTEGER liCount1;
	LARGE_INTEGER liCount2;
	QueryPerformanceCounter(&liCount1);
	FLOAT fError;
	do
	{
		fError=0.0;
	for (UINT i=0; i<SIZE; i++)
	{
		FLOAT fPixel[3]={pData[i*3]-127.0f,pData[i*3+1]-127.0f,pData[i*3+2]-127.f};
		Net.SetAxons(INPUT_LAYER,fPixel);
		Net.PropagateKohonen();
		fError+=Net.LearnKohonen();
		uiClusters[i]=Net.GetWinnerIndex();
		QueryPerformanceCounter(&liCount2);


		sMessage.Format(_T("Pixel: %d Error: %f Time: %f sec."),
			i,fError/65536,(float)(liCount2.QuadPart-liCount1.QuadPart)/liFreq.QuadPart);
		pMainFrame->SetMessageText(sMessage);
	}

	CStdioFile f("clusters.txt",CFile::modeCreate|CFile::modeWrite);

	UINT UnUsed=0;
	CString string;
	for (i=0; i<1024; i++)
	{
		
		string.Format("%d\n",i);
		f.WriteString(string);
		BOOL flag=false;
		for (UINT j=0; j<SIZE; j++)
			if (uiClusters[j]==i)
			{
				string.Format("%d %d %d\n",pData[j*3]-127,pData[j*3+1]-127,pData[j*3+2]-127);
				f.WriteString(string);
				flag=true;
			}
		if (!flag)
			UnUsed++;

	}
	string.Format("%d Clusters Unused",UnUsed);
	f.WriteString(string);
	f.Close();
	AfxMessageBox(string,MB_OK);

	}
	while (fError/65536>0.001);*/
	
/*	for (i=0; i<SIZE; i++)
	{
		pData[i*3]=(BYTE)(Net.GetWeight(GROSSBERG_LAYER,0,uiClusters[i])+0.5);
		pData[i*3+1]=(BYTE)(Net.GetWeight(GROSSBERG_LAYER,1,uiClusters[i])+0.5);
		pData[i*3+2]=(BYTE)(Net.GetWeight(GROSSBERG_LAYER,2,uiClusters[i])+0.5);
	}	*/

	/*CFile BMPFile;
	BMPFile.Open(_T("mandrill256.bmp"),CFile::modeRead);

	BITMAPFILEHEADER bmfHeader;
	BMPFile.Read(&bmfHeader,sizeof(BITMAPFILEHEADER));

	BITMAPINFOHEADER bmiHeader;
	BMPFile.Read(&bmiHeader,sizeof(BITMAPINFOHEADER));

	BMPFile.Close();
	BMPFile.Open(_T("image.bmp"),CFile::modeCreate | CFile.modeWrite);
	BMPFile.Write(&bmfHeader,sizeof(BITMAPFILEHEADER));
	BMPFile.Write(&bmiHeader,sizeof(BITMAPINFOHEADER));
	BMPFile.Write(pData,sizeof(BYTE)*(bmfHeader.bfSize - bmfHeader.bfOffBits));
	BMPFile.Close();*/
}

UINT StartBP(LPVOID pParam)
{
	UINT LayerRanks[3]={64,16,64};
	Net NetBP(3,LayerRanks);

//	NetBP.SetSigmoidType(SIGMOID_TYPE_HYPERTAN);
	NetBP.SetSigmoidType(SIGMOID_TYPE_ORIGINAL);
	NetBP.SetSigmoidAlpha(1.0f);
	NetBP.SetScaleParam(0.5f);
	NetBP.SetLearnRate(0.1f);
	NetBP.SetMomentum(0.1f);
	NetBP.SetErrorLimit(0.1f);
	//NetBP.AddBiasNeuron();

	NetBP.SetFullConnect();
	NetBP.RandomizeWeights(-0.5f,1.0f);

	FLOAT Segments[32][32][64];
	ImageKit Image;
	Image.DecodeBMP(_T("mandrillgray.bmp"));
	Image.DeleteAlignment();

	for (UINT i=0; i<32; i++)
		for (UINT j=0; j<32; j++)
		{
			CPoint point(j*8,i*8);
			CSize size(8,8);
			CRect rect(point,size);
			Image.GetSegment(rect,Segments[i][j],-0.5f,1.0f);
		}

	CNeuroNetView *pView=(CNeuroNetView *)pParam;
	CMainFrame *pMainFrame = (CMainFrame *)pView->GetParent();
		
	LARGE_INTEGER liFreq;
	QueryPerformanceFrequency(&liFreq);
	LARGE_INTEGER liCount1;
	LARGE_INTEGER liCount2;
	FLOAT fNetError;
	UINT uiCycle=0;
	CString sError;
//	//ofstream file("debug.txt",ios::out);
//	QueryPerformanceCounter(&liCount1);
//	do
//	{
//		//QueryPerformanceCounter(&liCount1);
//		NetBP.SetAxons(0,Segments);
//		NetBP.Propagate(0);
//		
//		if ((fNetError=NetBP.GetNetError(Segments))>0.01)
//		{
//			
//			NetBP.LearnBackPropagation(Segments);
//			
//		}
//		QueryPerformanceCounter(&liCount2);
//	//	file<<fNetError<<endl;
//		
//		sError.Format(_T("Cycle: %d Net Error: %f Time: %f sec."),uiCycle,
//			fNetError,(float)(liCount2.QuadPart-liCount1.QuadPart)/liFreq.QuadPart);
//		pMainFrame->SetMessageText(sError);
//		uiCycle++;
//	}
//	while (fNetError>0.01);
////	file.close();
	QueryPerformanceCounter(&liCount1);
	fNetError=0.0;
	for (i=0 ;i<32; i++)
		for (UINT j=0; j<32; j++)
		{
			NetBP.SetAxons(0,Segments[i][j]);
			NetBP.Propagate(0);
			fNetError+=NetBP.GetNetError(Segments[i][j]);
		}
	do
	{
		//QueryPerformanceCounter(&liCount1);
		if (fNetError>0.01)
		{
			fNetError=NetBP.LearnBackPropagation(&Segments[0][0][0],1024);
		}
		
		QueryPerformanceCounter(&liCount2);
		sError.Format(_T("Cycle: %d Net Error: %f Time: %f sec."),uiCycle,
		fNetError,(float)(liCount2.QuadPart-liCount1.QuadPart)/liFreq.QuadPart);
		pMainFrame->SetMessageText(sError);
		uiCycle++;
	}
	while (fNetError>0.01);
	return FALSE;
}

UINT StartBP1(LPVOID pParam)
{
	UINT LayerRanks[3]={256,64,256};
	Net NetBP(3,LayerRanks);

//	NetBP.SetSigmoidType(SIGMOID_TYPE_HYPERTAN);
	NetBP.SetSigmoidType(SIGMOID_TYPE_ORIGINAL);
	NetBP.SetSigmoidAlpha(1.0f);
	NetBP.SetScaleParam(0.5f);
	NetBP.SetLearnRate(0.1f);
	NetBP.SetMomentum(0.1f);
	NetBP.SetErrorLimit(0.1f);
	//NetBP.AddBiasNeuron();

	NetBP.SetFullConnect();
	NetBP.RandomizeWeights(-0.5f,1.0f);

/*	FLOAT Segments[16][16][256];

#define DEBUG_LEARN

	ImageKit Image;
	Image.DecodeBMP(_T("rmat256.bmp"));
	Image.DeleteAlignment();
	for (UINT i=0; i<16; i++)
		for (UINT j=0; j<16; j++)
		{	
			CPoint point(j*16,i*16);
			CSize size(16,16);
			CRect rect(point,size);
			Image.GetSegment(rect,Segments[i][j],-0.5f,1.0f);
		}*/
	FLOAT Segments[32][32][64];

#define DEBUG_LEARN

	ImageKit Image;
	Image.DecodeBMP(_T("rmat256.bmp"));
	Image.DeleteAlignment();
	for (UINT i=0; i<32; i++)
		for (UINT j=0; j<32; j++)
		{	
			CPoint point(j*8,i*8);
			CSize size(8,8);
			CRect rect(point,size);
			Image.GetSegment(rect,Segments[i][j],-0.5f,1.0f);
		}

	CFile f;
	f.Open("temp.dat",CFile::modeWrite|CFile::modeCreate);
	f.Write(Segments,sizeof(FLOAT)*32*32*64);
	f.Close();

#ifdef DEBUG_LEARN
//	UINT uiCycle=0;
	CString sError;
//	FLOAT fError;

	CNeuroNetView *pView=(CNeuroNetView *)pParam;
	/*CNeuroNetDoc* pDoc =pView->GetDocument();
	CMainFrame *pMainFrame = (CMainFrame *)pDoc->GetParentFrame();*/
	CMainFrame *pMainFrame = (CMainFrame *)pView->GetParent();
		
	BOOL flag=FALSE;
	LARGE_INTEGER liFreq;
	QueryPerformanceFrequency(&liFreq);
	LARGE_INTEGER liCount1;
	LARGE_INTEGER liCount2;
	QueryPerformanceCounter(&liCount1);
	//FLOAT fSec;
	FLOAT fNetError;
	int c=0;
	while (!flag)
	{
		c++;
		flag=TRUE;
	
		/*if ((fNetError=NetBP.LearnBackPropagation(&Segments[0][0][0],256))<300.0)
			flag=TRUE;*/
		NetBP.SetAxons(0,Segments[0][0]);
		NetBP.Propagate(0);
		if ((fNetError=NetBP.GetNetError(Segments[0][0]))>0.01)
		{
			NetBP.LearnBackPropagation(Segments[0][0]);
			flag=FALSE;
		}

		
		QueryPerformanceCounter(&liCount2);
		sError.Format(_T("Net Error: %f Time: %f sec."),
			fNetError,(float)(liCount2.QuadPart-liCount1.QuadPart)/liFreq.QuadPart);
		pMainFrame->SetMessageText(sError);

	}
	
/*	QueryPerformanceCounter(&liCount2);
	sError.Format(_T("Av. Error: %f Error: %f  Learn Cycle: %d Time: %f sec."),
				fAvError,fError,uiCycle,
				(float)(liCount2.QuadPart-liCount1.QuadPart)/liFreq.QuadPart);
				pMainFrame->SetMessageText(sError);*/
	NetBP.SaveWeightsToFile(_T("weights.dat"));
#endif DEBUG_LEARN

//#ifndef DEBUG_LEARN
	FLOAT Outputs[16][16][256];
	BYTE ImageData[256*256];
	NetBP.LoadWeightsFromFile(_T("weights.dat"));

	for (i=0; i<16; i++)
		for (UINT j=0; j<16; j++)
		{
			NetBP.SetAxons(0,Segments[i][j]);
			NetBP.Propagate();
			NetBP.GetAxons(2,Outputs[i][j]);
			for (UINT k=0; k<16; k++)
				for (UINT m=0; m<16; m++)
					ImageData[(k+16*i)*256+(m+j*16)]=
						floor((Outputs[i][j][k*16+m]+0.5f)/1.0f*255.0+0.5f);
		}

	CFile BMPFile;
	BMPFile.Open(_T("rmat256.bmp"),CFile::modeRead);

	BITMAPFILEHEADER bmfHeader;
	BMPFile.Read(&bmfHeader,sizeof(BITMAPFILEHEADER));

	BITMAPINFOHEADER bmiHeader;
	BMPFile.Read(&bmiHeader,sizeof(BITMAPINFOHEADER));

	BITMAPINFO *bmInfo=(BITMAPINFO *)(new BYTE[sizeof(BITMAPINFOHEADER) + 
		((1<<bmiHeader.biBitCount) * sizeof(RGBQUAD))]);
	ASSERT(bmInfo!=NULL);

	ASSERT(memcpy(&bmInfo->bmiHeader,&bmiHeader,sizeof(BITMAPINFOHEADER))!=NULL);

	BMPFile.Read(bmInfo->bmiColors,((1<<bmiHeader.biBitCount) * sizeof(RGBQUAD)));

	bmInfo->bmiHeader.biWidth=256;
	bmInfo->bmiHeader.biHeight=256;
	bmInfo->bmiHeader.biSize=bmInfo->bmiHeader.biHeight*bmInfo->bmiHeader.biWidth;
	bmfHeader.bfSize=bmfHeader.bfOffBits+bmInfo->bmiHeader.biSize;
	BMPFile.Close();
	BMPFile.Open(_T("image.bmp"),CFile::modeCreate | CFile.modeWrite);
	BMPFile.Write(&bmfHeader,sizeof(BITMAPFILEHEADER));
	BMPFile.Write(&bmiHeader,sizeof(BITMAPINFOHEADER));
	BMPFile.Write(bmInfo->bmiColors,((1<<bmiHeader.biBitCount) * sizeof(RGBQUAD)));
	BMPFile.Write(ImageData,sizeof(BYTE)*(bmfHeader.bfSize - bmfHeader.bfOffBits));
	BMPFile.Close();
//#endif DEBUG_LEARN
	return 0;
}
void CNeuroNetView::OnStartNetbp() 
{
	AfxBeginThread(StartBP,this);
	// TODO: Add your command handler code here
}
