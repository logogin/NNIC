#if !defined(AFX_DCTDLG_H__0D0217AA_F7E9_4EDA_9A4A_F6792643D6D5__INCLUDED_)
#define AFX_DCTDLG_H__0D0217AA_F7E9_4EDA_9A4A_F6792643D6D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DCTDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDCTDlg dialog

class CDCTDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CDCTDlg)

// Construction
public:
	void GetOptionsDCT(OPTIONSDCT *optionsDCT);
	CDCTDlg();
	CDCTDlg(const OPTIONSDCT &optionsCP);
	~CDCTDlg();

// Dialog Data
	//{{AFX_DATA(CDCTDlg)
	enum { IDD = IDD_PROPPAGE_DCT };
	BOOL	m_bShift;
	int		m_iQuality;
	CString	m_strQuality;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDCTDlg)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	OPTIONSDCT m_optionsDCT;
	// Generated message map functions
	//{{AFX_MSG(CDCTDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DCTDLG_H__0D0217AA_F7E9_4EDA_9A4A_F6792643D6D5__INCLUDED_)
