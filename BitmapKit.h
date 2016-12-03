// BitmapKit.h: interface for the CBitmapKit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BITMAPKIT_H__BFFA80A8_7A7D_4A61_A50D_DE7AEBBDEDED__INCLUDED_)
#define AFX_BITMAPKIT_H__BFFA80A8_7A7D_4A61_A50D_DE7AEBBDEDED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define BITS2BYTES(BITS) (DWORD)(BITS>>3)
#define ALIGNBYTES(BYTES) ((DWORD)((BYTES+3)&(~3)))  /* DWORD aligned ! */

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

class CResampleDlg;

class CBitmapKit : public CBitmap  
{
public:
	DOUBLE sinc(const DOUBLE fValue);
	HBITMAP CreateDIBSection(const BITMAP &bm, LPVOID *lpBits);
	HBITMAP Resample(const DWORD dwWidth,const DWORD dwHeight,CResampleDlg *pDlg);
	BOOLEAN CreateBitmap(const BITMAP &bm, const LPVOID lpBits);
	BOOLEAN Attach(HBITMAP hBitmap);
	BYTE GetBytesPixel(void);
	BOOLEAN SetBits(const LPVOID lpBits);
	LPBYTE * GetBits(void);
	DOUBLE GetPSNRFullChannel(const CBitmapKit *pBitmapKit);
	DOUBLE GetPSNR(const CBitmapKit *pBitmapKit, const BYTE bChannel);
	DWORD GetColors(void);
	BYTE GetBitsPixel(void);
	DWORD GetHeight(void);
	DWORD GetSizeClear(void);
	DWORD GetSizePixel(void);
	DWORD GetWidth(void);
	HBITMAP Detach(void);
	DWORD GetWidthBytes(void);
	DWORD GetSizeBytes(void);
	void GetSegment(
		const CRect &rect,
		PFLOAT pSegment,
		const BYTE bColor,
		const FLOAT fMinValue,
		const FLOAT fRange);
	HBITMAP Scale(CDC *pDC,const DWORD dwWidth, const DWORD dwHeight);
	BOOLEAN LoadBMP(const CString &strFileName);
	CBitmapKit();
	virtual ~CBitmapKit();

protected:
	DOUBLE Filter(const DOUBLE fSample);
	LPBYTE *m_lpBits;
	BYTE m_bBytesPixel;
	BOOLEAN InitBitmapBits(void);
	BITMAP m_bmBitmap;
	BOOLEAN m_bPresent;
};

#endif // !defined(AFX_BITMAPKIT_H__BFFA80A8_7A7D_4A61_A50D_DE7AEBBDEDED__INCLUDED_)
