#pragma once


// SpeedSetDlg 对话框

class SpeedSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SpeedSetDlg)

public:
	SpeedSetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SpeedSetDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_SPEEDSET };

	CEdit gSpeed;//运动全局速度
	CEdit sSpeed;//实时控制末端速度
	CEdit sSpeedE1;//实时控制轨道速度

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedbtnspeedset();
};
