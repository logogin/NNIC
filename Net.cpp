// Net.cpp: implementation of the Net class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NeuroNet.h"
#include "Net.h"
#include "Layer.h"
#include "Neuron.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Net::Net():m_vLayers(NULL),m_fLearnRate(1.0f),m_fScaleParam(0.0f),m_fMomentum(0.0f),
	m_fSigmoidAlpha(1.0f),m_uiSigmoidType(SIGMOID_TYPE_ORIGINAL),m_fErrorLimit(0.001f),
	vGradients(NULL),vWeights(NULL)
{
//	ofstream file;
//	file.open(_T("debug.txt"),ios::out);
//	file.close();
}

Net::Net(const UINT uiNetRank,const UINT *ptrLayerRank):m_uiNetRank(uiNetRank),m_fLearnRate(1.0f),
	m_fScaleParam(0.0f),m_fMomentum(0.0f),m_fSigmoidAlpha(1.0f),
	m_uiSigmoidType(SIGMOID_TYPE_ORIGINAL),m_fErrorLimit(0.001f),
	vGradients(NULL),vWeights(NULL)
{
//	ofstream file;
//	file.open(_T("debug.txt"),ios::out);
//	file.close();

	m_vLayers=new Layer[m_uiNetRank];
		ASSERT(m_vLayers!=NULL);
	for (UINT i=0; i<m_uiNetRank; i++)
//	{
		m_vLayers[i].SetLayerRank(ptrLayerRank[i]);		
		//Layer LayerTemp(ptrLayerRank[i]);
		//m_vLayers.push_back(LayerTemp);
//	}
	
}

Net::~Net()
{
	if (m_vLayers!=NULL)
		delete []m_vLayers;

	if (vGradients!=NULL)
		for (UINT i=0; i<m_uiNetRank-1; i++)
		{
			if (vGradients[i]!=NULL)
				delete []vGradients[i];

			if (vWeights[i]!=NULL)
				delete []vWeights[i];
		}

	if (vGradients!=NULL)
		delete []vGradients;
	if (vWeights!=NULL)
		delete []vWeights;
}

void Net::Propagate(const UINT uiFrom)
{
//	ofstream file;
//	file.open(_T("debug.txt"),ios::out | ios::app);

//	file<<endl<<"PROPAGATION"<<endl<<"Layer 0"<<endl;
//	for (UINT k=0; k<m_vLayers[0].GetLayerRank(); k++)
//		file<<GetAxon(0,k)<<" "<<flush;

//	file<<endl;
	for (UINT i=uiFrom+1; i<m_uiNetRank; i++)
	{
//		file<<"Layer "<<i<<endl;
		for (UINT j=0; j<m_vLayers[i].GetLayerRank(); j++)
		{
			FLOAT fAxon=0.0;
			for (UINT k=0; k<GetNeuronRank(i,j); k++)
				fAxon+=GetWeight(i,j,k)*GetAxon(i-1,k);
			if (!IsBiasNeuron(i,j))
				SetAxon(i,j,SigmoidFunction(fAxon));
//			file<<GetAxon(i,j)<<" "<<flush;
		}
//		file<<endl;
	}
//	file.close();
}

void Net::SetFullConnect()
{
	for (UINT i=1; i<m_uiNetRank; i++)
		for (UINT j=0; j<m_vLayers[i].GetLayerRank(); j++)
		{
			//Neuron temp=m_vLayers[i].m_vNeurons[j];
			if (!IsBiasNeuron(i,j))
				m_vLayers[i].m_vNeurons[j].SetNeuronRank(m_vLayers[i-1].GetLayerRank());
			//temp=m_vLayers[i].m_vNeurons[j];
		}
}

void Net::RandomizeWeights(const FLOAT fLeftValue, const FLOAT fRange)
{
//	ofstream file;
//	file.open(_T("debug.txt"),ios::out | ios::app);

//	file<<endl<<"RANDOMIZE"<<endl;
	UINT r=0;
	srand((unsigned)time(NULL));
	for (UINT i=1; i<m_uiNetRank; i++)
	{
//		file<<"Layer "<<i<<endl;
		for (UINT j=0; j<m_vLayers[i].GetLayerRank(); j++)
		{
//			file<<"Neuron "<<j<<endl;
			for (UINT k=0; k<GetNeuronRank(i,j); k++)
			{
				r++;
				SetWeight(i,j,k,/*fLeftValue+(FLOAT)rand()/RAND_MAX*fRange*/(r%100)/100.0-0.5f);
//				file<<m_vLayers[i].m_vNeurons[j].m_vWeights[k]<<" "<<flush;
			}
//			file<<endl;
		}
	}
//	file.close();
}

