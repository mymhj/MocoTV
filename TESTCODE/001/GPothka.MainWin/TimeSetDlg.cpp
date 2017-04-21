// TimeSetDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GPothka.MainWin.h"
#include "TimeSetDlg.h"
#include "afxdialogex.h"


// CTimeSetDlg �Ի���

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


// CTimeSetDlg ��Ϣ�������


void CTimeSetDlg::OnBnClickedbtntimeset()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString timeSet = "";
	timeSetEdit.GetWindowText(timeSet);
	runTime = atof(timeSet);

	CString waitTime = "";
	waitTimeEdit.GetWindowText(waitTime);
	wTime = atof(waitTime);
	
	GetDlgItem(IDOK)->EnableWindow(TRUE);



}
