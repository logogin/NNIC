// NetCP.h: interface for the NetCP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NETCP_H__F015352A_097D_45AD_BD96_EA31A10F0FEB__INCLUDED_)
#define AFX_NETCP_H__F015352A_097D_45AD_BD96_EA31A10F0FEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#define NETRANK 3
enum Layers {INPUT_LAYER,KOHONEN_LAYER,GROSSBERG_LAYER};
enum Neurons {KOHONEN_NEURON,GROSSBERG_NEURON};

#define MAX_NET_SIZE 3
#define NET_TYPE_KOHONEN_GROSSBERG 1
#define NET_TYPE_KOHONEN 2

#define MAX_COLOR_VALUE 256

class NetCP  
{
public:
	void CalculateLearnRate(FLOAT *pLearnRate);
	void SetLearnRate(const FLOAT fLearnRate);
	void FinalizeGrossberg(void);
	void SetWinnerNeuron(const WORD wWinner);
	void SetAxons(const BYTE bLayer, const FLOAT *vAxons,const BOOLEAN bNormalize=FALSE);
	void SetAxons(const BYTE bLayer, const BYTE *vAxons,const BOOLEAN bNormalize=FALSE);
	FLOAT GetWinnerDistance(void);
	void SetNeighbouringRadius(const WORD wNeighRadius);
//	void LearnKohonen(const FLOAT fLearnRate);
	void LearnKohonen(const FLOAT *pLearnRate);
	void LearnGrossberg(const BYTE *bTarget);
	FLOAT GetWeight(const BYTE bLayer,const WORD wNeuron,const WORD wWeight);
	UINT GetWinnerNeuron(void);
	void PropagateGrossberg(void);
	void PropagateKohonenMin(void);
	void PropagateKohonenMax(void);
	void NormalizeVector(FLOAT *vVector,const DWORD dwVectorSize);
	void InitWeights(void);
	void InitWeights(const FLOAT fMinValue,const FLOAT fRange);
	NetCP(const WORD *pRanks,const BYTE bNetType);
	virtual ~NetCP();

protected:
	BYTE m_bNetRank;
	WORD m_wWinnerNeuron;
	FLOAT m_fLearnRate;
	WORD m_wNeighRadius;
	WORD m_vLayerRank[MAX_NET_SIZE];
	WORD m_vNeuronRank[MAX_NET_SIZE-1];
	FLOAT *m_vAxons[MAX_NET_SIZE];
	FLOAT **m_vWeights[MAX_NET_SIZE-1];
	DWORD *m_vCounts;
	BYTE m_bNetType;
	BYTE m_bLearnType;
};

#endif // !defined(AFX_NETCP_H__F015352A_097D_45AD_BD96_EA31A10F0FEB__INCLUDED_)
