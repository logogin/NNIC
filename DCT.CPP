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

DCT::DCT(const BYTE bBlockSize):m_bBlockSize(bBlockSize)
{
	DWORD i,j;
	for (i=0; i<DCT_ARRAY_RANK; i++)
	{
		m_vMatrices[i]=AllocateFloatMatrix(m_bBlockSize,m_bBlockSize);
		ASSERT(m_vMatrices[i]!=NULL);
	}

	FLOAT fRoot=(FLOAT)(1.0f/sqrtf((FLOAT)m_bBlockSize));
	for (i=0; i<m_bBlockSize; i++)
	{
		m_vMatrices[CTM][0][i]=fRoot;
		m_vMatrices[TCTM][i][0]=fRoot;
	}
	for (i=1; i<m_bBlockSize; i++)
		for (j=0; j<m_bBlockSize; j++)
		{
			m_vMatrices[CTM][i][j]=(FLOAT)(M_SQRT2*fRoot*cos((2*j+1)*i*M_PI_2/m_bBlockSize));
			m_vMatrices[TCTM][j][i]=m_vMatrices[CTM][i][j];
		}
}

DCT::~DCT()
{
	DWORD i;
	for (i=0; i<DCT_ARRAY_RANK; i++)
		if (m_vMatrices[i]!=NULL)
			DestroyMatrix((LPVOID *)m_vMatrices[i]);
}

void DCT::DCTransform(const PFLOAT fBlock)
{
	DWORD i,j,k;
	DWORD dwIndex;
	for (i=0; i<m_bBlockSize; i++)
	{
		dwIndex=i*m_bBlockSize;
		for (j=0; j<m_bBlockSize; j++)
		{
			m_vMatrices[TEMP][i][j]=0.0;
			for (k=0; k<m_bBlockSize; k++)
				m_vMatrices[TEMP][i][j]+=fBlock[dwIndex+k]*m_vMatrices[TCTM][k][j];
		}
	}
	for (i=0; i<m_bBlockSize; i++)
		for (j=0; j<m_bBlockSize; j++)
		{
			m_vMatrices[RSLT][i][j]=0.0;
			for (k=0; k<m_bBlockSize; k++)
				m_vMatrices[RSLT][i][j]+=m_vMatrices[CTM][i][k]*m_vMatrices[TEMP][k][j];
		}
}

void DCT::IDCTransform()
{
	DWORD i,j,k;
	for (i=0; i<m_bBlockSize; i++)
		for (j=0; j<m_bBlockSize; j++)
		{
			m_vMatrices[TEMP][i][j]=0.0;
			for (k=0; k<m_bBlockSize; k++)
				m_vMatrices[TEMP][i][j]+=m_vMatrices[RSLT][i][k]*m_vMatrices[CTM][k][j];
		}

	for (i=0; i<m_bBlockSize; i++)
		for (j=0; j<m_bBlockSize; j++)
		{
			m_vMatrices[RSLT][i][j]=0.0;
			for (k=0; k<m_bBlockSize; k++)
				m_vMatrices[RSLT][i][j]+=m_vMatrices[TCTM][i][k]*m_vMatrices[TEMP][k][j];
		}
}

void DCT::IDCTransform(const PFLOAT fBlock)
{
	DWORD i,j,k;
	DWORD dwIndex;
	for (i=0; i<m_bBlockSize; i++)
	{
		dwIndex=i*m_bBlockSize;
		for (j=0; j<m_bBlockSize; j++)
		{
			m_vMatrices[TEMP][i][j]=0.0;
			for (k=0; k<m_bBlockSize; k++)
				m_vMatrices[TEMP][i][j]+=fBlock[dwIndex+k]*m_vMatrices[CTM][k][j];
		}
	}

	for (i=0; i<m_bBlockSize; i++)
		for (j=0; j<m_bBlockSize; j++)
		{
			m_vMatrices[RSLT][i][j]=0.0;
			for (k=0; k<m_bBlockSize; k++)
				m_vMatrices[RSLT][i][j]+=m_vMatrices[TCTM][i][k]*m_vMatrices[TEMP][k][j];
		}
}

void DCT::SetQuality(const BYTE bQuality)
{
	DWORD i,j;
	for (i=0; i<m_bBlockSize; i++)
		for (j=0; j<m_bBlockSize; j++)
			m_vMatrices[QUANT][i][j]=(FLOAT)(1+(1+i+j)*bQuality);
}

void DCT::QuantizateBlock()
{
	DWORD i,j;
	for (i=0; i<m_bBlockSize; i++)
		for (j=0; j<m_bBlockSize; j++)
		{
			m_vMatrices[RSLT][i][j]/=m_vMatrices[QUANT][i][j];
			m_vMatrices[RSLT][i][j]=(FLOAT)Round(m_vMatrices[RSLT][i][j]);
		}
}

void DCT::DequantizateBlock()
{
	DWORD i,j;
	for (i=0; i<m_bBlockSize; i++)
		for (j=0; j<m_bBlockSize; j++)
		{
			m_vMatrices[RSLT][i][j]*=m_vMatrices[QUANT][i][j];
			m_vMatrices[RSLT][i][j]=(FLOAT)Round(m_vMatrices[RSLT][i][j]);
		}
}

void DCT::GetBlock(PFLOAT fBlock)
{
	CopyMemory(fBlock,m_vMatrices[RSLT][0],sizeof(FLOAT)*m_bBlockSize*m_bBlockSize);
}

void DCT::ZigZagSequence(SHORT *sSequence)
{
	DWORD i,j;
	for (i=0; i<m_bBlockSize; i++)
		for (j=0; j<m_bBlockSize; j++)
			sSequence[ZigZag[i][j]]=(SHORT)m_vMatrices[RSLT][i][j];
}

void DCT::DeZigZagSequence(const SHORT * sSequence)
{
	DWORD i,j;
	for (i=0; i<m_bBlockSize; i++)
		for (j=0; j<m_bBlockSize; j++)
			m_vMatrices[RSLT][i][j]=sSequence[ZigZag[i][j]];
}
