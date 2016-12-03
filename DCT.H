// DCT.h: interface for the DCT class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCT_H__5C2C8F3D_5C97_4882_8221_4489BC0EE718__INCLUDED_)
#define AFX_DCT_H__5C2C8F3D_5C97_4882_8221_4489BC0EE718__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define ARRAY_RANK 5

#define CTM 0
#define TCTM 1
#define RSLT 2
#define TEMP 3
#define QUANT 4

#define M_PI_2 1.57079632679489661923
#define M_SQRT2 1.41421356237309504880

class DCT  
{
public:
	void IDCTransform(const FLOAT *fBlock);
	void IDCTransform(void);
	void GetBlock(FLOAT *fBlock);
	void DequantizateBlock(void);
	void QuantizateBlock(void);
	void SetQuality(const UINT uiQuality);
	void DCTransform(const FLOAT *fBlock);
	DCT(const UINT uiBlockSize);
	virtual ~DCT();

protected:
	UINT m_uiBlockSize;
	FLOAT **m_vMatrices[ARRAY_RANK];
};

#endif // !defined(AFX_DCT_H__5C2C8F3D_5C97_4882_8221_4489BC0EE718__INCLUDED_)
