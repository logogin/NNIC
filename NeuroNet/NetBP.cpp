// NetBP.cpp: implementation of the NetBP class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NetBP.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NetBP::NetBP(const BYTE bNetRank,const PWORD wLayerRank):m_bNetRank(bNetRank),
	m_bUseBias(FALSE),m_wLayerRank(NULL),m_wNeuronRank(NULL),m_vAxons(NULL),
	m_bFirstEpoch(TRUE)
	, m_dwBiasMatrixSize(0)
{
	m_wLayerRank=(PWORD)HeapAlloc(m_bNetRank*sizeof(WORD));
	ASSERT(m_wLayerRank!=NULL);

	DWORD i,j;
	for (i=0; i<m_bNetRank; i++)
		m_wLayerRank[i]=wLayerRank[i];

	m_wNeuronRank=(PWORD)HeapAlloc((m_bNetRank-1)*sizeof(WORD));
	ASSERT(m_wNeuronRank!=NULL);

	for (i=0; i<m_bNetRank-1; i++)
		m_wNeuronRank[i]=m_wLayerRank[i];

	HANDLE hHeap;
	for (i=0; i<BP_ARRAY_RANK; i++)
	{
		m_vWeights[i]=(PFLOAT **)HeapAlloc((m_bNetRank-1)*sizeof(PFLOAT *));
		ASSERT(m_vWeights[i]!=NULL);
		hHeap=GetHeapHandle(m_vWeights[i]);
		for (j=0; j<m_bNetRank-1; j++)
		{
			m_vWeights[i][j]=AllocateFloatMatrix(hHeap,m_wLayerRank[j+1],m_wNeuronRank[j],TRUE);
			ASSERT(m_vWeights[i][j]!=NULL);
		}
	}

	PDWORD dwLayerRank=(PDWORD)HeapAlloc(m_bNetRank*sizeof(DWORD));
	ASSERT(dwLayerRank!=NULL);
	for (i=0; i<m_bNetRank; i++)
		dwLayerRank[i]=m_wLayerRank[i];
	m_vAxons=AllocateFloatMatrix(m_bNetRank,dwLayerRank);
	ASSERT(m_vAxons!=NULL);
	
	m_vErrorSignal=AllocateFloatMatrix(m_bNetRank-1,dwLayerRank+1);
	ASSERT(m_vErrorSignal!=NULL);
	HeapFree(dwLayerRank);
}

NetBP::~NetBP()
{
	if (m_wLayerRank!=NULL)
		HeapFree(m_wLayerRank);
	if (m_wNeuronRank!=NULL)
		HeapFree(m_wNeuronRank);
	if (m_vAxons!=NULL)
		DestroyMatrix((LPVOID *)m_vAxons);
	if (m_vErrorSignal!=NULL)
		DestroyMatrix((LPVOID *)m_vErrorSignal);

	DWORD i;
	for (i=0; i<BP_ARRAY_RANK; i++)
	{
		if (m_vWeights[i]!=NULL)
			HeapFree(m_vWeights[i]);
		if (m_bUseBias&&m_vBiases[i]!=NULL)
			DestroyMatrix((LPVOID *)(m_vBiases[i]));
	}
}

void NetBP::SetSignalBoundaries(const FLOAT fMinSignal, const FLOAT fMaxSignal)
{
	m_fMinSignal=fMinSignal;
	m_fMaxSignal=fMaxSignal;
}

void NetBP::UseBiases(const BOOLEAN bFlag)
{
	m_bUseBias=bFlag;

	DWORD i;
	if (bFlag)
	{
		PDWORD dwLayerRank=(PDWORD)HeapAlloc((m_bNetRank-1)*sizeof(DWORD));
		m_dwBiasMatrixSize=0;
		for (i=0; i<m_bNetRank-1; i++)
		{
			dwLayerRank[i]=m_wLayerRank[i+1];
			m_dwBiasMatrixSize+=dwLayerRank[i];
		}
		for (i=0; i<BP_ARRAY_RANK; i++)
		{
			m_vBiases[i]=AllocateFloatMatrix(m_bNetRank-1,dwLayerRank,TRUE);
			ASSERT(m_vBiases[i]!=NULL);
		}
		HeapFree(dwLayerRank);
	}
}

