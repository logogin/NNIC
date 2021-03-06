// BitmapKit.cpp: implementation of the CBitmapKit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NNIC.h"
#include "BitmapKit.h"
#include "ResampleDlg.h"
#include ".\bitmapkit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBitmapKit::CBitmapKit():m_bPresent(FALSE),m_lpBits(NULL)
{
	ZeroMemory(&m_bmBitmap,sizeof(BITMAP));
}

CBitmapKit::~CBitmapKit()
{
	if (m_lpBits!=NULL)
		HeapFree(m_lpBits);
}

BOOLEAN CBitmapKit::LoadBMP(const CString &strFileName)
{
	if (m_bPresent)
	{
		CBitmapKit::Detach();
		CGdiObject::DeleteObject();
		m_bPresent=FALSE;
	}

	HBITMAP hBitmap=(HBITMAP)LoadImage(NULL,strFileName,IMAGE_BITMAP,0,0,
		LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);

	if (hBitmap==NULL)
		return FALSE;

	if (!CBitmapKit::Attach(hBitmap))
		return FALSE;

	m_bPresent=TRUE;

	return m_bPresent;
}

BOOL CBitmapKit::SaveBMP(CFile * pFile)
{
	if (!m_bPresent)
		return FALSE;

	BYTE bBitsPixel;
	BITMAPFILEHEADER hdr;
	PBITMAPINFO pbmi;

	// Convert the color format to a count of bits.
	bBitsPixel = m_bmBitmap.bmPlanes * m_bmBitmap.bmBitsPixel;

	// Allocate memory for the BITMAPINFO structure. (This structure 
    // contains a BITMAPINFOHEADER structure and an array of RGBQUAD 
	// data structures.)
	HANDLE hHeap=HeapCreate(GetPageSize());
	if (bBitsPixel != 24)
	{
		DWORD i;
		pbmi=(PBITMAPINFO)HeapAlloc(hHeap,
			sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*(1<<bBitsPixel),(BOOLEAN)TRUE);
		for (i=0; i<(1<<bBitsPixel); i++)
		{
			RGBQUAD rgb={(BYTE)i,(BYTE)i,(BYTE)i,0};
			CopyMemory(&pbmi->bmiColors[i],&rgb,sizeof(RGBQUAD));
		}
		pbmi->bmiHeader.biClrUsed=1<<bBitsPixel;
	}
	else
	// There is no RGBQUAD array for the 24-bit-per-pixel format. 
		pbmi=(PBITMAPINFO)HeapAlloc(hHeap,sizeof(BITMAPINFOHEADER),(BOOLEAN)TRUE);

	ASSERT(pbmi!=NULL);

	// Initialize the fields in the BITMAPINFO structure. 
	pbmi->bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	pbmi->bmiHeader.biWidth=m_bmBitmap.bmWidth; 
    pbmi->bmiHeader.biHeight=m_bmBitmap.bmHeight;
	pbmi->bmiHeader.biPlanes=m_bmBitmap.bmPlanes; 
    pbmi->bmiHeader.biBitCount=m_bmBitmap.bmBitsPixel;

	// If the bitmap is not compressed, set the BI_RGB flag. 
    pbmi->bmiHeader.biCompression=BI_RGB;

    // Compute the number of bytes in the array of color 
    // indices and store the result in biSizeImage. 
    // Width must be DWORD aligned unless bitmap is RLE compressed.
    pbmi->bmiHeader.biSizeImage = m_bmBitmap.bmWidthBytes*m_bmBitmap.bmHeight;

	hdr.bfType = 0x4d42; // 0x42 = "B" 0x4d = "M"
	// Compute the size of the entire file. 
	hdr.bfSize = sizeof(BITMAPFILEHEADER)+
		pbmi->bmiHeader.biSize+
		pbmi->bmiHeader.biClrUsed*sizeof(RGBQUAD)+
		pbmi->bmiHeader.biSizeImage;
    hdr.bfReserved1 = 0; 
    hdr.bfReserved2 = 0; 
	
	// Compute the offset to the array of color indices. 
	hdr.bfOffBits = sizeof(BITMAPFILEHEADER)+
		pbmi->bmiHeader.biSize+
		pbmi->bmiHeader.biClrUsed*sizeof (RGBQUAD);

	pFile->Write(&hdr,sizeof(BITMAPFILEHEADER));
	pFile->Write(pbmi,sizeof(BITMAPINFOHEADER)+pbmi->bmiHeader.biClrUsed*sizeof(RGBQUAD));
	pFile->Write(m_lpBits[0],pbmi->bmiHeader.biSizeImage);

	::HeapDestroy(hHeap);
	return TRUE;
}

