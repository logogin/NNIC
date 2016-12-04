// GeneralDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NNIC.h"
#include "GeneralDlg.h"
#include ".\generaldlg.h"


// CGeneralDlg dialog

IMPLEMENT_DYNAMIC(CGeneralDlg, CPropertyPage)
CGeneralDlg::CGeneralDlg()
	: CPropertyPage(CGeneralDlg::IDD)
	, m_strDefComprFileName(_T(""))
	, m_bPromtOverwrite(FALSE)
	, m_bShowReport(FALSE)
	, m_bWriteReport(FALSE)
	, m_strReportFileName(_T(""))
	, m_bLimitFileSize(FALSE)
	, m_wReportFileSize(0)
	
	, m_bStoringMethod(0)
{
}

CGeneralDlg::CGeneralDlg(const OPTIONSGENERAL &optionsGENERAL)
	: CPropertyPage(CGeneralDlg::IDD)
	, m_strDefComprFileName(_T(""))
	, m_bPromtOverwrite(FALSE)
	, m_bShowReport(FALSE)
	, m_bWriteReport(FALSE)
	, m_strReportFileName(_T(""))
	, m_bLimitFileSize(FALSE)
	, m_wReportFileSize(0)
{
	m_optionsGENERAL=optionsGENERAL;
}

CGeneralDlg::~CGeneralDlg()
{
}

void CGeneralDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_COMPRNAME, m_strDefComprFileName);
	DDX_Check(pDX, IDC_CHECK_PROMTOVERWRITE, m_bPromtOverwrite);
	DDX_Check(pDX, IDC_CHECK_SHOWREPORT, m_bShowReport);
	DDX_Check(pDX, IDC_CHECK_WRITEREPORT, m_bWriteReport);
	DDX_Text(pDX, IDC_EDIT_REPORTNAME, m_strReportFileName);
	DDX_Text(pDX, IDC_EDIT_FILESIZE, m_wReportFileSize);
	DDX_Check(pDX, IDC_CHECK_LIMITFILESIZE, m_bLimitFileSize);
	DDV_MinMaxUInt(pDX, m_wReportFileSize, 1, 2048);
	if (pDX->m_bSaveAndValidate)
	{
		if (m_strDefComprFileName.IsEmpty())
		{
			pDX->PrepareEditCtrl(IDC_EDIT_COMPRNAME);
			AfxMessageBox(_T("Please enter valid file name."));
			pDX->Fail();
		}
		if (m_strReportFileName.IsEmpty())
		{
			pDX->PrepareEditCtrl(IDC_EDIT_REPORTNAME);
			AfxMessageBox(_T("Please enter valid file name."));
			pDX->Fail();
		}
	}
}


BEGIN_MESSAGE_MAP(CGeneralDlg, CPropertyPage)
	ON_BN_CLICKED(IDC_RADIO_APPEND, OnBnClickedRadioAppend)
	ON_BN_CLICKED(IDC_RADIO_OVERWRITE, OnBnClickedRadioOverwrite)
	ON_BN_CLICKED(IDC_BUTTON_LOADDEAFULTS, OnBnClickedButtonLoadDeafults)
	ON_BN_CLICKED(IDC_BUTTON_SAVEDEFAULTS, OnBnClickedButtonSaveDefaults)
	ON_BN_CLICKED(IDC_CHECK_WRITEREPORT, OnBnClickedCheckWriteReport)
	ON_BN_CLICKED(IDC_CHECK_LIMITFILESIZE, OnBnClickedCheckLimitFileSize)
END_MESSAGE_MAP()


// CGeneralDlg message handlers

