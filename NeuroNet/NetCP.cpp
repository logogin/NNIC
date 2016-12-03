// NetCP.cpp: implementation of the NetCP class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NetCP.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NetCP::NetCP(const WORD *pRanks,const BYTE bNetType):m_bNetType(bNetType)
{
	DWORD i;
	switch (m_bNetType)
	{
	case NET_TYPE_KOHONEN_GROSSBERG:
		m_bNetRank=3;
		break;

	case NET_TYPE_KOHONEN:
		m_bNetRank=2;
		break;
	}

//	m_vLayerRank=new WORD[m_bNetRank];
//	ASSERT(m_vLayerRank!=NULL);
	CopyMemory(m_vLayerRank,pRanks,sizeof(WORD)*m_bNetRank);

//	DWORD *dwLayerRank=new DWORD[m_bNetRank];
//	ASSERT(dwLayerRank!=NULL);
	for (i=0; i<m_bNetRank; i++)
	{
//		dwLayerRank[i]=m_vLayerRank[i];
		m_vAxons[i]=new FLOAT[m_vLayerRank[i]];
		ASSERT(m_vAxons[i]!=NULL);
	}

//	ASSERT((m_vAxons=AllocateFloatMatrix(m_bNetRank,dwLayerRank))!=NULL);
//	delete []dwLayerRank;
//
//	m_vNeuronRank=new WORD[m_bNetRank-1];
//	ASSERT(m_vNeuronRank!=NULL);
//
//	m_vWeights=new FLOAT **[m_bNetRank-1];
//	ASSERT(m_vWeights!=NULL);

	for (i=0; i<m_bNetRank-1; i++)
	{
		m_vNeuronRank[i]=m_vLayerRank[i];
		m_vWeights[i]=AllocateFloatMatrix(m_vLayerRank[i+1],m_vNeuronRank[i]);
		ASSERT(m_vWeights[i]!=NULL);
	}
	
	m_vCounts=new DWORD[m_vLayerRank[KOHONEN_LAYER]];
	ASSERT(m_vCounts!=NULL);
	ZeroMemory(m_vCounts,sizeof(DWORD)*m_vLayerRank[KOHONEN_LAYER]);
}

NetCP::~NetCP()
{
	DWORD i;
	for (i=0; i<m_bNetRank; i++)
		if (m_vAxons[i]!=NULL)
			delete m_vAxons[i];
//	if (m_vAxons!=NULL)
//		DestroyMatrix((LPVOID *)m_vAxons);

	for (i=0; i<m_bNetRank-1; i++)
		if (m_vWeights[i]!=NULL)
			DestroyMatrix((LPVOID *)m_vWeights[i]);

//	if (m_vWeights!=NULL)
//		delete []m_vWeights;

//	if (m_vLayerRank!=NULL)
//		delete []m_vLayerRank;

	if (m_vCounts!=NULL)
		delete []m_vCounts;
}

void NetCP::InitWeights(const FLOAT fMinValue, const FLOAT fRange)
{
	DWORD i,j;

	srand((unsigned)time(NULL));

	for (i=0; i<m_vNeuronRank[KOHONEN_NEURON]; i++)
		for	(j=0; j<m_vLayerRank[KOHONEN_LAYER]; j++)
			m_vWeights[KOHONEN_NEURON][j][i]=fMinValue+(FLOAT)rand()/RAND_MAX*fRange;

	for	(i=0; i<m_vLayerRank[KOHONEN_LAYER]; i++)
		NormalizeVector(m_vWeights[KOHONEN_NEURON][i],m_vNeuronRank[KOHONEN_NEURON]);

	for (i=0; i<m_vLayerRank[GROSSBERG_LAYER]; i++)
		for (j=0; j<m_vNeuronRank[GROSSBERG_NEURON]; j++)
			m_vWeights[GROSSBERG_NEURON][i][j]=0.0f;
}

void NetCP::InitWeights(void)
{
	UINT i,j;
	FLOAT fValue;
	for (i=0; i<m_vLayerRank[KOHONEN_LAYER]; i++)
	{
		fValue=(FLOAT)(i%MAX_COLOR_VALUE);
		for (j=0; j<m_vNeuronRank[KOHONEN_NEURON]; j++)
			m_vWeights[KOHONEN_NEURON][i][j]=fValue;
	}

}

void NetCP::NormalizeVector(FLOAT *vVector,const DWORD dwVectorSize)
{
	DWORD i;
	FLOAT fNorm=0.0f;
	for (i=0; i<dwVectorSize; i++)
		fNorm+=vVector[i]*vVector[i];

	fNorm=(FLOAT)sqrt(fNorm);
	for (i=0; i<dwVectorSize; i++)
		vVector[i]/=fNorm;
}

void NetCP::SetAxons(const BYTE bLayer, const FLOAT *vAxons, const BOOLEAN bNormalize)
{
	ASSERT(bLayer<m_bNetRank);
	CopyMemory(m_vAxons[bLayer],vAxons,sizeof(FLOAT)*m_vLayerRank[bLayer]);
	if (bNormalize)
		NormalizeVector(m_vAxons[bLayer],m_vLayerRank[bLayer]);
}

