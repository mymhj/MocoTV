#pragma once
// GPothka.MainWinDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GPothka.MainWin.h"
#include "GPothka.MainWinDlg.h"
#include "afxdialogex.h"
#include <wtypes.h>
#include <string>
#include <conio.h>
#include ".\kinematic\ExportTrajectory.h"
#include <string>
#include <iostream> 
#include <stdio.h>
#include <d3d9.h>
//#include <d3dx9.h>
#include "../FREED/FreedDll.h"
//#include "FreedDll.h"
#include "../FREED/FreedCommunication.h"
#include "windows.h"
#include "CameraControlDlg.h"
#include "TimeSetDlg.h"
#include "GP_Lens_Fujinon.h"
#include "GP_Lens_Canon.h"
#include "TrackRepeatModule.h"
#include "SpeedSetDlg.h"
#include "SkinH.h"
#include "NameSetDlg.h"
#include "DeckLinkDevice.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CGPothkaMainWinDlg 对话框

CGPothkaMainWinDlg::CGPothkaMainWinDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CGPothkaMainWinDlg::IDD, pParent), m_deckLinkDiscovery(NULL),
m_selectedDevice(NULL)
	
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGPothkaMainWinDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_listKeyFrames, m_listKeyFrames);
	DDX_Control(pDX, IDC_cbxframesIndex, m_cbxframesIndex);
	DDX_Control(pDX, IDC_CameraList, m_cbxcameraList);

	//页码
	DDX_Control(pDX, IDC_pageIndex,page);

	//状态栏
	DDX_Control(pDX, IDC_editState, State);


}

BEGIN_MESSAGE_MAP(CGPothkaMainWinDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_btnStartListenServer, &CGPothkaMainWinDlg::OnBnClickedbtnStartListenServer)
	ON_COMMAND(IDC_btnNewFrameIndex, &CGPothkaMainWinDlg::Onbtnnewframeindex)
	ON_COMMAND(IDC_btnDeleteFrameIndex, &CGPothkaMainWinDlg::Onbtndeleteframeindex)
	ON_BN_CLICKED(IDC_btnAddNewKeyFrame, &CGPothkaMainWinDlg::OnBnClickedbtnaddNewkeyframe)
	ON_BN_CLICKED(IDC_btnUpdateFrameKeyFrame, &CGPothkaMainWinDlg::OnBnClickedbtnUpdateframekeyframe)
	ON_COMMAND(IDC_btnOutPutTrack, &CGPothkaMainWinDlg::Onbtnoutputtrack)
	ON_COMMAND(IDC_btnImportTrack, &CGPothkaMainWinDlg::Onbtnimporttrack)

	ON_BN_CLICKED(IDC_btnSingleMotion, &CGPothkaMainWinDlg::OnBnClickedbtnSinglemotion)
	ON_BN_CLICKED(IDC_btnRestorationMotion, &CGPothkaMainWinDlg::OnBnClickedbtnRestorationmotion)
	ON_BN_CLICKED(IDC_btnStartRepeatMotion, &CGPothkaMainWinDlg::OnBnClickedbtnStartrepeatmotion)
	ON_CBN_SELCHANGE(IDC_cbxframesIndex, &CGPothkaMainWinDlg::OnCbnSelchangecbxFramesIndex)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_listKeyFrames, &CGPothkaMainWinDlg::OnLvnItemchangedlistkeyframes)
	ON_BN_CLICKED(IDC_btnSuspend, &CGPothkaMainWinDlg::OnBnClickedbtnSuspend)
	

	ON_MESSAGE(WM_STICK_EVENT, StickEvent)
	ON_MESSAGE(WM_LIMIT_STATE, LimitState)

	ON_BN_CLICKED(IDOK, &CGPothkaMainWinDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDOK2, &CGPothkaMainWinDlg::OnCancel)
	ON_BN_CLICKED(IDC_btnCalibration, &CGPothkaMainWinDlg::OnBnClickedbtncalibration)
	ON_BN_CLICKED(IDC_btnSendCameraData, &CGPothkaMainWinDlg::OnBnClickedbtnsendcameradata)
	ON_COMMAND(IDC_btnKeyFrameTimeSet, &CGPothkaMainWinDlg::Onbtnkeyframetimeset)
	
	ON_BN_CLICKED(IDC_btnPrePage, &CGPothkaMainWinDlg::OnBnClickedbtnprepage)
	ON_BN_CLICKED(IDC_btnNextPage, &CGPothkaMainWinDlg::OnBnClickedbtnnextpage)
	ON_BN_CLICKED(IDC_btnInitPortCam, &CGPothkaMainWinDlg::OnBnClickedbtninitportcam)
	
	ON_COMMAND(IDC_btnFrameName, &CGPothkaMainWinDlg::OnbtnkeyframeNameSet)
	ON_COMMAND(IDC_btnDeleteFrame, &CGPothkaMainWinDlg::Onbtndeleteframe)
	ON_COMMAND(IDC_btnCameraControl, &CGPothkaMainWinDlg::Onbtncameracontrol)
	ON_COMMAND(IDC_btnSpeedControl, &CGPothkaMainWinDlg::Onbtnspeedcontrol)


	ON_COMMAND(IDC_btnRollLock, &CGPothkaMainWinDlg::Onbtnrolllock)
	ON_COMMAND(IDC_btnRollUnlock, &CGPothkaMainWinDlg::Onbtnrollunlock)

	ON_COMMAND(IDC_btnRobotLock, &CGPothkaMainWinDlg::Onbtnrobotlock)
	ON_COMMAND(IDC_btnRobotUnlock, &CGPothkaMainWinDlg::Onbtnrobotunlock)

	ON_COMMAND(IDC_btnCameraLock, &CGPothkaMainWinDlg::Onbtncameralock)
	ON_COMMAND(IDC_btnCameraUnlock, &CGPothkaMainWinDlg::Onbtncameraunlock)
	//自定义消息
	ON_MESSAGE(WM_ADD_DEVICE_MESSAGE, &CGPothkaMainWinDlg::OnAddDevice)
	ON_WM_CLOSE()
	ON_COMMAND(ID_btnHelp, &CGPothkaMainWinDlg::Onbtnhelp)
	ON_COMMAND(ID_32819, &CGPothkaMainWinDlg::OnbtnSoftLock)
	ON_COMMAND(32808, &CGPothkaMainWinDlg::OnbtnSoftUnlock)
END_MESSAGE_MAP()
LRESULT CGPothkaMainWinDlg::TraceLogger(WPARAM wParam, LPARAM lParam)
{   
	CString info=(char*)lParam;
	m_txtLogger.SetRedraw(FALSE);
	int infoLen = m_txtLogger.GetWindowTextLength ();
	m_txtLogger.SetSel(infoLen,infoLen); 
	info += _T("\r\n");
	m_txtLogger.ReplaceSel(info); 
	m_txtLogger.LineScroll(m_txtLogger.GetLineCount()); 

	m_txtLogger.SetRedraw(TRUE);
	m_txtLogger.SetFocus();

	return 0;
}


// CGPothkaMainWinDlg 消息处理程序
BOOL CGPothkaMainWinDlg::OnInitDialog()
{
	this->ShowWindow(SW_SHOWMAXIMIZED);
	CDialogEx::OnInitDialog();

	
	// 将“关于...”菜单项添加到系统菜单中。
	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	//皮肤
	SkinH_Attach();

	//初始化关键帧列表控件
	DWORD dwStyle = m_listKeyFrames.GetExtendedStyle();                
	dwStyle |= LVS_EX_FULLROWSELECT;            
	dwStyle |= LVS_EX_GRIDLINES;                
	m_listKeyFrames.SetExtendedStyle(dwStyle);
	m_listKeyFrames.InsertColumn(0,_T("IPOC"),LVCFMT_LEFT,55,-1);
	m_listKeyFrames.InsertColumn(1,_T("局部速度"),LVCFMT_LEFT,70);
	m_listKeyFrames.InsertColumn(2,_T("等待时间"),LVCFMT_LEFT,70);
	m_listKeyFrames.InsertColumn(3, _T("X"), LVCFMT_LEFT, 55);
	m_listKeyFrames.InsertColumn(4, _T("Y"), LVCFMT_LEFT, 55);
	m_listKeyFrames.InsertColumn(5, _T("Z"), LVCFMT_LEFT, 55);
	m_listKeyFrames.InsertColumn(6, _T("A"), LVCFMT_LEFT, 55);
	m_listKeyFrames.InsertColumn(7, _T("B"), LVCFMT_LEFT, 55);
	m_listKeyFrames.InsertColumn(8, _T("C"), LVCFMT_LEFT, 55);
	m_listKeyFrames.InsertColumn(9, _T("Zoom"), LVCFMT_LEFT, 55);
	m_listKeyFrames.InsertColumn(10, _T("Focus"), LVCFMT_LEFT, 55);
	m_listKeyFrames.InsertColumn(11, _T("运行到该点时间"), LVCFMT_LEFT, 100);
	m_listKeyFrames.InsertColumn(12, _T("最小运行时间"), LVCFMT_LEFT, 100);
	

	GetDlgItem(IDC_btnSuspend)->EnableWindow(FALSE);
	GetDlgItem(IDC_cbxframesIndex)->EnableWindow(FALSE);
	GetDlgItem(IDC_btnAddNewKeyFrame)->EnableWindow(FALSE);
	GetDlgItem(IDC_listKeyFrames)->EnableWindow(FALSE);

	GetDlgItem(IDC_btnUpdateFrameKeyFrame)->EnableWindow(FALSE);
	GetDlgItem(IDC_btnRestorationMotion)->EnableWindow(FALSE);
	GetDlgItem(IDC_btnStartRepeatMotion)->EnableWindow(FALSE);
	GetDlgItem(IDC_btnSingleMotion)->EnableWindow(FALSE);
	

	m_Menu.LoadMenuA(IDR_MENU1);
	SetMenu(&m_Menu);

	GetMenu()->GetSubMenu(0)->EnableMenuItem(0, MF_BYPOSITION | MF_DISABLED);
	GetMenu()->GetSubMenu(0)->EnableMenuItem(1, MF_BYPOSITION | MF_DISABLED);
	GetMenu()->GetSubMenu(0)->EnableMenuItem(3, MF_BYPOSITION | MF_DISABLED);
	GetMenu()->GetSubMenu(0)->EnableMenuItem(5, MF_BYPOSITION | MF_DISABLED);
	GetMenu()->GetSubMenu(0)->EnableMenuItem(6, MF_BYPOSITION | MF_DISABLED);
	GetMenu()->GetSubMenu(0)->EnableMenuItem(8, MF_BYPOSITION | MF_DISABLED);
	GetMenu()->GetSubMenu(0)->EnableMenuItem(9, MF_BYPOSITION | MF_DISABLED);

	GetMenu()->GetSubMenu(1)->EnableMenuItem(1, MF_BYPOSITION | MF_DISABLED);
	GetMenu()->GetSubMenu(1)->EnableMenuItem(4, MF_BYPOSITION | MF_DISABLED);
	GetMenu()->GetSubMenu(1)->EnableMenuItem(7, MF_BYPOSITION | MF_DISABLED);
	GetMenu()->GetSubMenu(1)->EnableMenuItem(10, MF_BYPOSITION | MF_DISABLED);


	//初始化decklink检测设备类对象
	m_deckLinkDiscovery = new DeckLinkDeviceDiscovery(this);
	m_deckLinkDiscovery->Enable();

	CRect rect;
	GetClientRect(&rect);

	mocoLog.Init("Log");
	mocoLog.StatusOut(Info, "软件初始化成功");

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


void CGPothkaMainWinDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGPothkaMainWinDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CGPothkaMainWinDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CGPothkaMainWinDlg::OnBnClickedbtnStartListenServer()
{
	//TODO: 在此添加控件通知处理程序代码 
	if (softLock == 1)
	{
		MessageBox("软件处于锁定状态！");
		return;
	}

	/*if(m_CAppWorkPlant.StartServer(this->m_hWnd,this->m_picVideo.m_hWnd)==true)
	{*/
		GetDlgItem(IDC_btnStartListenServer)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnSuspend)->EnableWindow(TRUE);
		GetDlgItem(IDC_cbxframesIndex)->EnableWindow(TRUE);
		GetMenu()->GetSubMenu(0)->EnableMenuItem(0, MF_BYPOSITION | MF_ENABLED);
		GetDlgItem(IDC_btnAddNewKeyFrame)->EnableWindow(FALSE);
		GetMenu()->GetSubMenu(0)->EnableMenuItem(3, MF_BYPOSITION | MF_DISABLED);
		

		//OnBnClickedbtninitfreed();
		//OnBnClickedbtnsendcameradata();
	
	    GetDlgItem(IDC_CameraList)->GetWindowTextA(camera);

		//获取帧//
		//m_selectedDevice->StartCapture(true);
		//m_selectedDevice->m_SaveFrameFlag = false;
	//}
	//else
	//{
	//	MessageBox("请连接摇杆！");
	//	GetDlgItem(IDC_btnStartListenServer)->EnableWindow(TRUE);
	//	return ;
	//}
	if (camera == "Fujinon")
	{
		CGPothkaMainWinDlg *pDlg = this;
		camCon = AfxBeginThread(camControl, pDlg, 0, 0, 0, NULL);
		pDlg->mocoLog.StatusOut(Info, "富士镜头控制线程开启");
	}
	else if (camera == "Canon")
	{
		CGPothkaMainWinDlg *pDlg = this;
		Canon = AfxBeginThread(CanonContrl, pDlg, 0, 0, 0, NULL);
		pDlg->mocoLog.StatusOut(Info, "佳能镜头控制线程开启");
	}
	GetDlgItem(IDC_CameraList)->EnableWindow(FALSE);

	//自动载入上次关闭时轨迹	
	//ImportTrack();

}

