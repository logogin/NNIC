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

NetBP::NetBP(const UINT uiNetRank,const UINT uiLayerRank[]):m_uiNetRank(uiNetRank),
	m_bUseBias(FALSE),m_uiLayerRank(NULL),m_uiNeuronRank(NULL),m_vAxons(NULL),
	m_bFirstEpoch(TRUE)
{
	m_uiLayerRank=new UINT[m_uiNetRank];
	ASSERT(m_uiLayerRank!=NULL);

	for (UINT i=0; i<m_uiNetRank; i++)
		m_uiLayerRank[i]=uiLayerRank[i];

	m_uiNeuronRank=new UINT[m_uiNetRank-1];
	ASSERT(m_uiNeuronRank!=NULL);

	for (i=0; i<m_uiNetRank-1; i++)
		m_uiNeuronRank[i]=m_uiLayerRank[i];

	for (i=0; i<ARRAY_SIZE; i++)
	{
		m_vWeights[i]=new FLOAT **[m_uiNetRank-1];
		ASSERT(m_vWeights[i]!=NULL);
		for (UINT j=0; j<m_uiNetRank-1; j++)
		{
			m_vWeights[i][j]=new FLOAT *[m_uiLayerRank[j+1]];
			ASSERT(m_vWeights[i][j]!=NULL);
			for (UINT k=0; k<m_uiLayerRank[j+1]; k++)
			{
				m_vWeights[i][j][k]=new FLOAT[m_uiNeuronRank[j]];
				ASSERT(m_vWeights[i][j][k]!=NULL);
				ZeroMemory(m_vWeights[i][j][k],sizeof(FLOAT)*m_uiNeuronRank[j]);
			}
		}
	}

	m_vAxons=new FLOAT *[m_uiNetRank];
	ASSERT(m_vAxons!=NULL);
	for (i=0; i<m_uiNetRank; i++)
	{
		m_vAxons[i]=new FLOAT[m_uiLayerRank[i]];
		ASSERT(m_vAxons[i]!=NULL);
	}

	m_vErrorSignal=new FLOAT *[m_uiNetRank-1];
	ASSERT(m_vErrorSignal!=NULL);
	for (i=1; i<m_uiNetRank; i++)
	{
		m_vErrorSignal[i-1]=new FLOAT[m_uiLayerRank[i]];
		ASSERT(m_vErrorSignal[i-1]!=NULL);
	}
}

NetBP::~NetBP()
{
	if (m_uiLayerRank!=NULL)
	{
		for (UINT i=0; i<ARRAY_SIZE; i++)
		{
			for (UINT j=0; j<m_uiNetRank-1; j++)
			{
				for (UINT k=0; k<m_uiLayerRank[j+1]; k++)
					delete []m_vWeights[i][j][k];
				delete []m_vWeights[i][j];
			}
			delete []m_vWeights[i];
		}

		if (m_bUseBias)
			for (i=0; i<ARRAY_SIZE; i++)
			{
				for (UINT j=0; j<m_uiNetRank-1; j++)
					delete []m_vBiases[i][j];
				delete []m_vBiases[i];
			}
	}

	for (UINT i=0; i<m_uiNetRank-1; i++)
		delete []m_vErrorSignal[i];
	delete []m_vErrorSignal;

	for (i=0; i<m_uiNetRank; i++)
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
		for (UINT i=0; i<ARRAY_SIZE; i++)
		{
			m_vBiases[i]=new FLOAT *[m_uiNetRank-1];
			ASSERT(m_vBiases[i]!=NULL);
			for (UINT j=0; j<m_uiNetRank-1; j++)
			{
				m_vBiases[i][j]=new FLOAT[m_uiLayerRank[j+1]];
				ASSERT(m_vBiases[i][j]!=NULL);
				ZeroMemory(m_vBiases[i][j],sizeof(FLOAT)*m_uiLayerRank[j+1]);
			}
		}
}