void NetCP::SetAxons(const BYTE bLayer, const BYTE *vAxons, const BOOLEAN bNormalize)
{
	ASSERT(bLayer<m_bNetRank);
	DWORD i;
	for (i=0; i<m_vLayerRank[bLayer]; i++)
		m_vAxons[bLayer][i]=vAxons[i];
	if (bNormalize)
		NormalizeVector(m_vAxons[bLayer],m_vLayerRank[bLayer]);
}

void NetCP::PropagateKohonenMax()
{
	WORD i,j;
	BOOLEAN bFirst=TRUE;
	FLOAT fMax;
	FLOAT fSum;

//	FLOAT fTime=0.0;
//	LARGE_INTEGER liFreq;
//	LARGE_INTEGER liCount1;
//	LARGE_INTEGER liCount2;
//	QueryPerformanceFrequency(&liFreq);

	for (i=0; i<m_vLayerRank[KOHONEN_LAYER]; i++)
	{
//		QueryPerformanceCounter(&liCount1);
		fSum=0.0f;
		for (j=0; j<m_vNeuronRank[KOHONEN_NEURON]; j++)
			fSum+=m_vWeights[KOHONEN_NEURON][i][j]*m_vAxons[INPUT_LAYER][j];
		
		if (bFirst)
		{
			fMax=fSum;
			m_wWinnerNeuron=i;
			bFirst=FALSE;
		}
		else
			if (fMax<fSum)
			{
				fMax=fSum;
				m_wWinnerNeuron=i;
			}
//		QueryPerformanceCounter(&liCount2);
//		fTime+=(FLOAT)(liCount2.QuadPart-liCount1.QuadPart)/liFreq.QuadPart;
	}
}

void NetCP::PropagateKohonenMin()
{
	WORD i,j;
	BOOLEAN bFirst=TRUE;
	FLOAT fMin;
	FLOAT fSum;

	for (i=0; i<m_vLayerRank[KOHONEN_LAYER]; i++)
	{
		fSum=0.0f;
		for (j=0; j<m_vNeuronRank[KOHONEN_NEURON]; j++)
			if (m_vAxons[INPUT_LAYER][j]>=m_vWeights[KOHONEN_NEURON][i][j])
				fSum+=m_vAxons[INPUT_LAYER][j]-m_vWeights[KOHONEN_NEURON][i][j];
			else
				fSum+=m_vWeights[KOHONEN_NEURON][i][j]-m_vAxons[INPUT_LAYER][j];
		if (bFirst)
		{
			fMin=fSum;
			m_wWinnerNeuron=i;
			bFirst=FALSE;
		}
		else
			if (fMin>fSum) 
			{
				fMin=fSum;
				m_wWinnerNeuron=i;
			}
	}
}
//void NetCP::PropagateGrossberg()
//{
///*	UINT uiOffset=m_uiQuadrant*m_uiQuadrantRank+m_uiCluster;
//	for (UINT i=0; i<m_uiLayerRank[GROSSBERG_LAYER]; i++)
//		m_vAxons[GROSSBERG_LAYER][i]=
//			m_vWeights[GROSSBERG_NEURON][i][uiOffset]*m_vAxons[KOHONEN_LAYER][uiOffset];*/
//}

//void NetCP::LearnGrossberg(const FLOAT fLearnRate,const FLOAT *fTarget)
//{
//	INT iFirstNeuron=m_uiWinnerNeuron-m_uiNeighborhoodSize;
//	if (iFirstNeuron<0)
//		iFirstNeuron=0;
//	INT iLastNeuron=m_uiWinnerNeuron+m_uiNeighborhoodSize;
//	if (iLastNeuron>=m_uiLayerRank[KOHONEN_LAYER])
//		iLastNeuron=m_uiLayerRank[KOHONEN_LAYER]-1;
//
//	for (UINT i=0; i<m_uiLayerRank[GROSSBERG_LAYER]; i++)
//		for (UINT j=iFirstNeuron; j<=iLastNeuron; j++)
//			m_vWeights[GROSSBERG_NEURON][i][j]+=fLearnRate*(fTarget[i]-
//				m_vWeights[GROSSBERG_NEURON][i][j]);
//}

void NetCP::LearnGrossberg(const BYTE *bTarget)
{
	DWORD i;
	m_vCounts[m_wWinnerNeuron]++;
	for (i=0; i<m_vLayerRank[GROSSBERG_LAYER]; i++)
		m_vWeights[GROSSBERG_NEURON][i][m_wWinnerNeuron]+=bTarget[i];
}

void NetCP::FinalizeGrossberg()
{
	DWORD i,j;
	for (i=0; i<m_vLayerRank[KOHONEN_LAYER]; i++)
		for (j=0; j<m_vLayerRank[GROSSBERG_LAYER]; j++)
			m_vWeights[GROSSBERG_NEURON][j][i]/=m_vCounts[i];
}