//佳能镜头控制线程
UINT __cdecl CanonContrl(LPVOID lpParamter)
{
	WORD FocusValue, ZoomValue;
	CGPothkaMainWinDlg* pCanonDlg = (CGPothkaMainWinDlg*)lpParamter;

	//镜头初始化
	pCanonDlg->OnBnClickedbtninitportcamCanon();

	int temp = 0;


	while (stopThread==1)
	{
		if (softControl == 0 && (focCon == 1 || zomCon == 1))
		{
			if (zomCon == 1)
			{

				pCanonDlg->m_Lens_Canon.ZoomSpeedSet(zomVal);
				pCanonDlg->m_Lens_Canon.GetLensMotorZoom(ZoomValue);
				Zoom = ZoomValue;

			}
			else if (focCon == 1)
			{
				temp = Focus;
				temp = temp + focVal;

				if (temp >= 60000)
				{
					temp = 60000;
				}
				else if (temp < 0)
				{
					temp = 0;
				}

				pCanonDlg->m_Lens_Canon.SetLensMotorFocus(temp);

				Focus = temp;

				focCon = 0;
			}
		}
		else
		{
			pCanonDlg->m_Lens_Canon.SetLensMotorZoom(Zoom);
			pCanonDlg->m_Lens_Canon.SetLensMotorFocus(Focus);
		}
	}

	return true;
}


//遥杆控制镜头线程
UINT __cdecl camControl(LPVOID lpParamter)
{
	WORD FocusValue, ZoomValue;
	CGPothkaMainWinDlg* mlg = (CGPothkaMainWinDlg*)lpParamter;
	
	//镜头初始化
	mlg->OnBnClickedbtninitportcam();
	
	int temp = 0;

	LARGE_INTEGER lageinter;
	LONGLONG CountBegin, CountEnd;
	double diFreq, diTime;

	while (stopThread==1)
	{
		if (softControl == 0 && (focCon == 1 || zomCon == 1))
		{
			
			if (focCon == 1)
			{
				temp = Focus;
				temp = temp + focVal;

				if (temp >= 4095)
				{
					temp = 4095;
				}
				else if (temp < 0)
				{
					temp = 0;
				}
				
				mlg->m_Lens_Fujinon.SetSwitch4Host(0xFC);
				mlg->m_Lens_Fujinon.SetLensMotorFocus(temp);

				Focus = temp;

				focCon = 0;
			}
			if (zomCon == 1)
			{
				mlg->m_Lens_Fujinon.SetSwitch4Host(0xFC);
				mlg->m_Lens_Fujinon.SetMotorSpeedsZoom(zomVal);
				
				mlg->m_Lens_Fujinon.GetLensMotorZoom(ZoomValue);
				Zoom = ZoomValue;

				zomCon = 0;
			}
			
		}
	
	}
	
	return 0;
}


void CGPothkaMainWinDlg::OnBnClickedbtnSuspend()
{
	 //TODO: 在此添加控件通知处理程序代码
	if (softLock == 1)
	{
		MessageBox("软件处于锁定状态！");
		return;
	}

	if(gRunState.Suspend==true)
	{
		  gRunState.Suspend=false;
		
		  GetDlgItem(IDC_btnSuspend)->SetWindowTextA("暂停");

		  GetDlgItem(IDC_btnRestorationMotion)->EnableWindow(TRUE);

		  mocoLog.StatusOut(Info,"软件暂停");
	}
	else
	{
		gRunState.Suspend=true;
		
		GetDlgItem(IDC_btnSuspend)->SetWindowTextA("取消暂停");

		GetDlgItem(IDC_btnRestorationMotion)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnStartRepeatMotion)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnSingleMotion)->EnableWindow(FALSE);
		mocoLog.StatusOut(Info, "软件暂停取消");
	}
}

//创建新轨迹
void CGPothkaMainWinDlg::Onbtnnewframeindex()
{
	// TODO:  在此添加命令处理程序代码
	if (softLock == 1)
	{
		MessageBox("软件处于锁定状态！");
		return;
	}

	frameNum = 0;
	totalPage = 0;
	currentPage = 0;

	loadIndex = 0;
	CString indexNewtrack;
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	indexNewtrack.Format(_T("%d%d%d%d%d%d"), sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond);
	if (m_CAppWorkPlant.m_TrackRepeatModule.AddNewTrackToIndexTrack(indexNewtrack.GetBuffer(0)) == true){
		mocoLog.StatusOut(Info, "创建新轨迹 %s", indexNewtrack);
		CString temp;
		if (m_cbxframesIndex.GetCount())
		{
			int tindex = m_cbxframesIndex.GetCurSel();
			m_cbxframesIndex.GetLBText(tindex, temp);
		}
		if (m_cbxframesIndex.GetCount() == 0 || temp != indexNewtrack)
		{
			int a = m_cbxframesIndex.AddString(indexNewtrack);
			m_cbxframesIndex.SetCurSel(a);

			if (m_cbxframesIndex.GetCount())
			{
				GetMenu()->GetSubMenu(0)->EnableMenuItem(1, MF_BYPOSITION | MF_ENABLED);
				GetDlgItem(IDC_btnAddNewKeyFrame)->EnableWindow(TRUE);
				GetMenu()->GetSubMenu(0)->EnableMenuItem(3, MF_BYPOSITION | MF_ENABLED);

				GetMenu()->GetSubMenu(0)->EnableMenuItem(9, MF_BYPOSITION | MF_ENABLED);
		
				GetDlgItem(IDC_listKeyFrames)->EnableWindow(TRUE);
				

			}
			frameNumCurrentPage = 0;
			pageImages();

			char c_logInfo[100] = "创建轨迹：";
			strcat_s(c_logInfo, 100, indexNewtrack.GetBuffer(0));
			
		}
	}
	else
	{	//创建失败
		char c_logInfo[100] = "失败创建轨迹：";
		strcat_s(c_logInfo, 100, indexNewtrack.GetBuffer(0));
		
	}

}

//导入轨迹时自动创建新轨迹
void CGPothkaMainWinDlg::Onbtnnewframeindex(CString dir)
{
	// TODO:  在此添加命令处理程序代码
	frameNum = 0;
	totalPage = 0;
	currentPage = 0;
	loadIndex = 1;
	if (m_CAppWorkPlant.m_TrackRepeatModule.AddNewTrackToIndexTrack(dir.GetBuffer(0)) == true){
		mocoLog.StatusOut(Info, "导入轨迹创建新轨迹 %s", dir);
		CString temp;
		if (m_cbxframesIndex.GetCount())
		{
			int tindex = m_cbxframesIndex.GetCurSel();
			m_cbxframesIndex.GetLBText(tindex, temp);
		}
		if (m_cbxframesIndex.GetCount() == 0 || temp != dir)
		{
			int a = m_cbxframesIndex.AddString(dir);
			m_cbxframesIndex.SetCurSel(a);

			if (m_cbxframesIndex.GetCount())
			{
				GetMenu()->GetSubMenu(0)->EnableMenuItem(1, MF_BYPOSITION | MF_ENABLED);
				GetDlgItem(IDC_btnAddNewKeyFrame)->EnableWindow(TRUE);
				GetMenu()->GetSubMenu(0)->EnableMenuItem(3, MF_BYPOSITION | MF_DISABLED);
				GetDlgItem(IDC_listKeyFrames)->EnableWindow(TRUE);
			}
			
			char c_logInfo[100] = "创建轨迹：";
			strcat_s(c_logInfo, 100, dir.GetBuffer(0));
		}
	}
	else
	{	//创建失败
		char c_logInfo[100] = "失败创建轨迹：";
		strcat_s(c_logInfo, 100, dir.GetBuffer(0));
	}
}

//删除轨迹
void CGPothkaMainWinDlg::Onbtndeleteframeindex()
{
	// TODO:  在此添加命令处理程序代码
	if (softLock == 1)
	{
		MessageBox("软件处于锁定状态！");
		return;
	}

	totalPage = 0;
	currentPage = 0;
	frameNum = 0;
	frameNumCurrentPage = 0;


	CString strCBText;
	int nIndex = m_cbxframesIndex.GetCurSel();
	m_cbxframesIndex.GetLBText(nIndex, strCBText);
	char* indexNewtrack = strCBText.GetBuffer(0);

	if (true == m_CAppWorkPlant.m_TrackRepeatModule.DeleteTrackToIndexTrack(indexNewtrack))
	{
		mocoLog.StatusOut(Info, "删除轨迹 %s", indexNewtrack);
		m_cbxframesIndex.DeleteString(nIndex);

		m_cbxframesIndex.SetWindowTextA("");

		char c_logInfo[100] = "成功删除轨迹：";
		strcat_s(c_logInfo, 100, indexNewtrack);
	
		if (m_cbxframesIndex.GetCount() != 0)
		{
			m_cbxframesIndex.SetCurSel(m_cbxframesIndex.GetCount() - 1);
		}

		m_listKeyFrames.DeleteAllItems();
		int count = m_cbxframesIndex.GetCount();
		if (m_cbxframesIndex.GetCount() == 0)
		{
			GetMenu()->GetSubMenu(0)->EnableMenuItem(1, MF_BYPOSITION | MF_DISABLED);
			GetDlgItem(IDC_btnAddNewKeyFrame)->EnableWindow(FALSE);
			GetMenu()->GetSubMenu(0)->EnableMenuItem(3, MF_BYPOSITION | MF_DISABLED);
			GetMenu()->GetSubMenu(0)->EnableMenuItem(8, MF_BYPOSITION | MF_DISABLED);
			GetMenu()->GetSubMenu(0)->EnableMenuItem(9, MF_BYPOSITION | MF_DISABLED);
			GetDlgItem(IDC_listKeyFrames)->EnableWindow(FALSE);
			
			GetDlgItem(IDC_btnUpdateFrameKeyFrame)->EnableWindow(FALSE);
			GetDlgItem(IDC_btnRestorationMotion)->EnableWindow(FALSE);
			GetDlgItem(IDC_btnStartRepeatMotion)->EnableWindow(FALSE);
			GetDlgItem(IDC_btnSingleMotion)->EnableWindow(FALSE);
			
			CString str;
			str.Format("  ");
			GetDlgItem(IDC_pageIndex)->SetWindowTextA(str);

		}
		if (m_cbxframesIndex.GetCount() != 0)
		{
			CString strCBText;
			int nIndex = m_cbxframesIndex.GetCurSel();
			m_cbxframesIndex.GetLBText(nIndex, strCBText);
			char* s_trackIndex = strCBText.GetBuffer(0);
			m_CAppWorkPlant.m_TrackRepeatModule.m_TrackIndex = s_trackIndex;

			vector<KeyFrame> keyFrameColl = m_CAppWorkPlant.m_TrackRepeatModule.GetKeyFrameCollToIndexTrack(m_CAppWorkPlant.m_TrackRepeatModule.m_TrackIndex);
			int index = 0;
			for (vector<KeyFrame>::iterator it = keyFrameColl.begin(); it != keyFrameColl.end(); it++)
			{
				index++;
			}
			while (index > frameNumEverPage)
			{
				totalPage++;
				index = index - frameNumEverPage;
			}
			frameNumCurrentPage = index;
			this->pageImages();
		}
	}
	else
	{
		char c_logInfo[100] = "失败删除轨迹：";
		strcat_s(c_logInfo, 100, indexNewtrack);
		
	}
}



