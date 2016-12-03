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
public:
	void GetOptionsBP(OPTIONSBP *optionsBP);
	CBPDlg(void);
	CBPDlg(const OPTIONSBP &optionsBP);
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
	int		m_iHidden;
	int		m_iInput;
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
	void EnableSecondHidden(const BOOL bFlag);
	void EnableLearnRate(const BOOL bFlag);
	void EnableMomentum(const BOOL bFlag);
	OPTIONSBP m_optionsBP; 
	// Generated message map functions
	//{{AFX_MSG(CBPDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioBatch();
	afx_msg void OnRadioSequential();
	afx_msg void OnRadioSigmoid();
	afx_msg void OnRadioHypertan();
	afx_msg void OnCheckSecond();
	afx_msg void OnCheckMomentum();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BPDLG_H__7BC99542_E6CF_4326_BA29_0E180ED45816__INCLUDED_)
