// ImageKit.cpp: implementation of the ImageKit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ImageKit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ImageKit::ImageKit():m_bmInfo(NULL),m_pImage(NULL)
{

}

ImageKit::ImageKit(const ImageKit &Image,const BYTE *pData)
{
	m_bmInfo=(BITMAPINFO *)new BYTE[sizeof(*Image.m_bmInfo)];
	ASSERT(m_bmInfo!=NULL);
	CopyMemory(m_bmInfo,Image.m_bmInfo,sizeof(*Image.m_bmInfo));
	CopyMemory(&m_ImageInfo,&Image.m_ImageInfo,sizeof(IMAGEKITINFO));
	m_pImage=new BYTE[m_ImageInfo.dwImageSize];
	ASSERT(m_pImage!=NULL);
	CopyMemory(m_pImage,pData,m_ImageInfo.dwImageSize);
}

ImageKit::~ImageKit()
{

}

BOOL ImageKit::DecodeBMP(const CString &sFileName)
{
	CFile BMPFile;
	BMPFile.Open(sFileName,CFile::modeRead);

	BITMAPFILEHEADER bmfHeader;

	BMPFile.Read(&bmfHeader,sizeof(BITMAPFILEHEADER));

	if (bmfHeader.bfType!=BFT_BITMAP)
	{
		BMPFile.Close();
		return FALSE;
	}

	BITMAPINFOHEADER bmiHeader;
	BMPFile.Read(&bmiHeader,sizeof(BITMAPINFOHEADER));

	switch (bmiHeader.biBitCount)
	{
	case 8:
		m_bmInfo=(BITMAPINFO *)(new BYTE[sizeof(BITMAPINFOHEADER) + 
			((1<<bmiHeader.biBitCount) * sizeof(RGBQUAD))]);

		ASSERT(m_bmInfo!=NULL);
		ASSERT(memcpy(&m_bmInfo->bmiHeader,&bmiHeader,sizeof(BITMAPINFOHEADER))!=NULL);

		BMPFile.Read(m_bmInfo->bmiColors,((1<<bmiHeader.biBitCount) * sizeof(RGBQUAD)));
		break;
	case 24:
		m_bmInfo=(BITMAPINFO *)(new BYTE[sizeof(BITMAPINFO)]);
		ASSERT(m_bmInfo!=NULL);
		ASSERT(memcpy(&m_bmInfo->bmiHeader,&bmiHeader,sizeof(BITMAPINFOHEADER))!=NULL);
		break;
	}
	

	m_pImage=new BYTE[bmfHeader.bfSize - bmfHeader.bfOffBits];
	ASSERT(m_pImage!=NULL);

	BMPFile.Read(m_pImage,sizeof(BYTE)*(bmfHeader.bfSize - bmfHeader.bfOffBits));
	
	BMPFile.Close();
	FillImageInfo();
	return TRUE;
}

void ImageKit::FillImageInfo()
{
	m_ImageInfo.bBitsPerPixel=(BYTE)m_bmInfo->bmiHeader.biBitCount;
	m_ImageInfo.bBytesPerPixel=(BYTE)BITS2BYTES(m_bmInfo->bmiHeader.biBitCount);
	m_ImageInfo.dwWidth=m_bmInfo->bmiHeader.biWidth;
	m_ImageInfo.dwHeight=m_bmInfo->bmiHeader.biHeight;
	m_ImageInfo.dwPaddedWidth=ALIGNBYTES(m_ImageInfo.dwWidth*m_ImageInfo.bBitsPerPixel);
	m_ImageInfo.dwImageSize=BITS2BYTES(m_ImageInfo.dwHeight*m_ImageInfo.dwWidth*m_ImageInfo.bBitsPerPixel);
}

void ImageKit::DeletePadding()
{
	if (m_ImageInfo.dwWidth==m_ImageInfo.dwPaddedWidth)
		return;

	BYTE *pClearImage=new BYTE[BITS2BYTES(m_ImageInfo.dwHeight*m_ImageInfo.dwWidth
		*m_ImageInfo.bBitsPerPixel)];
	ASSERT(pClearImage!=NULL);
	for (ULONG i=0; i<m_ImageInfo.dwHeight; i++)
		ASSERT(memcpy(pClearImage+m_ImageInfo.dwWidth*i,m_pImage+m_ImageInfo.dwPaddedWidth*i,
			sizeof(BYTE)*m_ImageInfo.dwWidth)!=NULL);

	delete []m_pImage;
	ASSERT(memcpy(m_pImage,pClearImage,sizeof(BYTE)*m_ImageInfo.dwImageSize)!=NULL);
	delete []pClearImage;
}

