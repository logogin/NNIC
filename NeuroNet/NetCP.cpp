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

NetCP::NetCP(const UINT uiRanks[NETRANK])
{
	CopyMemory(m_uiLayerRank,uiRanks,sizeof(UINT)*NETRANK);
	for (UINT i=0; i<NETRANK; i++)
	{
		m_vAxons[i]=new FLOAT[uiRanks[i]];
		ASSERT(m_vAxons[i]!=NULL);
	}

	for (i=0; i<NETRANK-1; i++)
	{
		m_uiNeuronRank[i]=m_uiLayerRank[i];
		m_vWeights[i]=new FLOAT *[m_uiLayerRank[i+1]];
		ASSERT(m_vWeights[i]!=NULL);
		for (UINT j=0; j<m_uiLayerRank[i+1]; j++)
		{
			m_vWeights[i][j]=new FLOAT[m_uiNeuronRank[i]];
			ASSERT(m_vWeights[i][j]!=NULL);
		}
	}
	m_uiCounts=new UINT[m_uiLayerRank[KOHONEN_LAYER]];
	ASSERT(m_uiCounts!=NULL);
	ZeroMemory(m_uiCounts,sizeof(UINT)*m_uiLayerRank[KOHONEN_LAYER]);
}

NetCP::~NetCP()
{
	for (UINT i=0; i<NETRANK; i++)
	//	if (m_vAxons[i]!=NULL)
		delete []m_vAxons[i];
	for (i=0; i<NETRANK-1; i++)
	//	if (m_vWeights[i]!=NULL)
	{
		for(UINT j=0; j<m_uiLayerRank[i+1]; j++) 
			delete []m_vWeights[i][j];
		delete []m_vWeights[i];
	}
	delete []m_uiCounts;
}

//DEL BOOL NetCP::MultMatrixVector(const FLOAT *vMatrix, 
//DEL 							 const CSize &MatrixSize, 
//DEL 							 const FLOAT *vVector, 
//DEL 							 const UINT uiVectorSize, 
//DEL 							 FLOAT *vResult,
//DEL 							 UINT *uiResultSize)
//DEL {
//DEL 	if (MatrixSize.cx!=uiVectorSize)
//DEL 		return FALSE;
//DEL 
//DEL 	*uiResultSize=MatrixSize.cy;
//DEL 	ASSERT(uiResultSize!=NULL);
//DEL 
//DEL 	vResult=new FLOAT[*uiResultSize];
//DEL 	ASSERT(vResult!=NULL);
//DEL 
//DEL 	for (UINT i=0; i<MatrixSize.cy; i++)
//DEL 	{
//DEL 		FLOAT fSum=0.0f;
//DEL 		for (UINT j=0; j<MatrixSize.cx; j++)
//DEL 			fSum+=vMatrix[i*MatrixSize.cx+j]*vVector[j];
//DEL 		vResult[i]=fSum;
//DEL 	}
//DEL 	return TRUE;
//DEL }

void NetCP::InitWeights(const FLOAT fMinValue, const FLOAT fRange)
{
	FLOAT *vWeights=new FLOAT[m_uiNeuronRank[KOHONEN_NEURON]];
	ASSERT(vWeights!=NULL);
	UINT n=1;

//	srand((unsigned int)time(NULL));

/*	for (UINT i=0; i<m_uiLayerRank[KOHONEN_LAYER]; i++)
	{
		for	(UINT j=0; j<m_uiNeuronRank[KOHONEN_NEURON]; j++)
		{
			n++;
			n%=100;
			m_vWeights[KOHONEN_NEURON][i][j]=2*(float(n/100.00))-1;
				//fMinValue+(FLOAT)rand()/RAND_MAX*fRange;
		}
		NormalizeVector(m_vWeights[KOHONEN_NEURON][i],m_uiNeuronRank[KOHONEN_NEURON]);
	}*/
	for (UINT i=0; i<m_uiNeuronRank[KOHONEN_NEURON]; i++)
	{
		for	(UINT j=0; j<m_uiLayerRank[KOHONEN_LAYER]; j++)
		{
		/*	n++;
			n%=100;*/
			m_vWeights[KOHONEN_NEURON][j][i]=fMinValue+(FLOAT)rand()/RAND_MAX*fRange;
				//2*(float(n/100.00))-1;
				//fMinValue+(FLOAT)rand()/RAND_MAX*fRange;
		}
		
	}
	for	(i=0; i<m_uiLayerRank[KOHONEN_LAYER]; i++)
		NormalizeVector(m_vWeights[KOHONEN_NEURON][i],m_uiNeuronRank[KOHONEN_NEURON]);

	for (i=0; i<m_uiLayerRank[GROSSBERG_LAYER]; i++)
		for (UINT j=0; j<m_uiNeuronRank[GROSSBERG_NEURON]; j++)
			//m_vWeights[GROSSBERG_NEURON][i][j]=/*-127.0f+*/(FLOAT)rand()/RAND_MAX*255.0f;
			m_vWeights[GROSSBERG_NEURON][i][j]=0.0f;
}

