#pragma once


// CDlgProgress 对话框

class CDlgProgress : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgProgress)

public:
	CDlgProgress(HANDLE cs,  CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgProgress();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PROGRESS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl m_ProgressBar;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();

	HANDLE m_cs;
};
