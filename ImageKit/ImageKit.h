// ImageKit.h: interface for the ImageKit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEKIT_H__3FD33732_CCCB_4CD9_8B06_71357C4C678B__INCLUDED_)
#define AFX_IMAGEKIT_H__3FD33732_CCCB_4CD9_8B06_71357C4C678B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define BFT_BITMAP 0x4D42   /* 'BM' */

#define ALIGNBYTES(BITS) ((ULONG)((BITS+31)&(~31))>>3)  /* ULONG aligned ! */
#define BITS2BYTES(BITS) (ULONG)(BITS>>3)  /* ULONG aligned ! */

typedef struct tagIMAGEKITINFO
{
	DWORD dwPaddedWidth;
	DWORD dwWidth;
	DWORD dwHeight;
	BYTE bBitsPerPixel;
	BYTE bBytesPerPixel;
	DWORD dwImageSize;
//	DWORD dwSegmentWidth;
//	DWORD dwSegmentHeight;
} IMAGEKITINFO;

class ImageKit  
{
public:
	DWORD GetImageSize(void);
	void FromObject(const ImageKit &Image,const BYTE *pData);
	DOUBLE GetPSNRFullChannel(const ImageKit &Image);
	DOUBLE GetPSNR(const UINT uiChannel,const ImageKit &Image);
	ULONG GetImageWidth(void);
	ULONG GetImageHeight(void);
	HBITMAP GetHandle(CDC *pDC);
	BYTE *GetBits(void);
	void GetSegment(const CRect rSeg,FLOAT *pSeg,const FLOAT fMinValue,const FLOAT fRange);
	void DeletePadding(void);
	BOOL DecodeBMP(const CString &sFileName);
	ImageKit();
	ImageKit(const ImageKit &Image,const BYTE *pData);
	virtual ~ImageKit();

protected:
	void FillImageInfo(void);
	BITMAPINFO *m_bmInfo;
	BYTE *m_pImage;
	IMAGEKITINFO m_ImageInfo;
};

#endif // !defined(AFX_IMAGEKIT_H__3FD33732_CCCB_4CD9_8B06_71357C4C678B__INCLUDED_)
