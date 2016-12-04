// Globals.h: interface for the Globals class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLOBALS_H__76BA1620_20A5_4B58_9D57_375E431EC133__INCLUDED_)
#define AFX_GLOBALS_H__76BA1620_20A5_4B58_9D57_375E431EC133__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define REPORT_OVERWRITE 0
#define REPORT_APPEND 1

typedef struct tagOPTIONSGENERAL
{
	tagOPTIONSGENERAL()
	{
	
	};
	tagOPTIONSGENERAL(
		const CString &strDefComprFileName,
		const BOOLEAN bPromtOverwrite,
		const BOOLEAN bShowReport,
		const BOOLEAN bWriteReport,
		const CString &strReportFileName,
		const BYTE bStoringMethod,
		const BOOLEAN bLimitFileSize,
		const WORD wReportFileSize)
	{
		st_strDefComprFileName=strDefComprFileName;
		st_bPromtOverwrite=bPromtOverwrite;
		st_bShowReport=bShowReport;
		st_bWriteReport=bWriteReport;
		st_strReportFileName=strReportFileName;
		st_bStoringMethod=bStoringMethod;
		st_bLimitFileSize=bLimitFileSize;
		st_wReportFileSize=wReportFileSize;
	};
	tagOPTIONSGENERAL &operator=(const tagOPTIONSGENERAL &optionsGENERAL)
	{
		st_strDefComprFileName=optionsGENERAL.st_strDefComprFileName;
		st_bPromtOverwrite=optionsGENERAL.st_bPromtOverwrite;
		st_bShowReport=optionsGENERAL.st_bShowReport;
		st_bWriteReport=optionsGENERAL.st_bWriteReport;
		st_strReportFileName=optionsGENERAL.st_strReportFileName;
		st_bStoringMethod=optionsGENERAL.st_bStoringMethod;
		st_bLimitFileSize=optionsGENERAL.st_bLimitFileSize;
		st_wReportFileSize=optionsGENERAL.st_wReportFileSize;
		return *this;
	};

	CString st_strDefComprFileName;
	BOOLEAN st_bPromtOverwrite;
	BOOLEAN st_bShowReport;
	BOOLEAN st_bWriteReport;
	CString st_strReportFileName;
	BYTE st_bStoringMethod;
	BOOLEAN st_bLimitFileSize;
	WORD st_wReportFileSize;
} OPTIONSGENERAL;

