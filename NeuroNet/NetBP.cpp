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

NetBP::NetBP(const BYTE bNetRank,const WORD *wLayerRank):m_bNetRank(bNetRank),
	m_bUseBias(FALSE),m_wLayerRank(NULL),m_wNeuronRank(NULL),m_vAxons(NULL),
	m_bFirstEpoch(TRUE)
	, m_dwBiasMatrixSize(0)
{
	m_wLayerRank=new WORD[m_bNetRank];
	ASSERT(m_wLayerRank!=NULL);

	DWORD i,j;
	for (i=0; i<m_bNetRank; i++)
		m_wLayerRank[i]=wLayerRank[i];

	m_wNeuronRank=new WORD[m_bNetRank-1];
	ASSERT(m_wNeuronRank!=NULL);

	for (i=0; i<m_bNetRank-1; i++)
		m_wNeuronRank[i]=m_wLayerRank[i];

	for (i=0; i<BP_ARRAY_RANK; i++)
	{
		m_vWeights[i]=new FLOAT **[m_bNetRank-1];
		ASSERT(m_vWeights[i]!=NULL);
		for (j=0; j<m_bNetRank-1; j++)
		{
			//m_vWeights[i][j]=new FLOAT *[m_wLayerRank[j+1]];
			m_vWeights[i][j]=AllocateFloatMatrix(m_wLayerRank[j+1],m_wNeuronRank[j]);
			ASSERT(m_vWeights[i][j]!=NULL);
			ZeroMemory(m_vWeights[i][j][0],sizeof(FLOAT)*m_wNeuronRank[j]*m_wLayerRank[j+1]);
			//for (UINT k=0; k<m_wLayerRank[j+1]; k++)
			//{
			//	m_vWeights[i][j][k]=new FLOAT[m_wNeuronRank[j]];
			//	ASSERT(m_vWeights[i][j][k]!=NULL);
			//	ZeroMemory(m_vWeights[i][j][k],sizeof(FLOAT)*m_wNeuronRank[j]);
			//}
		}
	}

	//m_vAxons=new FLOAT *[m_bNetRank];
	DWORD *dwLayerRank=new DWORD[m_bNetRank];
	ASSERT(dwLayerRank!=NULL);
	for (i=0; i<m_bNetRank; i++)
		dwLayerRank[i]=m_wLayerRank[i];
	m_vAxons=AllocateFloatMatrix(m_bNetRank,dwLayerRank);
	ASSERT(m_vAxons!=NULL);
	
	//for (i=0; i<m_bNetRank; i++)
	//{
	//	m_vAxons[i]=new FLOAT[m_wLayerRank[i]];
	//	ASSERT(m_vAxons[i]!=NULL);
	//}

	//m_vErrorSignal=new FLOAT *[m_bNetRank-1];
	m_vErrorSignal=AllocateFloatMatrix(m_bNetRank-1,dwLayerRank+1);
	ASSERT(m_vErrorSignal!=NULL);
	delete []dwLayerRank;
	//for (i=1; i<m_bNetRank; i++)
	//{
	//	m_vErrorSignal[i-1]=new FLOAT[m_wLayerRank[i]];
	//	ASSERT(m_vErrorSignal[i-1]!=NULL);
	//}
}

