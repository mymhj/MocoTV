#pragma once


// NameSetDlg 对话框

class NameSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(NameSetDlg)

public:
	NameSetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~NameSetDlg();

// 对话框数据
	enum { IDD = IDD_NAMESET };

	CEdit Name;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedbtnsavename();
};