void NetBP::InitWeights()
{
	FLOAT fRange=m_fMaxSignal-m_fMinSignal;

//	UINT r=0;
	srand((unsigned)time(NULL));
	for (UINT i=0; i<m_uiNetRank-1; i++)
		for (UINT j=0; j<m_uiLayerRank[i+1]; j++)
			for (UINT k=0; k<m_uiNeuronRank[i]; k++)
//			{
//				r++;
				m_vWeights[WEIGH][i][j][k]=
					m_fMinSignal+/*(r%100)/100.0f*/(FLOAT)rand()/RAND_MAX*fRange;
//			}
	if (m_bUseBias)
		for (i=0; i<m_uiNetRank-1; i++)
			for (UINT j=0; j<m_uiLayerRank[i+1]; j++)
				m_vBiases[WEIGH][i][j]=m_fMinSignal+(FLOAT)rand()/RAND_MAX*fRange;
}

void NetBP::SetAxons(const UINT uiLayer, const FLOAT *fAxons)
{
	ASSERT(uiLayer<m_uiNetRank);
	CopyMemory(m_vAxons[uiLayer],fAxons,sizeof(FLOAT)*m_uiLayerRank[uiLayer]);
}

void NetBP::ForwardPass(const UINT uiFrom)
{
	ASSERT(uiFrom<m_uiNetRank);

	/*DOUBLE*/FLOAT fNet;

//	ofstream file;
//	file.open(_T("debug.txt"),ios::out | ios::app);

//	file<<endl<<"PROPAGATION"<<endl<<"Layer 0"<<endl;
//	for (UINT m=0; m<m_uiLayerRank[0]; m++)
//		file<<m_vAxons[0][m]<<" "<<flush;

//	file<<endl;
	for (UINT i=uiFrom+1; i<m_uiNetRank; i++)
	{
//		file<<"Layer "<<i<<endl;
		for (UINT j=0; j<m_uiLayerRank[i]; j++)
		{
			fNet=0.0f;
			for (UINT k=0; k<m_uiNeuronRank[i-1]; k++)
				fNet+=m_vAxons[i-1][k]*m_vWeights[WEIGH][i-1][j][k];
			if (m_bUseBias)
				fNet+=m_fMaxSignal*m_vBiases[WEIGH][i-1][j];

			m_vAxons[i][j]=SigmoidFunction(fNet);
//			file<<m_vAxons[i][j]<<" "<<flush;
		}
//		file<<endl;
	}
//	file.close();
}

/*DOUBLE*/FLOAT NetBP::TargetFunction(const UINT uiLayer, const FLOAT *fTarget)
{
	ASSERT(uiLayer<m_uiNetRank);
	/*DOUBLE*/FLOAT fDist;
	/*DOUBLE*/FLOAT fError=0.0;
	for (UINT i=0; i<m_uiLayerRank[uiLayer]; i++)
	{
		fDist=m_vAxons[uiLayer][i]-fTarget[i];
		fError+=fDist*fDist;
	}
	return fError/2;
}

void NetBP::BackwardPass(const FLOAT *fTarget)
{
	UpdateGradients(fTarget);
	UpdateWeights();
/*	ofstream file("debug.txt",ios::out | ios::app);

	file<<endl<<endl<<"\nWEIGHTS"<<endl;
	for (i=0; i<m_uiNetRank-1; i++)
	{
		file<<"Layer "<<i+1<<endl;
		for (UINT j=0; j<m_uiLayerRank[i+1]; j++)
		{
			file<<"\tNeuron "<<j<<endl;
			for (UINT k=0; k<m_uiNeuronRank[i]; k++)
				file<<m_vWeights[WEIGH][i][j][k]<<" ";
			file<<endl;
		}
		file<<endl;
	}
	file.close();*/
}