void Net::SetAxons(const UINT uiLayer, const FLOAT *ptrAxons)
{
	ASSERT(uiLayer<m_uiNetRank);
	for (UINT i=0; i<m_vLayers[uiLayer].GetLayerRank(); i++)
		//if (!IsBiasNeuron(uiLayer,i))
		SetAxon(uiLayer,i,ptrAxons[i]);
}

FLOAT Net::LearnBackPropagation(const FLOAT *ptrTarget)
{
	//std::vector/*deque*/<FLOAT> vDelta1;
	//std::vector/*deque*/<FLOAT> vDelta2;
	static UINT uiLearnCycle=0;
	FLOAT *vDelta1=NULL;
	FLOAT *vDelta2=NULL;

//	ofstream file;
//	file.open(_T("debug.txt"),ios::out | ios::app);

//	file<<endl<<"LEARN CYCLE "<<uiLearnCycle<<endl;
//	file<<endl<<"CALCULATE DELTA"<<endl;
	vDelta1=new FLOAT[m_vLayers[m_uiNetRank-1].GetLayerRank()];
	ASSERT(vDelta1!=NULL);

//	file<<"Layer "<<m_uiNetRank-1<<endl;
	for (UINT i=0; i<m_vLayers[m_uiNetRank-1].GetLayerRank(); i++)
	{
		FLOAT fAxon=GetAxon(m_uiNetRank-1,i);
		vDelta1[i]=-(ptrTarget[i]-fAxon)*DSigmoidFunction(fAxon);
//		file<<"delta :"<<(ptrTarget[i]-fAxon)<<" dy/ds :"<<DSigmoidFunction(fAxon)
//			<<" error :"<<vDelta1[i]<<" "<<endl;
//		file<<vDelta1[i]<<" "<<flush;
	}

	for (i=m_uiNetRank-2; i>=1; i--)
	{
//		file<<endl<<"Layer "<<i<<endl;
		vDelta2=new FLOAT[m_vLayers[i].GetLayerRank()];
		ASSERT(vDelta2);
/***********************???????????????????????!!!!!!!!!!!!!!!!!!**********/
		for (UINT j=0; j<m_vLayers[i].GetLayerRank(); j++)
		{
//			file<<"Neuron "<<j<<endl;
			FLOAT fDeltaSum=0.0;
			FLOAT fAxon=GetAxon(i,j);
			for (UINT k=0; k<m_vLayers[i+1].GetLayerRank(); k++)
			{
				if (!IsBiasNeuron(i+1,k))
				{
					FLOAT fWeight=GetWeight(i+1,k,j);
					fDeltaSum+=vDelta1[k]*fWeight;
					FLOAT fDelta=-m_fLearnRate*((1.0-m_fMomentum)*vDelta1[k]*fAxon+
						m_fMomentum*GetDelta(i+1,k,j));
/*????????*/		SetDelta(i+1,k,j,fDelta);
					SetWeight(i+1,k,j,fWeight+/*m_fLearnRate**/fDelta);
				}
//				file<<m_fNiu*fDelta<<" "<<flush;
			}
			vDelta2[j]=fDeltaSum*DSigmoidFunction(fAxon);
//			file<<endl;
//			file<<vDelta2[j]<<" "<<flush;
		}

//		file<<endl;
		delete []vDelta1;
		vDelta1=new FLOAT[m_vLayers[i].GetLayerRank()];
		ASSERT(vDelta1!=NULL);
		memcpy(vDelta1,vDelta2,sizeof(FLOAT)*m_vLayers[i].GetLayerRank());

		delete []vDelta2;
		//vDelta1.clear();
		//vDelta1.swap(vDelta2);
	}
	for (i=0; i<m_vLayers[1].GetLayerRank(); i++)
		for (UINT j=0; j<m_vLayers[0].GetLayerRank(); j++)
		{
			if (!IsBiasNeuron(1,i))
			{
				FLOAT fDelta=-m_fLearnRate*((1.0f-m_fMomentum)*vDelta1[i]*GetAxon(0,j)+
					m_fMomentum*GetDelta(1,i,j));
//				float f1=m_fMomentum*GetDelta(1,i,j);
//				float f2=(1.0f-m_fMomentum)*vDelta1[i]*GetAxon(0,j);
//				float f3=-m_fLearnRate*(f1+f2);
				SetDelta(1,i,j,fDelta);
				SetWeight(1,i,j,GetWeight(1,i,j)+/*m_fLearnRate**/fDelta);
			}
		}


/*	file<<endl<<"\nWEIGHTS"<<endl;
	for (i=1; i<m_uiNetRank; i++)
	{
		file<<"Layer "<<i<<endl;
		for (UINT j=0; j<m_vLayers[i].GetLayerRank(); j++)
		{
			file<<"\tNeuron "<<j<<endl;
			for (UINT k=0; k<GetNeuronRank(i,j); k++)
				file<<GetWeight(i,j,k)<<" "<<flush;
			file<<endl;
		}
		file<<endl;
	}*/

	delete []vDelta1;

	uiLearnCycle++;
	
	FLOAT fError=GetNetError(ptrTarget);
//	file<<"ERROR "<<fError<<endl;
//	file.close();
	return fError;
}

