// NameSetDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GPothka.MainWin.h"
#include "NameSetDlg.h"
#include "afxdialogex.h"


// NameSetDlg �Ի���

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


// NameSetDlg ��Ϣ�������


void NameSetDlg::OnBnClickedbtnsavename()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	Name.GetWindowText(keyframeName);

	GetDlgItem(IDOK)->EnableWindow(TRUE);
}