FLOAT NetBP::BackwardPass(const UINT uiPatterns,const FLOAT **fPatterns,const FLOAT fPrevNetError)
{
//	FLOAT fGradNorm=0.0;
//
//	for (UINT i=0; i<m_uiNetRank-1; i++)
//		for (UINT j=0; j<m_uiLayerRank[i+1]; j++)
//			for (UINT k=0; k<m_uiNeuronRank[i]; k++)
//				fGradNorm+=m_vWeights[GRAD2][i][j][k]*m_vWeights[GRAD2][i][j][k];
//
//	if (m_bUseBias)
//		for (i=0; i<m_uiNetRank-1; i++)
//			for (UINT j=0; j<m_uiLayerRank[i+1]; j++)
//				fGradNorm+=m_vBiases[GRAD2][i][j]*m_vBiases[GRAD2][i][j];
//
//	m_fLearnRate=fPrevNetError/fGradNorm;
//
//	FLOAT fNetError;
//	
//	CopyWeights(DELTA,WEIGH);
//
//	BOOL bFlag=FALSE;
//	do
//	{
//		UpdateWeights(WEIGH,DELTA);
//		fNetError=0.0;
//		for (i=0 ;i<uiPatterns; i++)
//		{
//			SetAxons(0,fPatterns[i]);
//			ForwardPass(0);
//			fNetError+=TargetFunction(2,fPatterns[i]);
//		}
//		if ((fNetError-fPrevNetError)>(-0.5*m_fLearnRate*fGradNorm))
//			m_fLearnRate/=2;
//		else
//			bFlag=TRUE;
//	}
//	while (!bFlag);
//	ZeroWeights(GRAD2);

/****************/
	FLOAT fGradNorm=0.0;
	FLOAT fGrad=0.0;
	FLOAT fWeight=0.0;
	FLOAT fValue;
	BOOL bFlag;
	FLOAT fNetError;
	UINT i;
	UINT j;
	UINT k;

//	if (m_bFirstEpoch)
//	{
//		m_bFirstEpoch=FALSE;
//		m_fLearnRate=0.1f;
//		UpdateWeights(WEIGH,DELTA);
//		CopyWeights(GRAD1,GRAD2);
//		ZeroWeights(GRAD2);
//		return 0.0f;
//	}

	if (m_bFirstEpoch)
	{
		m_bFirstEpoch=FALSE;
		for (i=0; i<m_uiNetRank-1; i++)
			for (j=0; j<m_uiLayerRank[i+1]; j++)
				for (k=0; k<m_uiNeuronRank[i]; k++)
					fGradNorm+=m_vWeights[GRAD2][i][j][k]*m_vWeights[GRAD2][i][j][k];

		if (m_bUseBias)
			for (i=0; i<m_uiNetRank-1; i++)
				for (j=0; j<m_uiLayerRank[i+1]; j++)
					fGradNorm+=m_vBiases[GRAD2][i][j]*m_vBiases[GRAD2][i][j];

		m_fLearnRate=fPrevNetError/fGradNorm;

		CopyWeights(DELTA,WEIGH);

		bFlag=FALSE;
		do
		{
			UpdateWeights(WEIGH,DELTA);
			fNetError=0.0;
			for (i=0 ;i<uiPatterns; i++)
			{
				SetAxons(0,fPatterns[i]);
				ForwardPass(0);
				fNetError+=TargetFunction(m_uiNetRank-1,fPatterns[i]);
			}
			if ((fNetError-fPrevNetError)>(-0.5*m_fLearnRate*fGradNorm))
				m_fLearnRate/=2;
			else
				bFlag=TRUE;
		}
		while (!bFlag);
		CopyWeights(GRAD1,GRAD2);
		ZeroWeights(GRAD2);
		return fNetError;
	}

	for (i=0; i<m_uiNetRank-1; i++)
		for (j=0; j<m_uiLayerRank[i+1]; j++)
			for (k=0; k<m_uiNeuronRank[i]; k++)
			{
				fGradNorm+=m_vWeights[GRAD2][i][j][k]*m_vWeights[GRAD2][i][j][k];
				fValue=m_vWeights[GRAD2][i][j][k]-m_vWeights[GRAD1][i][j][k];
				fGrad+=fValue*fValue;
				fWeight+=m_vWeights[GRAD1][i][j][k]*m_vWeights[GRAD1][i][j][k];
			}

	if (m_bUseBias)
		for (i=0; i<m_uiNetRank-1; i++)
			for (j=0; j<m_uiLayerRank[i+1]; j++)
			{
				fGradNorm+=m_vBiases[GRAD2][i][j]*m_vBiases[GRAD2][i][j];
				fGradNorm+=m_vBiases[GRAD2][i][j]*m_vBiases[GRAD2][i][j];
				fValue=m_vBiases[GRAD2][i][j]-m_vBiases[GRAD1][i][j];
				fGrad+=fValue*fValue;
				fWeight+=m_vBiases[GRAD1][i][j]*m_vBiases[GRAD1][i][j];
			}

	m_fLearnRate=(FLOAT)(0.5f*sqrt(fWeight/fGrad)*m_fLearnRate);
	
	bFlag=FALSE;
	CopyWeights(DELTA,WEIGH);
	do
	{
		fNetError=0.0;
		UpdateWeights(WEIGH,DELTA);
		for (i=0; i<uiPatterns; i++)
		{
			SetAxons(0,fPatterns[i]);
			ForwardPass(0);
			fNetError+=TargetFunction(m_uiNetRank-1,fPatterns[i]);
		}
		if ((fNetError-fPrevNetError)>(-0.5*m_fLearnRate*fGradNorm))
			m_fLearnRate/=2;
		else
			bFlag=TRUE;
	}
	while(!bFlag);
	CopyWeights(GRAD1,GRAD2);
	ZeroWeights(GRAD2);
	return fNetError;
}