void NetBP::InitWeights()
{
	FLOAT fRange=m_fMaxSignal-m_fMinSignal;
	DWORD i,j,k;
	srand((unsigned)time(NULL));
	for (i=0; i<m_bNetRank-1; i++)
		for (j=0; j<m_wLayerRank[i+1]; j++)
			for (k=0; k<m_wNeuronRank[i]; k++)
				m_vWeights[WEIGH][i][j][k]=
					m_fMinSignal+(FLOAT)rand()/RAND_MAX*fRange;
	if (m_bUseBias)
		for (i=0; i<m_bNetRank-1; i++)
			for (j=0; j<m_wLayerRank[i+1]; j++)
				m_vBiases[WEIGH][i][j]=m_fMinSignal+(FLOAT)rand()/RAND_MAX*fRange;
}

void NetBP::InitWeights(const BYTE bLayer,const LPVOID pWeights)
{
	ASSERT(bLayer>0&&bLayer<m_bNetRank);
	DWORD dwWeightsSize=m_wLayerRank[bLayer]*m_wNeuronRank[bLayer-1];
	CopyMemory(m_vWeights[WEIGH][bLayer-1][0],pWeights,dwWeightsSize*sizeof(FLOAT));
	
	if (m_bUseBias)
		CopyMemory(m_vBiases[WEIGH][bLayer-1],(PFLOAT)pWeights+dwWeightsSize,
			m_wLayerRank[bLayer]*sizeof(FLOAT));
}

void NetBP::SetAxons(const BYTE bLayer, const PFLOAT fAxons)
{
	ASSERT(bLayer<m_bNetRank);
	CopyMemory(m_vAxons[bLayer],fAxons,sizeof(FLOAT)*m_wLayerRank[bLayer]);
}

void NetBP::ForwardPass(const BYTE bFrom)
{
	ASSERT(bFrom<m_bNetRank);

	FLOAT fNet;
	DWORD i,j,k;
	for (i=bFrom+1; i<m_bNetRank; i++)
		for (j=0; j<m_wLayerRank[i]; j++)
		{
			fNet=0.0f;
			for (k=0; k<m_wNeuronRank[i-1]; k++)
				fNet+=m_vAxons[i-1][k]*m_vWeights[WEIGH][i-1][j][k];
			if (m_bUseBias)
				fNet+=m_fMaxSignal*m_vBiases[WEIGH][i-1][j];

			m_vAxons[i][j]=SigmoidFunction(fNet);
		}
}

FLOAT NetBP::TargetFunction(const BYTE bLayer, const PFLOAT fTarget)
{
	ASSERT(bLayer<m_bNetRank);
	FLOAT fDist;
	FLOAT fError=0.0;
	DWORD i;
	for (i=0; i<m_wLayerRank[bLayer]; i++)
	{
		fDist=m_vAxons[bLayer][i]-fTarget[i];
		fError+=fDist*fDist;
	}
	return fError/2;
}

void NetBP::BackwardPass(const PFLOAT fTarget)
{
	UpdateGradients(fTarget);
	UpdateWeights();
}

