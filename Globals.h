// Globals.h: interface for the Globals class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLOBALS_H__76BA1620_20A5_4B58_9D57_375E431EC133__INCLUDED_)
#define AFX_GLOBALS_H__76BA1620_20A5_4B58_9D57_375E431EC133__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct tagOPTIONSBP
{
	tagOPTIONSBP(void)
	{
	
	}
	tagOPTIONSBP(
		const BYTE bLearnMode,
		const BYTE bSigmoidType,
		const FLOAT fSigmoidAlpha,
		const BOOLEAN bUseBiases,
		const BOOLEAN bUseMomentum,
		const DWORD dwLearnCycles,
		const FLOAT fMinError,
		const WORD wPatterns,
		const WORD wInputBlock,
		const WORD wHiddenBlock,
		const BOOLEAN bSecondHidden,
		const WORD wNeurons,
		const FLOAT fInitLearnRate,
		const FLOAT fFinalLearnRate,
		const DWORD dwLearnSteps,
		const FLOAT fLearnChangeRate,
		const FLOAT fInitMoment,
		const FLOAT fFinalMoment,
		const DWORD dwMomentSteps,
		const FLOAT fMomentChangeRate)
	{
		st_bLearnMode=bLearnMode;
		st_bSigmoidType=bSigmoidType;
		st_fSigmoidAlpha=fSigmoidAlpha;
		st_bUseBiases=bUseBiases;
		st_bUseMomentum=bUseMomentum;
		st_dwLearnCycles=dwLearnCycles;
		st_fMinError=fMinError;
		st_wPatterns=wPatterns;
		st_wInputBlock=wInputBlock;
		st_wHiddenBlock=wHiddenBlock;
		st_bSecondHidden=bSecondHidden;
		st_wNeurons=wNeurons;
		st_fInitLearnRate=fInitLearnRate;
		st_fFinalLearnRate=fFinalLearnRate;
		st_dwLearnSteps=dwLearnSteps;
		st_fLearnChangeRate=fLearnChangeRate;
		st_fInitMoment=fInitMoment;
		st_fFinalMoment=fFinalMoment;
		st_dwMomentSteps=dwMomentSteps;
		st_fMomentChangeRate=fMomentChangeRate;
	}

	BYTE st_bLearnMode;
	BYTE st_bSigmoidType;
	FLOAT st_fSigmoidAlpha;
	BOOLEAN st_bUseBiases;
	BOOLEAN st_bUseMomentum;
	DWORD st_dwLearnCycles;
	FLOAT st_fMinError;
	WORD st_wPatterns;
	WORD st_wInputBlock;
	WORD st_wHiddenBlock;
	BOOLEAN st_bSecondHidden;
	WORD st_wNeurons;
	FLOAT st_fInitLearnRate;
	FLOAT st_fFinalLearnRate;
	DWORD st_dwLearnSteps;
	FLOAT st_fLearnChangeRate;
	FLOAT st_fInitMoment;
	FLOAT st_fFinalMoment;
	DWORD st_dwMomentSteps;
	FLOAT st_fMomentChangeRate;
} OPTIONSBP;

typedef struct tagOPTIONSCP
{
	tagOPTIONSCP(void)
	{
	
	}

	tagOPTIONSCP(
		const BYTE bNetType,
		const WORD wClusters,
		const DWORD dwCycles,
		const FLOAT fMinDist,
		const BOOLEAN bTrainNeighbours,
		const FLOAT fInitLearnRate,
		const FLOAT fFinalLearnRate,
		const FLOAT fLearnChangeRate,
		const DWORD dwLearnRateSteps,
		const WORD wInitNeighRadius,
		const WORD wFinalNeighRadius,
		const WORD wNeighChangeRate,
		const DWORD dwNeighRadiusSteps)
	{
		st_bNetType=bNetType;
		st_wClusters=wClusters;
		st_dwCycles= dwCycles;
		st_fMinDist=fMinDist;
		st_bTrainNeighbours=bTrainNeighbours;
		st_fInitLearnRate=fInitLearnRate;
		st_fFinalLearnRate=fFinalLearnRate;
		st_fLearnChangeRate=fLearnChangeRate;
		st_dwLearnRateSteps=dwLearnRateSteps;
		st_wInitNeighRadius=wInitNeighRadius;
		st_wFinalNeighRadius=wFinalNeighRadius;
		st_wNeighChangeRate=wNeighChangeRate;
		st_dwNeighRadiusSteps=dwNeighRadiusSteps;
	}

	BYTE st_bNetType;
	WORD st_wClusters;
	DWORD st_dwCycles;
	FLOAT st_fMinDist;
	BOOLEAN st_bTrainNeighbours;
	FLOAT st_fInitLearnRate;
	FLOAT st_fFinalLearnRate;
	FLOAT st_fLearnChangeRate;
	DWORD st_dwLearnRateSteps;
	WORD st_wInitNeighRadius;
	WORD st_wFinalNeighRadius;
	WORD st_wNeighChangeRate;
	DWORD st_dwNeighRadiusSteps;
} OPTIONSCP;

