#if !defined(AFX_BPDLG_H__7BC99542_E6CF_4326_BA29_0E180ED45816__INCLUDED_)
#define AFX_BPDLG_H__7BC99542_E6CF_4326_BA29_0E180ED45816__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BPDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBPDlg dialog

class CBPDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CBPDlg)

// Construction
//public:
//	void GetOptionsBP(OPTIONSBP *optionsBP);
	CBPDlg(void);
	CBPDlg(OPTIONSBP *optionsBP);
	~CBPDlg();

// Dialog Data
	//{{AFX_DATA(CBPDlg)
	enum { IDD = IDD_PROPPAGE_BACK };
	float	m_fAlpha;
	float	m_fFinalLearnRate;
	float	m_fFinalMoment;
	float	m_fInitLearnRate;
	float	m_fInitMoment;
	float	m_fLearnChangeRate;
	DWORD	m_dwLearnSteps;
	float	m_fMinError;
	float	m_fMomentChangeRate;
	DWORD	m_dwMomentSteps;
	CString	m_strHidden;
	CString	m_strInput;
	CString	m_strRatio;
	BOOL	m_bBiases;
	BOOL	m_bMomentum;
	BOOL	m_bSecond;
	DWORD	m_dwNeurons;
	DWORD	m_dwPatterns;
	DWORD	m_dwCycles;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CBPDlg)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	OPTIONSBP *m_optionsBP; 
	// Generated message map functions
	//{{AFX_MSG(CBPDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	int m_iInput;
	int m_iHidden;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedCheckMomentum();
	afx_msg void OnBnClickedCheckSecond();
	afx_msg void OnBnClickedRadioSequential();
	afx_msg void OnBnClickedRadioBatch();
	afx_msg void OnBnClickedRadioSigmoid();
	afx_msg void OnBnClickedRadioHypertan();
protected:
	void EnableMomentum(const BOOLEAN bFlag);
	void EnableLearnRate(const BOOLEAN bFlag);
	void EnableSecond(const BOOLEAN bFlag);
	BYTE m_bLearnMode;
	BYTE m_bSigmoidType;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BPDLG_H__7BC99542_E6CF_4326_BA29_0E180ED45816__INCLUDED_)