//选择轨迹索引
void CGPothkaMainWinDlg::OnCbnSelchangecbxFramesIndex()
{
	// TODO: 在此添加控件通知处理程序代码
	mocoLog.StatusOut(Info, "切换轨迹");
	CString strCBText;
	int nIndex = m_cbxframesIndex.GetCurSel();
	m_cbxframesIndex.GetLBText(nIndex,strCBText);
	char* s_trackIndex=strCBText.GetBuffer(0);
	m_CAppWorkPlant.m_TrackRepeatModule.m_TrackIndex=s_trackIndex;  //设置关键帧索引

	vector<KeyFrame> keyFrameColl = m_CAppWorkPlant.m_TrackRepeatModule.GetKeyFrameCollToIndexTrack(m_CAppWorkPlant.m_TrackRepeatModule.m_TrackIndex);
	int index = 0;
	for (vector<KeyFrame>::iterator it = keyFrameColl.begin(); it != keyFrameColl.end(); it++)
	{
		index++;
	}
	totalPage = 0;
	while (index > frameNumEverPage)
	{
		totalPage++;
		index = index - frameNumEverPage;
	}
	frameNumCurrentPage = index;
	
	this->pageImages(strCBText);
}

//选择关键帧
void CGPothkaMainWinDlg::OnLvnItemchangedlistkeyframes(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	if(( pNMLV->uChanged&LVIF_STATE)&&(pNMLV-> uNewState&LVIS_SELECTED)&&!(pNMLV->uOldState & LVIS_FOCUSED))
	{
		int a=pNMLV->iItem;
		m_CAppWorkPlant.m_TrackRepeatModule.m_keyFrameIndex=a;
	}
}

//添加新的关键帧
void CGPothkaMainWinDlg::OnBnClickedbtnaddNewkeyframe()
{
	// TODO: 在此添加控件通知处理程序代码
	if (softLock == 1)
	{
		MessageBox("软件处于锁定状态！");
		return;
	}

	if (camera == "Fujinon")
	{
		m_Lens_Fujinon.GetLensMotorZoom(Zoom);
		m_Lens_Fujinon.GetLensMotorFocus(Focus);
	}
	
	CString str;
	int nIndex = m_cbxframesIndex.GetCurSel();
	m_cbxframesIndex.GetLBText(nIndex, str);
	string indexNewtrack = str.GetBuffer(0);

	float waitTime=0;
	float partSpeed=0;

	waitTime = wTime;
	partSpeed = 1; 

	CString strCBText;
	m_cbxframesIndex.GetWindowTextA(strCBText);
    char* s_trackIndex=strCBText.GetBuffer(0);

	//当前选择的关键帧
    POSITION pos=m_listKeyFrames.GetFirstSelectedItemPosition();
    int m_nIndex = m_listKeyFrames.GetNextSelectedItem(pos); 
	if(s_trackIndex=="")
	{
		char c_logInfo[100]="失败添加新的关键帧：";
		strcat_s(c_logInfo,100,"请先创建轨迹索引……");
		return;
	}

	if(m_nIndex<0)//添加关键帧
	{
		if (frameNum == 0)
		{
			////新建用于存放图片的文件夹
			CString csPath = "D:\\TEST\\";
			csPath += str;
			if (!PathIsDirectory(csPath))
			{
				CreateDirectory(csPath, 0);
			}
			picPath = csPath;
		}
		else
		{
			CString csPath = "D:\\TEST\\";
			picPath = csPath + str;
		}
		if(true==m_CAppWorkPlant.m_TrackRepeatModule.AddKeyFrame(gCurrentRobotLocation,s_trackIndex,waitTime,partSpeed))
		{
			//采集图片
			mocoLog.StatusOut(Info, "添加关键帧");
			int cout = 0;
			m_selectedDevice->m_SaveFrameFlag = true;
			while (m_selectedDevice->m_SaveFrameFlag == true&&(cout<10))
			{
				cout++;
				Sleep(10);
			}
			
			char c_logInfo[100]="成功添加新的关键帧：";
			strcat_s(c_logInfo,100,"……");
			//SendLogToWindows(this->m_hWnd,c_logInfo);

			
			m_CAppWorkPlant.m_TrackRepeatModule.frameMinTime(indexNewtrack);
			
			frameNum++;
			frameNumCurrentPage++;

			if (currentPage != totalPage)
			{
				currentPage = totalPage;
			}

			if (frameNumCurrentPage > frameNumEverPage)
			{
				totalPage++;
				currentPage++;
				m_listKeyFrames.DeleteAllItems();
				
				frameNumCurrentPage = 1;
			}
			
			pageImages();
			
			CString str;
			str.Format(" %d / %d", currentPage + 1, totalPage+1);
			GetDlgItem(IDC_pageIndex)->SetWindowTextA(str);

			if (currentPage != 0)
			{
				GetDlgItem(IDC_btnPrePage)->EnableWindow(TRUE);
			}
		
		}else{
			char c_logInfo[100]="失败添加新的关键帧：";
			strcat_s(c_logInfo,100,"……");
			
		}
	}
	else //插入关键帧
	{
		CString csPath = "D:\\TEST\\";
		csPath += str;
		picPath = csPath;

		if(true==m_CAppWorkPlant.m_TrackRepeatModule.InsertKeyFrame(gCurrentRobotLocation,s_trackIndex,m_nIndex,waitTime,partSpeed))
		{
			mocoLog.StatusOut(Info, "插入关键帧");
			char c_logInfo[100]="成功插入新的关键帧：";
			strcat_s(c_logInfo,100,"……");
			
			m_selectedDevice->m_SaveFrameFlag = true;
			while (m_selectedDevice->m_SaveFrameFlag == true)
			{
			}

			m_CAppWorkPlant.m_TrackRepeatModule.frameMinTime(indexNewtrack);

			frameNum++;

			if (frameNumCurrentPage != frameNumEverPage)
			{
				frameNumCurrentPage++;
			}
			else
			{
				totalPage++;
				frameNumCurrentPage=frameNumCurrentPage - frameNumEverPage + 1;
			}

			if (currentPage != totalPage)
			{
				currentPage = totalPage;
			}
		
			m_listKeyFrames.DeleteAllItems();
	
			pageImages();
			
			CString str;
			str.Format(" %d / %d", currentPage + 1, totalPage + 1);
			GetDlgItem(IDC_pageIndex)->SetWindowTextA(str);

			if (currentPage != 0)
			{
				GetDlgItem(IDC_btnPrePage)->EnableWindow(TRUE);
			}
			GetDlgItem(IDC_btnNextPage)->EnableWindow(FALSE);

		
		}else{
			char c_logInfo[100]="失败插入新的关键帧：";
			strcat_s(c_logInfo,100,"……");
		
		}
	}

	GetDlgItem(IDC_btnUpdateFrameKeyFrame)->EnableWindow(TRUE);
	
	GetMenu()->GetSubMenu(0)->EnableMenuItem(3, MF_BYPOSITION | MF_ENABLED);
	GetMenu()->GetSubMenu(0)->EnableMenuItem(4, MF_BYPOSITION | MF_ENABLED);
	GetMenu()->GetSubMenu(0)->EnableMenuItem(8, MF_BYPOSITION | MF_ENABLED);
	GetMenu()->GetSubMenu(0)->EnableMenuItem(6, MF_BYPOSITION | MF_ENABLED);
	GetDlgItem(IDC_listKeyFrames)->EnableWindow(TRUE);
	GetDlgItem(IDC_btnRestorationMotion)->EnableWindow(TRUE);
	GetDlgItem(IDC_btnNextPage)->EnableWindow(FALSE);
	GetDlgItem(IDC_listKeyFrames)->EnableWindow(TRUE);

	wTime = 0;

}


//删除关键帧
void CGPothkaMainWinDlg::Onbtndeleteframe()
{
	// TODO:  在此添加命令处理程序代码
	if (softLock == 1)
	{
		MessageBox("软件处于锁定状态！");
		return;
	}

	CString strCBText;
	m_cbxframesIndex.GetWindowTextA(strCBText);
	char* s_trackIndex = strCBText.GetBuffer(0);

	//当前选择的关键帧
	POSITION pos = m_listKeyFrames.GetFirstSelectedItemPosition();
	int m_nIndex = m_listKeyFrames.GetNextSelectedItem(pos);
	if (s_trackIndex == "")
	{
		char c_logInfo[100] = "失败的删除关键帧：";
		strcat_s(c_logInfo, 100, "请先创建轨迹索引……");
		
		return;
	}

	if (m_nIndex == -1)
	{
		m_nIndex = 0;
	}

	if (m_CAppWorkPlant.m_TrackRepeatModule.DeleteKeyFrame(m_nIndex + currentPage*frameNumEverPage, s_trackIndex))
	{
		mocoLog.StatusOut(Info, "删除关键帧");
		char c_logInfo[100] = "成功删除关键帧：";
		strcat_s(c_logInfo, 100, "……");
		
	}
	else{
		char c_logInfo[100] = "失败删除关键帧：";
		strcat_s(c_logInfo, 100, "……");
		
	}

	frameNum--;
	frameNumCurrentPage--;
	if (frameNumCurrentPage == 0 && totalPage != 0)
	{
		totalPage--;
		currentPage = totalPage;
		frameNumCurrentPage = frameNumEverPage;
	}

	if (currentPage == 0)
	{
		GetDlgItem(IDC_btnPrePage)->EnableWindow(FALSE);
	}
	if (currentPage == totalPage)
	{
		GetDlgItem(IDC_btnNextPage)->EnableWindow(FALSE);
	}

	//更新最小运行时间
	CString str;
	int nIndex = m_cbxframesIndex.GetCurSel();
	m_cbxframesIndex.GetLBText(nIndex, str);
	string indexNewtrack = str.GetBuffer(0);

	m_CAppWorkPlant.m_TrackRepeatModule.frameMinTime(indexNewtrack);
	m_listKeyFrames.DeleteAllItems();
	

	pageImages();
	
	CString strPage;
	strPage.Format(" %d / %d", currentPage + 1, totalPage + 1);
	GetDlgItem(IDC_pageIndex)->SetWindowTextA(strPage);

	if (frameNum == 0)
	{
		fstream file(m_CAppWorkPlant.m_TrackRepeatModule.m_TrackDataPath[strCBText.GetBuffer(0)], ios::out);

		Onbtndeleteframeindex();

	}
}

//更新关键帧
void CGPothkaMainWinDlg::OnBnClickedbtnUpdateframekeyframe()
{
	// TODO: 在此添加控件通知处理程序代码
	if (softLock == 1)
	{
		MessageBox("软件处于锁定状态！");
		return;
	}

	//当前选择的轨迹索引
	CString strCBText;
	m_cbxframesIndex.GetWindowTextA(strCBText);
	char* s_trackIndex=strCBText.GetBuffer(0);

	CString str;
	int nIndex = m_cbxframesIndex.GetCurSel();
	m_cbxframesIndex.GetLBText(nIndex, str);
	string indexNewtrack = str.GetBuffer(0);

	//当前选择的关键帧
	POSITION pos=m_listKeyFrames.GetFirstSelectedItemPosition();
	int m_nIndex = m_listKeyFrames.GetNextSelectedItem(pos); 
	if(s_trackIndex=="")
	{
		char c_logInfo[100]="失败的更新关键帧：";
		strcat_s(c_logInfo,100,"请先创建轨迹索引……");
		//SendLogToWindows(this->m_hWnd,c_logInfo);
		return;
	}

	float waitTime=0;
	double partSpeed=0;

	waitTime = wTime;
	partSpeed = 1;

	if (true == m_CAppWorkPlant.m_TrackRepeatModule.UpdateKeyFrame(gCurrentRobotLocation, m_nIndex + currentPage*frameNumEverPage, s_trackIndex, waitTime, partSpeed))
	{
		mocoLog.StatusOut(Info, "更新关键帧");
		char c_logInfo[100]="成功更新关键帧：";
		strcat_s(c_logInfo,100,"……");
	}else
	{

	}

	//更新图片
	m_selectedDevice->m_SaveFrameFlag = true;
	while (m_selectedDevice->m_SaveFrameFlag == true)
	{
	}

	m_CAppWorkPlant.m_TrackRepeatModule.frameMinTime(indexNewtrack);

	pageImages();
	
	CString pageStr;
	pageStr.Format(" %d / %d", currentPage + 1, totalPage + 1);
	GetDlgItem(IDC_pageIndex)->SetWindowTextA(pageStr);
	wTime = 0;
}

