// DCT.h: interface for the DCT class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCT_H__5C2C8F3D_5C97_4882_8221_4489BC0EE718__INCLUDED_)
#define AFX_DCT_H__5C2C8F3D_5C97_4882_8221_4489BC0EE718__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define DCT_SIZE 8
#define DCT_ARRAY_RANK 5

#define CTM 0
#define TCTM 1
#define RSLT 2
#define TEMP 3
#define QUANT 4

#define M_PI_2 1.57079632679489661923
#define M_SQRT2 1.41421356237309504880

const BYTE ZigZag[DCT_SIZE][DCT_SIZE]=
	{{0,1,5,6,14,15,27,28},
	{2,4,7,13,16,26,29,42},
	{3,8,12,17,25,30,41,43},
	{9,11,18,24,31,40,44,53},
	{10,19,23,32,39,45,52,54},
	{20,22,33,38,46,51,55,60},
	{21,34,37,47,50,56,59,61},
	{35,36,48,49,57,58,62,63}};

class DCT  
{
public:
	void ZigZagSequence(SHORT *sSequence);
	void IDCTransform(const FLOAT *fBlock);
	void IDCTransform(void);
	void GetBlock(FLOAT *fBlock);
	void DequantizateBlock(void);
	void QuantizateBlock(void);
	void SetQuality(const BYTE bQuality);
	void DCTransform(const FLOAT *fBlock);
	DCT(const BYTE bBlockSize);
	virtual ~DCT();

protected:
	BYTE m_bBlockSize;
	FLOAT **m_vMatrices[DCT_ARRAY_RANK];
};

#endif // !defined(AFX_DCT_H__5C2C8F3D_5C97_4882_8221_4489BC0EE718__INCLUDED_)
