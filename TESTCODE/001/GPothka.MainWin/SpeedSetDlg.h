#pragma once


// SpeedSetDlg �Ի���

class SpeedSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SpeedSetDlg)

public:
	SpeedSetDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~SpeedSetDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_SPEEDSET };

	CEdit gSpeed;//�˶�ȫ���ٶ�
	CEdit sSpeed;//ʵʱ����ĩ���ٶ�
	CEdit sSpeedE1;//ʵʱ���ƹ���ٶ�

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedbtnspeedset();
};