//导出轨迹
void CGPothkaMainWinDlg::Onbtnoutputtrack()
{
	// TODO:  在此添加命令处理程序代码
	if (softLock == 1)
	{
		MessageBox("软件处于锁定状态！");
		return;
	}

	CString saveFileName, saveFullPath;
	CString strCBText;
	m_cbxframesIndex.GetWindowText(strCBText);

	if (m_cbxframesIndex.GetCount() == 0)
	{
		GetMenu()->GetSubMenu(0)->EnableMenuItem(1, MF_BYPOSITION | MF_DISABLED);
		GetDlgItem(IDC_btnAddNewKeyFrame)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnDeletekeyFrame)->EnableWindow(FALSE);
		GetMenu()->GetSubMenu(0)->EnableMenuItem(3, MF_BYPOSITION | MF_DISABLED);
		GetMenu()->GetSubMenu(0)->EnableMenuItem(4, MF_BYPOSITION | MF_ENABLED);
		GetDlgItem(IDC_listKeyFrames)->EnableWindow(FALSE);
	}

	CFileDialog FileDlg(FALSE, "txt", strCBText, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT);
	FileDlg.m_ofn.lpstrInitialDir = "桌面:\\";
	if (FileDlg.DoModal() == IDOK)
	{
		mocoLog.StatusOut(Info, "导出轨迹 %s",strCBText);
		saveFullPath = FileDlg.GetPathName();
		m_CAppWorkPlant.m_TrackRepeatModule.SaveKeyFramesToLocal(saveFullPath.GetBuffer(0), strCBText.GetBuffer(0));
		return;
	}
}

//导入轨迹
void CGPothkaMainWinDlg::Onbtnimporttrack()
{
	// TODO:  在此添加命令处理程序代码
	if (softLock == 1)
	{
		MessageBox("软件处于锁定状态！");
		return;
	}

	CString filefullPath, fileName;
	CFileDialog fileDlg(TRUE, _T("txt"));
	if (fileDlg.DoModal() == IDOK){
		filefullPath = fileDlg.GetPathName().GetBuffer(0);
		fileName = fileDlg.GetFileTitle();

		loadIndex = 1;
		fileDirt = fileName;

		Onbtnnewframeindex(fileDirt);



		CString strCBText;

		m_cbxframesIndex.GetWindowTextA(strCBText);


		char* s_trackIndex = strCBText.GetBuffer(0);
		
		mocoLog.StatusOut(Info, "导入轨迹 %s", strCBText);
		m_CAppWorkPlant.m_TrackRepeatModule.LoadLocalKeyFrames(filefullPath.GetBuffer(0), s_trackIndex);

		while (frameNum >frameNumEverPage)
		{
			GetDlgItem(IDC_btnNextPage)->EnableWindow(TRUE);
			frameNum = frameNum - frameNumEverPage;
			totalPage++;
		}
		frameNumCurrentPage = frameNum;
		currentPage = 0;

		//pageImages(fileName);

		CString str;
		str.Format("%d", currentPage + 1);
		GetDlgItem(IDC_pageIndex)->SetWindowTextA(str);

		/*if (m_cbxframesIndex.GetCount()>0)
		{
			GetMenu()->GetSubMenu(0)->EnableMenuItem(1, MF_BYPOSITION | MF_ENABLED);
			GetDlgItem(IDC_btnAddNewKeyFrame)->EnableWindow(TRUE);
			GetMenu()->GetSubMenu(0)->EnableMenuItem(4, MF_BYPOSITION | MF_ENABLED);
			GetDlgItem(IDC_listKeyFrames)->EnableWindow(TRUE);			
			GetDlgItem(IDC_btnRestorationMotion)->EnableWindow(TRUE);
			GetDlgItem(IDC_btnUpdateFrameKeyFrame)->EnableWindow(TRUE);
		}
		return;*/
		GetDlgItem(IDC_btnRestorationMotion)->EnableWindow(TRUE);
	}
	else{

		return;
	}

}

//程序开启时自动加载轨迹
void CGPothkaMainWinDlg::ImportTrack()
{
	// TODO:  在此添加命令处理程序代码
	CString filefullPath, fileName;

	FILE *pfile;
	errno_t err = 1;
	int cout = 0;
	while (err != 0)
	{
		err = fopen_s(&pfile, "D:\\LastTrack\\Dir.txt","r");
		cout++;
		Sleep(10);
	}

	if (cout == 10)
	{
		MessageBox("文轨迹自动载入失败！");
		return;
	}

	if (err == 0)
	{
		char FileContent[100];
		memset(FileContent, 0, 100);
		fread(FileContent, 1, 100, pfile);
		
		fileName.Format("%s", FileContent);

		if (pfile != NULL)
			fclose(pfile);

		CString temp;
		temp = "D:\\LastTrack\\";
		filefullPath = temp + fileName;
		temp = ".txt";
		filefullPath += temp;


		loadIndex = 1;
		fileDirt = fileName;

		Onbtnnewframeindex(fileDirt);

		CString strCBText;

		m_cbxframesIndex.GetWindowTextA(strCBText);


		char* s_trackIndex = strCBText.GetBuffer(0);
		mocoLog.StatusOut(Info, "开机自动加载轨迹 %s", strCBText);
		m_CAppWorkPlant.m_TrackRepeatModule.LoadLocalKeyFrames(filefullPath.GetBuffer(0), s_trackIndex);	

		while (frameNum >frameNumEverPage)
		{
			GetDlgItem(IDC_btnNextPage)->EnableWindow(TRUE);
			frameNum = frameNum - frameNumEverPage;
			totalPage++;
		}
		frameNumCurrentPage = frameNum;
		currentPage = 0;

		pageImages(fileName);

		CString str;
		str.Format("%d", currentPage + 1);
		GetDlgItem(IDC_pageIndex)->SetWindowTextA(str);

		if (m_cbxframesIndex.GetCount()>0)
		{
			GetMenu()->GetSubMenu(0)->EnableMenuItem(1, MF_BYPOSITION | MF_ENABLED);
			GetDlgItem(IDC_btnAddNewKeyFrame)->EnableWindow(TRUE);
			GetMenu()->GetSubMenu(0)->EnableMenuItem(3, MF_BYPOSITION | MF_ENABLED);
			GetDlgItem(IDC_listKeyFrames)->EnableWindow(TRUE);
			GetDlgItem(IDC_btnRestorationMotion)->EnableWindow(TRUE);
			GetDlgItem(IDC_btnUpdateFrameKeyFrame)->EnableWindow(TRUE);
		}

	}
	
	return;
}


//运动控制
UINT __cdecl camSingle(LPVOID lpParamter)
{
	softControl = 1;
	
	CGPothkaMainWinDlg* m_dlg = (CGPothkaMainWinDlg*)lpParamter;
	if (camera == "Fujinon")
	{
		m_dlg->m_Lens_Fujinon.SetSwitch4Host(0xFC);

		int frameIndex = singleIndex;

		map<string, vector<KeyFrame>>::iterator itor;

		itor = CTrackRepeatModule::m_TrackKeyFrameDepository.find(CTrackRepeatModule::m_TrackIndex);

		if (itor == CTrackRepeatModule::m_TrackKeyFrameDepository.end())
		{
			return false;
		}
		//设置定时器变量
		LARGE_INTEGER lageinter;
		LONGLONG CountBegin, CountEnd;
		double diFreq, diTime;
		//读取位置信息
		float Time, TimeFrequency;
		WORD FocusValue, ZoomValue;

		WORD CurrentFocusPos;
		m_dlg->m_Lens_Fujinon.GetLensMotorFocus(CurrentFocusPos);
		WORD CurrentZoomPos;
		m_dlg->m_Lens_Fujinon.GetLensMotorZoom(CurrentZoomPos);

		if (itor != CTrackRepeatModule::m_TrackKeyFrameDepository.end())
		{
			FocusValue = itor->second[frameIndex].Focus;
			ZoomValue = itor->second[frameIndex].Zoom;

			Time = itor->second[frameIndex].FrameRunTime;

			//如果用户没有设置时间，镜头运动时间根据机器人运动进行计算
			if (Time == 0)
			{
				KeyFrame startFrame;
				startFrame.A1 = gCurrentRobotLocation.A1;
				startFrame.A2 = gCurrentRobotLocation.A2;
				startFrame.A3 = gCurrentRobotLocation.A3;
				startFrame.A4 = gCurrentRobotLocation.A4;
				startFrame.A5 = gCurrentRobotLocation.A5;
				startFrame.A6 = gCurrentRobotLocation.A6;
				startFrame.E1 = gCurrentRobotLocation.E1;

				Time=m_dlg->m_CAppWorkPlant.m_TrackRepeatModule.frameRunTime(startFrame, itor->second[frameIndex]);
				Time = Time*0.012;
			}

			TimeFrequency = Time / 0.025;
			float FocusStepValue = (FocusValue - CurrentFocusPos) / TimeFrequency;
			float ZoomStepValue = (ZoomValue - CurrentZoomPos) / TimeFrequency;
			float FocusSetValue = CurrentFocusPos;
			float ZoomSetValue = CurrentZoomPos;
			int i = 0;
			while (i<TimeFrequency)
			{
				QueryPerformanceFrequency(&lageinter);
				diFreq = lageinter.QuadPart;
				QueryPerformanceCounter(&lageinter);
				CountBegin = lageinter.QuadPart;
				FocusSetValue = FocusSetValue + FocusStepValue;
				ZoomSetValue = ZoomSetValue + ZoomStepValue;
				m_dlg->m_Lens_Fujinon.SetLensMotorFocus(FocusSetValue);
				m_dlg->m_Lens_Fujinon.SetLensMotorZoom(ZoomSetValue);

				Focus = FocusSetValue;
				Zoom = ZoomSetValue;

				i++;
				do
				{
					QueryPerformanceCounter(&lageinter);
					CountEnd = lageinter.QuadPart;
					diTime = (CountEnd - CountBegin) / diFreq;
				} while (diTime<0.025);
			}
			CurrentFocusPos = FocusSetValue;
			CurrentZoomPos = ZoomSetValue;

		}
		softControl = 0;
	}
	else if (camera == "Canon")
	{
		int frameIndex = singleIndex;

		map<string, vector<KeyFrame>>::iterator itor;

		itor = CTrackRepeatModule::m_TrackKeyFrameDepository.find(CTrackRepeatModule::m_TrackIndex);

		if (itor == CTrackRepeatModule::m_TrackKeyFrameDepository.end())
		{
			return false;
		}
		//设置定时器变量
		LARGE_INTEGER lageinter;
		LONGLONG CountBegin, CountEnd;
		double diFreq, diTime;
		//读取位置信息
		float Time, TimeFrequency;
		WORD FocusValue, ZoomValue;

		WORD CurrentFocusPos;
		CurrentFocusPos = Focus;
		WORD CurrentZoomPos;
		CurrentZoomPos = Zoom;

		if (itor != CTrackRepeatModule::m_TrackKeyFrameDepository.end())
		{
			FocusValue = itor->second[frameIndex].Focus;
			ZoomValue = itor->second[frameIndex].Zoom;

			Time = itor->second[frameIndex].FrameRunTime;
			if (Time == 0)
			{
				KeyFrame startFrame;
				startFrame.A1 = gCurrentRobotLocation.A1;
				startFrame.A2 = gCurrentRobotLocation.A2;
				startFrame.A3 = gCurrentRobotLocation.A3;
				startFrame.A4 = gCurrentRobotLocation.A4;
				startFrame.A5 = gCurrentRobotLocation.A5;
				startFrame.A6 = gCurrentRobotLocation.A6;
				startFrame.E1 = gCurrentRobotLocation.E1;

				Time = m_dlg->m_CAppWorkPlant.m_TrackRepeatModule.frameRunTime(startFrame, itor->second[frameIndex]);
				Time = Time*0.012;
			}

			TimeFrequency = Time / 0.025;
			float FocusStepValue = (FocusValue - CurrentFocusPos) / TimeFrequency;
			float ZoomStepValue = (ZoomValue - CurrentZoomPos) / TimeFrequency;
			float FocusSetValue = CurrentFocusPos;
			float ZoomSetValue = CurrentZoomPos;
			int i = 0;
			while (i<TimeFrequency)
			{
				QueryPerformanceFrequency(&lageinter);
				diFreq = lageinter.QuadPart;
				QueryPerformanceCounter(&lageinter);
				CountBegin = lageinter.QuadPart;
				FocusSetValue = FocusSetValue + FocusStepValue;
				ZoomSetValue = ZoomSetValue + ZoomStepValue;
				

				Focus = FocusSetValue;
				Zoom = ZoomSetValue;

				i++;
				do
				{
					QueryPerformanceCounter(&lageinter);
					CountEnd = lageinter.QuadPart;
					diTime = (CountEnd - CountBegin) / diFreq;
				} while (diTime<0.025);
			}
			CurrentFocusPos = FocusSetValue;
			CurrentZoomPos = ZoomSetValue;
		}
		softControl = 0;
	}

	return 0;
}


