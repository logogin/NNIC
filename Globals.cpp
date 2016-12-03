
#include "stdafx.h"
#include "Globals.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

BYTE **AllocateByteMatrix(const DWORD dwHeight,const DWORD dwWidth)
{
	DWORD i;
	BYTE **pMatrix=new BYTE *[dwHeight];
	if (pMatrix==NULL)
		return NULL;
	pMatrix[0]=new BYTE[dwHeight*dwWidth];
	if (pMatrix[0]==NULL)
		return NULL;

	for (i=1; i<dwHeight; i++)
		pMatrix[i]=&(pMatrix[0][dwWidth*i]);
	return pMatrix;
}

FLOAT **AllocateFloatMatrix(const DWORD dwHeight,const DWORD dwWidth)
{
	DWORD i;
	FLOAT **pMatrix=new FLOAT *[dwHeight];
	if (pMatrix==NULL)
		return NULL;

	pMatrix[0]=new FLOAT[dwHeight*dwWidth];
	if (pMatrix[0]==NULL)
		return NULL;
	
	for (i=1; i<dwHeight; i++)
		pMatrix[i]=&(pMatrix[0][dwWidth*i]);
	return pMatrix;
}

FLOAT **AllocateFloatMatrix(const DWORD dwHeight,const DWORD *pWidth)
{
	DWORD dwSize=0;
	DWORD i;
	FLOAT **pMatrix=new FLOAT *[dwHeight];
	if (pMatrix==NULL)
		return NULL;

	for (i=0; i<dwHeight; i++)
		dwSize+=pWidth[i];

	pMatrix[0]=new FLOAT[dwSize];
	if (pMatrix[0]==NULL)
		return NULL;
	
	dwSize=0;
	for (i=1; i<dwHeight; i++)
	{
		dwSize+=pWidth[i-1];
		pMatrix[i]=&(pMatrix[0][dwSize]);
	}

	return pMatrix;
}

void DestroyMatrix(LPVOID *pMatrix)
{
	delete []pMatrix[0];
	delete []pMatrix;
}

DWORD FindPrimeNumber(const DWORD dwFrom)
{
	DWORD dwNumber=dwFrom;
	BOOLEAN bFlag;
	BOOLEAN bFound;
	DWORD i;
	do
	{
		bFound=TRUE;
		bFlag=FALSE;
		for (i=2; i<=(DWORD)sqrt(dwNumber)&&!bFlag; i++)
			if (!(dwNumber%i))
			{
				bFlag=TRUE;
				bFound=FALSE;
			}
		if (!bFound)
			dwNumber++;
	}
	while (!bFound);
	return dwNumber;
}