NetBP::~NetBP()
{
	if (m_wLayerRank!=NULL)
		delete []m_wLayerRank;
	if (m_wNeuronRank!=NULL)
		delete []m_wNeuronRank;
	if (m_vAxons!=NULL)
		DestroyMatrix((LPVOID *)m_vAxons);
	DWORD i;
	for (i=0; i<BP_ARRAY_RANK; i++)
	{
		if (m_vWeights[i]!=NULL)
			DestroyMatrix((LPVOID *)(m_vWeights[i]));
		if (m_bUseBias&&m_vBiases[i]!=NULL)
			DestroyMatrix((LPVOID *)(m_vBiases[i]));
	}
	//if (m_wLayerRank!=NULL)
	//{

	//	for (UINT i=0; i<BP_ARRAY_RANK; i++)
	//	{
	//		for (UINT j=0; j<m_bNetRank-1; j++)
	//		{
	//			for (UINT k=0; k<m_wLayerRank[j+1]; k++)
	//				delete []m_vWeights[i][j][k];
	//			delete []m_vWeights[i][j];
	//		}
	//		delete []m_vWeights[i];
	//	}

	//	if (m_bUseBias)
	//		for (i=0; i<BP_ARRAY_RANK; i++)
	//		{
	//			for (UINT j=0; j<m_bNetRank-1; j++)
	//				delete []m_vBiases[i][j];
	//			delete []m_vBiases[i];
	//		}
	//}

	//for (UINT i=0; i<m_bNetRank-1; i++)
	//	delete []m_vErrorSignal[i];
	//delete []m_vErrorSignal;

	//for (i=0; i<m_bNetRank; i++)
	//		delete []m_vAxons[i];
	//delete []m_vAxons;
	//delete []m_wNeuronRank;
	//delete []m_wLayerRank;
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
		DWORD *dwLayerRank=new DWORD[m_bNetRank-1];
		m_dwBiasMatrixSize=0;
		for (i=0; i<m_bNetRank-1; i++)
		{
			dwLayerRank[i]=m_wLayerRank[i+1];
			m_dwBiasMatrixSize+=dwLayerRank[i];
		}
		for (i=0; i<BP_ARRAY_RANK; i++)
		{
			//m_vBiases[i]=new FLOAT *[m_bNetRank-1];
			m_vBiases[i]=AllocateFloatMatrix(m_bNetRank-1,dwLayerRank);
			ASSERT(m_vBiases[i]!=NULL);
			ZeroMemory(m_vBiases[i][0],sizeof(FLOAT)*m_dwBiasMatrixSize);
			//for (UINT j=0; j<m_bNetRank-1; j++)
			//{
			//	m_vBiases[i][j]=new FLOAT[m_wLayerRank[j+1]];
			//	ASSERT(m_vBiases[i][j]!=NULL);
			//	ZeroMemory(m_vBiases[i][j],sizeof(FLOAT)*m_wLayerRank[j+1]);
			//}
		}
		delete []dwLayerRank;
	}
}

void NetBP::InitWeights()
{
	FLOAT fRange=m_fMaxSignal-m_fMinSignal;
//	UINT r=0;
	DWORD i,j,k;
	srand((unsigned)time(NULL));
	for (i=0; i<m_bNetRank-1; i++)
		for (j=0; j<m_wLayerRank[i+1]; j++)
			for (k=0; k<m_wNeuronRank[i]; k++)
//			{
//				r++;
				m_vWeights[WEIGH][i][j][k]=
					m_fMinSignal+/*(r%100)/100.0f*/(FLOAT)rand()/RAND_MAX*fRange;
//			}
	if (m_bUseBias)
		for (i=0; i<m_bNetRank-1; i++)
			for (j=0; j<m_wLayerRank[i+1]; j++)
				m_vBiases[WEIGH][i][j]=m_fMinSignal+(FLOAT)rand()/RAND_MAX*fRange;
}

void NetBP::SetAxons(const BYTE bLayer, const FLOAT *fAxons)
{
	ASSERT(bLayer<m_bNetRank);
	CopyMemory(m_vAxons[bLayer],fAxons,sizeof(FLOAT)*m_wLayerRank[bLayer]);
}

