// NetCP.h: interface for the NetCP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NETCP_H__F015352A_097D_45AD_BD96_EA31A10F0FEB__INCLUDED_)
#define AFX_NETCP_H__F015352A_097D_45AD_BD96_EA31A10F0FEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum Layers {CP_INPUT_LAYER,KOHONEN_LAYER,GROSSBERG_LAYER};
enum Neurons {KOHONEN_NEURON,GROSSBERG_NEURON};

#define CP_MAX_NETSIZE 3
#define NET_TYPE_KOHONEN_GROSSBERG 1
#define NET_TYPE_KOHONEN 2

#define MAX_COLOR_VALUE 256

class NetCP  
{
public:
	void CalculateLearnRate(PFLOAT pLearnRate);
	void SetLearnRate(const FLOAT fLearnRate);
	void FinalizeGrossberg(void);
	void SetWinnerNeuron(const WORD wWinner);
	void SetAxons(const BYTE bLayer, const PFLOAT vAxons,const BOOLEAN bNormalize=FALSE);
	void SetAxons(const BYTE bLayer, const LPBYTE vAxons,const BOOLEAN bNormalize=FALSE);
	FLOAT GetWinnerDistance(void);
	void SetNeighbouringRadius(const WORD wNeighRadius);
	void LearnKohonen(const PFLOAT pLearnRate);
	void LearnGrossberg(const LPBYTE bTarget);
	FLOAT GetWeight(const BYTE bLayer,const WORD wNeuron,const WORD wWeight);
	UINT GetWinnerNeuron(void);
	void PropagateGrossberg(void);
	void PropagateKohonenMin(void);
	void PropagateKohonenMax(void);
	void NormalizeVector(PFLOAT vVector,const DWORD dwVectorSize);
	void InitWeights(void);
	void InitWeights(const FLOAT fMinValue,const FLOAT fRange);
	NetCP(const PWORD pRanks,const BYTE bNetType);
	virtual ~NetCP();

protected:
	BYTE m_bNetRank;
	WORD m_wWinnerNeuron;
	FLOAT m_fLearnRate;
	WORD m_wNeighRadius;
	WORD m_vLayerRank[CP_MAX_NETSIZE];
	WORD m_vNeuronRank[CP_MAX_NETSIZE-1];
	PFLOAT m_vAxons[CP_MAX_NETSIZE];
	PFLOAT m_vWeights[CP_MAX_NETSIZE-1];
	PDWORD m_vCounts;
	BYTE m_bNetType;
	BYTE m_bLearnType;
public:
	void GetWeightsToNeuron(const BYTE bLayer, const WORD wNeuron , PFLOAT pWeights);
	void GetWeightsFromNeuron(const BYTE bLayer, const WORD wNeuron, PFLOAT pWeights);
};

#endif // !defined(AFX_NETCP_H__F015352A_097D_45AD_BD96_EA31A10F0FEB__INCLUDED_)