//单步运动
void CGPothkaMainWinDlg::OnBnClickedbtnSinglemotion()
{
	if (softLock == 1)
	{
		MessageBox("软件处于锁定状态！");
		return;
	}

	GetDlgItem(IDC_btnStartRepeatMotion)->EnableWindow(FALSE);
	
	m_CAppWorkPlant.m_LocusRunCommand.clear();
	
	//当前选择的关键帧
	POSITION pos=m_listKeyFrames.GetFirstSelectedItemPosition();
	int m_nIndex = m_listKeyFrames.GetNextSelectedItem(pos); 

	if(m_nIndex<0) 
	{				
		MessageBox("执行单步运动前请先选择关键帧！");
		return;
	}
	else if (frameIndex == m_nIndex)
	{
		MessageBox("请选择其他关键帧！");
		return;
	}

	frameIndex = m_nIndex;

	singleIndex = m_nIndex;
	mocoLog.StatusOut(Info, "单步运动");
	// TODO: 在此添加控件通知处理程序代码
	m_CAppWorkPlant.m_TrackRepeatModule.ObtainSingleMotionCommandData(
		gCurrentRobotLocation,
		m_CAppWorkPlant.m_TrackRepeatModule.m_keyFrameIndex + currentPage*frameNumEverPage,
		m_CAppWorkPlant.m_TrackRepeatModule.m_TrackIndex,&m_CAppWorkPlant.m_PresetSendDataColl
		);
	m_CAppWorkPlant.isPresetCommand=true;

	CGPothkaMainWinDlg *pDlg = this;
	camSing = AfxBeginThread(camSingle, pDlg, 0, 0, 0, NULL);

}

//复位运动
void CGPothkaMainWinDlg::OnBnClickedbtnRestorationmotion()
{
	if (softLock == 1)
	{
		MessageBox("软件处于锁定状态！");
		return;
	}

	if (frameNum == 0)
	{
		return;
	}
	
	softControl = 1;
	
	WORD zom;
	frameIndex = 0;
	// TODO: 在此添加控件通知处理程序代码
	m_CAppWorkPlant.m_LocusRunCommand.clear();

	CString strCBText;
	int nIndex = m_cbxframesIndex.GetCurSel();
	m_cbxframesIndex.GetLBText(nIndex, strCBText);
	char* s_trackIndex = strCBText.GetBuffer(0);
	m_CAppWorkPlant.m_TrackRepeatModule.m_TrackIndex = s_trackIndex;  //设置关键帧索引

	map<string, vector<KeyFrame>>::iterator itor;
	itor = m_CAppWorkPlant.m_TrackRepeatModule.m_TrackKeyFrameDepository.find(m_CAppWorkPlant.m_TrackRepeatModule.m_TrackIndex);
	
	if (camera == "Fujinon")
	{
		m_Lens_Fujinon.SetSwitch4Host(0xFC);

		m_Lens_Fujinon.SetLensMotorZoom(itor->second[0].Zoom);
		m_Lens_Fujinon.SetLensMotorFocus(itor->second[0].Focus);

		Zoom = itor->second[0].Zoom;
		Focus = itor->second[0].Focus;
	}
	else if(camera == "Canon")
	{
		Zoom = itor->second[0].Zoom;
		Focus = itor->second[0].Focus;
	}

	mocoLog.StatusOut(Info, "复位运动");
	//机器人复位
	m_CAppWorkPlant.m_TrackRepeatModule.ObtainRestorationMotionCommandData(
		gCurrentRobotLocation,
		m_CAppWorkPlant.m_TrackRepeatModule.m_TrackIndex,
		&m_CAppWorkPlant.m_PresetSendDataColl
		);


	m_CAppWorkPlant.isPresetCommand=true;

	softControl = 0;
	GetDlgItem(IDC_btnUpdateFrameKeyFrame)->EnableWindow(TRUE);
    GetDlgItem(IDC_btnRestorationMotion)->EnableWindow(TRUE);
	GetDlgItem(IDC_btnStartRepeatMotion)->EnableWindow(TRUE);
    GetDlgItem(IDC_btnSingleMotion)->EnableWindow(TRUE);

}


UINT __cdecl camRepeat(LPVOID lpParamter)
{
	CGPothkaMainWinDlg* m_dlg = (CGPothkaMainWinDlg*)lpParamter;
	softControl = 1;
	if (camera == "Fujinon")
	{
		m_dlg->m_Lens_Fujinon.SetSwitch4Host(0xFC);

		CWnd* h_d = AfxGetApp()->GetMainWnd();


		map<string, vector<KeyFrame>>::iterator itor;

		itor = CTrackRepeatModule::m_TrackKeyFrameDepository.find(CTrackRepeatModule::m_TrackIndex);
		if (itor == CTrackRepeatModule::m_TrackKeyFrameDepository.end())
		{
			return false;
		}

		//设置定时器变量
		LARGE_INTEGER lageinter;
		LONGLONG CountBegin, CountEnd;
		double diFreq, diTime;
		//读取位置信息
		vector<KeyFrame>::iterator KeyFraVecItor = itor->second.begin();
		float FocusValue, ZoomValue, Time, TimeFrequency;
		float CurrentFocusPos = KeyFraVecItor->Focus;//Focus当前位置
		float CurrentZoomPos = KeyFraVecItor->Zoom;//Zoom当前位置

		KeyFrame startFrame;
		startFrame.A1 = KeyFraVecItor->A1;
		startFrame.A2 = KeyFraVecItor->A2;
		startFrame.A3 = KeyFraVecItor->A3;
		startFrame.A4 = KeyFraVecItor->A4;
		startFrame.A5 = KeyFraVecItor->A5;
		startFrame.A6 = KeyFraVecItor->A6;
		startFrame.E1 = KeyFraVecItor->E1;

		KeyFraVecItor++;

		KeyFrame endFrame;
		endFrame.A1 = KeyFraVecItor->A1;
		endFrame.A2 = KeyFraVecItor->A2;
		endFrame.A3 = KeyFraVecItor->A3;
		endFrame.A4 = KeyFraVecItor->A4;
		endFrame.A5 = KeyFraVecItor->A5;
		endFrame.A6 = KeyFraVecItor->A6;
		endFrame.E1 = KeyFraVecItor->E1;

		//急停后镜头的重复运动也停止
		while (KeyFraVecItor != itor->second.end() && gRunState.Suspend == false)
		{
			FocusValue = KeyFraVecItor->Focus;
			ZoomValue = KeyFraVecItor->Zoom;
			Time = KeyFraVecItor->FrameRunTime;
			if (Time == 0)
			{
				Time = m_dlg->m_CAppWorkPlant.m_TrackRepeatModule.frameRunTime(startFrame, endFrame);
				Time = Time*0.012;
			}


			TimeFrequency = Time / 0.025;
			float FocusStepValue = (FocusValue - CurrentFocusPos) / TimeFrequency;
			float ZoomStepValue = (ZoomValue - CurrentZoomPos) / TimeFrequency;
			float FocusSetValue = CurrentFocusPos;
			float ZoomSetValue = CurrentZoomPos;
			int i = 0;
			while (i<TimeFrequency)
			{
				QueryPerformanceFrequency(&lageinter);
				diFreq = lageinter.QuadPart;
				QueryPerformanceCounter(&lageinter);
				CountBegin = lageinter.QuadPart;
				FocusSetValue = FocusSetValue + FocusStepValue;
				ZoomSetValue = ZoomSetValue + ZoomStepValue;

				Focus = FocusSetValue;
				Zoom = ZoomSetValue;

				m_dlg->m_Lens_Fujinon.SetLensMotorFocus(FocusSetValue);
				m_dlg->m_Lens_Fujinon.SetLensMotorZoom(ZoomSetValue);
				i++;
				do
				{
					QueryPerformanceCounter(&lageinter);
					CountEnd = lageinter.QuadPart;
					diTime = (CountEnd - CountBegin) / diFreq;
				} while (diTime<0.025);
			}
			CurrentFocusPos = FocusSetValue;
			CurrentZoomPos = ZoomSetValue;
			KeyFraVecItor++;
		}
		softControl = 0;
	}
	else if (camera == "Canon")
	{
		map<string, vector<KeyFrame>>::iterator itor;

		itor = CTrackRepeatModule::m_TrackKeyFrameDepository.find(CTrackRepeatModule::m_TrackIndex);
		if (itor == CTrackRepeatModule::m_TrackKeyFrameDepository.end())
		{
			return false;
		}

		//设置定时器变量
		LARGE_INTEGER lageinter;
		LONGLONG CountBegin, CountEnd;
		double diFreq, diTime;
		//读取位置信息
		vector<KeyFrame>::iterator KeyFraVecItor = itor->second.begin();
		float FocusValue, ZoomValue, Time, TimeFrequency;
		float CurrentFocusPos = KeyFraVecItor->Focus;//Focus当前位置
		float CurrentZoomPos = KeyFraVecItor->Zoom;//Zoom当前位置

		KeyFrame startFrame;
		startFrame.A1 = KeyFraVecItor->A1;
		startFrame.A2 = KeyFraVecItor->A2;
		startFrame.A3 = KeyFraVecItor->A3;
		startFrame.A4 = KeyFraVecItor->A4;
		startFrame.A5 = KeyFraVecItor->A5;
		startFrame.A6 = KeyFraVecItor->A6;
		startFrame.E1 = KeyFraVecItor->E1;

		KeyFraVecItor++;

		KeyFrame endFrame;
		endFrame.A1 = KeyFraVecItor->A1;
		endFrame.A2 = KeyFraVecItor->A2;
		endFrame.A3 = KeyFraVecItor->A3;
		endFrame.A4 = KeyFraVecItor->A4;
		endFrame.A5 = KeyFraVecItor->A5;
		endFrame.A6 = KeyFraVecItor->A6;
		endFrame.E1 = KeyFraVecItor->E1;

		//急停后镜头的重复运动也停止
		while (KeyFraVecItor != itor->second.end() && gRunState.Suspend == false)
		{
			FocusValue = KeyFraVecItor->Focus;
			ZoomValue = KeyFraVecItor->Zoom;
			Time = KeyFraVecItor->FrameRunTime;

			if (Time == 0)
			{
				Time = m_dlg->m_CAppWorkPlant.m_TrackRepeatModule.frameRunTime(startFrame, endFrame);
				Time = Time*0.012;
			}

			TimeFrequency = Time / 0.025;
			float FocusStepValue = (FocusValue - CurrentFocusPos) / TimeFrequency;
			float ZoomStepValue = (ZoomValue - CurrentZoomPos) / TimeFrequency;
			float FocusSetValue = CurrentFocusPos;
			float ZoomSetValue = CurrentZoomPos;
			int i = 0;
			while (i<TimeFrequency)
			{
				QueryPerformanceFrequency(&lageinter);
				diFreq = lageinter.QuadPart;
				QueryPerformanceCounter(&lageinter);
				CountBegin = lageinter.QuadPart;
				FocusSetValue = FocusSetValue + FocusStepValue;
				ZoomSetValue = ZoomSetValue + ZoomStepValue;

				Focus = FocusSetValue;
				Zoom = ZoomSetValue;

				i++;
				do
				{
					QueryPerformanceCounter(&lageinter);
					CountEnd = lageinter.QuadPart;
					diTime = (CountEnd - CountBegin) / diFreq;
				} while (diTime<0.025);
			}
			CurrentFocusPos = FocusSetValue;
			CurrentZoomPos = ZoomSetValue;
			KeyFraVecItor++;
		}
		softControl = 0;
	}
	
	return 0;
}

//轨迹重复
void CGPothkaMainWinDlg::OnBnClickedbtnStartrepeatmotion()
{
	//TODO: 在此添加控件通知处理程序代码
	if (softLock == 1)
	{
		MessageBox("软件处于锁定状态！");
		return;
	}

	m_CAppWorkPlant.m_LocusRunCommand.clear();
	GetDlgItem(IDC_btnStartRepeatMotion)->EnableWindow(FALSE);
    GetDlgItem(IDC_btnSingleMotion)->EnableWindow(FALSE);


	CString strCBText;
	int nIndex = m_cbxframesIndex.GetCurSel();
	m_cbxframesIndex.GetLBText(nIndex,strCBText);
	string indexNewtrack=strCBText.GetBuffer(0);
	
	m_CAppWorkPlant.m_TrackRepeatModule.ObtainRepeatMotionCommandData(
		indexNewtrack,
		&m_CAppWorkPlant.m_PresetSendDataColl
		);

	m_CAppWorkPlant.isPresetCommand=true;

	CGPothkaMainWinDlg *pDlg = this;
	camRert = AfxBeginThread(camRepeat, pDlg, 0, 0, 0, NULL);
}