typedef struct tagOPTIONSBP
{
	tagOPTIONSBP(void)
	{
	
	};
	tagOPTIONSBP(
		const BYTE bLearnMode,
		const BYTE bSigmoidType,
		const FLOAT fSigmoidAlpha,
		const BOOLEAN bUseBiases,
		const BOOLEAN bUseMomentum,
		const DWORD dwLearnCycles,
		const FLOAT fMinError,
		const WORD wPatterns,
		const BYTE bInputBlock,
		const BYTE bHiddenBlock,
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
		st_bInputBlock=bInputBlock;
		st_bHiddenBlock=bHiddenBlock;
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
	};
	tagOPTIONSBP &operator=(const tagOPTIONSBP &optionsBP)
	{
		st_bLearnMode=optionsBP.st_bLearnMode;
		st_bSigmoidType=optionsBP.st_bSigmoidType;
		st_fSigmoidAlpha=optionsBP.st_fSigmoidAlpha;
		st_bUseBiases=optionsBP.st_bUseBiases;
		st_bUseMomentum=optionsBP.st_bUseMomentum;
		st_dwLearnCycles=optionsBP.st_dwLearnCycles;
		st_fMinError=optionsBP.st_fMinError;
		st_wPatterns=optionsBP.st_wPatterns;
		st_bInputBlock=optionsBP.st_bInputBlock;
		st_bHiddenBlock=optionsBP.st_bHiddenBlock;
		st_bSecondHidden=optionsBP.st_bSecondHidden;
		st_wNeurons=optionsBP.st_wNeurons;
		st_fInitLearnRate=optionsBP.st_fInitLearnRate;
		st_fFinalLearnRate=optionsBP.st_fFinalLearnRate;
		st_dwLearnSteps=optionsBP.st_dwLearnSteps;
		st_fLearnChangeRate=optionsBP.st_fLearnChangeRate;
		st_fInitMoment=optionsBP.st_fInitMoment;
		st_fFinalMoment=optionsBP.st_fFinalMoment;
		st_dwMomentSteps=optionsBP.st_dwMomentSteps;
		st_fMomentChangeRate=optionsBP.st_fMomentChangeRate;
		return *this;
	};

	BYTE st_bLearnMode;
	BYTE st_bSigmoidType;
	FLOAT st_fSigmoidAlpha;
	BOOLEAN st_bUseBiases;
	BOOLEAN st_bUseMomentum;
	DWORD st_dwLearnCycles;
	FLOAT st_fMinError;
	WORD st_wPatterns;
	BYTE st_bInputBlock;
	BYTE st_bHiddenBlock;
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
	
	};
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
		st_dwCycles=dwCycles;
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
	};
	tagOPTIONSCP &operator=(const tagOPTIONSCP &optionsCP)
	{
		st_bNetType=optionsCP.st_bNetType;
		st_wClusters=optionsCP.st_wClusters;
		st_dwCycles=optionsCP.st_dwCycles;
		st_fMinDist=optionsCP.st_fMinDist;
		st_bTrainNeighbours=optionsCP.st_bTrainNeighbours;
		st_fInitLearnRate=optionsCP.st_fInitLearnRate;
		st_fFinalLearnRate=optionsCP.st_fFinalLearnRate;
		st_fLearnChangeRate=optionsCP.st_fLearnChangeRate;
		st_dwLearnRateSteps=optionsCP.st_dwLearnRateSteps;
		st_wInitNeighRadius=optionsCP.st_wInitNeighRadius;
		st_wFinalNeighRadius=optionsCP.st_wFinalNeighRadius;
		st_wNeighChangeRate=optionsCP.st_wNeighChangeRate;
		st_dwNeighRadiusSteps=optionsCP.st_dwNeighRadiusSteps;
		return *this;
	};

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
	
	};
	tagOPTIONSDCT(const BYTE bQuality,const BOOLEAN bShift)
	{
		st_bQuality=bQuality;
		st_bShift=bShift;
	};
	tagOPTIONSDCT &operator=(const tagOPTIONSDCT &optionsDCT)
	{
		st_bQuality=optionsDCT.st_bQuality;
		st_bShift=optionsDCT.st_bShift;
		return *this;
	};

	BYTE st_bQuality;
	BOOLEAN st_bShift;
} OPTIONSDCT;

typedef struct tagOPTIONS
{
	tagOPTIONS(void)
	{
	
	};
	tagOPTIONS(
		const OPTIONSGENERAL &optionsGENERAL,
		const OPTIONSBP &optionsBP,
		const OPTIONSCP &optionsCP,
		const OPTIONSDCT &optionsDCT)
	{
		st_optionsGENERAL=optionsGENERAL;
		st_optionsBP=optionsBP;
		st_optionsCP=optionsCP;
		st_optionsDCT=optionsDCT;
	};
	tagOPTIONS &operator=(const tagOPTIONS &options)
	{
		st_optionsGENERAL=options.st_optionsGENERAL;
		st_optionsBP=options.st_optionsBP;
		st_optionsCP=options.st_optionsCP;
		st_optionsDCT=options.st_optionsDCT;
		return *this;
	};
	
	OPTIONSGENERAL st_optionsGENERAL;
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
	FLOAT st_fAverage;
	FLOAT st_fFull;
	FLOAT st_fTime;
} REPORT;

#define COMPRFILE_NNIC 0x49434E4E
#define COMPRTYPE_BP 0x1
#define COMPRTYPE_CP 0x2
#define COMPRTYPE_DCT 0x4
#define COMPRTYPE_DCTBACK 0x8

