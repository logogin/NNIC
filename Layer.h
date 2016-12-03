// Layer.h: interface for the Layer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LAYER_H__EBFFC544_9E7D_4401_BF1A_6DB11755812B__INCLUDED_)
#define AFX_LAYER_H__EBFFC544_9E7D_4401_BF1A_6DB11755812B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Net;
class Neuron;

class Layer  
{
	friend Net;

public:
	void AddBiasNeuron(void);
	void SetLayerRank(const UINT uiLayerRank);
	UINT GetLayerRank(void);
	Layer();
	Layer(const UINT uiLayerRank);
	virtual ~Layer();

protected:
	Neuron *m_vNeurons;
	UINT m_uiLayerRank;
	//std::vector/*deque*/<Neuron> m_vNeurons;
};

#endif // !defined(AFX_LAYER_H__EBFFC544_9E7D_4401_BF1A_6DB11755812B__INCLUDED_)
