// Layer.cpp: implementation of the Layer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NeuroNet.h"
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

Layer::Layer():m_vNeurons(NULL)
{

}

Layer::Layer(const UINT uiLayerRank):m_uiLayerRank(uiLayerRank)
{
	//std::vector/*deque*/<Neuron> vTemp(m_uiLayerRank);
	//m_vNeurons.swap(vTemp);
	m_vNeurons=new Neuron[m_uiLayerRank];
	ASSERT(m_vNeurons!=NULL);
}

Layer::~Layer()
{
	if (m_vNeurons!=NULL)
		delete []m_vNeurons;
}

UINT Layer::GetLayerRank()
{
	return m_uiLayerRank;
}

void Layer::SetLayerRank(const UINT uiLayerRank)
{
	m_uiLayerRank=uiLayerRank;
	if (m_vNeurons!=NULL)
		delete []m_vNeurons;
	m_vNeurons=new Neuron[m_uiLayerRank];
	ASSERT(m_vNeurons!=NULL);
}

void Layer::AddBiasNeuron()
{
	m_uiLayerRank++;
	Neuron *vTemp=new Neuron[m_uiLayerRank];
	ASSERT(vTemp!=NULL);

	ASSERT(memcpy(vTemp,m_vNeurons,sizeof(Neuron)*(m_uiLayerRank-1))!=NULL);
	vTemp[m_uiLayerRank-1].InitBiasNeuron();
	delete []m_vNeurons;
	
	m_vNeurons=new Neuron[m_uiLayerRank];
	ASSERT(m_vNeurons!=NULL);
	ASSERT(memcpy(m_vNeurons,vTemp,sizeof(Neuron)*m_uiLayerRank)!=NULL);
	delete []vTemp;
}
