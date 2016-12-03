// Neuron.cpp: implementation of the Neuron class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NeuroNet.h"
#include "Neuron.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Neuron::Neuron():m_uiNeuronRank(0),m_vWeights(NULL),m_vDeltas(NULL),m_vGradients(NULL),
	m_bIsBiasNeuron(FALSE)
{

}

Neuron::Neuron(const UINT uiNeuronRank):m_uiNeuronRank(uiNeuronRank),m_bIsBiasNeuron(FALSE)
{
	//std::vector/*deque*/<FLOAT> vTemp(m_uiNeuronRank);
	//m_vWeights.swap(vTemp);
	m_vWeights=new FLOAT[m_uiNeuronRank];
	ASSERT(m_vWeights!=NULL);

	m_vDeltas=new FLOAT[m_uiNeuronRank];
	ASSERT(m_vDeltas!=NULL);
	ASSERT(memset(m_vDeltas,(UINT)0.0f,sizeof(FLOAT)*m_uiNeuronRank)!=NULL);

	m_vGradients=new FLOAT[m_uiNeuronRank];
	ASSERT(m_vGradients!=NULL);
	ASSERT(memset(m_vGradients,(UINT)0.0f,sizeof(FLOAT)*m_uiNeuronRank)!=NULL);
}

Neuron::~Neuron()
{
	if (m_vWeights!=NULL)
		delete []m_vWeights;
	if (m_vDeltas!=NULL)
		delete []m_vDeltas;
	if (m_vGradients!=NULL)
		delete []m_vGradients;
}

/*FLOAT Neuron::ActiveFunction(const FLOAT fValue)
{
	switch (m_uiActiveType)
	{
	case ACTIVE_SIGMOID:
		return -0.5+1.0/(1.0-exp(-m_fAlpha*fValue));
		break;
	}
	return fValue;
}*/

/*FLOAT Neuron::DActiveFunction(const FLOAT fValue)
{
	switch (m_uiActiveType)
	{
	case ACTIVE_SIGMOID:
		return m_fAlpha(1.0-fValue)*fValue;
	}
	return fValue;
}*/

UINT Neuron::GetNeuronRank()
{
	return m_uiNeuronRank;
}

void Neuron::SetNeuronRank(const UINT uiNeuronRank)
{
	m_uiNeuronRank=uiNeuronRank;
	//std::vector/*deque*/<FLOAT> vTemp(m_uiNeuronRank);
	//m_vWeights.swap(vTemp);
	if (m_vWeights!=NULL)
		delete []m_vWeights;
	if (m_vDeltas!=NULL)
		delete []m_vDeltas;
	if (m_vGradients!=NULL)
		delete []m_vGradients;

	m_vWeights=new FLOAT[m_uiNeuronRank];
	ASSERT(m_vWeights!=NULL);

	m_vDeltas=new FLOAT[m_uiNeuronRank];
	ASSERT(m_vDeltas!=NULL);
	ASSERT(memset(m_vDeltas,(UINT)0.0f,sizeof(FLOAT)*m_uiNeuronRank)!=NULL);

	m_vGradients=new FLOAT[m_uiNeuronRank];
	ASSERT(m_vGradients!=NULL);
	ASSERT(memset(m_vGradients,(UINT)0.0f,sizeof(FLOAT)*m_uiNeuronRank)!=NULL);
}

void Neuron::InitBiasNeuron()
{
	m_fAxon=0.5f;
	m_bIsBiasNeuron=TRUE;
}

BOOL Neuron::IsBiasNeuron()
{
	return m_bIsBiasNeuron;
}