void CGPothkaMainWinDlg::SyncKeyFrames(){

	CString strCBText;
	char* s_trackIndex;
	int nIndex = m_cbxframesIndex.GetCurSel();

	m_cbxframesIndex.GetLBText(nIndex,strCBText);
	s_trackIndex=strCBText.GetBuffer(0);
	m_CAppWorkPlant.m_TrackRepeatModule.m_TrackIndex=s_trackIndex;  //设置关键帧索引
	m_listKeyFrames.DeleteAllItems();

	mocoLog.StatusOut(Info, "连续运动");
	vector<KeyFrame> keyFrameColl=m_CAppWorkPlant.m_TrackRepeatModule.GetKeyFrameCollToIndexTrack(m_CAppWorkPlant.m_TrackRepeatModule.m_TrackIndex);
	int index=0;
	for(vector<KeyFrame>::iterator it=keyFrameColl.begin();it!=keyFrameColl.end();it++)
	{
		CString s_Value;
		s_Value.Format("%u",it->IPOC);
		int row = m_listKeyFrames.InsertItem(index,s_Value);
		m_listKeyFrames.SetItemText(row,0,s_Value);
		s_Value.Format("%f",it->PartSpeed);
		m_listKeyFrames.SetItemText(row,1,s_Value);
		s_Value.Format("%d",it->WaitTime);
		m_listKeyFrames.SetItemText(row,2,s_Value);
		s_Value.Format("%f",it->X);
		m_listKeyFrames.SetItemText(row,3,s_Value);
		s_Value.Format("%f",it->Y);
		m_listKeyFrames.SetItemText(row,4,s_Value);
		s_Value.Format("%f",it->Z);
		m_listKeyFrames.SetItemText(row,5,s_Value);
		s_Value.Format("%f",it->A);
		m_listKeyFrames.SetItemText(row,6,s_Value);
		s_Value.Format("%f",it->B);
		m_listKeyFrames.SetItemText(row,7,s_Value);
		s_Value.Format("%f",it->C);
		m_listKeyFrames.SetItemText(row, 8, s_Value);
		s_Value.Format("%u", it->Zoom);
		m_listKeyFrames.SetItemText(row, 9, s_Value);
		s_Value.Format("%u", it->Focus);
		m_listKeyFrames.SetItemText(row, 10, s_Value);
		s_Value.Format("%f", it->FrameRunTime);
		m_listKeyFrames.SetItemText(row, 11, s_Value);
		s_Value.Format("%f", it->FrameMinTime);
		m_listKeyFrames.SetItemText(row, 12, s_Value);
		index++;
	}
}

//翻页更新关键帧
void CGPothkaMainWinDlg::pageKeyFrames(){

	CString strCBText;
	char* s_trackIndex;
	int nIndex = m_cbxframesIndex.GetCurSel();

	m_cbxframesIndex.GetLBText(nIndex, strCBText);
	s_trackIndex = strCBText.GetBuffer(0);
	m_CAppWorkPlant.m_TrackRepeatModule.m_TrackIndex = s_trackIndex;  //设置关键帧索引
	m_listKeyFrames.DeleteAllItems();

	vector<KeyFrame> keyFrameColl = m_CAppWorkPlant.m_TrackRepeatModule.GetKeyFrameCollToIndexTrack(m_CAppWorkPlant.m_TrackRepeatModule.m_TrackIndex);
	int index = 0;
	int kk = 0;
	for (vector<KeyFrame>::iterator it = keyFrameColl.begin() + (frameNumEverPage * currentPage); it != keyFrameColl.end() && kk!=frameNumEverPage; it++)
	{
		kk++;
		CString s_Value;
		s_Value.Format("%u", it->IPOC);
		int row = m_listKeyFrames.InsertItem(index, s_Value);

		m_listKeyFrames.SetItemText(row, 0, s_Value);
		s_Value.Format("%f", it->PartSpeed);
		m_listKeyFrames.SetItemText(row, 1, s_Value);
		s_Value.Format("%d", it->WaitTime);
		m_listKeyFrames.SetItemText(row, 2, s_Value);
		s_Value.Format("%f", it->X);
		m_listKeyFrames.SetItemText(row, 3, s_Value);
		s_Value.Format("%f", it->Y);
		m_listKeyFrames.SetItemText(row, 4, s_Value);
		s_Value.Format("%f", it->Z);
		m_listKeyFrames.SetItemText(row, 5, s_Value);
		s_Value.Format("%f", it->A);
		m_listKeyFrames.SetItemText(row, 6, s_Value);
		s_Value.Format("%f", it->B);
		m_listKeyFrames.SetItemText(row, 7, s_Value);
		s_Value.Format("%f", it->C);
		m_listKeyFrames.SetItemText(row, 8, s_Value);
		s_Value.Format("%d", it->Zoom);
		m_listKeyFrames.SetItemText(row, 9, s_Value);
		s_Value.Format("%d", it->Focus);
		m_listKeyFrames.SetItemText(row, 10, s_Value);
		s_Value.Format("%f", it->FrameRunTime);
		m_listKeyFrames.SetItemText(row, 11, s_Value);
		s_Value.Format("%f", it->FrameMinTime);
		m_listKeyFrames.SetItemText(row, 12, s_Value);
		index++;
	}
}

