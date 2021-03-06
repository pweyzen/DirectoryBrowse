
// DirectoryBrowseDlg.h : header file
//

#pragma once
#include "afxshelltreectrl.h"
#include "afxwin.h"
#include "afxshelllistctrl.h"
#include "afxeditbrowsectrl.h"
#include "afxcmn.h"
#include <vector>
#include <map>
#include <string>


// CDirectoryBrowseDlg dialog
class CDirectoryBrowseDlg : public CDialog
{

// Construction
public:
	typedef std::vector<std::wstring>				tStringList;
	typedef std::map<std::wstring, std::wstring>	tStringMap;
	struct FileInfo
	{
		struct Findings 
		{
			tStringList		dwAttributes;
			tStringMap		properties;
			void Reset() { dwAttributes.clear(); properties.clear(); }
		};

		bool			isDirectory;
		Findings		FindFirst;
		Findings		ByHandle;
		tStringList		alternateStreams;
		void Reset() { FindFirst.Reset(); ByHandle.Reset(); alternateStreams.clear(); }
	};

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
	CMFCShellListCtrl m_FileView;
	CTreeCtrl m_Tree;
	CStatic m_Msg;

	afx_msg void OnLvnItemchangedMfcshelllist1(NMHDR *pNMHDR, LRESULT *pResult);

	void DecodeAttributes(DWORD dwAttributes, tStringList& l);
	void DisplayFileDetail(const CString& filePath );
	void DisplayFindings(HTREEITEM hParent, const FileInfo::Findings& fi);
	void GetFileInformation(const CString& filename, FileInfo& fileInfo );
	afx_msg void OnBnClickedButton1();
};
