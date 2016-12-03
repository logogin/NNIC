// NetBP.h: interface for the NetBP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NETBP_H__752BE5F1_2382_4ABE_9B12_F27789C632EF__INCLUDED_)
#define AFX_NETBP_H__752BE5F1_2382_4ABE_9B12_F27789C632EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define SIGMOID_TYPE_ORIGINAL 1
#define SIGMOID_TYPE_HYPERTAN 2

#define ARRAY_SIZE 4
#define WEIGH 0
#define DELTA 1
#define GRAD1 2
#define GRAD2 3 

#define LEARN_TYPE_SEQUENTIAL 1
#define LEARN_TYPE_BATCH 2
#define LEARN_TYPE_MOMENTUM 4
class NetBP  
{
public:
	FLOAT GetLearnRate(void);
	void UpdateGradients(const FLOAT *fTarget);
	void SetLearnType(const UINT uiLearnType);
	FLOAT BackwardPass(const UINT uiPatterns,const FLOAT **fPatterns,const FLOAT fPrevNetError);
	void GetAxons(const UINT uiLayer,FLOAT *fOutput);
	void SetScaleParam(const FLOAT fScaleParam);
	void SetSigmoidAlpha(const FLOAT fSigmoidAlpha);
	void SetMomentumParam(const FLOAT fMomentumParam);
	void SetLearnRate(const FLOAT fLearnRate);
	/*DOUBLE*/FLOAT DSigmoidFunction(const /*DOUBLE*/FLOAT fValue);
	/*DOUBLE*/FLOAT SigmoidFunction(const /*DOUBLE*/FLOAT fValue);
	void SetSigmoidType(const UINT uiSigmoidType);
	void BackwardPass(const FLOAT *fTarget);
	/*DOUBLE*/FLOAT TargetFunction(const UINT uiLayer,const FLOAT *fTarget);
	void ForwardPass(const UINT uiFrom);
	void SetAxons(const UINT uiLayer,const FLOAT *fAxons);
	void InitWeights(void);
	void UseBiases(const BOOL bFlag);
	void SetSignalBoundaries(const FLOAT fMinSignal,const FLOAT fMaxSignal);
	NetBP(const UINT uiNetRank,const UINT uiLayerRank[]);
	virtual ~NetBP();

protected:
	void ZeroWeights(const UINT uiDestIndex);
	void CopyWeights(const UINT uiDestIndex,const UINT uiSrcIndex);
	void UpdateWeights(void);
	void UpdateWeights(const UINT uiNextIndex,const UINT uiPrevIndex);
	UINT m_uiNetRank;
	UINT *m_uiLayerRank;
	UINT *m_uiNeuronRank;
	FLOAT **m_vAxons;
	FLOAT ***m_vWeights[ARRAY_SIZE];
	FLOAT **m_vBiases[ARRAY_SIZE];
	FLOAT **m_vErrorSignal;
	BOOL m_bUseBias;
	FLOAT m_fMinSignal;
	FLOAT m_fMaxSignal;
	FLOAT m_fLearnRate;
	FLOAT m_fMomentumParam;
	FLOAT m_fScaleParam;
	UINT m_uiSigmoidType;
	UINT m_uiLearnType;
	FLOAT m_fSigmoidAlpha;
	BOOL m_bFirstEpoch;
};

#endif // !defined(AFX_NETBP_H__752BE5F1_2382_4ABE_9B12_F27789C632EF__INCLUDED_)
