
// DirectoryBrowseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DirectoryBrowse.h"
#include "DirectoryBrowseDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDirectoryBrowseDlg dialog



CDirectoryBrowseDlg::CDirectoryBrowseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIRECTORYBROWSE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDirectoryBrowseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCSHELLTREE1, m_DirectoryView);
	DDX_Control(pDX, IDC_MFCSHELLLIST1, m_FileView);
	DDX_Control(pDX, IDC_EDIT1, m_InfoCtrl);
	DDX_Control(pDX, IDC_EDIT2, m_DirectoryInfo);
}

BEGIN_MESSAGE_MAP(CDirectoryBrowseDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_CLICK, IDC_MFCSHELLTREE1, &CDirectoryBrowseDlg::OnNMClickMfcshelltree1)
	ON_NOTIFY(TVN_SELCHANGED, IDC_MFCSHELLTREE1, &CDirectoryBrowseDlg::OnTvnSelchangedMfcshelltree1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_MFCSHELLLIST1, &CDirectoryBrowseDlg::OnLvnItemchangedMfcshelllist1)
END_MESSAGE_MAP()


// CDirectoryBrowseDlg message handlers

BOOL CDirectoryBrowseDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon


	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDirectoryBrowseDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDirectoryBrowseDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDirectoryBrowseDlg::OnNMClickMfcshelltree1(NMHDR *pNMHDR, LRESULT *pResult)
{

	*pResult = 0;
}


void CDirectoryBrowseDlg::OnTvnSelchangedMfcshelltree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	CString filePath;
	m_DirectoryView.GetItemPath(filePath, m_DirectoryView.GetSelectedItem());

	m_FileView.DisplayFolder((LPCTSTR)filePath);

	DisplayDirectoryDetail(filePath);

	*pResult = 0;
}


void CDirectoryBrowseDlg::OnLvnItemchangedMfcshelllist1(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (!GetSafeHwnd())
		return;
	if (!m_FileView.GetSafeHwnd())
		return;
	if (!m_DirectoryView.GetSafeHwnd())
		return;


	NM_LISTVIEW* p = (NM_LISTVIEW*)pNMHDR;

	CString path;
	m_FileView.GetItemPath(path, p->iItem);
	DisplayFileDetail(path);

	*pResult = 0;
}

void CDirectoryBrowseDlg::DisplayDirectoryDetail(const CString& directoryPath)
{
	CString msg;

	GetFileInformation(directoryPath, msg);

	m_DirectoryInfo.SetWindowTextW(msg);

}
void CDirectoryBrowseDlg::DisplayFileDetail(const CString& filePath)
{
	CString msg;
	GetFileInformation(filePath, msg);
	m_InfoCtrl.SetWindowTextW(msg);

}

#define CHECKFLAG( F )  { if ( findData.dwFileAttributes & F ) { infoBuf += L#F; infoBuf += L" \r\n"; }}

void CDirectoryBrowseDlg::GetFileInformation(const CString& filename, CString& infoBuf)
{
	WIN32_FIND_DATA findData;
	HANDLE hFind = ::FindFirstFile((LPCTSTR)filename, &findData);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		FindClose(hFind);

		CHECKFLAG(FILE_ATTRIBUTE_READONLY);
		CHECKFLAG(FILE_ATTRIBUTE_HIDDEN);
		CHECKFLAG(FILE_ATTRIBUTE_SYSTEM);
		CHECKFLAG(FILE_ATTRIBUTE_DIRECTORY);
		CHECKFLAG(FILE_ATTRIBUTE_ARCHIVE);
		CHECKFLAG(FILE_ATTRIBUTE_DEVICE);
		CHECKFLAG(FILE_ATTRIBUTE_NORMAL);
		CHECKFLAG(FILE_ATTRIBUTE_TEMPORARY);
		CHECKFLAG(FILE_ATTRIBUTE_SPARSE_FILE);
		CHECKFLAG(FILE_ATTRIBUTE_REPARSE_POINT);
		CHECKFLAG(FILE_ATTRIBUTE_COMPRESSED);
		CHECKFLAG(FILE_ATTRIBUTE_OFFLINE);
		CHECKFLAG(FILE_ATTRIBUTE_NOT_CONTENT_INDEXED);
		CHECKFLAG(FILE_ATTRIBUTE_ENCRYPTED);
		CHECKFLAG(FILE_ATTRIBUTE_INTEGRITY_STREAM);
		CHECKFLAG(FILE_ATTRIBUTE_VIRTUAL);
		CHECKFLAG(FILE_ATTRIBUTE_NO_SCRUB_DATA);
		CHECKFLAG(FILE_ATTRIBUTE_EA);
		CHECKFLAG(FILE_ATTRIBUTE_PINNED);
		CHECKFLAG(FILE_ATTRIBUTE_UNPINNED);
		CHECKFLAG(FILE_ATTRIBUTE_RECALL_ON_OPEN);
		CHECKFLAG(FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS);
	}
}
