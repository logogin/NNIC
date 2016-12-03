// Net.h: interface for the Net class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NET_H__A5AA5227_8ECC_4AC0_904B_962DE3FD8311__INCLUDED_)
#define AFX_NET_H__A5AA5227_8ECC_4AC0_904B_962DE3FD8311__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define SIGMOID_TYPE_ORIGINAL 1
#define SIGMOID_TYPE_HYPERTAN 2

class Layer;
class Neuron;

class Net  
{
public:
	FLOAT LearnBackPropagation(const FLOAT *ptrPatterns,const UINT uiPatternsNumber);
	void GetAxons(const UINT uiLayer, FLOAT *ptrAxons);
	void LoadWeightsFromFile(const CString &sFileName);
	void SetErrorLimit(const FLOAT fErrorLimit);
	void SetSigmoidType(const UINT uiSigmoidType);
	void SetScaleParam(const FLOAT fScaleParam);
	BOOL IsBiasNeuron(const UINT uiLayer,const UINT uiNeuron);
	void AddBiasNeuron(void);
	void SetDelta(const UINT uiLayer,const UINT uiNeuron,const UINT uiDelta,const FLOAT fDelta);
	FLOAT GetDelta(const UINT uiLayer,const UINT uiNeuron,const UINT uiDelta);
	void SetMomentum(const FLOAT fMomentum);
	FLOAT GetNetError(const FLOAT *ptrTarget);
	void SaveWeightsToFile(const CString &sFileName);
	void SetSigmoidAlpha(const FLOAT fAlphaParam);
	void SetLearnRate(const FLOAT fLearnRate);
	FLOAT SigmoidFunction(const FLOAT fValue);
	FLOAT DSigmoidFunction(const FLOAT fValue);
	UINT GetNeuronRank(const UINT uiLayer,const UINT uiNeuron);
	void SetAxon(const UINT uiLayer,const UINT uiNeuron,const FLOAT fAxon);
	FLOAT GetAxon(const UINT uiLayer,const UINT uiNeuron);
	void SetWeight(const UINT uiLayer,const UINT uiNeuron,const UINT uiWeight,const FLOAT fWeight);
	FLOAT GetWeight(const UINT uiLayer,const UINT uiNeuron,const UINT uiWeight);
	Neuron &GetNeuron(const UINT uiLayer,const UINT uiNeuron);
	FLOAT LearnBackPropagation(const FLOAT *ptrTarget);
	void SetAxons(const UINT uiLayer,const FLOAT *ptrAxons);
	void RandomizeWeights(const FLOAT fLeftValue,const FLOAT fRange);
	void SetFullConnect(void);
	void Propagate(const UINT uiFrom=0);
	void SetLayerType(const UINT uiLayer,const UINT uiLayerType);
	Net();
	Net(const UINT uiNetRank,const UINT *ptrLayerRanks);
	virtual ~Net();

protected:
	FLOAT ** vWeights;
	FLOAT ** vGradients;
	FLOAT m_fNetError;
	FLOAT m_fErrorLimit;
	UINT m_uiSigmoidType;
	FLOAT m_fMomentum;
	Layer *m_vLayers;
	FLOAT m_fSigmoidAlpha;
	FLOAT m_fLearnRate;
	FLOAT m_fScaleParam;
	UINT m_uiNetRank;
	//std::vector/*deque*/<Layer> m_vLayers;
};

#endif // !defined(AFX_NET_H__A5AA5227_8ECC_4AC0_904B_962DE3FD8311__INCLUDED_)