void NetCP::NormalizeVector(FLOAT *vVector,const UINT uiVectorSize)
{
	FLOAT fNorm=0.0f;
	for (UINT i=0; i<uiVectorSize; i++)
		fNorm+=vVector[i]*vVector[i];

	fNorm=(FLOAT)sqrt(fNorm);
	for (i=0; i<uiVectorSize; i++)
		vVector[i]/=fNorm;
}

/*void NetCP::NormalizeVector(const FLOAT *vVector, const UINT uiVectorSize, FLOAT *vTarget)
{
	FLOAT fNorm=0.0f;
	for (UINT i=0; i<uiVectorSize; i++)
		fNorm+=vVector[i]*vVector[i];

	fNorm=(FLOAT)sqrt(fNorm);
	for (i=0; i<uiVectorSize; i++)
		vTarget[i]=vVector[i]/fNorm;
}*/

void NetCP::SetAxons(const UINT uiLayer, const FLOAT *vAxons, const BOOL bNormalize)
{
	ASSERT(uiLayer<NETRANK);
	CopyMemory(m_vAxons[INPUT_LAYER],vAxons,sizeof(FLOAT)*m_uiLayerRank[INPUT_LAYER]);
	if (bNormalize)
		NormalizeVector(m_vAxons[INPUT_LAYER],m_uiLayerRank[INPUT_LAYER]);
}

void NetCP::PropagateKohonen()
{
	BOOL bFirst=TRUE;
	FLOAT fMax;

//	FLOAT fTime=0.0;
//	LARGE_INTEGER liFreq;
//	LARGE_INTEGER liCount1;
//	LARGE_INTEGER liCount2;
//	QueryPerformanceFrequency(&liFreq);

	for (UINT i=0; i<m_uiLayerRank[KOHONEN_LAYER]; i++)
	{
//		QueryPerformanceCounter(&liCount1);
		FLOAT fSum=0.0f;
		for (UINT j=0; j<m_uiNeuronRank[KOHONEN_NEURON]; j++)
			fSum+=m_vWeights[KOHONEN_NEURON][i][j]*m_vAxons[INPUT_LAYER][j];
		
		if (bFirst)
		{
			fMax=fSum;
			m_uiWinnerNeuron=i;
			bFirst=FALSE;
		}
		else
			if (fMax<fSum)
			{
				fMax=fSum;
				m_uiWinnerNeuron=i;
			}
//		QueryPerformanceCounter(&liCount2);
//		fTime+=(FLOAT)(liCount2.QuadPart-liCount1.QuadPart)/liFreq.QuadPart;
	}
}

void NetCP::PropagateGrossberg()
{
/*	UINT uiOffset=m_uiQuadrant*m_uiQuadrantRank+m_uiCluster;
	for (UINT i=0; i<m_uiLayerRank[GROSSBERG_LAYER]; i++)
		m_vAxons[GROSSBERG_LAYER][i]=
			m_vWeights[GROSSBERG_NEURON][i][uiOffset]*m_vAxons[KOHONEN_LAYER][uiOffset];*/
}

void NetCP::LearnGrossberg(const FLOAT fLearnRate,const FLOAT *fTarget)
{
	INT iFirstNeuron=m_uiWinnerNeuron-m_uiNeighborhoodSize;
	if (iFirstNeuron<0)
		iFirstNeuron=0;
	INT iLastNeuron=m_uiWinnerNeuron+m_uiNeighborhoodSize;
	if (iLastNeuron>=m_uiLayerRank[KOHONEN_LAYER])
		iLastNeuron=m_uiLayerRank[KOHONEN_LAYER]-1;

	for (UINT i=0; i<m_uiLayerRank[GROSSBERG_LAYER]; i++)
		for (UINT j=iFirstNeuron; j<=iLastNeuron; j++)
			m_vWeights[GROSSBERG_NEURON][i][j]+=fLearnRate*(fTarget[i]-
				m_vWeights[GROSSBERG_NEURON][i][j]);
}

