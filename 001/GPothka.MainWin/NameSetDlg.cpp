// NameSetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GPothka.MainWin.h"
#include "NameSetDlg.h"
#include "afxdialogex.h"


// NameSetDlg 对话框

IMPLEMENT_DYNAMIC(NameSetDlg, CDialogEx)

NameSetDlg::NameSetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(NameSetDlg::IDD, pParent)
{

}

NameSetDlg::~NameSetDlg()
{
}

void NameSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDITframeName, Name);
}


BEGIN_MESSAGE_MAP(NameSetDlg, CDialogEx)
	ON_BN_CLICKED(IDC_btnSaveName, &NameSetDlg::OnBnClickedbtnsavename)
END_MESSAGE_MAP()


// NameSetDlg 消息处理程序


void NameSetDlg::OnBnClickedbtnsavename()
{
	// TODO:  在此添加控件通知处理程序代码
	Name.GetWindowText(keyframeName);

	GetDlgItem(IDOK)->EnableWindow(TRUE);
}
