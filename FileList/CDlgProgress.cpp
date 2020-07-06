// CDlgProgress.cpp: 实现文件
//

#include "pch.h"
#include "FileList.h"
#include "CDlgProgress.h"
#include "afxdialogex.h"


// CDlgProgress 对话框

IMPLEMENT_DYNAMIC(CDlgProgress, CDialogEx)

CDlgProgress::CDlgProgress(HANDLE cs, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent), m_cs(cs)
{
	
}

CDlgProgress::~CDlgProgress()
{
}

void CDlgProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_ProgressBar);
}


BEGIN_MESSAGE_MAP(CDlgProgress, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgProgress 消息处理程序


void CDlgProgress::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	static int bar = 0;
	DWORD ds = WaitForSingleObject(m_cs, 0);
	switch (ds)
	{
	case WAIT_OBJECT_0:
	case WAIT_FAILED:
		CloseHandle(m_cs);
		EndDialog(0);
		break;
	case WAIT_TIMEOUT:
		bar += 10;
		m_ProgressBar.SetPos(bar % 100);
		break;
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


BOOL CDlgProgress::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ProgressBar.SetRange(0, 100);
	m_ProgressBar.SetPos(0);
	SetTimer(0, 200, nullptr);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