void NetBP::UpdateGradients(const FLOAT *fTarget)
{
//	ofstream file("debug.txt",ios::out | ios::app);
//	file<<endl<<"CALCULATE DELTA"<<endl;
//	file<<"Layer "<<m_uiNetRank-1<<endl;
	for (UINT i=0; i<m_uiLayerRank[m_uiNetRank-1]; i++)
	{
		m_vErrorSignal[m_uiNetRank-2][i]=(m_vAxons[m_uiNetRank-1][i]-fTarget[i])*
					DSigmoidFunction(m_vAxons[m_uiNetRank-1][i]);
//		file<<"delta :"<<(m_vAxons[m_uiNetRank-1][i]-fTarget[i])
//			<<" dy/ds :"<<DSigmoidFunction(m_vAxons[m_uiNetRank-1][i])
//			<<" error :"<<m_vErrorSignal[m_uiNetRank-2][i]<<" "<<endl;
//		file<<m_vErrorSignal[m_uiNetRank-2][i]<<" "<<flush;
	}
	/*DOUBLE*/FLOAT fSum;
	for (i=m_uiNetRank-2; i>=1; i--)
	{
//		file<<endl<<"Layer "<<i<<endl;
		for (UINT j=0; j<m_uiLayerRank[i]; j++)
		{
			fSum=0.0f;
			for (UINT k=0; k<m_uiLayerRank[i+1]; k++)
				fSum+=m_vErrorSignal[i][k]*m_vWeights[WEIGH][i][k][j];
			m_vErrorSignal[i-1][j]=fSum*DSigmoidFunction(m_vAxons[i][j]);
//			file<<m_vErrorSignal[i-1][j]<<" "<<flush;
		}
	}

	if (m_uiLearnType & LEARN_TYPE_SEQUENTIAL)
	{
		for (i=0; i<m_uiNetRank-1; i++)
			for (UINT j=0; j<m_uiLayerRank[i+1]; j++)
				for (UINT k=0; k<m_uiNeuronRank[i]; k++)
					m_vWeights[GRAD2][i][j][k]=m_vErrorSignal[i][j]*m_vAxons[i][k];
		
		if (m_bUseBias)
			for (i=0; i<m_uiNetRank-1; i++)
				for (UINT j=0; j<m_uiLayerRank[i+1]; j++)
					m_vBiases[GRAD2][i][j]=m_vErrorSignal[i][j]*m_fMaxSignal;
	}
	if (m_uiLearnType & LEARN_TYPE_BATCH)
	{
		for (i=0; i<m_uiNetRank-1; i++)
			for (UINT j=0; j<m_uiLayerRank[i+1]; j++)
				for (UINT k=0; k<m_uiNeuronRank[i]; k++)
					m_vWeights[GRAD2][i][j][k]+=m_vErrorSignal[i][j]*m_vAxons[i][k];
				
		if (m_bUseBias)
			for (i=0; i<m_uiNetRank-1; i++)
				for (UINT j=0; j<m_uiLayerRank[i+1]; j++)
					m_vBiases[GRAD2][i][j]+=m_vErrorSignal[i][j]*m_fMaxSignal;
	}
//	file.close();
}

