#pragma once


// CameraControlDlg 对话框

class CameraControlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CameraControlDlg)

public:
	CameraControlDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CameraControlDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedbtngetcontrol();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	int i = 0;

	CSliderCtrl sldFoc;
	CSliderCtrl sldZom;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
