#pragma once


// NameSetDlg �Ի���

class NameSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(NameSetDlg)

public:
	NameSetDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~NameSetDlg();

// �Ի�������
	enum { IDD = IDD_NAMESET };

	CEdit Name;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedbtnsavename();
};
