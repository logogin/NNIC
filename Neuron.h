// Neuron.h: interface for the Neuron class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEURON_H__803BE14F_F361_4F24_A92C_2F8E6790A739__INCLUDED_)
#define AFX_NEURON_H__803BE14F_F361_4F24_A92C_2F8E6790A739__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Net;

class Neuron  
{
	friend Net;

public:
	BOOL IsBiasNeuron(void);
	void InitBiasNeuron(void);
	Neuron(const UINT uiNeuronRank);
	void SetNeuronRank(const UINT uiNeuronRank);
	UINT GetNeuronRank(void);
	//FLOAT DActiveFunction(const FLOAT fValue);
	//FLOAT ActiveFunction(const FLOAT fValue);
	Neuron();
	virtual ~Neuron();

protected:
	FLOAT m_fDelta;
	BOOL m_bIsBiasNeuron;
	FLOAT *m_vDeltas;
	FLOAT *m_vGradients;
	//std::vector/*deque*/<FLOAT> m_vWeights;
	FLOAT *m_vWeights;
	FLOAT m_fAxon;
	UINT m_uiNeuronRank;
};

#endif // !defined(AFX_NEURON_H__803BE14F_F361_4F24_A92C_2F8E6790A739__INCLUDED_)