typedef struct tagOPTIONSDCT
{
	tagOPTIONSDCT(void)
	{
	
	}

	tagOPTIONSDCT(const BYTE bQuality,const BOOLEAN bShift)
	{
		st_bQuality=bQuality;
		st_bShift=bShift;
	}

	BYTE st_bQuality;
	BOOLEAN st_bShift;
} OPTIONSDCT;

typedef struct tagOPTIONS
{
	tagOPTIONS(const OPTIONSBP &optionsBP,
		const OPTIONSCP &optionsCP,
		const OPTIONSDCT &optionsDCT)
	{
		CopyMemory(&st_optionsBP,&optionsBP,sizeof(OPTIONSBP));
		CopyMemory(&st_optionsCP,&optionsCP,sizeof(OPTIONSCP));
		CopyMemory(&st_optionsDCT,&optionsDCT,sizeof(OPTIONSDCT));
	}

	tagOPTIONS(void)
	{
	
	}
	OPTIONSBP st_optionsBP;
	OPTIONSCP st_optionsCP;
	OPTIONSDCT st_optionsDCT;
} OPTIONS;

typedef struct tagREPORT
{
	CString st_sFileName;
	CString st_sDirectory;
	DWORD st_dwWidth;
	DWORD st_dwHeigth;
	DWORD st_dwColors;
	CString st_sMethod;
	DWORD st_dwOrFileSize;
	DWORD st_dwComprFileSize;
	FLOAT st_fRatio;
	FLOAT st_fRed;
	FLOAT st_fGreen;
	FLOAT st_fBlue;
	FLOAT st_fAverange;
	FLOAT st_fFull;
	FLOAT st_fTime;
} REPORT;

#define COMPRFILE_NNIC 0x4E4E4943
#define COMPRTYPE_CP 0x1
#define COMPRTYPE_BP 0x2
#define COMPRTYPE_DCT 0x2

typedef struct tagCOMPRFILEINFO
{
	tagCOMPRFILEINFO(
		const DWORD dwFileType,
		const BYTE bComprMethod,
		const DWORD dwImageWidth,
		const DWORD dwImageHeight,
		const DWORD dwImageSize)
	{
		st_dwFileType=dwFileType;
		st_bComprMethod=bComprMethod;
		st_dwImageWidth=dwImageWidth;
		st_dwImageHeight=dwImageHeight;
		st_dwImageSize=dwImageSize;
	}

	DWORD st_dwFileType;
	BYTE st_bComprMethod;
	DWORD st_dwImageWidth;
	DWORD st_dwImageHeight;
	DWORD st_dwImageSize;
} COMPRFILEINFO;

typedef struct tagCOMPRDATAINFO
{
	tagCOMPRDATAINFO(const DWORD dwSrcLen, const DWORD dwDestLen)
	{
		st_dwSrcLen=dwSrcLen;
		st_dwDestLen=dwDestLen;
	}

	void Set(const DWORD dwSrcLen, const DWORD dwDestLen)
	{
		st_dwSrcLen=dwSrcLen;
		st_dwDestLen=dwDestLen;
	}

	DWORD st_dwSrcLen;
	DWORD st_dwDestLen;
} COMPRDATAINFO;

BYTE **AllocateByteMatrix(const DWORD dwHeight,const DWORD dwWidth);
FLOAT **AllocateFloatMatrix(const DWORD dwHeight,const DWORD dwWidth);
FLOAT **AllocateFloatMatrix(const DWORD dwHeight,const DWORD *pWidth);
void DestroyMatrix(LPVOID *pMatrix);

DWORD FindPrimeNumber(const DWORD dwFrom);
LONG Round(const FLOAT fValue);

#endif // !defined(AFX_GLOBALS_H__76BA1620_20A5_4B58_9D57_375E431EC133__INCLUDED_)
