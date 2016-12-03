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
//	m_bmInfo=(BITMAPINFO *)new BYTE[sizeof(*Image.m_bmInfo)];
//	ASSERT(m_bmInfo!=NULL);
//	CopyMemory(m_bmInfo,Image.m_bmInfo,sizeof(*Image.m_bmInfo));
//	CopyMemory(&m_ImageInfo,&Image.m_ImageInfo,sizeof(IMAGEKITINFO));
//	m_pImage=new BYTE[m_ImageInfo.dwImageSize];
//	ASSERT(m_pImage!=NULL);
//	CopyMemory(m_pImage,pData,m_ImageInfo.dwImageSize);
	FromObject(Image,pData);
}

ImageKit::~ImageKit()
{

}

BOOLEAN ImageKit::LoadBMP(const CString &sFileName)
{
	HBITMAP hBitmap=(HBITMAP)LoadImage(NULL,sFileName,IMAGE_BITMAP,0,0,
		LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	if (hBitmap==NULL)
		return FALSE;
//	m_bmMap;
//	if (GetObject(m_hBitmap,sizeof(BITMAP),&bm);

//	CFile BMPFile;
//	BMPFile.Open(sFileName,CFile::modeRead);
//
//	BITMAPFILEHEADER bmfHeader;
//
//	BMPFile.Read(&bmfHeader,sizeof(BITMAPFILEHEADER));
//
//	if (bmfHeader.bfType!=BFT_BITMAP)
//	{
//		BMPFile.Close();
//		return FALSE;
//	}
//
//	BITMAPINFOHEADER bmiHeader;
//	BMPFile.Read(&bmiHeader,sizeof(BITMAPINFOHEADER));
//
//	switch (bmiHeader.biBitCount)
//	{
//	case 8:
//		m_bmInfo=(BITMAPINFO *)(new BYTE[sizeof(BITMAPINFOHEADER) + 
//			((1<<bmiHeader.biBitCount) * sizeof(RGBQUAD))]);
//
//		ASSERT(m_bmInfo!=NULL);
//		CopyMemory(&m_bmInfo->bmiHeader,&bmiHeader,sizeof(BITMAPINFOHEADER));
//
//		BMPFile.Read(m_bmInfo->bmiColors,((1<<bmiHeader.biBitCount) * sizeof(RGBQUAD)));
//		break;
//	case 24:
//		m_bmInfo=(BITMAPINFO *)(new BYTE[sizeof(BITMAPINFO)]);
//		ASSERT(m_bmInfo!=NULL);
//		CopyMemory(&m_bmInfo->bmiHeader,&bmiHeader,sizeof(BITMAPINFOHEADER));
//		break;
//	}
//	
//
//	m_pImage=new BYTE[bmfHeader.bfSize - bmfHeader.bfOffBits];
//	ASSERT(m_pImage!=NULL);
//
//	BMPFile.Read(m_pImage,sizeof(BYTE)*(bmfHeader.bfSize - bmfHeader.bfOffBits));
//	
//	BMPFile.Close();
//	FillImageInfo();
	return TRUE;
}

void ImageKit::FillImageInfo()
{
	m_ImageInfo.st_bBitsPerPixel=(BYTE)m_bmInfo->bmiHeader.biBitCount;
	m_ImageInfo.st_bBytesPerPixel=(BYTE)BITS2BYTES(m_bmInfo->bmiHeader.biBitCount);
	m_ImageInfo.st_dwWidth=m_bmInfo->bmiHeader.biWidth;
	m_ImageInfo.st_dwHeight=m_bmInfo->bmiHeader.biHeight;
	m_ImageInfo.st_dwPaddedWidth=
		ALIGNBYTES(m_ImageInfo.st_dwWidth*m_ImageInfo.st_bBitsPerPixel);
	m_ImageInfo.st_dwImageSize=
		BITS2BYTES(m_ImageInfo.st_dwHeight*m_ImageInfo.st_dwWidth*m_ImageInfo.st_bBitsPerPixel);
}

void ImageKit::DeletePadding()
{
	DWORD dwBytesPerRow=BITS2BYTES(m_ImageInfo.st_dwWidth*m_ImageInfo.st_bBitsPerPixel);
	if (dwBytesPerRow==m_ImageInfo.st_dwPaddedWidth)
		return;

	BYTE *pClearImage=new BYTE[dwBytesPerRow*m_ImageInfo.st_dwHeight];
	ASSERT(pClearImage!=NULL);
	DWORD i;
	for (i=0; i<m_ImageInfo.st_dwHeight; i++)
		CopyMemory(pClearImage+i*dwBytesPerRow,
			m_pImage+i*m_ImageInfo.st_dwPaddedWidth,dwBytesPerRow);

	delete []m_pImage;
	m_pImage=new BYTE[dwBytesPerRow*m_ImageInfo.st_dwHeight];
	ASSERT(m_pImage!=NULL);
	CopyMemory(m_pImage,pClearImage,m_ImageInfo.st_dwImageSize);
	delete []pClearImage;
}

void ImageKit::GetSegment(const CRect rSeg, FLOAT *pSeg,
						  const BYTE bColor,
						  const FLOAT fMinValue,
						  const FLOAT fRange)
{
	DWORD dwWidth=rSeg.Width();
	ASSERT(rSeg.left<m_ImageInfo.st_dwWidth);
	ASSERT(rSeg.top<m_ImageInfo.st_dwHeight);

	DWORD i,j;
	for (i=0; i<rSeg.Height(); i++)
		for (j=0; j<rSeg.Width(); j++)
			if ((i+rSeg.top)<m_ImageInfo.st_dwHeight&&(j+rSeg.left)<m_ImageInfo.st_dwWidth)
				pSeg[i*dwWidth+j]=fMinValue+
					m_pImage[((i+rSeg.top)*m_ImageInfo.st_dwWidth+
						(j+rSeg.left))*m_ImageInfo.st_bBytesPerPixel+bColor]/255.0f*fRange;
			else
				pSeg[i*dwWidth+j]=0.0f;
}

BYTE *ImageKit::GetBits()
{
	return m_pImage;
}

HBITMAP ImageKit::GetHandle(CDC *pDC)
{
	//ASSERT(pDC!=NULL);
//	return m_hBitmap;
	HBITMAP hBitmap=CreateDIBitmap(pDC->GetSafeHdc(), 
		&m_bmInfo->bmiHeader, CBM_INIT, m_pImage, m_bmInfo, DIB_RGB_COLORS);
	return hBitmap;

}

ULONG ImageKit::GetImageHeight()
{
	return m_ImageInfo.st_dwHeight;
}

ULONG ImageKit::GetImageWidth()
{
	return m_ImageInfo.st_dwWidth;
}

DOUBLE ImageKit::GetPSNR(const BYTE bChannel, const ImageKit &Image)
{
	const DWORD dwChannelWidth=65025; /*255*255*/
	if (bChannel>=m_ImageInfo.st_bBytesPerPixel)
		return -1.0;
	DOUBLE fDist;
	DOUBLE fMSE=0.0;
	DWORD dwSize=m_ImageInfo.st_dwWidth*m_ImageInfo.st_dwHeight;
	DWORD i;
	for (i=0; i<dwSize; i++)
	{
		fDist=(FLOAT)m_pImage[i*m_ImageInfo.st_bBytesPerPixel+bChannel]-
			(FLOAT)Image.m_pImage[i*m_ImageInfo.st_bBytesPerPixel+bChannel];
		fMSE+=fDist*fDist;
	}
	fMSE/=dwSize;

	return 10.0*log10(dwChannelWidth/fMSE);
}

DOUBLE ImageKit::GetPSNRFullChannel(const ImageKit &Image)
{
	const DWORD dwChannelWidth=(1<<m_ImageInfo.st_bBitsPerPixel)-1;
	DOUBLE fDist;
	DOUBLE fMSE=0.0f;
	DWORD dwSize=m_ImageInfo.st_dwWidth*m_ImageInfo.st_dwHeight;
	DWORD dwDist1;
	DWORD dwDist2;
	DWORD i;
	for (i=0; i<dwSize; i++)
	{
		dwDist1=dwDist2=0;
		CopyMemory(&dwDist1,&m_pImage[i*m_ImageInfo.st_bBytesPerPixel],
			m_ImageInfo.st_bBytesPerPixel);
		CopyMemory(&dwDist2,&Image.m_pImage[i*m_ImageInfo.st_bBytesPerPixel],
			m_ImageInfo.st_bBytesPerPixel);
		fDist=(DOUBLE)dwDist1-(DOUBLE)dwDist2;
		fMSE+=fDist*fDist;
	}
	fMSE/=dwSize;

	return 10.0*(2.0*log10(dwChannelWidth)-log10(fMSE));
}

void ImageKit::FromObject(const ImageKit &Image, const BYTE *pData)
{
	
	CopyMemory(&m_ImageInfo,&Image.m_ImageInfo,sizeof(IMAGEKITINFO));

	if (m_bmInfo!=NULL)
		delete []m_bmInfo;

	DWORD dwPaletteSize;
	switch (m_ImageInfo.st_bBitsPerPixel)
	{
	case 8:
		dwPaletteSize=sizeof(BITMAPINFOHEADER) + 
			((1<<m_ImageInfo.st_bBitsPerPixel) * sizeof(RGBQUAD));
		m_bmInfo=(BITMAPINFO *)(new BYTE[dwPaletteSize]);
		ASSERT(m_bmInfo!=NULL);
		CopyMemory(m_bmInfo,Image.m_bmInfo,dwPaletteSize);
		break;
	case 24:
		m_bmInfo=(BITMAPINFO *)(new BYTE[sizeof(BITMAPINFO)]);
		ASSERT(m_bmInfo!=NULL);
		CopyMemory(m_bmInfo,Image.m_bmInfo,sizeof(BITMAPINFO));
		break;
	}

	if (m_pImage!=NULL)
		delete []m_pImage;
	m_pImage=new BYTE[m_ImageInfo.st_dwImageSize];
	ASSERT(m_pImage!=NULL);
	CopyMemory(m_pImage,pData,m_ImageInfo.st_dwImageSize);
}

DWORD ImageKit::GetImageSize()
{
	return m_ImageInfo.st_dwImageSize;
}

BYTE ImageKit::GetBitsPerPixel()
{
	return m_ImageInfo.st_bBitsPerPixel;
}

DWORD ImageKit::GetColors()
{
	return (1<<m_ImageInfo.st_bBitsPerPixel);
}

BYTE ImageKit::GetBytesPerPixel()
{
	return m_ImageInfo.st_bBytesPerPixel;
}

HBITMAP ImageKit::Resample(CDC *pDC,
						   const DWORD dwWidth,
						   const DWORD dwHeight,
						   const BOOLEAN bChange)
{
	const DOUBLE fXScale=(DOUBLE)dwWidth/m_ImageInfo.st_dwWidth;
	const DOUBLE fYScale=(DOUBLE)dwHeight/m_ImageInfo.st_dwHeight;
	
	DOUBLE fWidth;
	DOUBLE fFilterScale;
	
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

	DWORD i,k,m;
	LONG j; 
	LONG lPixel;
	DOUBLE fCenter;
	LONG lLeft;
	LONG lRight;
	DOUBLE fWeight;

	CArray<CList<CONTRIBUTOR,CONTRIBUTOR>,CList<CONTRIBUTOR,CONTRIBUTOR>&> array;
	array.SetSize(dwWidth);
	for (i=0; i<dwWidth; i++)
	{
		fCenter=(DOUBLE)i/fXScale;
		lLeft=(LONG)ceil(fCenter-fWidth);
		lRight=(LONG)floor(fCenter+fWidth);
		for (j=lLeft; j<=lRight; j++)
		{
			fWeight=Filter((fCenter-j)/fFilterScale)/fFilterScale;
			lPixel=j;
			if (lPixel<0)
				lPixel=-lPixel;
			if (lPixel>=m_ImageInfo.st_dwWidth)
				lPixel=m_ImageInfo.st_dwWidth-(lPixel-m_ImageInfo.st_dwWidth)-1;
			array[i].AddTail(CONTRIBUTOR((DWORD)lPixel,fWeight));
		}
	}

	BYTE **pTmpImage=AllocateByteMatrix(m_ImageInfo.st_dwHeight,
		dwWidth*m_ImageInfo.st_bBytesPerPixel);
	ASSERT(pTmpImage!=NULL);

	DOUBLE *pWeight=new DOUBLE[m_ImageInfo.st_bBytesPerPixel];
	POSITION pos;
	CONTRIBUTOR contributor;
	ASSERT(pWeight!=NULL);
	for (i=0; i<m_ImageInfo.st_dwHeight; i++)
		for (j=0; j<dwWidth; j++)
		{
			ZeroMemory(pWeight,sizeof(DOUBLE)*m_ImageInfo.st_bBytesPerPixel);
			pos=array[(DWORD)j].GetHeadPosition();
			for (k=0; k<array[(DWORD)j].GetCount(); k++)
			{
				contributor=array[(DWORD)j].GetNext(pos);
				for (m=0; m<m_ImageInfo.st_bBytesPerPixel; m++)
					pWeight[m]+=m_pImage[(i*m_ImageInfo.st_dwWidth+contributor.st_dwPixel)*
						m_ImageInfo.st_bBytesPerPixel+m]*contributor.st_fWeight;
			}
			for (k=0; k<m_ImageInfo.st_bBytesPerPixel; k++)
			{
				pWeight[k]+=0.5;
				if (pWeight[k]<0.0)
					pWeight[k]=0.0;
				if (pWeight[k]>255.0)
					pWeight[k]=255.0;
				pTmpImage[i][j*m_ImageInfo.st_bBytesPerPixel+k]=(BYTE)pWeight[k];
			}
		}
	array.RemoveAll();
	array.SetSize(dwHeight);
	
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

	for (i=0; i<dwHeight; i++)
	{
		fCenter=(DOUBLE)i/fYScale;
		lLeft=(LONG)(fCenter-fWidth+0.5);
		lRight=(LONG)(fCenter+fWidth-0.5);
		for (j=lLeft; j<=lRight; j++)
		{
			fWeight=Filter((fCenter-j)/fFilterScale)/fFilterScale;
			lPixel=j;
			if (lPixel<0)
				lPixel=-lPixel;
			if (lPixel>=m_ImageInfo.st_dwHeight)
				lPixel=m_ImageInfo.st_dwHeight-(lPixel-m_ImageInfo.st_dwHeight)-1;
			array[i].AddTail(CONTRIBUTOR((DWORD)lPixel,fWeight));
		}
	}

	BYTE **pResultImage=AllocateByteMatrix(dwHeight,dwWidth*m_ImageInfo.st_bBytesPerPixel);
	ASSERT(pResultImage!=NULL);
	for (i=0; i<dwWidth; i++)
		for (j=0; j<dwHeight; j++)
		{
			ZeroMemory(pWeight,sizeof(DOUBLE)*m_ImageInfo.st_bBytesPerPixel);
			pos=array[(DWORD)j].GetHeadPosition();
			for (k=0; k<array[(DWORD)j].GetCount(); k++)
			{
				contributor=array[(DWORD)j].GetNext(pos);
				for (m=0; m<m_ImageInfo.st_bBytesPerPixel; m++)
					pWeight[m]+=pTmpImage[contributor.st_dwPixel][i*
						m_ImageInfo.st_bBytesPerPixel+m]*contributor.st_fWeight;
			}
			for (k=0; k<m_ImageInfo.st_bBytesPerPixel; k++)
			{
				pWeight[k]+=0.5;
				if (pWeight[k]<0.0)
					pWeight[k]=0.0;
				if (pWeight[k]>255.0)
					pWeight[k]=255.0;
				pResultImage[j][i*m_ImageInfo.st_bBytesPerPixel+k]=(BYTE)pWeight[k];
			}
		}

	delete []pWeight;
	array.RemoveAll();
	DestroyMatrix((LPVOID *)pTmpImage);

	ASSERT(pDC!=NULL);
	m_bmInfo->bmiHeader.biWidth=dwWidth;
	m_bmInfo->bmiHeader.biHeight=dwHeight;
	
	HBITMAP hBitmap=CreateDIBitmap(pDC->GetSafeHdc(), 
		&m_bmInfo->bmiHeader, CBM_INIT, pResultImage[0], m_bmInfo, DIB_RGB_COLORS);

	if (bChange)
	{
		delete []m_pImage;
		m_pImage=new BYTE[dwHeight,dwWidth*m_ImageInfo.st_bBytesPerPixel];
		ASSERT(m_pImage!=NULL);
		CopyMemory(m_pImage,pResultImage[0],dwHeight*dwWidth*m_ImageInfo.st_bBytesPerPixel);
		FillImageInfo();
		DestroyMatrix((LPVOID *)pResultImage);
		return hBitmap;
	}

	m_bmInfo->bmiHeader.biWidth=m_ImageInfo.st_dwWidth;
	m_bmInfo->bmiHeader.biHeight=m_ImageInfo.st_dwHeight;
	DestroyMatrix((LPVOID *)pResultImage);
	return hBitmap;
}

DOUBLE ImageKit::Filter(const DOUBLE fSample)
{
	DOUBLE fValue=fSample;
	if (fValue<0.0)
		fValue=-fValue;
	if (fValue<3.0)
		return sinc(fValue)*sinc(fValue/3.0);
	return 0.0;
}

DOUBLE ImageKit::sinc(const double fValue)
{
	DOUBLE fTmp=fValue*M_PI;
	if (fTmp==0.0)
		return 1.0;
	return sin(fTmp)/fTmp;
}

//return (d>=0 ? (int)(d+0.5) : (int)(d-0.5));