void NetBP::UpdateWeights()
{
	if (m_uiLearnType & LEARN_TYPE_MOMENTUM)
	{
		FLOAT fDelta;
		for (UINT i=0; i<m_uiNetRank-1; i++)
			for (UINT j=0; j<m_uiLayerRank[i+1]; j++)
				for (UINT k=0; k<m_uiNeuronRank[i]; k++)
				{
					fDelta=-m_fLearnRate*((1.0f-m_fMomentumParam)*m_vWeights[GRAD2][i][j][k]+
						m_fMomentumParam*m_vWeights[DELTA][i][j][k]);
					m_vWeights[DELTA][i][j][k]=fDelta;
					m_vWeights[WEIGH][i][j][k]+=fDelta;
				}
		if (m_bUseBias)
			for (i=0; i<m_uiNetRank-1; i++)
				for (UINT j=0; j<m_uiLayerRank[i+1]; j++)
				{
					fDelta=-m_fLearnRate*((1.0f-m_fMomentumParam)*m_vBiases[GRAD2][i][j]+
						m_fMomentumParam*m_vBiases[DELTA][i][j]);
					m_vBiases[DELTA][i][j]=fDelta;
					m_vBiases[WEIGH][i][j]+=fDelta;
				}
	}
	else
	{
		for (UINT i=0; i<m_uiNetRank-1; i++)
			for (UINT j=0; j<m_uiLayerRank[i+1]; j++)
				for (UINT k=0; k<m_uiNeuronRank[i]; k++)
					m_vWeights[WEIGH][i][j][k]+=-m_fLearnRate*m_vWeights[GRAD2][i][j][k];
		if (m_bUseBias)
			for (i=0; i<m_uiNetRank-1; i++)
				for (UINT j=0; j<m_uiLayerRank[i+1]; j++)
					m_vBiases[WEIGH][i][j]+=-m_fLearnRate*m_vBiases[GRAD2][i][j];
	}
}

void NetBP::UpdateWeights(const UINT uiNextIndex,const UINT uiPrevIndex)
{
	for (UINT i=0; i<m_uiNetRank-1; i++)
		for (UINT j=0; j<m_uiLayerRank[i+1]; j++)
			for (UINT k=0; k<m_uiNeuronRank[i]; k++)
				m_vWeights[uiNextIndex][i][j][k]=m_vWeights[uiPrevIndex][i][j][k]
					-m_fLearnRate*m_vWeights[GRAD2][i][j][k];
	if (m_bUseBias)
		for (i=0; i<m_uiNetRank-1; i++)
			for (UINT j=0; j<m_uiLayerRank[i+1]; j++)
				m_vBiases[uiNextIndex][i][j]=m_vBiases[uiPrevIndex][i][j]
					-m_fLearnRate*m_vBiases[GRAD2][i][j];
}