FLOAT NetBP::BackwardPass(const DWORD dwPatterns,const PFLOAT *fPatterns,const FLOAT fPrevNetError)
{
	FLOAT fGradNorm=0.0;
	FLOAT fGrad=0.0;
	FLOAT fWeight=0.0;
	FLOAT fValue;
	BOOLEAN bFlag;
	FLOAT fPrevLearnRate=m_fLearnRate;
	FLOAT fNetError;
	DWORD i,j,k;
	DWORD dwCount=0;

	if (m_bFirstEpoch)
	{
		m_bFirstEpoch=FALSE;
		for (i=0; i<m_bNetRank-1; i++)
			for (j=0; j<m_wLayerRank[i+1]; j++)
				for (k=0; k<m_wNeuronRank[i]; k++)
					fGradNorm+=m_vWeights[GRAD2][i][j][k]*m_vWeights[GRAD2][i][j][k];

		if (m_bUseBias)
			for (i=0; i<m_bNetRank-1; i++)
				for (j=0; j<m_wLayerRank[i+1]; j++)
					fGradNorm+=m_vBiases[GRAD2][i][j]*m_vBiases[GRAD2][i][j];

		m_fLearnRate=fPrevNetError/sqrt(fGradNorm);

		CopyWeights(DELTA,WEIGH);

		bFlag=FALSE;
		do
		{
			UpdateWeights(WEIGH,DELTA);
			fNetError=0.0;
			for (i=0; i<dwPatterns; i++)
			{
				SetAxons(BP_INPUT_LAYER,fPatterns[i]);
				ForwardPass(BP_INPUT_LAYER);
				fNetError+=TargetFunction(m_bNetRank-1,fPatterns[i]);
			}
			if ((fNetError-fPrevNetError)>(-0.5*m_fLearnRate*fGradNorm))
			{
				m_fLearnRate/=2.0;
				dwCount++;
			}
			else
				bFlag=TRUE;
		}
		while (!bFlag&&dwCount<MIT);
		if (!bFlag||!_finite(m_fLearnRate))
			m_fLearnRate=fPrevLearnRate;
		CopyWeights(GRAD1,GRAD2);
		ZeroWeights(GRAD2);
		return fNetError;
	}

	for (i=0; i<m_bNetRank-1; i++)
		for (j=0; j<m_wLayerRank[i+1]; j++)
			for (k=0; k<m_wNeuronRank[i]; k++)
			{
				fGradNorm+=m_vWeights[GRAD2][i][j][k]*m_vWeights[GRAD2][i][j][k];
				fValue=m_vWeights[GRAD2][i][j][k]-m_vWeights[GRAD1][i][j][k];
				fGrad+=fValue*fValue;
				fWeight+=m_vWeights[GRAD1][i][j][k]*m_vWeights[GRAD1][i][j][k];
			}

	if (m_bUseBias)
		for (i=0; i<m_bNetRank-1; i++)
			for (j=0; j<m_wLayerRank[i+1]; j++)
			{
				fGradNorm+=m_vBiases[GRAD2][i][j]*m_vBiases[GRAD2][i][j];
				fGradNorm+=m_vBiases[GRAD2][i][j]*m_vBiases[GRAD2][i][j];
				fValue=m_vBiases[GRAD2][i][j]-m_vBiases[GRAD1][i][j];
				fGrad+=fValue*fValue;
				fWeight+=m_vBiases[GRAD1][i][j]*m_vBiases[GRAD1][i][j];
			}

	m_fLearnRate=(FLOAT)(0.5*sqrt(fWeight/fGrad)*m_fLearnRate);
	
	bFlag=FALSE;
	CopyWeights(DELTA,WEIGH);
	do
	{
		fNetError=0.0;
		UpdateWeights(WEIGH,DELTA);
		for (i=0; i<dwPatterns; i++)
		{
			SetAxons(BP_INPUT_LAYER,fPatterns[i]);
			ForwardPass(BP_INPUT_LAYER);
			fNetError+=TargetFunction(m_bNetRank-1,fPatterns[i]);
		}
		if ((fNetError-fPrevNetError)>(-0.5*m_fLearnRate*fGradNorm))
		{
			m_fLearnRate/=2.0;
			dwCount++;
		}
		else
			bFlag=TRUE;
	}
	while(!bFlag&&dwCount<MIT);
	if (!bFlag)
		m_fLearnRate=fPrevLearnRate;
	CopyWeights(GRAD1,GRAD2);
	ZeroWeights(GRAD2);
	return fNetError;
}

