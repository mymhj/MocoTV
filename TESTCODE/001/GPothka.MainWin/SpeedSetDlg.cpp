// SpeedSetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GPothka.MainWin.h"
#include "SpeedSetDlg.h"
#include "afxdialogex.h"


// SpeedSetDlg 对话框

IMPLEMENT_DYNAMIC(SpeedSetDlg, CDialogEx)

SpeedSetDlg::SpeedSetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(SpeedSetDlg::IDD, pParent)
{

}

SpeedSetDlg::~SpeedSetDlg()
{
}

void SpeedSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_gWholeSpeed, gSpeed);
	DDX_Control(pDX, IDC_sWholeSpeed, sSpeed);
	DDX_Control(pDX, IDC_sWholeSpeedE1, sSpeedE1);
}


BEGIN_MESSAGE_MAP(SpeedSetDlg, CDialogEx)
	ON_BN_CLICKED(IDC_btnSpeedSet, &SpeedSetDlg::OnBnClickedbtnspeedset)
END_MESSAGE_MAP()

BOOL SpeedSetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CString str;
	str.Format("%.0f", gWholeSpeed*5);
	gSpeed.SetWindowTextA(str);

	str.Format("%.0f", sWholeSpeed*10);
	sSpeed.SetWindowTextA(str);

	str.Format("%.0f", sWholeSpeedE1 * 10);
	sSpeedE1.SetWindowTextA(str);
	return TRUE;
}

// SpeedSetDlg 消息处理程序


void SpeedSetDlg::OnBnClickedbtnspeedset()
{
	// TODO:  在此添加控件通知处理程序代码
	CString str = "";
	gSpeed.GetWindowText(str);
	gWholeSpeed = 0.2*atof(str);
	if (gWholeSpeed == 0 || gWholeSpeed > 1.65)
	{
		MessageBox("设定速度超范围（1-8）!");
		if (gWholeSpeed == 0)
			gWholeSpeed = 0.2;
		else
			gWholeSpeed = 1.6;
		str.Format("%.0f", gWholeSpeed * 5);
		gSpeed.SetWindowTextA(str);
	}

	sSpeed.GetWindowText(str);
	sWholeSpeed = 0.1*atof(str);
	if (sWholeSpeed == 0 || sWholeSpeed > 0.85)
	{
		MessageBox("设定速度超范围（1-8）!");
		if (sWholeSpeed == 0)
			sWholeSpeed = 0.1;
		else
			sWholeSpeed = 0.8;
		str.Format("%.0f", sWholeSpeed * 10);
		sSpeed.SetWindowTextA(str);
	}

	sSpeedE1.GetWindowText(str);
	sWholeSpeedE1 = 0.1*atof(str);
	if (sWholeSpeedE1 == 0 || sWholeSpeedE1 > 0.85)
	{
		MessageBox("设定速度超范围（1-8）!");
		if (sWholeSpeedE1 == 0)
			sWholeSpeedE1 = 0.1;
		else
			sWholeSpeedE1 = 0.8;
		str.Format("%.0f", sWholeSpeedE1 * 10);
		sSpeedE1.SetWindowTextA(str);
	}

	GetDlgItem(IDOK)->EnableWindow(TRUE);
}
