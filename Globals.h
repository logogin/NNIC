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
	tagOPTIONSBP(const WORD wLearnMode,const WORD wSigmoidType,const FLOAT fSigmoidAlpha,
		const BOOL bUseBiases,const BOOL bUseMomentum,const DWORD dwLearnCycles,
		const FLOAT fMinError,const WORD wPatterns,const WORD wInputBlock,const WORD wHiddenBlock,
		const BOOL bSecondHidden,const WORD wNeurons,const FLOAT fInitLearnRate,
		const FLOAT fFinalLearnRate,const DWORD dwLearnSteps,const FLOAT fLearnChangeRate,
		const FLOAT fInitMoment,const FLOAT fFinalMoment,const DWORD dwMomentSteps,
		const FLOAT fMomentChangeRate)
	{
		st_wLearnMode=wLearnMode;
		st_wSigmoidType=wSigmoidType;
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

	WORD st_wLearnMode;
	WORD st_wSigmoidType;
	FLOAT st_fSigmoidAlpha;
	BOOL st_bUseBiases;
	BOOL st_bUseMomentum;
	DWORD st_dwLearnCycles;
	FLOAT st_fMinError;
	WORD st_wPatterns;
	WORD st_wInputBlock;
	WORD st_wHiddenBlock;
	BOOL st_bSecondHidden;
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
	tagOPTIONSCP(const WORD wClusters,
		const ULONG ulCycles,
		const FLOAT fMinDist,
		const FLOAT fInitLearnRate,
		const FLOAT fFinalLearnRate,
		const ULONG ulLearnRateEpochs,
		const FLOAT fLearnChangeRate,
		const WORD wInitNeighSize,
		const WORD wFinalNeighSize,
		const ULONG ulNeighSizeEpochs,
		const WORD wNeighChangeRate)
	{
		st_wClusters=wClusters;
		st_ulCycles=ulCycles;
		st_fMinDist=fMinDist;
		st_fInitLearnRate=fInitLearnRate;
		st_fFinalLearnRate=fFinalLearnRate;
		st_ulLearnRateEpochs=ulLearnRateEpochs;
		st_fLearnChangeRate=fLearnChangeRate;
		st_wInitNeighSize=wInitNeighSize;
		st_wFinalNeighSize=wFinalNeighSize;
		st_ulNeighSizeEpochs=ulNeighSizeEpochs;
		st_wNeighChangeRate=wNeighChangeRate;
	}

	tagOPTIONSCP(void)
	{
	
	}

	WORD st_wClusters;
	ULONG st_ulCycles;
	FLOAT st_fMinDist;
	FLOAT st_fInitLearnRate;
	FLOAT st_fFinalLearnRate;
	ULONG st_ulLearnRateEpochs;
	FLOAT st_fLearnChangeRate;
	WORD st_wInitNeighSize;
	WORD st_wFinalNeighSize;
	ULONG st_ulNeighSizeEpochs;
	WORD st_wNeighChangeRate;
} OPTIONSCP;


typedef struct tagOPTIONS
{
	tagOPTIONS(const OPTIONSBP &optionsBP,const OPTIONSCP optionsCP)
	{
		CopyMemory(&st_optionsBP,&optionsBP,sizeof(OPTIONSBP));
		CopyMemory(&st_optionsCP,&optionsCP,sizeof(OPTIONSCP));
	}

	tagOPTIONS(void)
	{
	
	}
	OPTIONSBP st_optionsBP;
	OPTIONSCP st_optionsCP;
} OPTIONS;

typedef struct tagREPORT
{
	CString st_sFileName;
	CString st_sDirectory;
	UINT st_uiWidth;
	UINT st_uiHeigth;
	ULONG st_ulColors;
	CString st_sMethod;
	ULONG st_ulOrFileSize;
	ULONG st_ulComprFileSize;
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

typedef struct tagCOMPRFILEINFO
{
	tagCOMPRFILEINFO(const DWORD dwFileType,const BYTE bComprMethod,const DWORD dwImageWidth,
		const DWORD dwImageHeight,const DWORD dwImageSize):
		st_dwFileType(dwFileType),st_bComprMethod(bComprMethod),st_dwImageWidth(dwImageWidth),
		st_dwImageHeight(dwImageHeight),st_dwImageSize(dwImageSize)
	{
	
	}

	DWORD st_dwFileType;
	BYTE st_bComprMethod;
	DWORD st_dwImageWidth;
	DWORD st_dwImageHeight;
	DWORD st_dwImageSize;
} COMPRFILEINFO;

#endif // !defined(AFX_GLOBALS_H__76BA1620_20A5_4B58_9D57_375E431EC133__INCLUDED_)
