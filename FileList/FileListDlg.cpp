
// FileListDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "FileList.h"
#include "FileListDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include<algorithm>

#include "CDlgProgress.h"


using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}




// CFileListDlg 对话框



CFileListDlg::CFileListDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILELIST_DIALOG, pParent)
	, m_dstDir(_T(""))
	, m_filesuffix(_T(""))
	, m_filelist(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFileListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_dstDir);
	DDX_Text(pDX, IDC_EDIT3, m_filesuffix);
	DDX_Text(pDX, IDC_EDIT1, m_filelist);
	DDX_Control(pDX, IDOK, m_ButtonOK);
}

BEGIN_MESSAGE_MAP(CFileListDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CFileListDlg::OnBnClickedButton1)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDOK, &CFileListDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CFileListDlg 消息处理程序

BOOL CFileListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

		//process
	char temp[1024];
	memset(temp, 0, sizeof(temp));
	GetPrivateProfileString("INI", "SUFFIX", "exe", temp, sizeof(temp) - 1, "./FileList.ini");
	m_filesuffix = temp;
	UpdateData(FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CFileListDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFileListDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFileListDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void GetFileList(CString DstDir, vector<string> &FileList);

std::vector<std::string> allfiles;
unsigned int __stdcall WINAPI GetFileListThread(PVOID param)
{
	GetFileList((char *)param, allfiles);
	return true;
}


void CFileListDlg::OnBnClickedButton1()
{
	TCHAR szDir[MAX_PATH];
	BROWSEINFO bi;
	ITEMIDLIST* pidl;
	bi.hwndOwner = this->m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szDir;//这个是输出缓冲区  
	bi.lpszTitle = _T("选择输出文件夹："); //标题 
	bi.ulFlags = BIF_NEWDIALOGSTYLE;//使用新的界面,在win7中效果较好//BIF_RETURNONLYFSDIRS;  
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	pidl = SHBrowseForFolder(&bi);//弹出对话框  
	if (pidl == NULL)//点了取消，或者选择了无效的文件夹则返回NULL 
		return ;

	if (SHGetPathFromIDList(pidl, szDir))
	{
		m_dstDir = szDir;

		DWORD dwFlag = GetFileAttributes(szDir);
		if (dwFlag & FILE_ATTRIBUTE_DIRECTORY){
			m_dstDir = szDir;
		}else{
			m_dstDir = "";
		}
		UpdateData(FALSE);
	}
}


void CFileListDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	WritePrivateProfileString("INI", "SUFFIX", m_filesuffix, "./FileList.ini");

	CDialogEx::OnClose();
}

void GetFileList(CString DstDir, vector<string> &FileList)
{
	CFileFind  finder;
	CString    filename_with_path;
	CString    filename_no_path;
	CString    strWildcard(DstDir);
	strWildcard += _T("\\*.*");

	BOOL bWorking = finder.FindFile(strWildcard);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		filename_no_path = finder.GetFileName();
		filename_with_path = DstDir;
		filename_with_path += "\\";
		filename_with_path += filename_no_path;

		if (filename_no_path == "." || filename_no_path == "..")
			continue;

		if (GetFileAttributes(filename_with_path) & FILE_ATTRIBUTE_DIRECTORY){
			GetFileList(filename_with_path, FileList);
		}
		else{
			FileList.push_back(filename_no_path.GetBuffer());
		}
	}
}

//比较算法，用于sort调用中排序
bool comp(string a, string b)
{
	transform(a.begin(), a.end(), a.begin(), ::toupper);
	transform(b.begin(), b.end(), b.begin(), ::toupper);
	return a < b;
}

//显示进度条
unsigned int __stdcall WINAPI ShowProgressThread(PVOID param)
{
	CDlgProgress dlg((HANDLE)param);
	dlg.DoModal();
	return true;
}


//输入文件名列表 std::vector<string>& list
//后缀  std::string targetSfx
//分隔符 std::string sep
//返回 结果排序 || 不排序
//输入 "a.exe b.html c.exe" "exe" ","
//返回 "a.exe,c.exe"
class CTargetFileList
{
public:

	typedef BOOL(*COMP)(std::string, std::string);

	CTargetFileList(std::vector<string>& list, std::string targetSfx, std::string sep)
		: m_list(list), m_sfx(targetSfx), m_sep(sep){};
	
	std::string GetStringNoSort();
	std::string GetStringWithSort(COMP);

private:
	std::vector<string>& m_list;
	std::string m_sfx;
	std::string m_sep;
};

std::string CTargetFileList::GetStringNoSort()
{
	std::vector<std::string> temp;
	string res;

	temp.clear();
	for (auto v : m_list)
	{
		int pos = v.rfind('.');
		if (pos > 0 && v.substr(pos + 1) == m_sfx)
		{
			temp.push_back(v);
		}
	}

	for (auto v : temp) {
		res += v.c_str();
		res += m_sep;
		res += "\r\n";
	}

	return res;
}


std::string CTargetFileList::GetStringWithSort(COMP comp)
{
	std::vector<std::string> temp;
	string res;

	temp.clear();
	for (auto v : m_list)
	{
		int pos = v.rfind('.');
		if (pos > 0 && v.substr(pos + 1) == m_sfx)
		{
			temp.push_back(v);
		}
	}
	sort(begin(temp), end(temp), comp);
	for (auto v : temp) {
		res += v.c_str();
		res += m_sep;
		res += "\r\n";
	}
	return res;
}


void CFileListDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	vector<string> temp;

	
	if (GetFileAttributes(m_dstDir) == INVALID_FILE_ATTRIBUTES)
	{
		m_filelist = "INVALID_FILE_ATTRIBUTES";
		UpdateData(FALSE);
		return;
	}

	if (!(GetFileAttributes(m_dstDir) & FILE_ATTRIBUTE_DIRECTORY))
	{
		m_filelist = "INVALID_DIRECTORY";
		UpdateData(FALSE);
		return;
	}
	
	m_ButtonOK.EnableWindow(FALSE);

	//创建互斥量用于线程同步，当互斥量出于触发状态时，进度条对话框退出
	HANDLE hcs;
	hcs = CreateSemaphore(NULL, 0, 1, "9ee0152b-55e2-4d17-9a5e-304447c155fc");


	//开启线程遍历文件
	allfiles.clear();
	HANDLE fileThread = (HANDLE)_beginthreadex(NULL, 0, &GetFileListThread, (void*)m_dstDir.GetBuffer(), 0, NULL);
	HANDLE progThread = (HANDLE)_beginthreadex(NULL, 0, &ShowProgressThread, (void*)hcs, 0, NULL);
	while (fileThread) //使用事件触发+多线程，防止文件过多界面卡主，影响用户体验
	{
		MSG msg;
		switch (MsgWaitForMultipleObjectsEx(1, &fileThread, 120000, QS_ALLEVENTS, MWMO_INPUTAVAILABLE))  //等待线程完成
		//switch (WaitForSingleObject(fileThread, 120000))  //等待线程完成
		{
		case WAIT_FAILED:   //句柄无效
			ReleaseSemaphore(hcs, 1, NULL);  //通知进度线程退出
			m_filelist = "WAIT_FAILED";
			break;
		case WAIT_TIMEOUT:  //超时
			ReleaseSemaphore(hcs, 1, NULL);
			m_filelist = "WAIT_TIMEOUT";
			break;
		case WAIT_OBJECT_0: //完成
			ReleaseSemaphore(hcs, 1, NULL);
			{
				CTargetFileList res(allfiles, m_filesuffix.GetBuffer(), ",");
				m_filelist = res.GetStringWithSort((CTargetFileList::COMP)comp).c_str();
			}
			UpdateData(FALSE);
			break;

		case WAIT_OBJECT_0 + 1:
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE | PM_QS_INPUT | PM_QS_PAINT | PM_QS_POSTMESSAGE | PM_QS_SENDMESSAGE))   //接收消息
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			continue;

		default:
			break;
		}
		break;
	}
	
	m_ButtonOK.EnableWindow(TRUE);
	CloseHandle(progThread);
	CloseHandle(fileThread);
}
