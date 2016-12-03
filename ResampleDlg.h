#if !defined(AFX_RESAMPLEDLG_H__9EE82823_B62A_41C6_ABED_E381495F0586__INCLUDED_)
#define AFX_RESAMPLEDLG_H__9EE82823_B62A_41C6_ABED_E381495F0586__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ResampleDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CResampleDlg dialog
UINT ResampleProgress(LPVOID pParam);

class CResampleDlg : public CDialog
{
	DECLARE_DYNCREATE(CResampleDlg)
// Construction
public:
	void SetHandle(HBITMAP hBitmap);
	HBITMAP GetHandle(void);
	CBitmapKit * GetBitmapKit(void);
	void StepIt(void);
	INT SetStep(const INT iStep);
	CResampleDlg(CWnd* pParent = NULL);   // standard constructor
	CResampleDlg(CBitmapKit *pBitmap,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CResampleDlg)
	enum { IDD = IDD_DIALOG_RESAMPLE };
	CProgressCtrl	m_progressResample;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResampleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HBITMAP m_hBitmap;
	CBitmapKit * m_pBitmapKit;

	// Generated message map functions
	//{{AFX_MSG(CResampleDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESAMPLEDLG_H__9EE82823_B62A_41C6_ABED_E381495F0586__INCLUDED_)
