// NetCP.h: interface for the NetCP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NETCP_H__F015352A_097D_45AD_BD96_EA31A10F0FEB__INCLUDED_)
#define AFX_NETCP_H__F015352A_097D_45AD_BD96_EA31A10F0FEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define NETRANK 3
enum Layers {INPUT_LAYER,KOHONEN_LAYER,GROSSBERG_LAYER};
enum Neurons {KOHONEN_NEURON,GROSSBERG_NEURON};

const INT bQuadrants[8][3]={{1,1,1},{-1,1,1},{1,-1,1},{1,1,-1},
								{-1,-1,1},{-1,1,-1},{1,-1,-1},{-1,-1,-1}};
class NetCP  
{
public:
	FLOAT GetTargetDistance(const FLOAT *fTarget);
	FLOAT GetWinnerDistance(void);
	void SetNeighborhoodSize(const UINT uiNeighborhoodSize);
	void LearnKohonen(const FLOAT fLearnRate);
	FLOAT GetWeight(const UINT uiLayer,const UINT uiNeuron,const UINT uiWeight);
	UINT GetWinnerNeuron(void);
	
	void LearnGrossberg(const FLOAT fLearnRate,const FLOAT *fTarget);
	void PropagateGrossberg(void);
	void PropagateKohonen(void);
	void SetAxons(const UINT uiLayer, const FLOAT *vAxons);
	void NormalizeVector(FLOAT *vVector,const UINT uiVectorSize);
	void InitWeights(const FLOAT fMinValue,const FLOAT fRange);
	NetCP(const UINT uiRanks[NETRANK]);
	virtual ~NetCP();

protected:
	UINT m_uiWinnerNeuron;
	UINT m_uiNeighborhoodSize;
	BOOL MultMatrixVector(const FLOAT *vMatrix,
		const CSize &MatrixSize,
		const FLOAT *vVector,
		const UINT uiVectorSize,
		FLOAT *vResult,
		UINT *uiResultSize);

	UINT m_uiLayerRank[NETRANK];
	UINT m_uiNeuronRank[NETRANK-1];
	FLOAT *m_vAxons[NETRANK];
	FLOAT **m_vWeights[NETRANK-1];
};

#endif // !defined(AFX_NETCP_H__F015352A_097D_45AD_BD96_EA31A10F0FEB__INCLUDED_)
