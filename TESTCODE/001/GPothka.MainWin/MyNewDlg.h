#pragma once
#include "GP_Lens_Fujinon.h"
// CMyNewDlg 对话框

class CMyNewDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMyNewDlg)

public:
	CMyNewDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyNewDlg();
	friend class CGPothkaMainWinDlg;
// 对话框数据
	enum { IDD = IDD_DIALOG_CONFIG };

protected:
	HICON m_hIcon;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:

	friend class CGPothkaMainWinDlg;

	afx_msg void OnBnClickedbtninitport();
	
	GP_Lens_Fujinon m_Lens_Fujinon;
		
	CComboBox m_cbxSerialPort;
	CSliderCtrl m_sldFocusMotorValue;
	CSliderCtrl m_sldZoomMotorValue;

	int GetComList_Reg(CComboBox * CCombox);

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedbtnsetswitch4();
	WORD ZoomResultValue, FocusResultValue;

};
