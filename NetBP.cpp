// NetBP.cpp: implementation of the NetBP class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NNIC.h"
#include "NetBP.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NetBP::NetBP(const UINT uiNetRank,const UINT uiLayerRank[]):m_uiNetRank(uiNetRank),
	m_bUseBias(FALSE),m_uiLayerRank(NULL),m_uiNeuronRank(NULL),m_vWeights(NULL),m_vAxons(NULL),
	m_vBiases(NULL),m_vWeightsDelta(NULL),m_vBiasesDelta(NULL)
{
	m_uiLayerRank=new UINT[m_uiNetRank];
	ASSERT(m_uiLayerRank!=NULL);

	for (UINT i=0; i<m_uiNetRank; i++)
		m_uiLayerRank[i]=uiLayerRank[i];

	m_uiNeuronRank=new UINT[m_uiNetRank-1];
	ASSERT(m_uiNeuronRank!=NULL);

	for (i=0; i<m_uiNetRank-1; i++)
		m_uiNeuronRank[i]=m_uiLayerRank[i];

	m_vWeights=new FLOAT **[m_uiNetRank-1];
	ASSERT(m_vWeights!=NULL);
	for (i=0; i<m_uiNetRank-1; i++)
	{
		m_vWeights[i]=new FLOAT *[m_uiLayerRank[i+1]];
		ASSERT(m_vWeights[i]!=NULL);
		for (UINT j=0; j<m_uiLayerRank[i+1]; j++)
		{
			m_vWeights[i][j]=new FLOAT[m_uiNeuronRank[i]];
			ASSERT(m_vWeights[i][j]!=NULL);
		}
	}

	m_vWeightsDelta=new FLOAT **[m_uiNetRank-1];
	ASSERT(m_vWeightsDelta!=NULL);
	for (i=0; i<m_uiNetRank-1; i++)
	{
		m_vWeightsDelta[i]=new FLOAT *[m_uiLayerRank[i+1]];
		ASSERT(m_vWeightsDelta[i]!=NULL);
		for (UINT j=0; j<m_uiLayerRank[i+1]; j++)
		{
			m_vWeightsDelta[i][j]=new FLOAT[m_uiNeuronRank[i]];
			ASSERT(m_vWeightsDelta[i][j]!=NULL);
			ZeroMemory(m_vWeightsDelta[i][j],sizeof(FLOAT)*m_uiNeuronRank[i]);
		}
	}

	m_vAxons=new FLOAT *[m_uiNetRank];
	ASSERT(m_vAxons!=NULL);
	for (i=0; i<m_uiNetRank; i++)
	{
		m_vAxons[i]=new FLOAT[m_uiLayerRank[i]];
		ASSERT(m_vAxons[i]!=NULL);
	}
}

NetBP::~NetBP()
{
	if (m_uiLayerRank!=NULL)
	{
		for (UINT i=0; i<m_uiNetRank-1; i++)
		{
			for (UINT j=0; j<m_uiLayerRank[i+1]; j++)
			{
				delete []m_vWeights[i][j];
				delete []m_vWeightsDelta[i][j];
			}
			delete []m_vWeights[i];
			delete []m_vWeightsDelta[i];
		}
		delete []m_vWeights;
		delete []m_vWeightsDelta;

		if (m_bUseBias)
		{
			for (i=0; i<m_uiNetRank-1; i++)
			{
				delete []m_vBiases[i];
				delete []m_vBiasesDelta[i];
			}
			delete []m_vBiases;
			delete []m_vBiasesDelta;
		}
	}
	for (UINT i=0; i<m_uiNetRank; i++)
			delete []m_vAxons[i];
	delete []m_vAxons;
	delete []m_uiNeuronRank;
	delete []m_uiLayerRank;
}

void NetBP::SetSignalBoundaries(const FLOAT fMinSignal, const FLOAT fMaxSignal)
{
	m_fMinSignal=fMinSignal;
	m_fMaxSignal=fMaxSignal;
}

void NetBP::UseBiases(const BOOL bFlag)
{
	m_bUseBias=bFlag;
	if (bFlag)
	{
		m_vBiases=new FLOAT *[m_uiNetRank-1];
		ASSERT(m_vBiases!=NULL);
		for (UINT i=0; i<m_uiNetRank-1; i++)
		{
			m_vBiases[i]=new FLOAT[m_uiNeuronRank[i]];
			ASSERT(m_vBiases[i]!=NULL);
		}

		m_vBiasesDelta=new FLOAT *[m_uiNetRank-1];
		ASSERT(m_vBiasesDelta!=NULL);
		for (i=0; i<m_uiNetRank-1; i++)
		{
			m_vBiasesDelta[i]=new FLOAT[m_uiNeuronRank[i]];
			ASSERT(m_vBiasesDelta[i]!=NULL);
			ZeroMemory(m_vBiasesDelta[i],sizeof(FLOAT)*m_uiNeuronRank[i]);
		}
	}
}

