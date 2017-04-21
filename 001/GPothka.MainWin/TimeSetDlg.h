#pragma once


// CTimeSetDlg 对话框

class CTimeSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTimeSetDlg)

public:
	CTimeSetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTimeSetDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_TIMESET };

	CEdit timeSetEdit;
	CEdit waitTimeEdit;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedbtntimeset();
};
