
// DirectoryBrowseDlg.h : header file
//

#pragma once
#include "afxshelltreectrl.h"
#include "afxwin.h"
#include "afxshelllistctrl.h"


// CDirectoryBrowseDlg dialog
class CDirectoryBrowseDlg : public CDialog
{
// Construction
public:
	CDirectoryBrowseDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIRECTORYBROWSE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CMFCShellTreeCtrl m_DirectoryView;
	CMFCShellListCtrl m_FileView;

	afx_msg void OnNMClickMfcshelltree1(NMHDR *pNMHDR, LRESULT *pResult);
	CEdit m_InfoCtrl;
	afx_msg void OnTvnSelchangedMfcshelltree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedMfcshelllist1(NMHDR *pNMHDR, LRESULT *pResult);
	CEdit m_DirectoryInfo;

	void DisplayDirectoryDetail(const CString& directoryPath);
	void DisplayFileDetail(const CString& filePath);
	void GetFileInformation(const CString& filename, CString& infoBuf);
};