void NetBP::UpdateGradients(const PFLOAT fTarget)
{
	DWORD i,j,k;
	for (i=0; i<m_wLayerRank[m_bNetRank-1]; i++)
		m_vErrorSignal[m_bNetRank-2][i]=(m_vAxons[m_bNetRank-1][i]-fTarget[i])*
					DSigmoidFunction(m_vAxons[m_bNetRank-1][i]);

	FLOAT fSum;
	for (i=m_bNetRank-2; i>=1; i--)
		for (j=0; j<m_wLayerRank[i]; j++)
		{
			fSum=0.0f;
			for (k=0; k<m_wLayerRank[i+1]; k++)
				fSum+=m_vErrorSignal[i][k]*m_vWeights[WEIGH][i][k][j];
			m_vErrorSignal[i-1][j]=fSum*DSigmoidFunction(m_vAxons[i][j]);
		}

	if (m_bLearnType & LEARN_TYPE_SEQUENTIAL)
	{
		for (i=0; i<m_bNetRank-1; i++)
			for (j=0; j<m_wLayerRank[i+1]; j++)
				for (k=0; k<m_wNeuronRank[i]; k++)
					m_vWeights[GRAD2][i][j][k]=m_vErrorSignal[i][j]*m_vAxons[i][k];

		if (m_bUseBias)
			for (i=0; i<m_bNetRank-1; i++)
				for (j=0; j<m_wLayerRank[i+1]; j++)
					m_vBiases[GRAD2][i][j]=m_vErrorSignal[i][j]*m_fMaxSignal;
	}

	if (m_bLearnType & LEARN_TYPE_BATCH)
	{
		for (i=0; i<m_bNetRank-1; i++)
			for (j=0; j<m_wLayerRank[i+1]; j++)
				for (k=0; k<m_wNeuronRank[i]; k++)
					m_vWeights[GRAD2][i][j][k]+=m_vErrorSignal[i][j]*m_vAxons[i][k];
				
		if (m_bUseBias)
			for (i=0; i<m_bNetRank-1; i++)
				for (UINT j=0; j<m_wLayerRank[i+1]; j++)
					m_vBiases[GRAD2][i][j]+=m_vErrorSignal[i][j]*m_fMaxSignal;
	}
}

void NetBP::UpdateWeights()
{
	DWORD i,j,k;
	FLOAT fDelta;
	if (m_bLearnType & LEARN_TYPE_MOMENTUM)
	{
		for (i=0; i<m_bNetRank-1; i++)
			for (j=0; j<m_wLayerRank[i+1]; j++)
				for (k=0; k<m_wNeuronRank[i]; k++)
				{
					fDelta=/*-m_fLearnRate**/((1.0f-m_fMomentumParam)*m_vWeights[GRAD2][i][j][k]+
						m_fMomentumParam*m_vWeights[DELTA][i][j][k]);
					m_vWeights[DELTA][i][j][k]=fDelta;
					m_vWeights[WEIGH][i][j][k]+=-m_fLearnRate*fDelta;
				}
		if (m_bUseBias)
			for (i=0; i<m_bNetRank-1; i++)
				for (j=0; j<m_wLayerRank[i+1]; j++)
				{
					fDelta=/*-m_fLearnRate**/((1.0f-m_fMomentumParam)*m_vBiases[GRAD2][i][j]+
						m_fMomentumParam*m_vBiases[DELTA][i][j]);
					m_vBiases[DELTA][i][j]=fDelta;
					m_vBiases[WEIGH][i][j]+=-m_fLearnRate*fDelta;
				}
	}
	else
	{
		for (i=0; i<m_bNetRank-1; i++)
			for (j=0; j<m_wLayerRank[i+1]; j++)
				for (k=0; k<m_wNeuronRank[i]; k++)
					m_vWeights[WEIGH][i][j][k]+=-m_fLearnRate*m_vWeights[GRAD2][i][j][k];
		if (m_bUseBias)
			for (i=0; i<m_bNetRank-1; i++)
				for (j=0; j<m_wLayerRank[i+1]; j++)
					m_vBiases[WEIGH][i][j]+=-m_fLearnRate*m_vBiases[GRAD2][i][j];
	}
}

void NetBP::UpdateWeights(const BYTE bNextIndex,const BYTE bPrevIndex)
{
	DWORD i,j,k;
	for (i=0; i<m_bNetRank-1; i++)
		for (j=0; j<m_wLayerRank[i+1]; j++)
			for (k=0; k<m_wNeuronRank[i]; k++)
				m_vWeights[bNextIndex][i][j][k]=m_vWeights[bPrevIndex][i][j][k]
					-m_fLearnRate*m_vWeights[GRAD2][i][j][k];
	if (m_bUseBias)
		for (i=0; i<m_bNetRank-1; i++)
			for (j=0; j<m_wLayerRank[i+1]; j++)
				m_vBiases[bNextIndex][i][j]=m_vBiases[bPrevIndex][i][j]
					-m_fLearnRate*m_vBiases[GRAD2][i][j];
}

void NetBP::SetSigmoidType(const BYTE bSigmoidType)
{
	m_bSigmoidType=bSigmoidType;
}

