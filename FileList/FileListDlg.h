
// FileListDlg.h: 头文件
//

#pragma once

#include <vector>
#include <iostream>

struct threadArg
{
	char* arg1;
	std::vector<std::string>* arg2;
};

// CFileListDlg 对话框
class CFileListDlg : public CDialogEx
{


// 构造
public:
	CFileListDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILELIST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CString m_dstDir;
	// 文件名后缀
	CString m_filesuffix;
	afx_msg void OnClose();
	afx_msg void OnBnClickedOk();
	CString m_filelist;
	CButton m_ButtonOK;
};
