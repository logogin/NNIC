// ImageKit.h: interface for the ImageKit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEKIT_H__3FD33732_CCCB_4CD9_8B06_71357C4C678B__INCLUDED_)
#define AFX_IMAGEKIT_H__3FD33732_CCCB_4CD9_8B06_71357C4C678B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define BFT_BITMAP 0x4D42   /* 'BM' */

#define ALIGNBYTES(BITS) ((DWORD)((BITS+31)&(~31))>>3)  /* ULONG aligned ! */
#define BITS2BYTES(BITS) (DWORD)(BITS>>3)  /* ULONG aligned ! */

#define M_PI 3.14159265358979323846
#define FILTER_WIDTH (3.0)

typedef struct tagCONTRIBUTOR
{
	tagCONTRIBUTOR(void)
	{
	
	}

	tagCONTRIBUTOR(const DWORD dwPixel, const DOUBLE fWeight)
	{
		st_dwPixel=dwPixel;
		st_fWeight=fWeight;
	}

	tagCONTRIBUTOR operator=(const tagCONTRIBUTOR &contributor)
	{
		st_dwPixel=contributor.st_dwPixel;
		st_fWeight=contributor.st_fWeight;

		return *this;
	}

	DWORD st_dwPixel;
	DOUBLE st_fWeight;
} CONTRIBUTOR;

typedef struct tagIMAGEKITINFO
{
	DWORD st_dwPaddedWidth;
	DWORD st_dwWidth;
	DWORD st_dwHeight;
	BYTE st_bBitsPerPixel;
	BYTE st_bBytesPerPixel;
	DWORD st_dwImageSize;
} IMAGEKITINFO;

class ImageKit  
{
public:
	DOUBLE sinc(const double fValue);
	HBITMAP Resample(
		CDC *pDC,
		const DWORD dwDestHeight,
		const DWORD dwDestWidth,
		const BOOLEAN bChange=FALSE);
	BYTE GetBytesPerPixel(void);
	DWORD GetColors(void);
	BYTE GetBitsPerPixel(void);
	DWORD GetImageSize(void);
	void FromObject(const ImageKit &Image,const BYTE *pData);
	DOUBLE GetPSNRFullChannel(const ImageKit &Image);
	DOUBLE GetPSNR(const BYTE bChannel,const ImageKit &Image);
	DWORD GetImageWidth(void);
	DWORD GetImageHeight(void);
	HBITMAP GetHandle(CDC *pDC);
	BYTE *GetBits(void);
	void GetSegment(const CRect rSeg,
		FLOAT *pSeg,
		const BYTE bColor,
		const FLOAT fMinValue,
		const FLOAT fRange);
	void DeletePadding(void);
	BOOLEAN DecodeBMP(const CString &sFileName);
	ImageKit();
	ImageKit(const ImageKit &Image,const BYTE *pData);
	virtual ~ImageKit();

protected:
	DOUBLE Filter(const DOUBLE fSample);
	void FillImageInfo(void);
	BITMAPINFO *m_bmInfo;
	BYTE *m_pImage;
	IMAGEKITINFO m_ImageInfo;
};

#endif // !defined(AFX_IMAGEKIT_H__3FD33732_CCCB_4CD9_8B06_71357C4C678B__INCLUDED_)
