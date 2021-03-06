
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
	DDX_Control(pDX, IDC_MFCSHELLLIST1, m_FileView);
	DDX_Control(pDX, IDC_TREE_PROPS, m_Tree);
	DDX_Control(pDX, IDC_STATIC_PATH, m_Msg);
}

BEGIN_MESSAGE_MAP(CDirectoryBrowseDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_MFCSHELLLIST1, &CDirectoryBrowseDlg::OnLvnItemchangedMfcshelllist1)
	ON_BN_CLICKED(IDC_BUTTON1, &CDirectoryBrowseDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDirectoryBrowseDlg message handlers

BOOL CDirectoryBrowseDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_Tree.EnableToolTips();

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




void CDirectoryBrowseDlg::OnLvnItemchangedMfcshelllist1(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (!GetSafeHwnd())
		return;
	if (!m_FileView.GetSafeHwnd())
		return;

	NM_LISTVIEW* p = (NM_LISTVIEW*)pNMHDR;

	CString path;
	m_FileView.GetItemPath(path, p->iItem);
	DisplayFileDetail( path );


	*pResult = 0;
}

void CDirectoryBrowseDlg::DisplayFindings(HTREEITEM hParent, const FileInfo::Findings& fi)
{
	if (fi.dwAttributes.size() > 0)
	{
		HTREEITEM AttributesNode = m_Tree.InsertItem(L"Attributes", hParent);
		for (auto iter = fi.dwAttributes.begin(); iter != fi.dwAttributes.end(); ++iter)
		{
			auto& attr = (*iter);
			HTREEITEM item = m_Tree.InsertItem(attr.c_str(), AttributesNode);
			m_Tree.SetItemData(item, 1);
		}
		m_Tree.Expand(AttributesNode, TVE_EXPAND);
	}
	if (fi.properties.size() > 0)
	{
		HTREEITEM PropsNode = m_Tree.InsertItem(L"Properties", hParent);
		for (auto iter = fi.properties.begin(); iter != fi.properties.end(); ++iter)
		{
			const auto& nm = (*iter).first;
			const auto& vl = (*iter).second;

			wchar_t buf[1024];
			swprintf_s(buf, L"%s:     %s", nm.c_str(), vl.c_str());

			HTREEITEM item = m_Tree.InsertItem(buf, PropsNode);
			m_Tree.SetItemData(item, 2);
		}
		m_Tree.Expand(PropsNode, TVE_EXPAND);
	}
	m_Tree.Expand(hParent, TVE_EXPAND);
}

void CDirectoryBrowseDlg::DisplayFileDetail(const CString& filePath )
{
	m_Tree.LockWindowUpdate();
	m_Tree.DeleteAllItems();

	FileInfo fileInfo;
	GetFileInformation(filePath, fileInfo );

	DisplayFindings(m_Tree.InsertItem(L"FindFirst"), fileInfo.FindFirst);
	DisplayFindings(m_Tree.InsertItem(L"ByHandle"), fileInfo.ByHandle);

	if (fileInfo.alternateStreams.size() > 0)
	{
		HTREEITEM StreamsNode = m_Tree.InsertItem(L"File Streams");
		for (auto iter = fileInfo.alternateStreams.begin(); iter != fileInfo.alternateStreams.end(); ++iter)
		{
			const auto& nm = (*iter);
			HTREEITEM item = m_Tree.InsertItem(nm.c_str(), StreamsNode);
			m_Tree.SetItemData(item, 3);
		}
		m_Tree.Expand(StreamsNode, TVE_EXPAND);
	}
	m_Tree.UnlockWindowUpdate();

	m_Msg.SetWindowTextW(filePath);
}

static void AddStringItem(CDirectoryBrowseDlg::tStringMap& attributes, const std::wstring& nm, const std::wstring& v)
{
	attributes[nm] = v;
}
static void AddNumber(CDirectoryBrowseDlg::tStringMap& attributes, const std::wstring& nm, unsigned __int64 v)
{
	wchar_t buf[1024];
	swprintf_s(buf, L"%lld", v);
	attributes[nm] = buf;
}
static void AddHex(CDirectoryBrowseDlg::tStringMap& attributes, const std::wstring& nm, unsigned __int64 v)
{
	wchar_t buf[1024];
	swprintf_s(buf, L"0x%llX", v);
	attributes[nm] = buf;
}
static void AddFileTime(CDirectoryBrowseDlg::tStringMap& attributes, const std::wstring& name, FILETIME ft)
{
	attributes[name] = (LPCWSTR)CTime(ft).Format(L"%c");
}
static unsigned __int64 MakeInt64(DWORD high, DWORD low)
{
	unsigned __int64 l = low;
	unsigned __int64 h = high;
	return (l | (h << 32));
}

#define CHECKFLAG( F )  { if ( dwAttributes & F ) { l.push_back(L#F);  }}
void CDirectoryBrowseDlg::DecodeAttributes(DWORD dwAttributes, tStringList& l)
{
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
	CHECKFLAG(SECURITY_SQOS_PRESENT);
	CHECKFLAG(SECURITY_ANONYMOUS);
	CHECKFLAG(SECURITY_CONTEXT_TRACKING);
	CHECKFLAG(SECURITY_DELEGATION);
	CHECKFLAG(SECURITY_EFFECTIVE_ONLY);
	CHECKFLAG(SECURITY_IDENTIFICATION);
	CHECKFLAG(SECURITY_IMPERSONATION);
}

void CDirectoryBrowseDlg::GetFileInformation(const CString& filename, FileInfo& fileInfo )
{
	fileInfo.Reset();

	WIN32_FIND_DATA findData;
	memset(&findData, 0, sizeof(findData));
	HANDLE hFind = ::FindFirstFile((LPCTSTR)filename, &findData);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		FindClose(hFind);

		fileInfo.isDirectory = findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
		AddStringItem(fileInfo.FindFirst.properties, L"cFileName", findData.cFileName);
		AddStringItem(fileInfo.FindFirst.properties, L"cAlternateFileName", findData.cAlternateFileName);

		AddHex(fileInfo.FindFirst.properties, L"dwFileAttributes", findData.dwFileAttributes);
		AddNumber(fileInfo.FindFirst.properties, L"nFileSize", MakeInt64(findData.nFileSizeHigh, findData.nFileSizeLow));

		AddFileTime(fileInfo.FindFirst.properties, L"ftLastWriteTime", findData.ftLastWriteTime);
		AddFileTime(fileInfo.FindFirst.properties, L"ftLastAccessTime", findData.ftLastAccessTime);
		AddFileTime(fileInfo.FindFirst.properties, L"ftCreationTime", findData.ftCreationTime);

		AddHex(fileInfo.FindFirst.properties, L"dwReserved0", findData.dwReserved0);
		AddHex(fileInfo.FindFirst.properties, L"dwReserved1", findData.dwReserved1);

		DecodeAttributes(findData.dwFileAttributes, fileInfo.FindFirst.dwAttributes);
	}

	HANDLE hFile = CreateFile(filename, 0, 0, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		BY_HANDLE_FILE_INFORMATION fi;
		memset(&fi, 0, sizeof(BY_HANDLE_FILE_INFORMATION));
		if (GetFileInformationByHandle(hFile, &fi))
		{
			DecodeAttributes(fi.dwFileAttributes, fileInfo.ByHandle.dwAttributes);

			AddHex(fileInfo.ByHandle.properties, L"dwVolumeSerialNumber", fi.dwVolumeSerialNumber);
			AddNumber(fileInfo.ByHandle.properties, L"nNumberOfLinks", fi.nNumberOfLinks);
			AddHex(fileInfo.ByHandle.properties, L"nFileIndex", MakeInt64(fi.nFileIndexHigh, fi.nFileIndexLow));

			AddHex(fileInfo.ByHandle.properties, L"dwFileAttributes", fi.dwFileAttributes);
			AddNumber(fileInfo.FindFirst.properties, L"nFileSize", MakeInt64(fi.nFileSizeHigh, fi.nFileSizeLow));

			AddFileTime(fileInfo.ByHandle.properties, L"ftLastWriteTime", fi.ftLastWriteTime);
			AddFileTime(fileInfo.ByHandle.properties, L"ftLastAccessTime", fi.ftLastAccessTime);
			AddFileTime(fileInfo.ByHandle.properties, L"ftCreationTime", fi.ftCreationTime);
		}
		CloseHandle(hFile);
	}

	WIN32_FIND_STREAM_DATA streamInfo;
	memset(&streamInfo, 0, sizeof(WIN32_FIND_STREAM_DATA));
	HANDLE hFindStreams = FindFirstStreamW(filename, FindStreamInfoStandard, (LPVOID)&streamInfo, 0);
	if (hFindStreams != INVALID_HANDLE_VALUE)
	{
		do
		{
			fileInfo.alternateStreams.push_back(streamInfo.cStreamName);
			memset(&streamInfo, 0, sizeof(WIN32_FIND_STREAM_DATA));

		} while (FindNextStreamW(hFindStreams, (LPVOID)&streamInfo));

		FindClose(hFindStreams);
	}

}

void CDirectoryBrowseDlg::OnBnClickedButton1()
{
	m_FileView.LockWindowUpdate();
	m_FileView.DisplayParentFolder();
	m_FileView.UnlockWindowUpdate();
}