void NetBP::SetSigmoidType(const UINT uiSigmoidType)
{
	m_uiSigmoidType=uiSigmoidType;
}

/*DOUBLE*/FLOAT NetBP::SigmoidFunction(const /*DOUBLE*/FLOAT fValue)
{
	switch (m_uiSigmoidType)
	{
		case SIGMOID_TYPE_ORIGINAL:
			return (FLOAT)(-m_fScaleParam+1.0f/(1.0f+exp(-m_fSigmoidAlpha*fValue)));
		case SIGMOID_TYPE_HYPERTAN:
			return (FLOAT)(m_fScaleParam*tanh(m_fSigmoidAlpha*fValue));
	}
	return fValue;
}

/*DOUBLE*/FLOAT NetBP::DSigmoidFunction(const /*DOUBLE*/FLOAT fValue)
{
	switch (m_uiSigmoidType)
	{
	case SIGMOID_TYPE_ORIGINAL:
		return m_fSigmoidAlpha*((1.0f-m_fScaleParam)-fValue)*(m_fScaleParam+fValue);
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

void NetBP::GetAxons(const UINT uiLayer,FLOAT *fOutput)
{
	ASSERT(uiLayer<m_uiNetRank);
	CopyMemory(fOutput,m_vAxons[uiLayer],sizeof(FLOAT)*m_uiLayerRank[uiLayer]);
}

//void NetBP::ResetGradients()
//{
//	//m_bFirstEpoch=TRUE;
////	for (UINT i=0; i<2; i++)
//		for (UINT j=0; j<m_uiNetRank-1; j++)
//			for (UINT k=0; k<m_uiLayerRank[j+1]; k++)
//				ZeroMemory(m_vWeightsGrad[1][j][k],sizeof(FLOAT)*m_uiNeuronRank[j]);
//	if (m_bUseBias)
////		for (i=0; i<2; i++)
//			for (UINT j=0; j<m_uiNetRank-1; j++)
//				ZeroMemory(m_vBiasesGrad[1][j],sizeof(FLOAT)*m_uiLayerRank[j+1]);
//}

void NetBP::CopyWeights(const UINT uiDestIndex,const UINT uiSrcIndex)
{
	for (UINT i=0; i<m_uiNetRank-1; i++)
		for (UINT j=0; j<m_uiLayerRank[i+1]; j++)
			CopyMemory(m_vWeights[uiDestIndex][i][j],
				m_vWeights[uiSrcIndex][i][j],sizeof(FLOAT)*m_uiNeuronRank[i]);
	if (m_bUseBias)
		for (UINT i=0; i<m_uiNetRank-1; i++)
			CopyMemory(m_vBiases[uiDestIndex][i],
				m_vBiases[uiSrcIndex][i],sizeof(FLOAT)*m_uiLayerRank[i+1]);
}

void NetBP::ZeroWeights(const UINT uiDestIndex)
{
	for (UINT i=0; i<m_uiNetRank-1; i++)
		for (UINT j=0; j<m_uiLayerRank[i+1]; j++)
			ZeroMemory(m_vWeights[uiDestIndex][i][j],sizeof(FLOAT)*m_uiNeuronRank[i]);
	if (m_bUseBias)
		for (UINT i=0; i<m_uiNetRank-1; i++)
			ZeroMemory(m_vBiases[uiDestIndex][i],sizeof(FLOAT)*m_uiLayerRank[i+1]);
}

void NetBP::SetLearnType(const UINT uiLearnType)
{
	m_uiLearnType=uiLearnType;
}

FLOAT NetBP::GetLearnRate()
{
	return m_fLearnRate;
}

void NetBP::CopyWeights(LPVOID pDest,const UINT uiLayer)
{
	ASSERT(uiLayer<m_uiNetRank);
	CopyMemory(pDest,m_vWeights[WEIGH][uiLayer],
		m_uiNeuronRank[uiLayer]*m_uiLayerRank[uiLayer]*sizeof(FLOAT));
}