BOOL CGeneralDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	switch (m_optionsGENERAL.st_bStoringMethod)
	{
	case REPORT_OVERWRITE:
        CheckRadioButton(IDC_RADIO_OVERWRITE,IDC_RADIO_APPEND,IDC_RADIO_OVERWRITE);
		break;
	case REPORT_APPEND:
		CheckRadioButton(IDC_RADIO_OVERWRITE,IDC_RADIO_APPEND,IDC_RADIO_APPEND);
		break;
	}

	// TODO:  Add extra initialization here
	m_strDefComprFileName=m_optionsGENERAL.st_strDefComprFileName;
	m_bPromtOverwrite=m_optionsGENERAL.st_bPromtOverwrite;
	m_bShowReport=m_optionsGENERAL.st_bShowReport;
	m_bWriteReport=m_optionsGENERAL.st_bWriteReport;
	m_strReportFileName=m_optionsGENERAL.st_strReportFileName;
	m_bStoringMethod=m_optionsGENERAL.st_bStoringMethod;
	m_bLimitFileSize=m_optionsGENERAL.st_bLimitFileSize;
	m_wReportFileSize=m_optionsGENERAL.st_wReportFileSize;

	UpdateData(FALSE);
	EnableWriteReport(m_bWriteReport);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CGeneralDlg::OnBnClickedCheckWriteReport()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	EnableWriteReport(m_bWriteReport);
}

void CGeneralDlg::OnBnClickedRadioAppend()
{
	// TODO: Add your control notification handler code here
	m_bStoringMethod=REPORT_APPEND;
	GetDlgItem(IDC_CHECK_LIMITFILESIZE)->EnableWindow(TRUE);
	EnableLimitFileSize(TRUE);
}

void CGeneralDlg::OnBnClickedRadioOverwrite()
{
	// TODO: Add your control notification handler code here
	m_bStoringMethod=REPORT_OVERWRITE;
	GetDlgItem(IDC_CHECK_LIMITFILESIZE)->EnableWindow(FALSE);
	EnableLimitFileSize(FALSE);
}

void CGeneralDlg::OnBnClickedButtonLoadDeafults()
{
	// TODO: Add your control notification handler code here
}

void CGeneralDlg::OnBnClickedButtonSaveDefaults()
{
	// TODO: Add your control notification handler code here
}

void CGeneralDlg::EnableWriteReport(const BOOLEAN bFlag)
{
	GetDlgItem(IDC_EDIT_REPORTNAME)->EnableWindow(bFlag);
	GetDlgItem(IDC_RADIO_OVERWRITE)->EnableWindow(bFlag);
	GetDlgItem(IDC_RADIO_APPEND)->EnableWindow(bFlag);
	if (m_bStoringMethod==REPORT_OVERWRITE)
	{
		GetDlgItem(IDC_CHECK_LIMITFILESIZE)->EnableWindow(FALSE);
		EnableLimitFileSize(FALSE);
		return;
	}
	UpdateData(TRUE);
	GetDlgItem(IDC_CHECK_LIMITFILESIZE)->EnableWindow(bFlag);
	EnableLimitFileSize(bFlag & m_bLimitFileSize);
}

void CGeneralDlg::EnableLimitFileSize(const BOOLEAN bFlag)
{
	GetDlgItem(IDC_EDIT_FILESIZE)->EnableWindow(bFlag);
	GetDlgItem(IDC_STATIC_KB)->EnableWindow(bFlag);
}

void CGeneralDlg::OnBnClickedCheckLimitFileSize()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	EnableLimitFileSize(m_bLimitFileSize);
}

void CGeneralDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);
	m_optionsGENERAL.st_strDefComprFileName=m_strDefComprFileName;
	m_optionsGENERAL.st_bPromtOverwrite=m_bPromtOverwrite;
	m_optionsGENERAL.st_bShowReport=m_bShowReport;
	m_optionsGENERAL.st_bWriteReport=m_bWriteReport;
	m_optionsGENERAL.st_strReportFileName=m_strReportFileName;
	m_optionsGENERAL.st_bStoringMethod=m_bStoringMethod;
	m_optionsGENERAL.st_bLimitFileSize=m_bLimitFileSize;
	m_optionsGENERAL.st_wReportFileSize=(WORD)m_wReportFileSize;

	CPropertyPage::OnOK();
}

void CGeneralDlg::GetOptionsGENERAL(OPTIONSGENERAL * optionsGENERAL)
{
	*optionsGENERAL=m_optionsGENERAL;
}
