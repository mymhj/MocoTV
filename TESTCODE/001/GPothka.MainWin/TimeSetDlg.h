#pragma once


// CTimeSetDlg �Ի���

class CTimeSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTimeSetDlg)

public:
	CTimeSetDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTimeSetDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_TIMESET };

	CEdit timeSetEdit;
	CEdit waitTimeEdit;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedbtntimeset();
};