void NetCP::LearnGrossberg(const BYTE *bTarget)
{
	m_uiCounts[m_uiWinnerNeuron]++;
	for (UINT i=0; i<m_uiLayerRank[GROSSBERG_LAYER]; i++)
		m_vWeights[GROSSBERG_NEURON][i][m_uiWinnerNeuron]+=bTarget[i];
}

void NetCP::FinilizeGrossberg()
{
	for (UINT i=0; i<m_uiLayerRank[KOHONEN_LAYER]; i++)
		for (UINT j=0; j<m_uiLayerRank[GROSSBERG_LAYER]; j++)
			m_vWeights[GROSSBERG_NEURON][j][i]/=m_uiCounts[i];
}

void NetCP::LearnKohonen(const FLOAT fLearnRate)
{
	INT iFirstNeuron=m_uiWinnerNeuron-m_uiNeighborhoodSize;
	if (iFirstNeuron<0)
		iFirstNeuron=0;
	INT iLastNeuron=m_uiWinnerNeuron+m_uiNeighborhoodSize;
	if (iLastNeuron>=m_uiLayerRank[KOHONEN_LAYER])
		iLastNeuron=m_uiLayerRank[KOHONEN_LAYER]-1;

/*	for (UINT i=iFirstNeuron; i<iLastNeuron; i++)
		for (UINT j=0; j<m_uiNeuronRank[KOHONEN_NEURON]; j++)
			m_vWeights[KOHONEN_NEURON][i][j]+=fLearnRate*(m_vAxons[INPUT_LAYER][j]-
				m_vWeights[KOHONEN_NEURON][i][j]);*/
	for (UINT j=0; j<m_uiNeuronRank[KOHONEN_NEURON]; j++)
		for (UINT i=iFirstNeuron; i<=iLastNeuron; i++)
			m_vWeights[KOHONEN_NEURON][i][j]+=fLearnRate*(m_vAxons[INPUT_LAYER][j]-
				m_vWeights[KOHONEN_NEURON][i][j]);

/*	ofstream out("weights.txt",ios::out);
	for (j=0; j<m_uiNeuronRank[KOHONEN_NEURON]; j++)
	{
		for (UINT i=0; i<m_uiLayerRank[KOHONEN_LAYER]; i++)
			out<<m_vWeights[KOHONEN_NEURON][i][j]<<" "<<flush;
		out<<endl;
	}
	out.close();*/
}

UINT NetCP::GetWinnerNeuron()
{
	return m_uiWinnerNeuron;
}

FLOAT NetCP::GetWeight(const UINT uiLayer, const UINT uiNeuron, const UINT uiWeight)
{
	ASSERT(uiLayer<NETRANK&&uiLayer);
	ASSERT(uiNeuron<m_uiLayerRank[uiLayer]);
	ASSERT(uiWeight<m_uiNeuronRank[uiLayer-1]);
	return m_vWeights[uiLayer-1][uiNeuron][uiWeight];
}

void NetCP::SetNeighborhoodSize(const UINT uiNeighborhoodSize)
{
	m_uiNeighborhoodSize=uiNeighborhoodSize;
}

FLOAT NetCP::GetWinnerDistance()
{
	FLOAT fDistance;
	FLOAT fSum=0.0f;
	for (UINT i=0; i<m_uiNeuronRank[KOHONEN_NEURON]; i++)
	{
		fDistance=m_vAxons[INPUT_LAYER][i]-m_vWeights[KOHONEN_NEURON][m_uiWinnerNeuron][i];
		fSum+=fDistance*fDistance;
	}

	return (FLOAT)sqrt(fSum);
}

void NetCP::SetWinnerNeuron(const UINT uiWinner)
{
	ASSERT(uiWinner<m_uiLayerRank[KOHONEN_LAYER]);
	m_uiWinnerNeuron=uiWinner;
}

FLOAT NetCP::GetTargetDistance(const FLOAT *fTarget)
{
	FLOAT fDistance;
	FLOAT fSum=0.0f;
	for (UINT i=0; i<m_uiLayerRank[GROSSBERG_LAYER]; i++)
	{
		fDistance=fTarget[i]-m_vWeights[GROSSBERG_NEURON][i][m_uiWinnerNeuron];
		fSum+=fDistance*fDistance;
	}

	return (FLOAT)sqrt(fSum);
}