BOOLEAN CBitmapKit::InitBitmapBits()
{
	if (m_lpBits!=NULL)
		HeapFree(m_lpBits);

	if (!CBitmap::GetBitmap(&m_bmBitmap))
		return FALSE;

	m_bBytesPixel=(BYTE)BITS2BYTES(m_bmBitmap.bmBitsPixel*m_bmBitmap.bmPlanes);

	if (m_bmBitmap.bmBits==NULL)
		return FALSE;

	m_lpBits=(LPBYTE *)HeapAlloc(m_bmBitmap.bmHeight*sizeof(LPBYTE));
	ASSERT(m_lpBits!=NULL);

	DWORD i;
	m_lpBits[0]=(LPBYTE)m_bmBitmap.bmBits;
	for (i=1; i<m_bmBitmap.bmHeight; i++)
		m_lpBits[i]=(LPBYTE)m_bmBitmap.bmBits+(i*m_bmBitmap.bmWidthBytes);
	
	return TRUE;
}

BOOLEAN CBitmapKit::SetBits(const LPVOID lpBits)
{
	if (!CBitmap::SetBitmapBits(GetSizeBytes(),lpBits))
		return FALSE;
	return InitBitmapBits();
}

BOOLEAN CBitmapKit::Attach(HBITMAP hBitmap)
{
	if (hBitmap==NULL)
		return FALSE;

	if (m_bPresent)
	{
		CBitmapKit::Detach();
		CGdiObject::DeleteObject();
		m_bPresent=FALSE;
	}

	if (!CBitmap::Attach(hBitmap))
		return FALSE;

	m_bPresent=TRUE;

	return InitBitmapBits();
}

HBITMAP CBitmapKit::Detach()
{
	m_bPresent=FALSE;

	if (m_lpBits!=NULL)
		HeapFree(m_lpBits);
	m_lpBits=NULL;

	HBITMAP hBitmap=(HBITMAP)CGdiObject::Detach();

	ZeroMemory(&m_bmBitmap,sizeof(BITMAP));
	return hBitmap;
}

void CBitmapKit::GetBlock(const CRect &rect,
							PFLOAT pSegment,
							const BYTE bColor,
							const FLOAT fMinValue,
							const FLOAT fRange)
{
	ASSERT(rect.right<=m_bmBitmap.bmWidth);
	ASSERT(rect.bottom<=m_bmBitmap.bmHeight);

	DWORD dwSegWidth=rect.Width();

	DWORD i,j;
	DWORD dwIndex;
	for (i=0; i<rect.Height(); i++)
	{
		dwIndex=i*dwSegWidth;
		for (j=0; j<dwSegWidth; j++)	
			pSegment[dwIndex+j]=fMinValue+(m_lpBits[i+rect.top][(j+rect.left)*
				m_bBytesPixel+bColor]/255.0f)*fRange;
	}
}

HBITMAP CBitmapKit::Scale(CDC *pDC,const DWORD dwWidth, const DWORD dwHeight)
{
	CDC mem_dc1;
    mem_dc1.CreateCompatibleDC(NULL);
	CBitmap* old_bmp1 = mem_dc1.SelectObject(this);

	CDC mem_dc2;
	mem_dc2.CreateCompatibleDC(NULL);
	CBitmap scaled_bmp;
	scaled_bmp.CreateCompatibleBitmap(pDC, dwWidth, dwHeight);
	CBitmap* old_bmp2 = mem_dc2.SelectObject(&scaled_bmp);
 
	mem_dc2.SetStretchBltMode(HALFTONE);
	BOOL result = mem_dc2.StretchBlt(0, 0, dwWidth, dwHeight,&mem_dc1, 0, 0, m_bmBitmap.bmWidth,m_bmBitmap.bmHeight, SRCCOPY);
	ASSERT(result);
 
	mem_dc1.SelectObject(old_bmp1);
	mem_dc2.SelectObject(old_bmp2);

    return (HBITMAP)(scaled_bmp.Detach());
}

DWORD CBitmapKit::GetSizeBytes()
{
	return m_bmBitmap.bmWidthBytes*m_bmBitmap.bmHeight;	
}

DWORD CBitmapKit::GetWidthBytes()
{
	return m_bmBitmap.bmWidthBytes;
}

DWORD CBitmapKit::GetWidth()
{
	return m_bmBitmap.bmWidth;
}

DWORD CBitmapKit::GetHeight()
{
	return m_bmBitmap.bmHeight;
}

DWORD CBitmapKit::GetSizePixels()
{
	return m_bmBitmap.bmWidth*m_bmBitmap.bmHeight;
}