FLOAT Net::LearnBackPropagation(const FLOAT *ptrPatterns, const UINT uiPatternsNumber)
{
//	LARGE_INTEGER liFreq;
//	QueryPerformanceFrequency(&liFreq);
//	LARGE_INTEGER liCount[12];

	UINT uiM=1;
	static BOOL bFirstIteration=TRUE;
	if (bFirstIteration)
		m_fNetError=0.0f;

	if (bFirstIteration)
	{
		

		vGradients=new FLOAT *[m_uiNetRank-1];
		ASSERT(vGradients!=NULL);

		vWeights=new FLOAT *[m_uiNetRank-1];
		ASSERT(vWeights!=NULL);

		for (UINT i=1; i<m_uiNetRank; i++)
		{
			
			UINT uiSize=m_vLayers[i].m_uiLayerRank*m_vLayers[i-1].m_uiLayerRank;
			
		/*for (UINT j=0; j<m_vLayers[i].GetLayerRank(); j++)
			uiSize+=m_vLayers[i].m_vNeurons[j].GetNeuronRank();*/
//			QueryPerformanceCounter(&liCount[0]);

			vGradients[i-1]=new FLOAT[uiSize];

//			QueryPerformanceCounter(&liCount[1]);
			ASSERT(vGradients[i-1]!=NULL);

			
			ASSERT(ZeroMemory(vGradients[i-1],sizeof(FLOAT)*uiSize)!=NULL);
			

			vWeights[i-1]=new FLOAT[uiSize];
			ASSERT(vWeights[i-1]!=NULL);
			//ASSERT(ZeroMemory(vWeights[i-1],sizeof(FLOAT)*uiSize)!=NULL);
		}

		
	}

	for (UINT i=0; i<uiPatternsNumber; i++)
	{

//		QueryPerformanceCounter(&liCount[0]);
		SetAxons(0,&ptrPatterns[i*m_vLayers[0].GetLayerRank()]);
		Propagate();
//		QueryPerformanceCounter(&liCount[1]);
		

		if (bFirstIteration)
			m_fNetError+=GetNetError(&ptrPatterns[i*m_vLayers[0].m_uiLayerRank]);

		for (UINT j=0; j<m_vLayers[m_uiNetRank-1].GetLayerRank(); j++)
		{
			FLOAT fAxon=GetAxon(m_uiNetRank-1,j);
			FLOAT fDelta=-(ptrPatterns[i*m_vLayers[0].m_uiLayerRank+j]-
				fAxon)*DSigmoidFunction(fAxon);
			m_vLayers[m_uiNetRank-1].m_vNeurons[j].m_fDelta=fDelta;
		}

		for (j=m_uiNetRank-2; j>=1; j--)
			for (UINT k=0; k<m_vLayers[j].GetLayerRank(); k++)
			{
				FLOAT fDeltaSum=0.0;
				FLOAT fAxon=GetAxon(j,k);
				for (UINT m=0; m<m_vLayers[j+1].GetLayerRank(); m++)
				{
					FLOAT fWeight=GetWeight(j+1,m,k);
					fDeltaSum+=m_vLayers[j+1].m_vNeurons[m].m_fDelta*fWeight;
				}
				m_vLayers[j].m_vNeurons[k].m_fDelta=fDeltaSum*DSigmoidFunction(fAxon);
			}

		for (j=1; j<m_uiNetRank; j++)
		{
			UINT uiIndex=0;
			for (UINT k=0; k<m_vLayers[j].GetLayerRank(); k++)
			{
				FLOAT fDelta=m_vLayers[j].m_vNeurons[k].m_fDelta;
				UINT uiNeuronRank=m_vLayers[j].m_vNeurons[k].m_uiNeuronRank;
				for (UINT m=0; m<uiNeuronRank; m++)
					vGradients[j-1][uiIndex++]+=GetAxon(j-1,m)*fDelta;
			}
		}
		
	}

	if (bFirstIteration)
		bFirstIteration=FALSE;

//	QueryPerformanceCounter(&liCount[4]);
	for (i=1; i<m_uiNetRank; i++)
	{
		UINT uiIndex=0;
		for (UINT j=0; j<m_vLayers[i].GetLayerRank(); j++)
		{
			ASSERT(CopyMemory(&vWeights[i-1][uiIndex],
					m_vLayers[i].m_vNeurons[j].m_vWeights,
					sizeof(FLOAT)*m_vLayers[i].m_vNeurons[j].m_uiNeuronRank)!=NULL);
			uiIndex+=m_vLayers[i].m_vNeurons[j].m_uiNeuronRank;
		}
	}

//	QueryPerformanceCounter(&liCount[5]);

	FLOAT fGradientNorm=0.0;

//	QueryPerformanceCounter(&liCount[6]);
	for (i=1; i<m_uiNetRank; i++)
		for (UINT j=0; j<m_vLayers[i].GetLayerRank(); j++)
			for (UINT k=0; k<m_vLayers[i].m_vNeurons[j].GetNeuronRank(); k++)
			{
				FLOAT fGradient=vGradients[i-1][j*m_vLayers[i-1].m_uiLayerRank+k];
				fGradientNorm+=fGradient*fGradient;
			}

//	QueryPerformanceCounter(&liCount[7]);

	m_fLearnRate=/*0.9f;//*/m_fNetError/fGradientNorm;

	BOOL bFlag=FALSE;
	do
	{
//		QueryPerformanceCounter(&liCount[8]);
		FLOAT fNetError=0.0f;
		for (i=1; i<m_uiNetRank; i++)
			for (UINT j=0; j<m_vLayers[i].GetLayerRank(); j++)
				for (UINT k=0; k<m_vLayers[i].m_vNeurons[j].GetNeuronRank(); k++)
					SetWeight(i,j,k,vWeights[i-1][j*m_vLayers[i-1].m_uiLayerRank+k]/*+*/-
						m_fLearnRate*vGradients[i-1][j*m_vLayers[i-1].m_uiLayerRank+k]);
//		QueryPerformanceCounter(&liCount[9]);	

		for (i=0; i<uiPatternsNumber; i++)
		{

///			QueryPerformanceCounter(&liCount[10]);

			SetAxons(0,&ptrPatterns[i*m_vLayers[0].m_uiLayerRank]);
			Propagate();

			fNetError+=GetNetError(&ptrPatterns[i*m_vLayers[0].m_uiLayerRank]);	
//			QueryPerformanceCounter(&liCount[11]);
		}

		if ((fNetError-m_fNetError)>(-0.5*m_fLearnRate*fGradientNorm))
		{
			uiM*=2;
			m_fLearnRate/=uiM;
		}
		else
		{
			bFlag=TRUE;
			m_fNetError=fNetError;
		}
	}
	while(!bFlag);
//	FLOAT counts[6];
//	for (i=0; i<6; i++)
//		counts[i]=(liCount[i*2+1].QuadPart-liCount[i*2].QuadPart)/(float)liFreq.QuadPart;

	for (i=1; i<m_uiNetRank; i++)
		ASSERT(ZeroMemory(vGradients[i-1],
			sizeof(FLOAT)*m_vLayers[i].m_uiLayerRank*m_vLayers[i-1].m_uiLayerRank)!=NULL);

	return m_fNetError;
}