typedef struct tagCOMPRFILEINFO
{
	tagCOMPRFILEINFO(void)
	{
	
	};
	tagCOMPRFILEINFO(
		const DWORD dwFileType,
		const BYTE bComprMethod,
		const BITMAP &bm)
	{
		st_dwFileType=dwFileType;
		st_bComprMethod=bComprMethod;
		CopyMemory(&st_bmBitmap,&bm,sizeof(BITMAP));
		bmBits=NULL;
		st_dwImageSize=bmHeight*bmWidthBytes;
	};

	DWORD st_dwFileType;
	BYTE st_bComprMethod;
	union
	{
		BITMAP st_bmBitmap;
		struct
		{
			DWORD  bmType; 
			LONG   bmWidth; 
			LONG   bmHeight; 
			LONG   bmWidthBytes; 
			WORD   bmPlanes; 
			WORD   bmBitsPixel; 
			LPVOID bmBits;
		};
	};
	DWORD st_dwImageSize;
} COMPRFILEINFO;

typedef struct tagCOMPRDATAINFO
{
	tagCOMPRDATAINFO(void)
	{
	
	};
	tagCOMPRDATAINFO(const DWORD dwSrcLen, const DWORD dwDestLen)
	{
		st_dwSrcLen=dwSrcLen;
		st_dwDestLen=dwDestLen;
	};
	void Set(const DWORD dwSrcLen, const DWORD dwDestLen)
	{
		st_dwSrcLen=dwSrcLen;
		st_dwDestLen=dwDestLen;
	};

	DWORD st_dwSrcLen;
	DWORD st_dwDestLen;
} COMPRDATAINFO;

typedef struct tagCOMPRDATABP
{
	tagCOMPRDATABP(void)
	{
	
	};
	tagCOMPRDATABP(
		const FLOAT fShift,
		const FLOAT fSignalWidth,
		const FLOAT fScaleParam,
		const BYTE bSigmoidType,
		const FLOAT fSigmoidAlpha,
		const BOOLEAN bUseBiases, 
		const WORD wInputLayer,
		const WORD wOutputLayer)
	{
		st_fShift=fShift;
		st_fSignalWidth=fSignalWidth;
		st_fScaleParam=fScaleParam;
		st_bSigmoidType=bSigmoidType;
		st_fSigmoidAlpha=fSigmoidAlpha;
		st_bUseBiases=bUseBiases;
		st_wInputLayer=wInputLayer;
		st_wOutputLayer=wOutputLayer;
	};

	FLOAT st_fShift;
	FLOAT st_fSignalWidth;
	FLOAT st_fScaleParam;
	BYTE st_bSigmoidType;
	FLOAT st_fSigmoidAlpha;
	BOOLEAN st_bUseBiases;
	WORD st_wInputLayer;
	WORD st_wOutputLayer;
} COMPRDATABP;

typedef struct tagCOMPRDATADCT
{
	tagCOMPRDATADCT(void)
	{

	};
	tagCOMPRDATADCT(
		const BYTE bDCTSize,
		const BYTE bQuality,
		const FLOAT fShift,
		const FLOAT fSignalWidth)
	{
		st_bDCTSize=bDCTSize;
		st_bQuality=bQuality;
		st_fShift=fShift;
		st_fSignalWidth=fSignalWidth;
	};

	BYTE st_bDCTSize;
	BYTE st_bQuality;
	FLOAT st_fShift;
	FLOAT st_fSignalWidth;
} COMPRDATADCT;

BYTE **AllocateByteMatrix(const DWORD dwHeight,const DWORD dwWidth);
FLOAT **AllocateFloatMatrix(const DWORD dwHeight,const DWORD dwWidth);
FLOAT **AllocateFloatMatrix(const DWORD dwHeight,const DWORD *pWidth);
WORD **AllocateWordMatrix(const DWORD dwHeight,const DWORD dwWidth);
void DestroyMatrix(LPVOID *pMatrix);

DWORD FindPrimeNumber(const DWORD dwFrom);
LONG Round(const FLOAT fValue);

DWORD GetBufferSize(const DWORD dwSrcLen);
BOOLEAN QueryZLibVersion(void);
BOOLEAN Compress2(LPBYTE lpDest,
				  PDWORD dwDestLen,
				  const LPBYTE lpSrc,
				  const DWORD dwSrcLen,
				  const bLevel);

BOOLEAN UnCompress(LPBYTE lpDest,
				  PDWORD dwDestLen,
				  const LPBYTE lpSrc,
				  const DWORD dwSrcLen);

BOOLEAN FileExists(const CString &strFileName);

#endif // !defined(AFX_GLOBALS_H__76BA1620_20A5_4B58_9D57_375E431EC133__INCLUDED_)