//刷新图片
void CGPothkaMainWinDlg::pageImages()
{
	
	m_listKeyFrames.DeleteAllItems();
	int ImgNum = frameNumCurrentPage + totalPage*frameNumEverPage;
	int RowNum = 5;
	int ColNum = 4;


	CRect ControlRect;
	m_listKeyFrames.GetClientRect(&ControlRect);
	int CtrlWidth = ControlRect.Width();
	int CtrlHeight = ControlRect.Height();
	int ImgWidth = (CtrlWidth - RowNum * 42) / RowNum - 1;
	int ImgHeight = (CtrlHeight - ColNum * 42) / ColNum - 1;

	CImageList ImgLit;
	ImgLit.Create(ImgWidth, ImgHeight, ILC_COLOR32 | ILC_MASK, RowNum, ColNum);

	unsigned char* pBitImgData = (unsigned char *)malloc(ImgWidth*ImgHeight * 4);


	CString strCBText;
	int nIndex = m_cbxframesIndex.GetCurSel();
	m_cbxframesIndex.GetLBText(nIndex, strCBText);
	char* s_trackIndex = strCBText.GetBuffer(0);
	m_CAppWorkPlant.m_TrackRepeatModule.m_TrackIndex = s_trackIndex;

	vector<KeyFrame> keyFrameColl = m_CAppWorkPlant.m_TrackRepeatModule.GetKeyFrameCollToIndexTrack(m_CAppWorkPlant.m_TrackRepeatModule.m_TrackIndex);
	vector<KeyFrame>::iterator it = keyFrameColl.begin();

	vector<KeyFrame>::iterator itimg = keyFrameColl.begin();

	CBitmap cBitImg;

	HBITMAP hBitmap;

	int j = 0;//关键帧列表计数
	while (j < frameNumEverPage * currentPage)
	{
		j++;
		itimg++;
	}

	int temp=0;
	if (currentPage != totalPage)
	{
		temp = 20 ;
	}
	else
	{
		temp = frameNumCurrentPage;
	}

	//加载图片
	for (int n = 0; n < temp; n++)
	{
		
		CString path;
		CString temp;
		path = "D:\\Test\\";
		path += strCBText;
		path += "\\";

		temp.Format("%d", itimg->IPOC);
		path += temp;
		path += ".bmp";


		hBitmap = (HBITMAP)::LoadImage(::AfxGetInstanceHandle(),path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

		cBitImg.Attach(hBitmap);
		ImgLit.Add(&cBitImg, RGB(255, 255, 255));
		cBitImg.Detach();

		itimg++;
	}

	m_listKeyFrames.SetImageList(&ImgLit, LVSIL_NORMAL);
	CString strTemp;
	int k = 0;
	
    j = 0;//关键帧列表计数
	while (j < frameNumEverPage * currentPage)
	{
		j++;
		it++;
	}

	for (int n = 0; n < temp; n++) {
		CString temp = it->name.c_str();

		strTemp.Format(_T("名称:%s  运行时间:%.1fs  最小时间:%.2fs 等待时间:%ds"), temp, it->FrameRunTime, it->FrameMinTime,it->WaitTime);

		m_listKeyFrames.InsertItem(k, strTemp, n);
		k++;
		it++;
	}
	ImgLit.Detach();
	mocoLog.StatusOut(Info, "刷新图片 %s", strCBText);
	return ;

}

//导入轨迹时刷新图片
void CGPothkaMainWinDlg::pageImages(CString filename)
{
	m_listKeyFrames.DeleteAllItems();
	int ImgNum = frameNumCurrentPage + totalPage*frameNumEverPage;
	int RowNum = 5;
	int ColNum = 4;

	CRect ControlRect;
	m_listKeyFrames.GetClientRect(&ControlRect);
	int CtrlWidth = ControlRect.Width();
	int CtrlHeight = ControlRect.Height();
	int ImgWidth = (CtrlWidth - RowNum * 42) / RowNum - 1;
	int ImgHeight = (CtrlHeight - ColNum * 42) / ColNum - 1;

	CImageList ImgLit;
	ImgLit.Create(ImgWidth, ImgHeight, ILC_COLOR32 | ILC_MASK, RowNum, ColNum);

	unsigned char* pBitImgData = (unsigned char *)malloc(ImgWidth*ImgHeight * 4);

	CString strCBText;
	int nIndex = m_cbxframesIndex.GetCurSel();
	m_cbxframesIndex.GetLBText(nIndex, strCBText);
	char* s_trackIndex = strCBText.GetBuffer(0);
	m_CAppWorkPlant.m_TrackRepeatModule.m_TrackIndex = s_trackIndex;
	

	vector<KeyFrame> keyFrameColl = m_CAppWorkPlant.m_TrackRepeatModule.GetKeyFrameCollToIndexTrack(m_CAppWorkPlant.m_TrackRepeatModule.m_TrackIndex);
	vector<KeyFrame>::iterator it = keyFrameColl.begin();

	vector<KeyFrame>::iterator itimg = keyFrameColl.begin();

	CBitmap cBitImg;
	
	HBITMAP hBitmap;

	Sleep(100);

	//加载图片
	for (int n = frameNumEverPage * currentPage; n < ImgNum; n++)
	{
		int j = 0;//关键帧列表计数
		while (j < frameNumEverPage * currentPage)
		{
			j++;
			itimg++;
		}

		CString path;
		CString temp;
		path = "D:\\Test\\";
		path += filename;
		path += "\\";

		temp.Format("%d", itimg->IPOC);
		path += temp;
		path += ".bmp";


		hBitmap = (HBITMAP)::LoadImage(::AfxGetInstanceHandle(), path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

		cBitImg.Attach(hBitmap);

		ImgLit.Add(&cBitImg, RGB(255, 255, 255));
		cBitImg.Detach();
		itimg++;
	}

	m_listKeyFrames.SetImageList(&ImgLit, LVSIL_NORMAL);
	CString strTemp;
	int k = 0;
	for (int n = frameNumEverPage * currentPage; n < ImgNum; n++) {
		int j = 0;//关键帧列表计数
		while (j < frameNumEverPage * currentPage)
		{
			j++;
			it++;
		}

		CString temp = it->name.c_str();

		strTemp.Format(_T("点位:%s  运行时间:%.1fs  最小时间:%.2fs 等待时间:%ds"), temp, it->FrameRunTime, it->FrameMinTime,it->WaitTime);

		m_listKeyFrames.InsertItem(k, strTemp, n);
		k++;
		it++;
	}
	ImgLit.Detach();
	mocoLog.StatusOut(Info, "轨迹导入刷新图片 %s", strCBText);
	return;

}

//前一页更新关键帧
void CGPothkaMainWinDlg::prePageKeyFrames(){

	CString strCBText;
	char* s_trackIndex;
	int nIndex = m_cbxframesIndex.GetCurSel();

	m_cbxframesIndex.GetLBText(nIndex, strCBText);
	s_trackIndex = strCBText.GetBuffer(0);
	m_CAppWorkPlant.m_TrackRepeatModule.m_TrackIndex = s_trackIndex;  //设置关键帧索引
	m_listKeyFrames.DeleteAllItems();
	
	vector<KeyFrame> keyFrameColl = m_CAppWorkPlant.m_TrackRepeatModule.GetKeyFrameCollToIndexTrack(m_CAppWorkPlant.m_TrackRepeatModule.m_TrackIndex);
	int index = 0;

	int kk = 0;

	for (vector<KeyFrame>::iterator it = keyFrameColl.begin() + (frameNumEverPage * currentPage); kk != frameNumEverPage; it++)
	{
		kk++;
		CString s_Value;
		s_Value.Format("%u", it->IPOC);
		int row = m_listKeyFrames.InsertItem(index, s_Value);
		m_listKeyFrames.SetItemText(row, 0, s_Value);
		s_Value.Format("%f", it->PartSpeed);
		m_listKeyFrames.SetItemText(row, 1, s_Value);
		s_Value.Format("%d", it->WaitTime);
		m_listKeyFrames.SetItemText(row, 2, s_Value);
		s_Value.Format("%f", it->X);
		m_listKeyFrames.SetItemText(row, 3, s_Value);
		s_Value.Format("%f", it->Y);
		m_listKeyFrames.SetItemText(row, 4, s_Value);
		s_Value.Format("%f", it->Z);
		m_listKeyFrames.SetItemText(row, 5, s_Value);
		s_Value.Format("%f", it->A);
		m_listKeyFrames.SetItemText(row, 6, s_Value);
		s_Value.Format("%f", it->B);
		m_listKeyFrames.SetItemText(row, 7, s_Value);
		s_Value.Format("%f", it->C);
		m_listKeyFrames.SetItemText(row, 8, s_Value);
		s_Value.Format("%u", it->Zoom);
		m_listKeyFrames.SetItemText(row, 9, s_Value);
		s_Value.Format("%u", it->Focus);
		m_listKeyFrames.SetItemText(row, 10, s_Value);
		s_Value.Format("%f", it->FrameRunTime);
		m_listKeyFrames.SetItemText(row, 11, s_Value);
		s_Value.Format("%f", it->FrameMinTime);
		m_listKeyFrames.SetItemText(row, 12, s_Value);
		index++;
	}
}

//后一页关键帧更新
void CGPothkaMainWinDlg::nextPageKeyFrames(){

	CString strCBText;
	char* s_trackIndex;
	int nIndex = m_cbxframesIndex.GetCurSel();

	m_cbxframesIndex.GetLBText(nIndex, strCBText);
	s_trackIndex = strCBText.GetBuffer(0);
	m_CAppWorkPlant.m_TrackRepeatModule.m_TrackIndex = s_trackIndex;  //设置关键帧索引
	m_listKeyFrames.DeleteAllItems();

	vector<KeyFrame> keyFrameColl = m_CAppWorkPlant.m_TrackRepeatModule.GetKeyFrameCollToIndexTrack(m_CAppWorkPlant.m_TrackRepeatModule.m_TrackIndex);
	int index = 0;
	int kk = 0;
	
	for (vector<KeyFrame>::iterator it = keyFrameColl.begin() + (frameNumEverPage * currentPage); it != keyFrameColl.end() && kk != frameNumEverPage; it++)
	{
		kk++;
		CString s_Value;
		s_Value.Format("%u", it->IPOC);
		int row = m_listKeyFrames.InsertItem(index, s_Value);
		m_listKeyFrames.SetItemText(row, 0, s_Value);
		s_Value.Format("%f", it->PartSpeed);
		m_listKeyFrames.SetItemText(row, 1, s_Value);
		s_Value.Format("%d", it->WaitTime);
		m_listKeyFrames.SetItemText(row, 2, s_Value);
		s_Value.Format("%f", it->X);
		m_listKeyFrames.SetItemText(row, 3, s_Value);
		s_Value.Format("%f", it->Y);
		m_listKeyFrames.SetItemText(row, 4, s_Value);
		s_Value.Format("%f", it->Z);
		m_listKeyFrames.SetItemText(row, 5, s_Value);
		s_Value.Format("%f", it->A);
		m_listKeyFrames.SetItemText(row, 6, s_Value);
		s_Value.Format("%f", it->B);
		m_listKeyFrames.SetItemText(row, 7, s_Value);
		s_Value.Format("%f", it->C);
		m_listKeyFrames.SetItemText(row, 8, s_Value);
		s_Value.Format("%u", it->Zoom);
		m_listKeyFrames.SetItemText(row, 9, s_Value);
		s_Value.Format("%u", it->Focus);
		m_listKeyFrames.SetItemText(row, 10, s_Value);
		s_Value.Format("%f", it->FrameRunTime);
		m_listKeyFrames.SetItemText(row, 11, s_Value);
		s_Value.Format("%f", it->FrameMinTime);
		m_listKeyFrames.SetItemText(row, 12, s_Value);
		index++;
	}
}

LRESULT CGPothkaMainWinDlg::StickEvent(WPARAM wParam, LPARAM lParam) //遥感按钮事件触发各个功能
{
	int a = wParam;
	mocoLog.StatusOut(Info, "快捷功能 %d", a);
	switch (a)
	{
	case 12:
		this->OnBnClickedbtnRestorationmotion();
		break;
	case 0:
		this->OnBnClickedbtnSinglemotion();
		break;
	case 1:
		this->OnBnClickedbtnStartrepeatmotion();
		break;
	case 2:
		this->OnBnClickedbtnSuspend();
		break;
	case 14:
		this->OnBnClickedbtnaddNewkeyframe();
		break;
	case 6:
		this->OnBnClickedbtnUpdateframekeyframe();
		break;
	case 7:
		this->OnBnClickedbtnprepage();
		break;
	case 8:
		this->OnBnClickedbtnnextpage();
		break;

	default:
		break;
	}

	return 0;
}

LRESULT CGPothkaMainWinDlg::LimitState(WPARAM wParam, LPARAM lParam)
{
	int a = wParam;
	mocoLog.StatusOut(Info, "限位判断 %d", a);
	switch (a)
	{
	case 0:
		State.SetWindowTextA("正常！");
		break;
	case 1:
		State.SetWindowTextA("轨道- 到达软限位！");
		break;
	case 2:
		State.SetWindowTextA("轨道+ 到达软限位！");
		break;
	case 3:
		State.SetWindowTextA("A1- 到达软限位！");
		break;
	case 4:
		State.SetWindowTextA("A1+ 到达软限位！");
		break;
	case 5:
		State.SetWindowTextA("A2- 到达软限位！");
		break;
	case 6:
		State.SetWindowTextA("A2+ 到达软限位！");
		break;
	case 7:
		State.SetWindowTextA("A3- 到达软限位！");
		break;
	case 8:
		State.SetWindowTextA("A3+ 到达软限位！");
		break;
	case 9:
		State.SetWindowTextA("A4- 到达软限位！");
		break;
	case 10:
		State.SetWindowTextA("A4+ 到达软限位！");
		break;
	case 11:
		State.SetWindowTextA("A5- 到达软限位！");
		break;
	case 12:
		State.SetWindowTextA("A5+ 到达软限位！");
		break;
	case 13:
		State.SetWindowTextA("A6- 到达软限位！");
		break;
	case 14:
		State.SetWindowTextA("A6+ 到达软限位！");
		break;
	

	default:
		break;
	}

	return 0;
}

void CGPothkaMainWinDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}

void CGPothkaMainWinDlg::OnCancel()
{
	CDialogEx::OnCancel();
}


void CGPothkaMainWinDlg::OnBnClickedbtncalibration()
{
	// TODO: 在此添加控件通知处理程序代码
   
	//校正参数
	CString caliX,caliY,caliZ,caliPan,caliTilt,caliRoll;

	GetDlgItem(IDC_calX)->GetWindowTextA(caliX);
	GetDlgItem(IDC_calY)->GetWindowTextA(caliY);
	GetDlgItem(IDC_calZ)->GetWindowTextA(caliZ);
	GetDlgItem(IDC_calPan)->GetWindowTextA(caliPan);
	GetDlgItem(IDC_calTilt)->GetWindowTextA(caliTilt);
	GetDlgItem(IDC_calRoll)->GetWindowTextA(caliRoll);

	m_CAppWorkPlant.m_TrackRepeatModule.camCalibration(atof(caliX),atof(caliY),atof(caliZ),atof(caliPan),atof(caliTilt),atof(caliRoll));

}

UINT __cdecl sendFreed(LPVOID lpParamter)
{	
	CAppWorkPlant m_CAppWorkPlant;
	CGPothkaMainWinDlg* m_dlg = (CGPothkaMainWinDlg*)lpParamter;
	
	m_dlg->mocoLog.StatusOut(Info, "发送Freed数据线程开启");

	CWnd* h_d = AfxGetApp()->GetMainWnd();

	WORD zom, foc;
	
	LARGE_INTEGER Liter;
	LONGLONG Q1, Q2;
	double dfreq, dFtim;

	//打开接收同步信号串口
	GP_Lens_Fujinon Lens;
	CString pComPort = "COM6";
	UINT pBautRate = CBR_2400;
	int b = Lens.InitComPort(pComPort, pBautRate);

	unsigned int length, length2;
	byte m_ReceiveData[10];

	int i = 0;
	while (stopThread == 1)
	{

		length = Lens.m_SerialPort->GetBytesInCOM();
		Lens.m_SerialPort->Read(m_ReceiveData, length);

		
		if (m_dlg->m_selectedDevice->GetSynicSignal())
		{
		
			LONGLONG F1, F2;
			QueryPerformanceFrequency(&Liter);
			dfreq = (double)Liter.QuadPart;
			QueryPerformanceCounter(&Liter);
			F1 = Liter.QuadPart;
	
			do
			{
				QueryPerformanceCounter(&Liter);
				F2 = Liter.QuadPart;
				dFtim = (F2 - F1) / dfreq*1000;
			} while (dFtim < 10);


			cameraLocationY += gCurrentRobotLocation.E1;

			Freed_SendD1Msg(gCurrentRobotLocation.A, gCurrentRobotLocation.B, gCurrentRobotLocation.C, gCurrentRobotLocation.X, gCurrentRobotLocation.Y + gCurrentRobotLocation.E1, gCurrentRobotLocation.Z, Zoom, Focus);

			QueryPerformanceFrequency(&Liter);
			dfreq = (double)Liter.QuadPart;
			QueryPerformanceCounter(&Liter);
			Q1 = Liter.QuadPart;
			Q2 = Liter.QuadPart;
			dFtim = (Q2 - Q1) / dfreq;

			while (dFtim<0.02)
			{
				QueryPerformanceCounter(&Liter);
				Q2 = Liter.QuadPart;
				dFtim = (Q2 - Q1) / dfreq;
			}

			cameraLocationY += gCurrentRobotLocation.E1;

			Freed_SendD1Msg(gCurrentRobotLocation.A, gCurrentRobotLocation.B, gCurrentRobotLocation.C, gCurrentRobotLocation.X, gCurrentRobotLocation.Y + gCurrentRobotLocation.E1, gCurrentRobotLocation.Z, Zoom, Focus);

			length = Lens.m_SerialPort->GetBytesInCOM();
			m_ReceiveData[0] = 0x00;
		}

	}
	return 0;
}

void CGPothkaMainWinDlg::OnBnClickedbtnsendcameradata()
{
	// TODO: 在此添加控件通知处理程序代码
	//开启计算并发送Freed的线程
	CGPothkaMainWinDlg *pDlg = this;
	sendF = AfxBeginThread(sendFreed,pDlg,0,0,0,NULL);

}

void CGPothkaMainWinDlg::OnBnClickedbtninitfreed()
{
	// TODO: 在此添加控件通知处理程序代码
	mocoLog.StatusOut(Info, "Freed串口初始化");
	Freed_Init(1);
	stopThread = 1;
}

void CGPothkaMainWinDlg::Onbtnkeyframetimeset()
{
	// TODO:  在此添加命令处理程序代码
	mocoLog.StatusOut(Info, "设置关键帧时间");
	if (softLock == 1)
	{
		MessageBox("软件处于锁定状态！");
		return;
	}

	POSITION pos = m_listKeyFrames.GetFirstSelectedItemPosition();
	keyFrameIndex = m_listKeyFrames.GetNextSelectedItem(pos);
	if (keyFrameIndex<0)
	{
		MessageBox ("请先选择关键帧！");
		return;
	}
	else if (keyFrameIndex + currentPage*frameNumEverPage == 0)
	{
		MessageBox("请从第2个关键帧开始设置");
		return;
	}
	else
	{
		CTimeSetDlg  Dlg;
		Dlg.DoModal();
	}

	CString strCBText;
	m_cbxframesIndex.GetWindowTextA(strCBText);
	char* s_trackIndex = strCBText.GetBuffer(0);

	m_CAppWorkPlant.m_TrackRepeatModule.UpdataKeyTime(keyFrameIndex + currentPage*frameNumEverPage, s_trackIndex, runTime,wTime);
	if (runTimeOK == 0)
	{
		MessageBox("该关键帧设置运行时间小于最小运行时间，请重新设置！");
		GetDlgItem(IDC_btnRestorationMotion)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_btnRestorationMotion)->EnableWindow(TRUE);
	}
	pageImages();
}

