
#include "stdafx.h"
#include "Globals.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

BYTE **AllocateByteMatrix(const DWORD dwHeight,const DWORD dwWidth)
{
	BYTE **pMatrix=new BYTE *[dwHeight];
	if (pMatrix==NULL)
		return NULL;
	pMatrix[0]=new BYTE[dwHeight*dwWidth];
	LONG i;
	for (i=1; i<dwHeight; i++)
		pMatrix[i]=&(pMatrix[0][dwWidth*i]);
	return pMatrix;
}

void DestroyMatrix(LPVOID *pMatrix)
{
	delete []pMatrix[0];
	delete []pMatrix;
}