DWORD CBitmapKit::GetSizeClear()
{
	return m_bmBitmap.bmWidth*m_bmBitmap.bmHeight*m_bBytesPixel;
}

BYTE CBitmapKit::GetBitsPixel()
{
	return (BYTE)m_bmBitmap.bmBitsPixel;
}

DWORD CBitmapKit::GetColors()
{
	return (1<<m_bmBitmap.bmBitsPixel);
}

DOUBLE CBitmapKit::GetPSNR(const CBitmapKit *pBitmapKit, const BYTE bChannel)
{
	const DWORD dwChannelWidth=65025; /*255*255*/
	if (bChannel>=m_bBytesPixel)
		return -1.0;
	DOUBLE fDist;
	DOUBLE fMSE=0.0;
	DWORD i,j;
	DWORD dwIndex;
	for (j=0; j<m_bmBitmap.bmWidth; j++)
	{
		dwIndex=j*m_bBytesPixel+bChannel;
		for (i=0; i<m_bmBitmap.bmHeight; i++)
		{
			fDist=(FLOAT)m_lpBits[i][dwIndex]-
				(FLOAT)pBitmapKit->m_lpBits[i][dwIndex];
			fMSE+=fDist*fDist;
		}
	}
	fMSE/=GetSizePixels();

	return 10.0*log10(dwChannelWidth/fMSE);
}

DOUBLE CBitmapKit::GetPSNRFullChannel(const CBitmapKit *pBitmapKit)
{
	const DWORD dwChannelWidth=GetColors()-1;

	DOUBLE fDist;
	DOUBLE fMSE=0.0f;
	DWORD dwDist1;
	DWORD dwDist2;
	DWORD i,j;
	DWORD dwIndex;
	for (j=0; j<m_bmBitmap.bmWidth; j++)
	{
		dwIndex=j*m_bBytesPixel;
		for (i=0; i<m_bmBitmap.bmHeight; i++)
		{
			dwDist1=dwDist2=0;
			CopyMemory(&dwDist1,&(m_lpBits[i][j*m_bBytesPixel]),m_bBytesPixel);
			CopyMemory(&dwDist2,&(pBitmapKit->m_lpBits[i][dwIndex]),m_bBytesPixel);
			fDist=(DOUBLE)dwDist1-(DOUBLE)dwDist2;
			fMSE+=fDist*fDist;
		}
	}
	fMSE/=GetSizePixels();

	return 10.0*(2.0*log10l(dwChannelWidth)-log10(fMSE));
}

LPBYTE * CBitmapKit::GetBits()
{
	return m_lpBits;
}

BYTE CBitmapKit::GetBytesPixel()
{
	return m_bBytesPixel;
}

BOOLEAN CBitmapKit::CreateBitmap(const BITMAP &bm,const LPVOID lpBits)
{
	ASSERT(lpBits!=NULL);

	if (m_bPresent)
	{
		CBitmapKit::Detach();
		CGdiObject::DeleteObject();
		m_bPresent=FALSE;
	}

	LPVOID pBits;
	HBITMAP hBitmap=CBitmapKit::CreateDIBSection(bm,&pBits);

	ASSERT(pBits!=NULL);

	CopyMemory(pBits,lpBits,bm.bmWidthBytes*bm.bmHeight);

	return CBitmapKit::Attach(hBitmap);
}

HBITMAP CBitmapKit::CreateDIBSection(const BITMAP &bm, LPVOID *lpBits)
{
	PBITMAPINFO pbmi; 
    BYTE bBitsPixel;

    // Convert the color format to a count of bits.
	bBitsPixel = bm.bmPlanes * bm.bmBitsPixel;
    
	// Allocate memory for the BITMAPINFO structure. (This structure 
    // contains a BITMAPINFOHEADER structure and an array of RGBQUAD 
	// data structures.) 
	HANDLE hHeap=HeapCreate(GetPageSize());

	if (bBitsPixel != 24)
	{
		DWORD i;
		pbmi=(PBITMAPINFO)HeapAlloc(hHeap,
			sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*(1<<bBitsPixel),(BOOLEAN)TRUE);
		for (i=0; i<(1<<bBitsPixel); i++)
		{
			RGBQUAD rgb={(BYTE)i,(BYTE)i,(BYTE)i,0};
			CopyMemory(&pbmi->bmiColors[i],&rgb,sizeof(RGBQUAD));
		}
	}
	else
	// There is no RGBQUAD array for the 24-bit-per-pixel format. 
		pbmi=(PBITMAPINFO)HeapAlloc(hHeap,sizeof(BITMAPINFOHEADER),(BOOLEAN)TRUE);

	ASSERT(pbmi!=NULL);

	ZeroMemory(pbmi,sizeof(BITMAPINFOHEADER));
	// Initialize the fields in the BITMAPINFO structure. 
	pbmi->bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	pbmi->bmiHeader.biWidth=bm.bmWidth; 
    pbmi->bmiHeader.biHeight=bm.bmHeight;
	pbmi->bmiHeader.biPlanes=bm.bmPlanes; 
    pbmi->bmiHeader.biBitCount=bm.bmBitsPixel;
	pbmi->bmiHeader.biClrUsed=0;//(1<<bBitsPixel);

	// If the bitmap is not compressed, set the BI_RGB flag. 
    pbmi->bmiHeader.biCompression=BI_RGB;

    // Compute the number of bytes in the array of color 
    // indices and store the result in biSizeImage. 
    // Width must be DWORD aligned unless bitmap is RLE compressed.
    pbmi->bmiHeader.biSizeImage = bm.bmWidthBytes*bm.bmHeight;
	HBITMAP hBitmap=::CreateDIBSection(NULL,pbmi,DIB_RGB_COLORS,lpBits,NULL,0);
	::HeapDestroy(hHeap);
   	return hBitmap;
}