void NetBP::ForwardPass(const BYTE bFrom)
{
	ASSERT(bFrom<m_bNetRank);

	/*DOUBLE*/FLOAT fNet;

//	ofstream file;
//	file.open(_T("debug.txt"),ios::out | ios::app);

//	file<<endl<<"PROPAGATION"<<endl<<"Layer 0"<<endl;
//	for (UINT m=0; m<m_wLayerRank[0]; m++)
//		file<<m_vAxons[0][m]<<" "<<flush;

//	file<<endl;
	DWORD i,j,k;
	for (i=bFrom+1; i<m_bNetRank; i++)
	{
//		file<<"Layer "<<i<<endl;
		for (j=0; j<m_wLayerRank[i]; j++)
		{
			fNet=0.0f;
			for (k=0; k<m_wNeuronRank[i-1]; k++)
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

FLOAT NetBP::TargetFunction(const BYTE bLayer, const FLOAT *fTarget)
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

void NetBP::BackwardPass(const FLOAT *fTarget)
{
	UpdateGradients(fTarget);
	UpdateWeights();
/*	ofstream file("debug.txt",ios::out | ios::app);

	file<<endl<<endl<<"\nWEIGHTS"<<endl;
	for (i=0; i<m_bNetRank-1; i++)
	{
		file<<"Layer "<<i+1<<endl;
		for (UINT j=0; j<m_wLayerRank[i+1]; j++)
		{
			file<<"\tNeuron "<<j<<endl;
			for (UINT k=0; k<m_wNeuronRank[i]; k++)
				file<<m_vWeights[WEIGH][i][j][k]<<" ";
			file<<endl;
		}
		file<<endl;
	}
	file.close();*/
}

FLOAT NetBP::BackwardPass(const DWORD dwPatterns,const FLOAT **fPatterns,const FLOAT fPrevNetError)
{
//	FLOAT fGradNorm=0.0;
//
//	for (UINT i=0; i<m_bNetRank-1; i++)
//		for (UINT j=0; j<m_wLayerRank[i+1]; j++)
//			for (UINT k=0; k<m_wNeuronRank[i]; k++)
//				fGradNorm+=m_vWeights[GRAD2][i][j][k]*m_vWeights[GRAD2][i][j][k];
//
//	if (m_bUseBias)
//		for (i=0; i<m_bNetRank-1; i++)
//			for (UINT j=0; j<m_wLayerRank[i+1]; j++)
//				fGradNorm+=m_vBiases[GRAD2][i][j]*m_vBiases[GRAD2][i][j];
//
//	m_fLearnRate=fPrevNetError/fGradNorm;
//
//	FLOAT fNetError;
//	
//	CopyWeights(DELTA,WEIGH);
//
//	BOOLEAN bFlag=FALSE;
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
	BOOLEAN bFlag;
	FLOAT fNetError;
	DWORD i,j,k;

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
		for (i=0; i<m_bNetRank-1; i++)
			for (j=0; j<m_wLayerRank[i+1]; j++)
				for (k=0; k<m_wNeuronRank[i]; k++)
					fGradNorm+=m_vWeights[GRAD2][i][j][k]*m_vWeights[GRAD2][i][j][k];

		if (m_bUseBias)
			for (i=0; i<m_bNetRank-1; i++)
				for (j=0; j<m_wLayerRank[i+1]; j++)
					fGradNorm+=m_vBiases[GRAD2][i][j]*m_vBiases[GRAD2][i][j];

		m_fLearnRate=fPrevNetError/fGradNorm;

		CopyWeights(DELTA,WEIGH);

		bFlag=FALSE;
		do
		{
			UpdateWeights(WEIGH,DELTA);
			fNetError=0.0;
			for (i=0; i<dwPatterns; i++)
			{
				SetAxons(NET_INPUT_LAYER,fPatterns[i]);
				ForwardPass(NET_INPUT_LAYER);
				fNetError+=TargetFunction(m_bNetRank-1,fPatterns[i]);
			}
			if ((fNetError-fPrevNetError)>(-0.5*m_fLearnRate*fGradNorm))
				m_fLearnRate/=2.0;
			else
				bFlag=TRUE;
		}
		while (!bFlag);
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
			SetAxons(NET_INPUT_LAYER,fPatterns[i]);
			ForwardPass(NET_INPUT_LAYER);
			fNetError+=TargetFunction(m_bNetRank-1,fPatterns[i]);
		}
		if ((fNetError-fPrevNetError)>(-0.5*m_fLearnRate*fGradNorm))
			m_fLearnRate/=2.0;
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
//	file<<"Layer "<<m_bNetRank-1<<endl;
	DWORD i,j,k;
	for (i=0; i<m_wLayerRank[m_bNetRank-1]; i++)
	{
		m_vErrorSignal[m_bNetRank-2][i]=(m_vAxons[m_bNetRank-1][i]-fTarget[i])*
					DSigmoidFunction(m_vAxons[m_bNetRank-1][i]);
//		file<<"delta :"<<(m_vAxons[m_bNetRank-1][i]-fTarget[i])
//			<<" dy/ds :"<<DSigmoidFunction(m_vAxons[m_bNetRank-1][i])
//			<<" error :"<<m_vErrorSignal[m_bNetRank-2][i]<<" "<<endl;
//		file<<m_vErrorSignal[m_bNetRank-2][i]<<" "<<flush;
	}
	/*DOUBLE*/FLOAT fSum;
	for (i=m_bNetRank-2; i>=1; i--)
	{
//		file<<endl<<"Layer "<<i<<endl;
		for (j=0; j<m_wLayerRank[i]; j++)
		{
			fSum=0.0f;
			for (k=0; k<m_wLayerRank[i+1]; k++)
				fSum+=m_vErrorSignal[i][k]*m_vWeights[WEIGH][i][k][j];
			m_vErrorSignal[i-1][j]=fSum*DSigmoidFunction(m_vAxons[i][j]);
//			file<<m_vErrorSignal[i-1][j]<<" "<<flush;
		}
	}

	//if (m_bLearnType & LEARN_TYPE_SEQUENTIAL)
	//{
	for (i=0; i<m_bNetRank-1; i++)
		for (j=0; j<m_wLayerRank[i+1]; j++)
			for (k=0; k<m_wNeuronRank[i]; k++)
				m_vWeights[GRAD2][i][j][k]=m_vErrorSignal[i][j]*m_vAxons[i][k];
	
	if (m_bUseBias)
		for (i=0; i<m_bNetRank-1; i++)
			for (j=0; j<m_wLayerRank[i+1]; j++)
				m_vBiases[GRAD2][i][j]=m_vErrorSignal[i][j]*m_fMaxSignal;
	//}
	//if (m_bLearnType & LEARN_TYPE_BATCH)
	//{
	//	for (i=0; i<m_bNetRank-1; i++)
	//		for (j=0; j<m_wLayerRank[i+1]; j++)
	//			for (k=0; k<m_wNeuronRank[i]; k++)
	//				m_vWeights[GRAD2][i][j][k]+=m_vErrorSignal[i][j]*m_vAxons[i][k];
	//			
	//	if (m_bUseBias)
	//		for (i=0; i<m_bNetRank-1; i++)
	//			for (UINT j=0; j<m_wLayerRank[i+1]; j++)
	//				m_vBiases[GRAD2][i][j]+=m_vErrorSignal[i][j]*m_fMaxSignal;
	//}
//	file.close();
}

void NetBP::UpdateWeights()
{
	DWORD i,j,k;
	FLOAT fDelta;
	if (m_bLearnType & LEARN_TYPE_MOMENTUM)
	{
		//FLOAT fDelta;
		for (i=0; i<m_bNetRank-1; i++)
			for (j=0; j<m_wLayerRank[i+1]; j++)
				for (k=0; k<m_wNeuronRank[i]; k++)
				{
					fDelta=-m_fLearnRate*((1.0f-m_fMomentumParam)*m_vWeights[GRAD2][i][j][k]+
						m_fMomentumParam*m_vWeights[DELTA][i][j][k]);
					m_vWeights[DELTA][i][j][k]=fDelta;
					m_vWeights[WEIGH][i][j][k]+=fDelta;
				}
		if (m_bUseBias)
			for (i=0; i<m_bNetRank-1; i++)
				for (j=0; j<m_wLayerRank[i+1]; j++)
				{
					fDelta=-m_fLearnRate*((1.0f-m_fMomentumParam)*m_vBiases[GRAD2][i][j]+
						m_fMomentumParam*m_vBiases[DELTA][i][j]);
					m_vBiases[DELTA][i][j]=fDelta;
					m_vBiases[WEIGH][i][j]+=fDelta;
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
			return (FLOAT)(-m_fScaleParam+1.0f/(1.0f+exp(-m_fSigmoidAlpha*fValue)));
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

void NetBP::GetAxons(const BYTE bLayer,FLOAT *fOutput)
{
	ASSERT(bLayer<m_bNetRank);
	CopyMemory(fOutput,m_vAxons[bLayer],sizeof(FLOAT)*m_wLayerRank[bLayer]);
}

//void NetBP::ResetGradients()
//{
//	//m_bFirstEpoch=TRUE;
////	for (UINT i=0; i<2; i++)
//		for (UINT j=0; j<m_bNetRank-1; j++)
//			for (UINT k=0; k<m_wLayerRank[j+1]; k++)
//				ZeroMemory(m_vWeightsGrad[1][j][k],sizeof(FLOAT)*m_wNeuronRank[j]);
//	if (m_bUseBias)
////		for (i=0; i<2; i++)
//			for (UINT j=0; j<m_bNetRank-1; j++)
//				ZeroMemory(m_vBiasesGrad[1][j],sizeof(FLOAT)*m_wLayerRank[j+1]);
//}

void NetBP::CopyWeights(const BYTE bDestIndex,const BYTE bSrcIndex)
{
	DWORD i;
	for (i=0; i<m_bNetRank-1; i++)
		//for (j=0; j<m_wLayerRank[i+1]; j++)
		CopyMemory(m_vWeights[bDestIndex][i][0],
			m_vWeights[bSrcIndex][i][0],sizeof(FLOAT)*m_wLayerRank[i+1]*m_wNeuronRank[i]);
	if (m_bUseBias)
		//for (UINT i=0; i<m_bNetRank-1; i++)
		CopyMemory(m_vBiases[bDestIndex][0],
			m_vBiases[bSrcIndex][0],sizeof(FLOAT)*m_dwBiasMatrixSize);
}

void NetBP::ZeroWeights(const BYTE bDestIndex)
{
	DWORD i;
	for (i=0; i<m_bNetRank-1; i++)
		//for (UINT j=0; j<m_wLayerRank[i+1]; j++)
		ZeroMemory(m_vWeights[bDestIndex][i][0],sizeof(FLOAT)*m_wLayerRank[i+1]*m_wNeuronRank[i]);
	if (m_bUseBias)
		//for (UINT i=0; i<m_bNetRank-1; i++)
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
	CopyMemory(pDest,m_vWeights[WEIGH][bLayer],
		m_wNeuronRank[bLayer]*m_wLayerRank[bLayer]*sizeof(FLOAT));
}
