// DCT.cpp: implementation of the DCT class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NNIC.h"
#include "DCT.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DCT::DCT(const UINT uiBlockSize):m_uiBlockSize(uiBlockSize)
{
	for (UINT i=0; i<ARRAY_RANK; i++)
	{
		m_vMatrices[i]=new FLOAT *[m_uiBlockSize];
		ASSERT(m_vMatrices[i]!=NULL);
		for (UINT j=0; j<m_uiBlockSize; j++)
		{
			m_vMatrices[i][j]=new FLOAT[m_uiBlockSize];
			ASSERT(m_vMatrices[i][j]!=NULL);
		}
	}

	FLOAT fRoot=(FLOAT)(1.0f/sqrt(m_uiBlockSize));
	for (i=0; i<m_uiBlockSize; i++)
	{
		m_vMatrices[CTM][0][i]=fRoot;
		m_vMatrices[TCTM][i][0]=fRoot;
	}
	for (i=1; i<m_uiBlockSize; i++)
		for (UINT j=0; j<m_uiBlockSize; j++)
		{
			m_vMatrices[CTM][i][j]=(FLOAT)(M_SQRT2*fRoot*cos((2*j+1)*i*M_PI_2/m_uiBlockSize));
			m_vMatrices[TCTM][j][i]=m_vMatrices[CTM][i][j];
		}
}

DCT::~DCT()
{
	for (UINT i=0; i<ARRAY_RANK; i++)
	{
		for (UINT j=0; j<m_uiBlockSize; j++)
			delete []m_vMatrices[i][j];
		delete []m_vMatrices[i];
	}
}

void DCT::DCTransform(const FLOAT *fBlock)
{
	for (UINT i=0; i<m_uiBlockSize; i++)
		for (UINT j=0; j<m_uiBlockSize; j++)
		{
			m_vMatrices[TEMP][i][j]=0.0;
			for (UINT k=0; k<m_uiBlockSize; k++)
				m_vMatrices[TEMP][i][j]+=fBlock[i*m_uiBlockSize+k]*m_vMatrices[TCTM][k][j];
		}

	for (i=0; i<m_uiBlockSize; i++)
		for (UINT j=0; j<m_uiBlockSize; j++)
		{
			m_vMatrices[RSLT][i][j]=0.0;
			for (UINT k=0; k<m_uiBlockSize; k++)
				m_vMatrices[RSLT][i][j]+=m_vMatrices[CTM][i][k]*m_vMatrices[TEMP][k][j];
		}
}

void DCT::IDCTransform()
{
	for (UINT i=0; i<m_uiBlockSize; i++)
		for (UINT j=0; j<m_uiBlockSize; j++)
		{
			m_vMatrices[TEMP][i][j]=0.0;
			for (UINT k=0; k<m_uiBlockSize; k++)
				m_vMatrices[TEMP][i][j]+=m_vMatrices[RSLT][i][k]*m_vMatrices[CTM][k][j];
		}

	for (i=0; i<m_uiBlockSize; i++)
		for (UINT j=0; j<m_uiBlockSize; j++)
		{
			m_vMatrices[RSLT][i][j]=0.0;
			for (UINT k=0; k<m_uiBlockSize; k++)
				m_vMatrices[RSLT][i][j]+=m_vMatrices[TCTM][i][k]*m_vMatrices[TEMP][k][j];
		}
}

void DCT::IDCTransform(const FLOAT *fBlock)
{
	for (UINT i=0; i<m_uiBlockSize; i++)
		for (UINT j=0; j<m_uiBlockSize; j++)
		{
			m_vMatrices[TEMP][i][j]=0.0;
			for (UINT k=0; k<m_uiBlockSize; k++)
				m_vMatrices[TEMP][i][j]+=fBlock[i*m_uiBlockSize+k]*m_vMatrices[CTM][k][j];
		}

	for (i=0; i<m_uiBlockSize; i++)
		for (UINT j=0; j<m_uiBlockSize; j++)
		{
			m_vMatrices[RSLT][i][j]=0.0;
			for (UINT k=0; k<m_uiBlockSize; k++)
				m_vMatrices[RSLT][i][j]+=m_vMatrices[TCTM][i][k]*m_vMatrices[TEMP][k][j];
		}
}

void DCT::SetQuality(const UINT uiQuality)
{
	for (UINT i=0; i<m_uiBlockSize; i++)
		for (UINT j=0; j<m_uiBlockSize; j++)
			m_vMatrices[QUANT][i][j]=(FLOAT)(1+(1+i+j)*uiQuality);
}

void DCT::QuantizateBlock()
{
	for (UINT i=0; i<m_uiBlockSize; i++)
		for (UINT j=0; j<m_uiBlockSize; j++)
		{
			m_vMatrices[RSLT][i][j]/=m_vMatrices[QUANT][i][j];
			m_vMatrices[RSLT][i][j]=(FLOAT)floor(m_vMatrices[RSLT][i][j]+0.5);
		}
}

void DCT::DequantizateBlock()
{
	for (UINT i=0; i<m_uiBlockSize; i++)
		for (UINT j=0; j<m_uiBlockSize; j++)
		{
			m_vMatrices[RSLT][i][j]*=m_vMatrices[QUANT][i][j];
			m_vMatrices[RSLT][i][j]=(FLOAT)floor(m_vMatrices[RSLT][i][j]+0.5);
		}
}

void DCT::GetBlock(FLOAT *fBlock)
{
	for (UINT i=0; i<m_uiBlockSize; i++)
		CopyMemory(&fBlock[i*m_uiBlockSize],m_vMatrices[RSLT][i],sizeof(FLOAT)*m_uiBlockSize);
}

void DCT::ZigZagSequence(SHORT *sSequence)
{
	for (UINT i=0; i<m_uiBlockSize; i++)
		for (UINT j=0; j<m_uiBlockSize; j++)
			sSequence[ZigZag[i][j]]=(SHORT)m_vMatrices[RSLT][i][j];
}
