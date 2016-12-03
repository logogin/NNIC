// ImageKit.cpp: implementation of the ImageKit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NeuroNet.h"
#include "ImageKit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ImageKit::ImageKit()
{

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
	m_ImageInfo.bBitsPerPixel=m_bmInfo->bmiHeader.biBitCount;
	m_ImageInfo.bBytesPerPixel=BITS2BYTES(m_bmInfo->bmiHeader.biBitCount);
	m_ImageInfo.dwWidth=m_bmInfo->bmiHeader.biWidth;
	m_ImageInfo.dwHeight=m_bmInfo->bmiHeader.biHeight;
	m_ImageInfo.dwAlignedWidth=ALIGNBYTES(m_ImageInfo.dwWidth*m_ImageInfo.bBitsPerPixel);
	m_ImageInfo.dwImageSize=BITS2BYTES(m_ImageInfo.dwHeight*m_ImageInfo.dwWidth*m_ImageInfo.bBitsPerPixel);
}

void ImageKit::DeleteAlignment()
{
	if (m_ImageInfo.dwWidth==m_ImageInfo.dwAlignedWidth)
		return;

	BYTE *pClearImage=new BYTE[BITS2BYTES(m_ImageInfo.dwHeight*m_ImageInfo.dwWidth
		*m_ImageInfo.bBitsPerPixel)];
	ASSERT(pClearImage!=NULL);
	for (ULONG i=0; i<m_ImageInfo.dwHeight; i++)
		ASSERT(memcpy(pClearImage+m_ImageInfo.dwWidth*i,m_pImage+m_ImageInfo.dwAlignedWidth*i,
			sizeof(BYTE)*m_ImageInfo.dwWidth)!=NULL);

	delete []m_pImage;
	ASSERT(memcpy(m_pImage,pClearImage,sizeof(BYTE)*m_ImageInfo.dwImageSize)!=NULL);
	delete []pClearImage;
}

void ImageKit::GetSegment(const CRect rSeg, FLOAT *pSeg, const FLOAT fMinValue,const FLOAT fRange)
{
	UINT uiWidth=rSeg.Width();
	ASSERT(rSeg.left<m_ImageInfo.dwWidth);
	ASSERT(rSeg.top<m_ImageInfo.dwHeight);
	
	for (UINT i=0; i<rSeg.Height(); i++)
		for (UINT j=0; j<rSeg.Width(); j++)
			if ((i+rSeg.top)<m_ImageInfo.dwHeight&&(j+rSeg.left)<m_ImageInfo.dwWidth)
				pSeg[i*uiWidth+j]=fMinValue+
					m_pImage[(i+rSeg.top)*m_ImageInfo.dwWidth+(j+rSeg.left)]/255.0*fRange;
			else
				pSeg[i*uiWidth+j]=0.0f;
}

BYTE *ImageKit::GetBits()
{
	return m_pImage;
}

void Zoom(const CSize DestSize,const DOUBLE fFilterWidth)
{
	DOUBLE fXScale=(DOUBLE)m_ImageInfo.dwWidth/DestSize.cx;
	DOUBLE fYScale=(DOUBLE)m_ImageInfo.dwHeight/DestSize.cy;


}