//修改关键帧名称
void CGPothkaMainWinDlg::OnbtnkeyframeNameSet()
{
	mocoLog.StatusOut(Info, "修改关键帧名称");
	if (softLock == 1)
	{
		MessageBox("软件处于锁定状态！");
		return;
	}

	POSITION pos = m_listKeyFrames.GetFirstSelectedItemPosition();
	keyFrameIndex = m_listKeyFrames.GetNextSelectedItem(pos);
	if (keyFrameIndex<0)
	{
		MessageBox("请先选择关键帧！");
		return;
	}
	else
	{
		NameSetDlg  Dlg;
		Dlg.DoModal();
	}
	
	CString strCBText;
	m_cbxframesIndex.GetWindowTextA(strCBText);
	char* s_trackIndex = strCBText.GetBuffer(0);

	m_CAppWorkPlant.m_TrackRepeatModule.UpdataKeyName(keyFrameIndex + currentPage*frameNumEverPage, s_trackIndex, keyframeName);

    pageImages();
	
}

void CGPothkaMainWinDlg::OnBnClickedbtnprepage()
{
	// TODO:  在此添加控件通知处理程序代码
	mocoLog.StatusOut(Info, "前一页");
	if (softLock == 1)
	{
		MessageBox("软件处于锁定状态！");
		return;
	}

	if (currentPage == 0)
	{
		return;
	}
	
	currentPage--;
	m_listKeyFrames.DeleteAllItems();

	pageImages();

	CString str;
	str.Format("%d / %d", currentPage+1,totalPage+1);
	GetDlgItem(IDC_pageIndex)->SetWindowTextA(str);

	if (currentPage == 0)
	{
		GetDlgItem(IDC_btnPrePage)->EnableWindow(FALSE);
	}
	GetDlgItem(IDC_btnNextPage)->EnableWindow(TRUE);
}


void CGPothkaMainWinDlg::OnBnClickedbtnnextpage()
{
	// TODO:  在此添加控件通知处理程序代码
	mocoLog.StatusOut(Info, "后一页");
	if (softLock == 1)
	{
		MessageBox("软件处于锁定状态！");
		return;
	}

	if (currentPage == totalPage)
	{
		return;
	}
	
	currentPage++;
	m_listKeyFrames.DeleteAllItems();
	
	pageImages();

	CString str;
	str.Format("%d / %d", currentPage + 1, totalPage + 1);
	GetDlgItem(IDC_pageIndex)->SetWindowTextA(str);
	
	if (currentPage == totalPage)
	{
		GetDlgItem(IDC_btnNextPage)->EnableWindow(FALSE);
	}
	GetDlgItem(IDC_btnPrePage)->EnableWindow(TRUE);
}


void CGPothkaMainWinDlg::OnBnClickedbtninitportcam()
{
	// TODO:  在此添加控件通知处理程序代码
	mocoLog.StatusOut(Info, "富士镜头初始化");
	CString pComPort="COM4";
	UINT pBautRate = CBR_38400;
	bool ini=true;
	int b = m_Lens_Fujinon.InitComPort(pComPort, pBautRate);
	ini=m_Lens_Fujinon.IsConnect();
	if (ini == false)
	{
		MessageBox("镜头串口初始化失败,请检查连接是否正确！");
		return ;
	}
	m_Lens_Fujinon.SetSwitch4Host(0xFC);

	m_Lens_Fujinon.GetLensMotorZoom(ZoomResultValue);
	m_Lens_Fujinon.GetLensMotorFocus(FocusResultValue);
	Zoom = ZoomResultValue;
	Focus = FocusResultValue;

}

void CGPothkaMainWinDlg::OnBnClickedbtninitportcamCanon()
{
	// TODO:  在此添加控件通知处理程序代码
	mocoLog.StatusOut(Info, "佳能镜头初始化");
	CString strComPort;
	UINT pBautRate = 76800;
	strComPort = "COM2";

	if (m_Lens_Canon.InitCommPort(strComPort, pBautRate))
	{
		m_Lens_Canon.PcControllerCommd();
		m_Lens_Canon.InitCompletedCommd();
		
		m_Lens_Canon.GetLensMotorFocus(FocusResultValue);
		m_Lens_Canon.GetLensMotorZoom(ZoomResultValue);
		
		WORD PcControlZoom = 0, PcControlFocus = 0;
		while (PcControlZoom != 0x50)
		{
			PcControlZoom = m_Lens_Canon.PcSwitchCommd(0x83);
			m_Lens_Canon.SetLensMotorZoom(ZoomResultValue);
			m_Lens_Canon.PcControllerCommd();
		}

		while (PcControlFocus != 0x50)
		{
			PcControlFocus = m_Lens_Canon.PcSwitchCommd(0x85);
			m_Lens_Canon.SetLensMotorFocus(FocusResultValue);
			m_Lens_Canon.PcControllerCommd();
		}

		Zoom = ZoomResultValue;
		Focus = FocusResultValue;

		return;
	}
	else
	{
		MessageBox(_T("初始化失败"));
	}
}


void CGPothkaMainWinDlg::OnBnClickedbtnsetswitch4()
{
	// TODO:  在此添加控件通知处理程序代码
	m_Lens_Fujinon.SetSwitch4Host(0xFC);
}


void CGPothkaMainWinDlg::Onbtncameracontrol()
{
	// TODO:  在此添加命令处理程序代码
	mocoLog.StatusOut(Info, "软件控制菜单");
	if (softLock == 1)
	{
		MessageBox("软件处于锁定状态！");
		return;
	}

	if (m_CAppWorkPlant.isPresetCommand == true)
	{
		MessageBox("软件处于运动状态！");
		return;
	}

	CameraControlDlg Dlg;
	softControl = 1;
	Dlg.DoModal();
	softControl = 0;
}


void CGPothkaMainWinDlg::Onbtnspeedcontrol()
{
	// TODO:  在此添加命令处理程序代码
	mocoLog.StatusOut(Info, "速度控制菜单");
	if (softLock == 1)
	{
		MessageBox("软件处于锁定状态！");
		return;
	}

	SpeedSetDlg Dlg;
	Dlg.DoModal();
}

void CGPothkaMainWinDlg::Onbtnrolllock()
{
	mocoLog.StatusOut(Info, "Roll锁定");
	if (softLock == 1)
	{
		MessageBox("软件处于锁定状态！");
		return;
	}

	GetMenu()->GetSubMenu(1)->EnableMenuItem(3, MF_BYPOSITION | MF_DISABLED);
	GetMenu()->GetSubMenu(1)->EnableMenuItem(4, MF_BYPOSITION | MF_ENABLED);
	rollLock = 1;

	m_CAppWorkPlant.m_TrackRepeatModule.ObtainRollLockMotionCommandData(gCurrentRobotLocation,&m_CAppWorkPlant.m_PresetSendDataColl);
	m_CAppWorkPlant.isPresetCommand = true;

}
void CGPothkaMainWinDlg::Onbtnrollunlock()
{
	mocoLog.StatusOut(Info, "Roll解锁");
	if (softLock == 1)
	{
		MessageBox("软件处于锁定状态！");
		return;
	}

	GetMenu()->GetSubMenu(1)->EnableMenuItem(3, MF_BYPOSITION | MF_ENABLED);
	GetMenu()->GetSubMenu(1)->EnableMenuItem(4, MF_BYPOSITION | MF_DISABLED);
	rollLock = 0;
}

void CGPothkaMainWinDlg::Onbtnrobotlock()
{
	mocoLog.StatusOut(Info, "机器人控制锁定");
	if (softLock == 1)
	{
		MessageBox("软件处于锁定状态！");
		return;
	}

	GetMenu()->GetSubMenu(1)->EnableMenuItem(0, MF_BYPOSITION | MF_DISABLED);
	GetMenu()->GetSubMenu(1)->EnableMenuItem(1, MF_BYPOSITION | MF_ENABLED);

	GetMenu()->GetSubMenu(3)->EnableMenuItem(0, MF_BYPOSITION | MF_DISABLED);

	robotLock = 1;
}
void CGPothkaMainWinDlg::Onbtnrobotunlock()
{
	mocoLog.StatusOut(Info, "机器人控制解锁");
	if (softLock == 1)
	{
		MessageBox("软件处于锁定状态！");
		return;
	}

	GetMenu()->GetSubMenu(1)->EnableMenuItem(0, MF_BYPOSITION | MF_ENABLED);
	GetMenu()->GetSubMenu(1)->EnableMenuItem(1, MF_BYPOSITION | MF_DISABLED);

	if (cameraLock == 0)
	{
		GetMenu()->GetSubMenu(3)->EnableMenuItem(0, MF_BYPOSITION | MF_ENABLED);
	}

	robotLock = 0;
}

void CGPothkaMainWinDlg::Onbtncameralock()
{
	mocoLog.StatusOut(Info, "镜头控制锁定");
	if (softLock == 1)
	{
		MessageBox("软件处于锁定状态！");
		return;
	}

	GetMenu()->GetSubMenu(1)->EnableMenuItem(6, MF_BYPOSITION | MF_DISABLED);
	GetMenu()->GetSubMenu(1)->EnableMenuItem(7, MF_BYPOSITION | MF_ENABLED);

	GetMenu()->GetSubMenu(3)->EnableMenuItem(0, MF_BYPOSITION | MF_DISABLED);

	cameraLock = 1;
}
void CGPothkaMainWinDlg::Onbtncameraunlock()
{
	mocoLog.StatusOut(Info, "镜头控制解锁");
	if (softLock == 1)
	{
		MessageBox("软件处于锁定状态！");
		return;
	}

	GetMenu()->GetSubMenu(1)->EnableMenuItem(6, MF_BYPOSITION | MF_ENABLED);
	GetMenu()->GetSubMenu(1)->EnableMenuItem(7, MF_BYPOSITION | MF_DISABLED);

	if (robotLock == 0)
	{
		GetMenu()->GetSubMenu(3)->EnableMenuItem(0, MF_BYPOSITION | MF_ENABLED);
	}

	cameraLock = 0;
}

LRESULT CGPothkaMainWinDlg::OnAddDevice(WPARAM wParam, LPARAM lParam)
{

	m_selectedDevice = new DeckLinkDevice((IDeckLink*)wParam);

	if (!m_selectedDevice->Init())
	{
		m_selectedDevice->Release();
		return 0;
	}

}


void CGPothkaMainWinDlg::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	stopThread = 0;
	if (m_deckLinkDiscovery != NULL)
	{
		m_deckLinkDiscovery->Disable();
		m_deckLinkDiscovery->Release();
		m_deckLinkDiscovery = NULL;
	}
	if (m_selectedDevice!=NULL)
	{
		m_selectedDevice->StopCapture();
		m_selectedDevice->Release();
	}
	
	CString strCBText;
	CString saveFullPath;
	CString temp;

	m_cbxframesIndex.GetWindowText(strCBText);

	if (strCBText == "")
	{
		SYSTEMTIME sys;
		GetLocalTime(&sys);
		strCBText.Format(_T("%d%d%d%d%d%d"), sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond);
	}

	temp = "D:\\LastTrack\\";
	saveFullPath = temp + strCBText;
	temp = ".txt";
	saveFullPath += temp;

	ofstream file(saveFullPath, ios_base::out);
	file.clear();
	file.close();
	mocoLog.StatusOut(Info, "关机自动保存轨迹 %s", strCBText);
	m_CAppWorkPlant.m_TrackRepeatModule.SaveKeyFramesToLocal(saveFullPath.GetBuffer(0), strCBText.GetBuffer(0));

    ofstream outfile("D:\\LastTrack\\Dir.txt", ios_base::out);
	
	if (!outfile.is_open())
	{
		Sleep(100);
	}

	char *dir=(char*)malloc(2048);
	dir = (LPSTR)(LPCTSTR)strCBText;
	outfile << dir;
	outfile.clear();
	outfile.close();
	Sleep(100);
	CDialogEx::OnClose();
}


void CGPothkaMainWinDlg::Onbtnhelp()
{
	// TODO:  在此添加命令处理程序代码
	MessageBox("版本修改日期：2017/3/18");
}


void CGPothkaMainWinDlg::OnbtnSoftLock()
{
	// TODO:  在此添加命令处理程序代码
	mocoLog.StatusOut(Info, "软件锁定");
	softLock = 1;
	GetMenu()->GetSubMenu(1)->EnableMenuItem(9, MF_BYPOSITION | MF_DISABLED);
	GetMenu()->GetSubMenu(1)->EnableMenuItem(10, MF_BYPOSITION | MF_ENABLED);
}


void CGPothkaMainWinDlg::OnbtnSoftUnlock()
{
	// TODO:  在此添加命令处理程序代码
	mocoLog.StatusOut(Info, "软件解锁");
	softLock = 0;
	GetMenu()->GetSubMenu(1)->EnableMenuItem(9, MF_BYPOSITION | MF_ENABLED);
	GetMenu()->GetSubMenu(1)->EnableMenuItem(10, MF_BYPOSITION | MF_DISABLED);
}