Neuron &Net::GetNeuron(const UINT uiLayer, const UINT uiNeuron)
{
	ASSERT(uiLayer<m_uiNetRank);
	ASSERT(uiNeuron<m_vLayers[uiLayer].GetLayerRank());
	return m_vLayers[uiLayer].m_vNeurons[uiNeuron];
}

FLOAT Net::GetWeight(const UINT uiLayer, const UINT uiNeuron, const UINT uiWeight)
{
	ASSERT(uiLayer<m_uiNetRank);
	ASSERT(uiNeuron<m_vLayers[uiLayer].m_uiLayerRank);
	ASSERT(uiWeight<m_vLayers[uiLayer].m_vNeurons[uiNeuron].m_uiNeuronRank);
	return m_vLayers[uiLayer].m_vNeurons[uiNeuron].m_vWeights[uiWeight];
}

void Net::SetWeight(const UINT uiLayer, const UINT uiNeuron, const UINT uiWeight,const FLOAT fWeight)
{
	ASSERT(uiLayer<m_uiNetRank);
	ASSERT(uiNeuron<m_vLayers[uiLayer].m_uiLayerRank);
	ASSERT(uiWeight<m_vLayers[uiLayer].m_vNeurons[uiNeuron].m_uiNeuronRank);
/*	Layer temp1=m_vLayers[uiLayer];
	Neuron temp2=m_vLayers[uiLayer].m_vNeurons[uiNeuron];
	FLOAT temp3=m_vLayers[uiLayer].m_vNeurons[uiNeuron].m_vWeights[uiWeight];*/
	m_vLayers[uiLayer].m_vNeurons[uiNeuron].m_vWeights[uiWeight]=fWeight;
}

