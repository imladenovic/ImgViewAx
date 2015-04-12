
// iaxTestDlg.h : header file
//

#pragma once
#include "imgx1.h"


// CiaxTestDlg dialog
class CiaxTestDlg : public CDialogEx
{
// Construction
public:
	CiaxTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_IAXTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonLoad();
protected:
	CImgx1 m_iax;
public:
	afx_msg void OnBnClickedButtonPrint();
};
