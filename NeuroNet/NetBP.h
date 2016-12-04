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

#define BP_INPUT_LAYER 0

#define BP_ARRAY_RANK 4
#define WEIGH 0
#define DELTA 1
#define GRAD1 2
#define GRAD2 3 

#define MIT 100

#define LEARN_TYPE_SEQUENTIAL 1
#define LEARN_TYPE_BATCH 2
#define LEARN_TYPE_MOMENTUM 4
class NetBP  
{
public:
	void CopyWeights(LPVOID pDest,const BYTE bLayer);
	FLOAT GetLearnRate(void);
	void UpdateGradients(const PFLOAT fTarget);
	void SetLearnType(const BYTE bLearnType);
	FLOAT BackwardPass(const DWORD dwPatterns,const PFLOAT *fPatterns,const FLOAT fPrevNetError);
	void GetAxons(const BYTE bLayer,PFLOAT fOutput);
	void SetScaleParam(const FLOAT fScaleParam);
	void SetSigmoidAlpha(const FLOAT fSigmoidAlpha);
	void SetMomentumParam(const FLOAT fMomentumParam);
	void SetLearnRate(const FLOAT fLearnRate);
	void SetSigmoidType(const BYTE bSigmoidType);
	void BackwardPass(const PFLOAT fTarget);
	FLOAT TargetFunction(const BYTE bLayer,const PFLOAT fTarget);
	void ForwardPass(const BYTE bFrom);
	void SetAxons(const BYTE bLayer,const PFLOAT fAxons);
	void InitWeights(void);
	void InitWeights(const BYTE bLayer, const LPVOID pWeights);
	void UseBiases(const BOOLEAN bFlag);
	void SetSignalBoundaries(const FLOAT fMinSignal,const FLOAT fMaxSignal);
	NetBP(const BYTE bNetRank,const PWORD wLayerRank);
	virtual ~NetBP();
protected:
	FLOAT DSigmoidFunction(const FLOAT fValue);
	FLOAT SigmoidFunction(const FLOAT fValue);
	void ZeroWeights(const BYTE bDestIndex);
	void CopyWeights(const BYTE bDestIndex,const BYTE bSrcIndex);
	void UpdateWeights(void);
	void UpdateWeights(const BYTE bNextIndex,const BYTE bPrevIndex);
	BYTE m_bNetRank;
	PWORD m_wLayerRank;
	PWORD m_wNeuronRank;
	PFLOAT *m_vAxons;
	PFLOAT **m_vWeights[BP_ARRAY_RANK];
	PFLOAT *m_vBiases[BP_ARRAY_RANK];
	PFLOAT *m_vErrorSignal;
	BOOLEAN m_bUseBias;
	FLOAT m_fMinSignal;
	FLOAT m_fMaxSignal;
	FLOAT m_fLearnRate;
	FLOAT m_fMomentumParam;
	FLOAT m_fScaleParam;
	BYTE m_bSigmoidType;
	BYTE m_bLearnType;
	FLOAT m_fSigmoidAlpha;
	BOOLEAN m_bFirstEpoch;
	DWORD m_dwBiasMatrixSize;
public:
	DWORD GetWeightsSize(const BYTE bLayer);
};

#endif // !defined(AFX_NETBP_H__752BE5F1_2382_4ABE_9B12_F27789C632EF__INCLUDED_)