void NetBP::InitWeights()
{
	FLOAT fRange=m_fMaxSignal-m_fMinSignal;

	srand((unsigned)time(NULL));
	for (UINT i=0; i<m_uiNetRank-1; i++)
		for (UINT j=0; j<m_uiLayerRank[i+1]; j++)
			for (UINT k=0; k<m_uiNeuronRank[i]; k++)
				m_vWeights[i][j][k]=m_fMinSignal+(FLOAT)rand()/RAND_MAX*fRange;
	if (m_bUseBias)
		for (i=0; i<m_uiNetRank-1; i++)
			for (UINT j=0; j<m_uiNeuronRank[i]; j++)
				m_vBiases[i][j]=m_fMinSignal+(FLOAT)rand()/RAND_MAX*fRange;
}

void NetBP::SetAxons(const UINT uiLayer, const FLOAT *fAxons)
{
	ASSERT(uiLayer<m_uiNetRank);
	CopyMemory(m_vAxons[uiLayer],fAxons,sizeof(FLOAT)*m_uiLayerRank[uiLayer]);
}

void NetBP::ForwardPass(const UINT uiFrom)
{
	ASSERT(uiFrom<m_uiNetRank);

	FLOAT fNet;

	for (UINT i=uiFrom+1; i<m_uiNetRank; i++)
		for (UINT j=0; j<m_uiLayerRank[i]; j++)
		{
			fNet=0.0f;
			for (UINT k=0; k<m_uiNeuronRank[i-1]; k++)
				fNet+=m_vAxons[i-1][k]*m_vWeights[i][j][k];
			if (m_bUseBias)
				fNet+=m_fMaxSignal*m_vBiases[i-1][j];

			m_vAxons[i][j]=SigmoidFunction(fNet);
		}
}

DOUBLE NetBP::TargetFunction(const UINT uiLayer, const FLOAT *fTarget)
{
	ASSERT(uiLayer<m_uiNetRank);
	DOUBLE fDist;
	DOUBLE fError=0.0;
	for (UINT i=0; i<m_uiLayerRank[uiLayer]; i++)
	{
		fDist=m_vAxons[uiLayer][i]-fTarget[i];
		fError+=fDist*fDist;
	}
	return fError/2;
}

void NetBP::BackwardPass(const FLOAT *fTarget)
{
	FLOAT **vErrorSignal=new FLOAT *[m_uiNetRank-1];
	ASSERT(vErrorSignal!=NULL);
	for (UINT i=1; i<m_uiNetRank; i++)
	{
		vErrorSignal[i-1]=new FLOAT[m_uiLayerRank[i]];
		ASSERT(vErrorSignal[i-1]!=NULL);
	}

	for (i=0; i<m_uiLayerRank[m_uiNetRank-1]; i++)
		vErrorSignal[m_uiNetRank-2][i]=(m_vAxons[m_uiNetRank-1][i]-fTarget[i])*
					DSigmoidFunction(m_vAxons[m_uiNetRank-1][i]);

	FLOAT fSum;
	for (i=m_uiNetRank-2; i>=1; i--)
		for (UINT j=0; j<m_uiLayerRank[i]; j++)
		{
			fSum=0.0f;
			for (UINT k=0; k<m_uiLayerRank[i+1]; k++)
				fSum+=vErrorSignal[i][k]*m_vWeights[i][k][j];
			vErrorSignal[i-1][j]=fSum*DSigmoidFunction(m_vAxons[i][j]);
		}

	FLOAT fDelta;
	for (i=0; i<m_uiNetRank-1; i++)
		for (UINT j=0; j<m_uiLayerRank[i+1]; j++)
			for (UINT k=0; k<m_uiNeuronRank[i]; k++)
			{
				fDelta=-m_fLearnRate*(m_fMomentumParam*m_vWeightsDelta[i][j][k]+
					(1-m_fMomentumParam)*vErrorSignal[i][j]*m_vAxons[i][k]);
				m_vWeights[i][j][k]+=fDelta;
				m_vWeightsDelta[i][j][k]=fDelta;
			}
	if (m_bUseBias)
	{
		for (i=0; i<m_uiNetRank-1; i++)
			for (UINT j=0; j<m_uiLayerRank[i+1]; j++)
			{
				fDelta=-m_fLearnRate*(m_fMomentumParam*m_vBiasesDelta[i][j]+
					(1-m_fMomentumParam)*vErrorSignal[i][j]*m_fMaxSignal);
				m_vBiases[i][j]+=fDelta;
				m_vBiasesDelta[i][j]=fDelta;
			}
	}
	for (i=0; i<m_uiNetRank-1; i++)
		delete []vErrorSignal[i];

	delete []vErrorSignal;
}

void NetBP::SetSigmoidType(const UINT uiSigmoidType)
{
	m_uiSigmoidType=uiSigmoidType;
}

FLOAT NetBP::SigmoidFunction(const FLOAT fValue)
{
	switch (m_uiSigmoidType)
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
	switch (m_uiSigmoidType)
	{
	case SIGMOID_TYPE_ORIGINAL:
		return (FLOAT)(m_fSigmoidAlpha*((1.0-m_fScaleParam)-fValue)*(m_fScaleParam+fValue));
	case SIGMOID_TYPE_HYPERTAN:
		return m_fSigmoidAlpha/m_fScaleParam*(m_fScaleParam-fValue)*(m_fScaleParam+fValue);
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