FLOAT Net::GetAxon(const UINT uiLayer, const UINT uiNeuron)
{
	ASSERT(uiLayer<m_uiNetRank);
	ASSERT(uiNeuron<m_vLayers[uiLayer].m_uiLayerRank);
	return m_vLayers[uiLayer].m_vNeurons[uiNeuron].m_fAxon;
}

void Net::SetAxon(const UINT uiLayer, const UINT uiNeuron, const FLOAT fAxon)
{
	ASSERT(uiLayer<m_uiNetRank);
	ASSERT(uiNeuron<m_vLayers[uiLayer].m_uiLayerRank);
	if (!IsBiasNeuron(uiLayer,uiNeuron))
		m_vLayers[uiLayer].m_vNeurons[uiNeuron].m_fAxon=fAxon;
}

UINT Net::GetNeuronRank(const UINT uiLayer, const UINT uiNeuron)
{
	ASSERT(uiLayer<m_uiNetRank);
	ASSERT(uiNeuron<m_vLayers[uiLayer].m_uiLayerRank);
	//Layer temp1=m_vLayers[uiLayer];
	//Neuron temp2=m_vLayers[uiLayer].m_vNeurons[uiNeuron];
	return m_vLayers[uiLayer].m_vNeurons[uiNeuron].m_uiNeuronRank;
}

FLOAT Net::DSigmoidFunction(const FLOAT fValue)
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

FLOAT Net::SigmoidFunction(const FLOAT fValue)
{
	switch (m_uiSigmoidType)
	{
		case SIGMOID_TYPE_ORIGINAL:
			return -m_fScaleParam+1.0f/(1.0f+exp(-m_fSigmoidAlpha*fValue));
		case SIGMOID_TYPE_HYPERTAN:
			return m_fScaleParam*tanh(m_fSigmoidAlpha*fValue);
	}
	return fValue;
}

void Net::SetLearnRate(const FLOAT fLearnRate)
{
	m_fLearnRate=fLearnRate;
}

