
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
	{
		delete []pMatrix;
		return NULL;
	}

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
	{
		delete []pMatrix;
		return NULL;
	}
	
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
	{
		delete []pMatrix;
		return NULL;
	}
	
	dwSize=0;
	for (i=1; i<dwHeight; i++)
	{
		dwSize+=pWidth[i-1];
		pMatrix[i]=&(pMatrix[0][dwSize]);
	}

	return pMatrix;
}

WORD **AllocateWordMatrix(const DWORD dwHeight,const DWORD dwWidth)
{
	DWORD i;
	WORD **pMatrix=new WORD *[dwHeight];
	if (pMatrix==NULL)
		return NULL;
	pMatrix[0]=new WORD[dwHeight*dwWidth];
	if (pMatrix[0]==NULL)
	{
		delete []pMatrix;
		return NULL;
	}

	for (i=1; i<dwHeight; i++)
		pMatrix[i]=&(pMatrix[0][dwWidth*i]);
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
		for (i=2; i<=(DWORD)sqrtf((FLOAT)dwNumber)&&!bFlag; i++)
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

LONG Round(const FLOAT fValue)
{
	if (fValue<0.0)
		return (LONG)(fValue-0.5);
	return (LONG)(fValue+0.5);
}

DWORD GetBufferSize(const DWORD dwSrcLen)
{
	return (DWORD)(1.001*dwSrcLen+0.5)+12;
}

BOOLEAN QueryZLibVersion(void)
{
	if (zlibVersion()[0] != ZLIB_VERSION[0]) 
	{
		AfxMessageBox(_T("ERROR: Incompatible ZLib versions!"),MB_OK);
        return FALSE;

    } 
	else 
		if (strcmp(zlibVersion(), ZLIB_VERSION) != 0) 
			AfxMessageBox(_T("WARNING: different ZLib versions!"),MB_OK);
	return TRUE;
}

BOOLEAN Compress2(LPBYTE lpDest,
				  PDWORD dwDestLen,
				  const LPBYTE lpSrc,
				  const DWORD dwSrcLen,
				  const bLevel)
{
	INT iRetVal=compress2(lpDest,dwDestLen,lpSrc,dwSrcLen,bLevel);
	if (iRetVal==Z_OK)
		return TRUE;
	if (iRetVal==Z_MEM_ERROR)
	{
		AfxMessageBox(_T("ERROR: Not enough memory for ZLib compression!"),MB_OK);
		return FALSE;
	}
	if (iRetVal==Z_BUF_ERROR)
	{
		AfxMessageBox(_T("ERROR: Not enough memory in output buffer for ZLib compression!"),MB_OK);
		return FALSE;
	}
	if (iRetVal==Z_STREAM_ERROR)
	{
		AfxMessageBox(_T("ERROR: Invalid ZLib compression level parameter!"),MB_OK);
		return FALSE;
	}

	AfxMessageBox(_T("ERROR: Unexpected ZLib error!"),MB_OK);
	return FALSE;
}

BOOLEAN UnCompress(LPBYTE lpDest,
				  PDWORD dwDestLen,
				  const LPBYTE lpSrc,
				  const DWORD dwSrcLen)
{
	INT iRetVal=uncompress(lpDest,dwDestLen,lpSrc,dwSrcLen);
	if (iRetVal==Z_OK)
		return TRUE;
	if (iRetVal==Z_MEM_ERROR)
	{
		AfxMessageBox(_T("ERROR: Not enough memory for ZLib decompression!"),MB_OK);
		return FALSE;
	}
	if (iRetVal==Z_BUF_ERROR)
	{
		AfxMessageBox(_T("ERROR: Not enough memory in output buffer for ZLib decompression!"),MB_OK);
		return FALSE;
	}
	if (iRetVal==Z_DATA_ERROR)
	{
		AfxMessageBox(_T("ERROR: The input ZLib data was corrupted!"),MB_OK);
		return FALSE;
	}

	AfxMessageBox(_T("ERROR: Unexpected ZLib error!"),MB_OK);
	return FALSE;
}

BOOLEAN FileExists(const CString &strFileName)
{
	if (::GetFileAttributes(strFileName)==INVALID_FILE_ATTRIBUTES)
		return FALSE;
	else
		return TRUE;
}