FLOAT NetBP::SigmoidFunction(const FLOAT fValue)
{
	switch (m_bSigmoidType)
	{
		case SIGMOID_TYPE_ORIGINAL:
			return (FLOAT)(-m_fScaleParam+1.0/(1.0+exp(-m_fSigmoidAlpha*fValue)));
		case SIGMOID_TYPE_HYPERTAN:
			return (FLOAT)(m_fScaleParam*tanh(m_fSigmoidAlpha*fValue));
	}
	return fValue;
}

FLOAT NetBP::DSigmoidFunction(const FLOAT fValue)
{
	switch (m_bSigmoidType)
	{
	case SIGMOID_TYPE_ORIGINAL:
		return /*m_fSigmoidAlpha*((1.0f-m_fScaleParam)-fValue)*(m_fScaleParam+fValue);*/
			m_fSigmoidAlpha*(fValue+m_fScaleParam)*(1.0f-(fValue+m_fScaleParam));
	case SIGMOID_TYPE_HYPERTAN:
		return /*m_fSigmoidAlpha/m_fScaleParam*(m_fScaleParam-fValue)*(m_fScaleParam+fValue);*/
			m_fSigmoidAlpha*(m_fScaleParam-(fValue*fValue)/m_fScaleParam);
	}
	return fValue;
}

void NetBP::SetLearnRate(const FLOAT fLearnRate)
{
	m_fLearnRate=fLearnRate;
}

void NetBP::SetMomentumParam(const FLOAT fMomentumParam)
{
	m_fMomentumParam=fMomentumParam;
}

void NetBP::SetSigmoidAlpha(const FLOAT fSigmoidAlpha)
{
	m_fSigmoidAlpha=fSigmoidAlpha;
}

void NetBP::SetScaleParam(const FLOAT fScaleParam)
{
	m_fScaleParam=fScaleParam;
}

void NetBP::GetAxons(const BYTE bLayer,PFLOAT fOutput)
{
	ASSERT(bLayer<m_bNetRank);
	CopyMemory(fOutput,m_vAxons[bLayer],sizeof(FLOAT)*m_wLayerRank[bLayer]);
}

void NetBP::CopyWeights(const BYTE bDestIndex,const BYTE bSrcIndex)
{
	DWORD i;
	for (i=0; i<m_bNetRank-1; i++)
		CopyMemory(m_vWeights[bDestIndex][i][0],
			m_vWeights[bSrcIndex][i][0],sizeof(FLOAT)*m_wLayerRank[i+1]*m_wNeuronRank[i]);
	if (m_bUseBias)
		CopyMemory(m_vBiases[bDestIndex][0],
			m_vBiases[bSrcIndex][0],sizeof(FLOAT)*m_dwBiasMatrixSize);
}

void NetBP::ZeroWeights(const BYTE bDestIndex)
{
	DWORD i;
	for (i=0; i<m_bNetRank-1; i++)
		ZeroMemory(m_vWeights[bDestIndex][i][0],sizeof(FLOAT)*m_wLayerRank[i+1]*m_wNeuronRank[i]);
	if (m_bUseBias)
		ZeroMemory(m_vBiases[bDestIndex][0],sizeof(FLOAT)*m_dwBiasMatrixSize);
}

void NetBP::SetLearnType(const BYTE bLearnType)
{
	m_bLearnType=bLearnType;
}

FLOAT NetBP::GetLearnRate()
{
	return m_fLearnRate;
}

void NetBP::CopyWeights(LPVOID pDest,const BYTE bLayer)
{
	ASSERT(bLayer<m_bNetRank);
	CopyMemory(pDest,m_vWeights[WEIGH][bLayer-1][0],
		m_wNeuronRank[bLayer-1]*m_wLayerRank[bLayer]*sizeof(FLOAT));
	if (m_bUseBias)
		CopyMemory((PFLOAT)pDest+m_wNeuronRank[bLayer-1]*m_wLayerRank[bLayer],
			m_vBiases[WEIGH][bLayer-1],m_wLayerRank[bLayer]*sizeof(FLOAT));
}


DWORD NetBP::GetWeightsSize(const BYTE bLayer)
{
	ASSERT(bLayer<m_bNetRank);
	DWORD dwSize=m_wLayerRank[bLayer]*m_wNeuronRank[bLayer-1];
	if (m_bUseBias)
		dwSize+=m_wLayerRank[bLayer];
	return (dwSize*sizeof(FLOAT));
}