void Net::SetSigmoidAlpha(const FLOAT fSigmoidAlpha)
{
	m_fSigmoidAlpha=fSigmoidAlpha;
}

void Net::SaveWeightsToFile(const CString &sFileName)
{
	CFile file;
	file.Open(sFileName,CFile::modeCreate | CFile::modeWrite);
	for (UINT i=1; i<m_uiNetRank; i++)
		for (UINT j=0; j<m_vLayers[i].GetLayerRank(); j++)
			for (UINT k=0; k<GetNeuronRank(i,j); k++)
			{
				FLOAT fWeight=GetWeight(i,j,k);
				file.Write(&fWeight,sizeof(FLOAT));
			}
	file.Close();
}

FLOAT Net::GetNetError(const FLOAT *ptrTarget)
{
	FLOAT fSum=0.0;
	for (UINT i=0; i<m_vLayers[m_uiNetRank-1].GetLayerRank(); i++)
	{
		FLOAT fError=ptrTarget[i]-GetAxon(m_uiNetRank-1,i);
		fSum+=fError*fError;
	}
	return fSum/2;
}

void Net::SetMomentum(const FLOAT fMomentum)
{
	m_fMomentum=fMomentum;
}

FLOAT Net::GetDelta(const UINT uiLayer, const UINT uiNeuron,const UINT uiDelta)
{
	ASSERT(uiLayer<m_uiNetRank);
	ASSERT(uiNeuron<m_vLayers[uiLayer].m_uiLayerRank);
	ASSERT(uiDelta<m_vLayers[uiLayer].m_vNeurons[uiNeuron].m_uiNeuronRank);
	return m_vLayers[uiLayer].m_vNeurons[uiNeuron].m_vDeltas[uiDelta];
}

void Net::SetDelta(const UINT uiLayer, const UINT uiNeuron,const UINT uiDelta, const FLOAT fDelta)
{
	ASSERT(uiLayer<m_uiNetRank);
	ASSERT(uiNeuron<m_vLayers[uiLayer].m_uiLayerRank);
	ASSERT(uiDelta<m_vLayers[uiLayer].m_vNeurons[uiNeuron].m_uiNeuronRank);
	m_vLayers[uiLayer].m_vNeurons[uiNeuron].m_vDeltas[uiDelta]=fDelta;
}

void Net::AddBiasNeuron()
{
	for (UINT i=0; i<m_uiNetRank-1; i++)
		m_vLayers[i].AddBiasNeuron();
}

BOOL Net::IsBiasNeuron(const UINT uiLayer, const UINT uiNeuron)
{
	ASSERT(uiLayer<m_uiNetRank);
	ASSERT(uiNeuron<m_vLayers[uiLayer].m_uiLayerRank);
	return m_vLayers[uiLayer].m_vNeurons[uiNeuron].m_bIsBiasNeuron;
}

void Net::SetScaleParam(const FLOAT fScaleParam)
{
	m_fScaleParam=fScaleParam;
}

void Net::SetSigmoidType(const UINT uiSigmoidType)
{
	m_uiSigmoidType=uiSigmoidType;
}

void Net::SetErrorLimit(const FLOAT fErrorLimit)
{
	m_fErrorLimit=fErrorLimit;
}

void Net::LoadWeightsFromFile(const CString &sFileName)
{
	CFile file;
	file.Open(sFileName,CFile::modeRead);
	for (UINT i=1; i<m_uiNetRank; i++)
		for (UINT j=0; j<m_vLayers[i].GetLayerRank(); j++)
			for (UINT k=0; k<GetNeuronRank(i,j); k++)
			{
				FLOAT fWeight;
				file.Read(&fWeight,sizeof(FLOAT));
				SetWeight(i,j,k,fWeight);
			}
	file.Close();
}

void Net::GetAxons(const UINT uiLayer, FLOAT *ptrAxons)
{
	ASSERT(uiLayer<m_uiNetRank);
	UINT j=0;
	for (UINT i=0; i<m_vLayers[uiLayer].GetLayerRank(); i++)
		if (!IsBiasNeuron(uiLayer,i))
		{
			ptrAxons[j]=GetAxon(uiLayer,i);
			j++;
		}
}