//void NetCP::LearnKohonen(const FLOAT fLearnRate)
//{
//	DWORD i,j;
//	INT iFirstNeuron=m_wWinnerNeuron-m_wNeighRadius;
//	if (iFirstNeuron<0)
//		iFirstNeuron=0;
//	INT iLastNeuron=m_wWinnerNeuron+m_wNeighRadius;
//	if (iLastNeuron>=m_vLayerRank[KOHONEN_LAYER])
//		iLastNeuron=m_vLayerRank[KOHONEN_LAYER]-1;
//
//	for (j=0; j<m_vNeuronRank[KOHONEN_NEURON]; j++)
//		for (i=iFirstNeuron; i<=iLastNeuron; i++)
//			m_vWeights[KOHONEN_NEURON][i][j]+=fLearnRate*(m_vAxons[INPUT_LAYER][j]-
//				m_vWeights[KOHONEN_NEURON][i][j]);
//}

void NetCP::LearnKohonen(const FLOAT *pLearnRate)
{
	DWORD i,j;
	INT iFirstNeuron=m_wWinnerNeuron-m_wNeighRadius;
	if (iFirstNeuron<0)
		iFirstNeuron=0;
	INT iLastNeuron=m_wWinnerNeuron+m_wNeighRadius;
	if (iLastNeuron>=m_vLayerRank[KOHONEN_LAYER])
		iLastNeuron=m_vLayerRank[KOHONEN_LAYER]-1;

	
	for (i=iFirstNeuron; i<=iLastNeuron; i++)
		for (j=0; j<m_vNeuronRank[KOHONEN_NEURON]; j++)
			m_vWeights[KOHONEN_NEURON][i][j]+=
				pLearnRate[labs(m_wWinnerNeuron-i)]*(m_vAxons[INPUT_LAYER][j]
					-m_vWeights[KOHONEN_NEURON][i][j]);
//	DWORD i,j;
//	INT iFirstNeuron=m_wWinnerNeuron-m_wNeighRadius;
//	if (iFirstNeuron<0)
//		iFirstNeuron=0;
//	INT iLastNeuron=m_wWinnerNeuron+m_wNeighRadius;
//	if (iLastNeuron>=m_vLayerRank[KOHONEN_LAYER])
//		iLastNeuron=m_vLayerRank[KOHONEN_LAYER]-1;
//
//	for (i=iFirstNeuron; i<=iLastNeuron; i++)
//		for (j=0; j<m_vNeuronRank[KOHONEN_NEURON]; j++)
//			m_vWeights[KOHONEN_NEURON][i][j]+=m_fLearnRate*(m_vAxons[INPUT_LAYER][j]-
//				m_vWeights[KOHONEN_NEURON][i][j]);
}

UINT NetCP::GetWinnerNeuron()
{
	return m_wWinnerNeuron;
}

FLOAT NetCP::GetWeight(const BYTE bLayer, const WORD wNeuron, const WORD wWeight)
{
	ASSERT(bLayer<m_bNetRank&&bLayer);
	ASSERT(wNeuron<m_vLayerRank[bLayer]);
	ASSERT(wWeight<m_vNeuronRank[bLayer-1]);
	return m_vWeights[bLayer-1][wNeuron][wWeight];
}

void NetCP::SetNeighbouringRadius(const WORD wNeighRadius)
{
	m_wNeighRadius=wNeighRadius;
}

FLOAT NetCP::GetWinnerDistance()
{
	DWORD i;
	FLOAT fDistance;
	FLOAT fSum=0.0f;
	for (i=0; i<m_vNeuronRank[KOHONEN_NEURON]; i++)
	{
		fDistance=m_vAxons[INPUT_LAYER][i]-m_vWeights[KOHONEN_NEURON][m_wWinnerNeuron][i];
		fSum+=fDistance*fDistance;
	}

	return (FLOAT)sqrt(fSum);
}

void NetCP::SetWinnerNeuron(const WORD wWinner)
{
	ASSERT(wWinner<m_vLayerRank[KOHONEN_LAYER]);
	m_wWinnerNeuron=wWinner;
}

//FLOAT NetCP::GetTargetDistance(const FLOAT *fTarget)
//{
//	FLOAT fDistance;
//	FLOAT fSum=0.0f;
//	for (UINT i=0; i<m_uiLayerRank[GROSSBERG_LAYER]; i++)
//	{
//		fDistance=fTarget[i]-m_vWeights[GROSSBERG_NEURON][i][m_uiWinnerNeuron];
//		fSum+=fDistance*fDistance;
//	}
//
//	return (FLOAT)sqrt(fSum);
//}

void NetCP::SetLearnRate(const FLOAT fLearnRate)
{
	m_fLearnRate=fLearnRate;
}

void NetCP::CalculateLearnRate(FLOAT *pLearnRate)
{
	DWORD i;
	if (m_wNeighRadius)
		for (i=0; i<=m_wNeighRadius; i++)
			pLearnRate[i]=m_fLearnRate*(1.0f-(FLOAT)i*i/(m_wNeighRadius*m_wNeighRadius));
	else
		for (i=0; i<=m_wNeighRadius; i++)
			pLearnRate[i]=m_fLearnRate;
}