HBITMAP CBitmapKit::Resample(const DWORD dwWidth,
							 const DWORD dwHeight,
							 CResampleDlg *pDlg,
							 const BOOLEAN bChange)
{
	const DOUBLE fXScale=(DOUBLE)dwWidth/m_bmBitmap.bmWidth;
	const DOUBLE fYScale=(DOUBLE)dwHeight/m_bmBitmap.bmHeight;

	DOUBLE fWidth;
	DOUBLE fFilterScale;
	DOUBLE fCenter;
	LONG lLeft;
	LONG lRight;
	LONG lPixel;
	DOUBLE fWeight;

	DWORD i,k,m;
	LONG j;
	DWORD dwCount;
	DWORD dwProgressCount=0;
	DWORD dwInc;

	CArray<CArray<CONTRIBUTOR,CONTRIBUTOR &>,CArray<CONTRIBUTOR,CONTRIBUTOR &> &> contributorY;
	contributorY.SetSize(dwHeight);

	pDlg->SetStep(1);

	if (fYScale<1.0)
	{
		fWidth=FILTER_WIDTH/fYScale;
		fFilterScale=1.0/fYScale;
	}
	else
	{
		fWidth=FILTER_WIDTH;
		fFilterScale=1.0;
	}

	dwInc=(DWORD)((dwHeight+dwWidth*(m_bmBitmap.bmHeight+dwHeight))/100);
	for (i=0; i<dwHeight; i++)
	{
		contributorY[i].SetSize((DWORD)(2.0*fWidth+1));
		fCenter=(DOUBLE)i/fYScale;
		lLeft=(LONG)(fCenter-fWidth+0.5);
		lRight=(LONG)(fCenter+fWidth-0.5);
		dwCount=0;
		for (j=lLeft; j<=lRight; j++)
		{
			fWeight=Filter((fCenter-j)/fFilterScale)/fFilterScale;
			lPixel=j;
			if (lPixel<0)
				lPixel=-lPixel;
			if (lPixel>=m_bmBitmap.bmHeight)
				lPixel=m_bmBitmap.bmHeight-(lPixel-m_bmBitmap.bmHeight)-1;
			contributorY[i][dwCount++]=CONTRIBUTOR((DWORD)lPixel,fWeight);
		}
		dwProgressCount++;
		if (dwProgressCount==dwInc)
		{
			dwProgressCount=0;
			pDlg->StepIt();
		}
	}

	BITMAP bm;
	CopyMemory(&bm,&m_bmBitmap,sizeof(BITMAP));

	bm.bmWidth=dwWidth;
	bm.bmHeight=dwHeight;
	bm.bmWidthBytes=ALIGNBYTES(dwWidth*m_bBytesPixel);

	LPBYTE lpDestBits=NULL;
	HBITMAP hBitmap=CBitmapKit::CreateDIBSection(bm,(LPVOID *)&lpDestBits);

	ASSERT(lpDestBits!=NULL);

	LPBYTE *lpColumn=AllocateByteMatrix(m_bmBitmap.bmHeight,m_bBytesPixel);
	ASSERT(lpColumn!=NULL);

	DOUBLE *pWeight=(DOUBLE *)HeapAlloc(m_bBytesPixel*sizeof(DOUBLE));
	ASSERT(pWeight!=NULL);

	CArray<CONTRIBUTOR,CONTRIBUTOR &> contributorX;

	if (fXScale<1.0)
	{
		fWidth=FILTER_WIDTH/fXScale;
		fFilterScale=1.0/fXScale;
	}
	else
	{
		fWidth=FILTER_WIDTH;
		fFilterScale=1.0;
	}

	CONTRIBUTOR contributor;
	contributorX.SetSize((DWORD)(2.0*fWidth+1));
	for (i=0; i<dwWidth; i++)
	{
		fCenter=(DOUBLE)i/fXScale;
		lLeft=(LONG)ceil(fCenter-fWidth);
		lRight=(LONG)floor(fCenter+fWidth);
		dwCount=0;
		for (j=lLeft; j<=lRight; j++)
		{
			fWeight=Filter((fCenter-j)/fFilterScale)/fFilterScale;
			lPixel=j;
			if (lPixel<0)
				lPixel=-lPixel;
			if (lPixel>=m_bmBitmap.bmWidth)
				lPixel=m_bmBitmap.bmWidth-(lPixel-m_bmBitmap.bmWidth)-1;
			contributorX[dwCount++]=CONTRIBUTOR((DWORD)lPixel,fWeight);
		}


		for (j=0; j<m_bmBitmap.bmHeight; j++)
		{
			ZeroMemory(pWeight,sizeof(DOUBLE)*m_bBytesPixel);
			for (k=0; k<contributorX.GetSize(); k++)
			{
				contributor=contributorX[k];
				for (m=0; m<m_bBytesPixel; m++)
					pWeight[m]+=m_lpBits[j][contributor.st_dwPixel*
						m_bBytesPixel+m]*contributor.st_fWeight;
			}
			for (k=0; k<m_bBytesPixel; k++)
			{
				pWeight[k]+=0.5;
				if (pWeight[k]<0.0)
					pWeight[k]=0.0;
				if (pWeight[k]>255.0)
					pWeight[k]=255.0;
				lpColumn[j][k]=(BYTE)pWeight[k];
			}
			dwProgressCount++;
			if (dwProgressCount==dwInc)
			{
				dwProgressCount=0;
				pDlg->StepIt();
			}
		}

		for (j=0; j<dwHeight; j++)
		{
			ZeroMemory(pWeight,sizeof(DOUBLE)*m_bBytesPixel);
			for (k=0; k<contributorY[j].GetSize(); k++)
			{
				contributor=contributorY[j][k];
				for (m=0; m<m_bBytesPixel; m++)
					pWeight[m]+=lpColumn[contributor.st_dwPixel][m]*contributor.st_fWeight;
			}
			for (k=0; k<m_bBytesPixel; k++)
			{
				pWeight[k]+=0.5;
				if (pWeight[k]<0.0)
					pWeight[k]=0.0;
				if (pWeight[k]>255.0)
					pWeight[k]=255.0;

				lpDestBits[j*bm.bmWidthBytes+i*m_bBytesPixel+k]=(BYTE)pWeight[k];
			}
			dwProgressCount++;
			if (dwProgressCount==dwInc)
			{
				dwProgressCount=0;
				pDlg->StepIt();
			}
		}
	}
	contributorX.RemoveAll();
	contributorY.RemoveAll();

	DestroyMatrix((LPVOID *)lpColumn);
	HeapFree(pWeight);

	if (bChange)
	{
		CBitmapKit::Detach();
		CGdiObject::DeleteObject();
		CBitmapKit::Attach(hBitmap);
	}

	return hBitmap;
}

DOUBLE CBitmapKit::Filter(const DOUBLE fSample)
{
	DOUBLE fValue=fSample;
	if (fValue<0.0)
		fValue=-fValue;
	if (fValue<3.0)
		return sinc(fValue)*sinc(fValue/3.0);
	return 0.0;
}

DOUBLE CBitmapKit::sinc(const DOUBLE fValue)
{
	DOUBLE fTmp=fValue*M_PI;
	if (fTmp==0.0)
		return 1.0;
	return sin(fTmp)/fTmp;

}

BOOLEAN CBitmapKit::CheckBitmap(const BYTE bBitsPixel,
							   const DWORD dwWidthMultiple,
							   const DWORD dwHeightMultiple)
{
	if (bBitsPixel&&bBitsPixel!=m_bmBitmap.bmBitsPixel)
		return FALSE;
	if (dwWidthMultiple&&m_bmBitmap.bmWidth%dwWidthMultiple)
		return FALSE;
	if (dwHeightMultiple&&m_bmBitmap.bmHeight%dwHeightMultiple)
		return FALSE;

	return TRUE;
}
BOOLEAN CBitmapKit::IsPresent(void)
{
	return m_bPresent;
}
