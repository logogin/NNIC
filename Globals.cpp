
#include "stdafx.h"
#include "Globals.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

DWORD GetPageSize(void)
{
	SYSTEM_INFO sysInfo;
	::GetSystemInfo(&sysInfo);
	return sysInfo.dwPageSize;
}

HANDLE HeapCreate(const DWORD dwInitalSize,const DWORD dwMaximumSize)
{
	return ::HeapCreate(HEAP_GENERATE_EXCEPTIONS | HEAP_NO_SERIALIZE,dwInitalSize,dwMaximumSize);
}

BOOLEAN HeapFree(HANDLE hHeap,LPVOID lpMem)
{
	return HeapFree(hHeap,HEAP_NO_SERIALIZE,lpMem);
}

HANDLE GetHeapHandle(const LPVOID lpMem)
{
	return *((PHANDLE)lpMem-1);
}

BOOLEAN HeapFree(LPVOID lpMem)
{
	HANDLE hHeap=*((PHANDLE)lpMem-1);
	return ::HeapDestroy(hHeap);
}

LPVOID HeapAlloc(const HANDLE hHeap,const SIZE_T dwBytes,const BOOLEAN bZero)
{
	DWORD dwFlags=HEAP_GENERATE_EXCEPTIONS | HEAP_NO_SERIALIZE;
	if (bZero)
		dwFlags |=HEAP_ZERO_MEMORY;
	return ::HeapAlloc(hHeap,dwFlags,dwBytes); 
}

LPVOID HeapAlloc(const SIZE_T dwBytes,const BOOLEAN bZero)
{
	HANDLE hHeap=HeapCreate(dwBytes+sizeof(HANDLE));
	LPVOID lpMem=HeapAlloc(hHeap,dwBytes+sizeof(HANDLE),bZero);
	*((PHANDLE)lpMem)=hHeap;
	return (PHANDLE)lpMem+1;
}

LPBYTE *AllocateByteMatrix(const DWORD dwHeight,const DWORD dwWidth,const BOOLEAN bZero)
{
	DWORD i;
	LPBYTE *pMatrix=(LPBYTE *)HeapAlloc(dwHeight*sizeof(LPBYTE));
	if (pMatrix==NULL)
		return NULL;

	HANDLE hHeap=GetHeapHandle(pMatrix);
	pMatrix[0]=(LPBYTE)HeapAlloc(hHeap,dwHeight*dwWidth,bZero);
	if (pMatrix[0]==NULL)
	{
		HeapFree(pMatrix);
		return NULL;
	}

	for (i=1; i<dwHeight; i++)
		pMatrix[i]=&(pMatrix[0][dwWidth*i]);
	return pMatrix;
}

PFLOAT *AllocateFloatMatrix(const DWORD dwHeight,const DWORD dwWidth,const BOOLEAN bZero)
{
	DWORD i;
	PFLOAT *pMatrix=(PFLOAT *)HeapAlloc(dwHeight*sizeof(PFLOAT));
	if (pMatrix==NULL)
		return NULL;

	HANDLE hHeap=GetHeapHandle(pMatrix);
	pMatrix[0]=(PFLOAT)HeapAlloc(hHeap,dwHeight*dwWidth*sizeof(FLOAT),bZero);
	if (pMatrix[0]==NULL)
	{
		HeapFree(pMatrix);
		return NULL;
	}
	
	for (i=1; i<dwHeight; i++)
		pMatrix[i]=&(pMatrix[0][dwWidth*i]);
	return pMatrix;
}

PFLOAT *AllocateFloatMatrix(HANDLE hHeap,
							const DWORD dwHeight,
							const DWORD dwWidth,
							const BOOLEAN bZero)
{
	DWORD i;
	PFLOAT *pMatrix=(PFLOAT *)HeapAlloc(hHeap,dwHeight*sizeof(PFLOAT));
	if (pMatrix==NULL)
		return NULL;

	pMatrix[0]=(PFLOAT)HeapAlloc(hHeap,dwHeight*dwWidth*sizeof(FLOAT),bZero);
	if (pMatrix[0]==NULL)
	{
		HeapDestroy(hHeap);
		return NULL;
	}
	
	for (i=1; i<dwHeight; i++)
		pMatrix[i]=&(pMatrix[0][dwWidth*i]);
	return pMatrix;
}

PFLOAT *AllocateFloatMatrix(const DWORD dwHeight,const PDWORD pWidth,const BOOLEAN bZero)
{
	DWORD dwSize=0;
	DWORD i;
	PFLOAT *pMatrix=(PFLOAT *)HeapAlloc(dwHeight*sizeof(PFLOAT));
	if (pMatrix==NULL)
		return NULL;

	for (i=0; i<dwHeight; i++)
		dwSize+=pWidth[i];

	HANDLE hHeap=GetHeapHandle(pMatrix);
	pMatrix[0]=(PFLOAT)HeapAlloc(hHeap,dwSize*sizeof(FLOAT),bZero);
	if (pMatrix[0]==NULL)
	{
		HeapFree(pMatrix);
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

PWORD *AllocateWordMatrix(const DWORD dwHeight,const DWORD dwWidth,const BOOLEAN bZero)
{
	DWORD i;
	PWORD *pMatrix=(PWORD *)HeapAlloc(dwHeight*sizeof(PWORD));
	if (pMatrix==NULL)
		return NULL;

	HANDLE hHeap=GetHeapHandle(pMatrix);
	pMatrix[0]=(PWORD)HeapAlloc(hHeap,dwHeight*dwWidth*sizeof(WORD),bZero);
	if (pMatrix[0]==NULL)
	{
		HeapFree(pMatrix);
		return NULL;
	}

	for (i=1; i<dwHeight; i++)
		pMatrix[i]=&(pMatrix[0][dwWidth*i]);
	return pMatrix;
}

BOOLEAN DestroyMatrix(LPVOID *pMatrix)
{
	HANDLE hHeap=GetHeapHandle(pMatrix);
	HeapFree(hHeap,pMatrix[0]);
	return HeapFree(pMatrix);
}

//DWORD FindPrimeNumber(const DWORD dwFrom)
//{
//	DWORD dwNumber=dwFrom;
//	BOOLEAN bFlag;
//	BOOLEAN bFound;
//	DWORD i;
//	do
//	{
//		bFound=TRUE;
//		bFlag=FALSE;
//		for (i=2; i<=(DWORD)sqrtf((FLOAT)dwNumber)&&!bFlag; i++)
//			if (!(dwNumber%i))
//			{
//				bFlag=TRUE;
//				bFound=FALSE;
//			}
//		if (!bFound)
//			dwNumber++;
//	}
//	while (!bFound);
//	return dwNumber;
//}

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