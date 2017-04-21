// TimeSetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GPothka.MainWin.h"
#include "TimeSetDlg.h"
#include "afxdialogex.h"


// CTimeSetDlg 对话框

IMPLEMENT_DYNAMIC(CTimeSetDlg, CDialogEx)

CTimeSetDlg::CTimeSetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTimeSetDlg::IDD, pParent)
{

}

CTimeSetDlg::~CTimeSetDlg()
{
}

void CTimeSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_mTimeSetEdit, timeSetEdit);
	DDX_Control(pDX, IDC_wTime, waitTimeEdit);
}


BEGIN_MESSAGE_MAP(CTimeSetDlg, CDialogEx)
	ON_BN_CLICKED(IDC_btnTimeSet, &CTimeSetDlg::OnBnClickedbtntimeset)
END_MESSAGE_MAP()


// CTimeSetDlg 消息处理程序


void CTimeSetDlg::OnBnClickedbtntimeset()
{
	// TODO:  在此添加控件通知处理程序代码
	CString timeSet = "";
	timeSetEdit.GetWindowText(timeSet);
	runTime = atof(timeSet);

	CString waitTime = "";
	waitTimeEdit.GetWindowText(waitTime);
	wTime = atof(waitTime);
	
	GetDlgItem(IDOK)->EnableWindow(TRUE);



}