void ImageKit::GetSegment(const CRect rSeg, FLOAT *pSeg, const FLOAT fMinValue,const FLOAT fRange)
{
	ULONG uiWidth=rSeg.Width();
	ASSERT(rSeg.left<m_ImageInfo.dwWidth);
	ASSERT(rSeg.top<m_ImageInfo.dwHeight);
	
	for (ULONG i=0; i<rSeg.Height(); i++)
		for (ULONG j=0; j<rSeg.Width(); j++)
			if ((i+rSeg.top)<m_ImageInfo.dwHeight&&(j+rSeg.left)<m_ImageInfo.dwWidth)
				pSeg[i*uiWidth+j]=fMinValue+
					m_pImage[(i+rSeg.top)*m_ImageInfo.dwWidth+(j+rSeg.left)]/255.0f*fRange;
			else
				pSeg[i*uiWidth+j]=0.0f;
}

BYTE *ImageKit::GetBits()
{
	return m_pImage;
}

HBITMAP ImageKit::GetHandle(CDC *pDC)
{
	ASSERT(pDC!=NULL);
	HBITMAP hBitmap =CreateDIBitmap(pDC->GetSafeHdc(), 
		&m_bmInfo->bmiHeader, CBM_INIT, m_pImage, m_bmInfo, DIB_RGB_COLORS);
	return hBitmap;
}

ULONG ImageKit::GetImageHeight()
{
	return m_ImageInfo.dwHeight;
}

ULONG ImageKit::GetImageWidth()
{
	return m_ImageInfo.dwWidth;
}

DOUBLE ImageKit::GetPSNR(const UINT uiChannel, const ImageKit &Image)
{
	const UINT uiChannelWidth=65025; /*255*255*/
	if (uiChannel>=m_ImageInfo.bBytesPerPixel)
		return -1.0;
	DOUBLE fDist;
	DOUBLE fMSE=0.0;
	DOUBLE uiSize=m_ImageInfo.dwWidth*m_ImageInfo.dwHeight;
	for (UINT i=0; i<uiSize; i++)
	{
		fDist=(FLOAT)m_pImage[i*m_ImageInfo.bBytesPerPixel+uiChannel]-
			(FLOAT)Image.m_pImage[i*m_ImageInfo.bBytesPerPixel+uiChannel];
		fMSE+=fDist*fDist;
	}
	fMSE/=uiSize;

	return 10.0*log10(uiChannelWidth/fMSE);
}

DOUBLE ImageKit::GetPSNRFullChannel(const ImageKit &Image)
{
	const UINT uiChannelWidth=(1<<m_ImageInfo.bBitsPerPixel)-1;
	DOUBLE fDist;
	DOUBLE fMSE=0.0f;
	UINT uiSize=m_ImageInfo.dwWidth*m_ImageInfo.dwHeight;
	ULONG uiDist1;
	ULONG uiDist2;
	for (UINT i=0; i<uiSize; i++)
	{
		uiDist1=uiDist2=0;
		CopyMemory(&uiDist1,&m_pImage[i*m_ImageInfo.bBytesPerPixel],m_ImageInfo.bBytesPerPixel);
		CopyMemory(&uiDist2,&Image.m_pImage[i*m_ImageInfo.bBytesPerPixel],m_ImageInfo.bBytesPerPixel);
		fDist=(DOUBLE)uiDist1-(DOUBLE)uiDist2;
		fMSE+=fDist*fDist;
	}
	fMSE/=uiSize;

	return 10.0*(2.0*log10(uiChannelWidth)-log10(fMSE));
}

void ImageKit::FromObject(const ImageKit &Image, const BYTE *pData)
{
	
	CopyMemory(&m_ImageInfo,&Image.m_ImageInfo,sizeof(IMAGEKITINFO));

	if (m_bmInfo!=NULL)
		delete []m_bmInfo;

	ULONG ulPaletteSize;
	switch (m_ImageInfo.bBitsPerPixel)
	{
	case 8:
		ulPaletteSize=sizeof(BITMAPINFOHEADER) + 
			((1<<m_ImageInfo.bBitsPerPixel) * sizeof(RGBQUAD));
		m_bmInfo=(BITMAPINFO *)(new BYTE[ulPaletteSize]);
		ASSERT(m_bmInfo!=NULL);
		CopyMemory(m_bmInfo,Image.m_bmInfo,ulPaletteSize);
		break;
	case 24:
		m_bmInfo=(BITMAPINFO *)(new BYTE[sizeof(BITMAPINFO)]);
		ASSERT(m_bmInfo!=NULL);
		CopyMemory(m_bmInfo,Image.m_bmInfo,sizeof(BITMAPINFO));
		break;
	}

	if (m_pImage!=NULL)
		delete []m_pImage;
	m_pImage=new BYTE[m_ImageInfo.dwImageSize];
	ASSERT(m_pImage!=NULL);
	CopyMemory(m_pImage,pData,m_ImageInfo.dwImageSize);
}

DWORD ImageKit::GetImageSize()
{
	return m_ImageInfo.dwImageSize;
}