#pragma once
// GPothka.MainWinDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ʵ��
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

// CGPothkaMainWinDlg �Ի���

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

	//ҳ��
	DDX_Control(pDX, IDC_pageIndex,page);

	//״̬��
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
	//�Զ�����Ϣ
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


// CGPothkaMainWinDlg ��Ϣ�������
BOOL CGPothkaMainWinDlg::OnInitDialog()
{
	this->ShowWindow(SW_SHOWMAXIMIZED);
	CDialogEx::OnInitDialog();

	
	// ��������...���˵�����ӵ�ϵͳ�˵��С�
	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	//Ƥ��
	SkinH_Attach();

	//��ʼ���ؼ�֡�б�ؼ�
	DWORD dwStyle = m_listKeyFrames.GetExtendedStyle();                
	dwStyle |= LVS_EX_FULLROWSELECT;            
	dwStyle |= LVS_EX_GRIDLINES;                
	m_listKeyFrames.SetExtendedStyle(dwStyle);
	m_listKeyFrames.InsertColumn(0,_T("IPOC"),LVCFMT_LEFT,55,-1);
	m_listKeyFrames.InsertColumn(1,_T("�ֲ��ٶ�"),LVCFMT_LEFT,70);
	m_listKeyFrames.InsertColumn(2,_T("�ȴ�ʱ��"),LVCFMT_LEFT,70);
	m_listKeyFrames.InsertColumn(3, _T("X"), LVCFMT_LEFT, 55);
	m_listKeyFrames.InsertColumn(4, _T("Y"), LVCFMT_LEFT, 55);
	m_listKeyFrames.InsertColumn(5, _T("Z"), LVCFMT_LEFT, 55);
	m_listKeyFrames.InsertColumn(6, _T("A"), LVCFMT_LEFT, 55);
	m_listKeyFrames.InsertColumn(7, _T("B"), LVCFMT_LEFT, 55);
	m_listKeyFrames.InsertColumn(8, _T("C"), LVCFMT_LEFT, 55);
	m_listKeyFrames.InsertColumn(9, _T("Zoom"), LVCFMT_LEFT, 55);
	m_listKeyFrames.InsertColumn(10, _T("Focus"), LVCFMT_LEFT, 55);
	m_listKeyFrames.InsertColumn(11, _T("���е��õ�ʱ��"), LVCFMT_LEFT, 100);
	m_listKeyFrames.InsertColumn(12, _T("��С����ʱ��"), LVCFMT_LEFT, 100);
	

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


	//��ʼ��decklink����豸�����
	m_deckLinkDiscovery = new DeckLinkDeviceDiscovery(this);
	m_deckLinkDiscovery->Enable();

	CRect rect;
	GetClientRect(&rect);

	mocoLog.Init("Log");
	mocoLog.StatusOut(Info, "�����ʼ���ɹ�");

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CGPothkaMainWinDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CGPothkaMainWinDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CGPothkaMainWinDlg::OnBnClickedbtnStartListenServer()
{
	//TODO: �ڴ���ӿؼ�֪ͨ���������� 
	if (softLock == 1)
	{
		MessageBox("�����������״̬��");
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

		//��ȡ֡//
		//m_selectedDevice->StartCapture(true);
		//m_selectedDevice->m_SaveFrameFlag = false;
	//}
	//else
	//{
	//	MessageBox("������ҡ�ˣ�");
	//	GetDlgItem(IDC_btnStartListenServer)->EnableWindow(TRUE);
	//	return ;
	//}
	if (camera == "Fujinon")
	{
		CGPothkaMainWinDlg *pDlg = this;
		camCon = AfxBeginThread(camControl, pDlg, 0, 0, 0, NULL);
		pDlg->mocoLog.StatusOut(Info, "��ʿ��ͷ�����߳̿���");
	}
	else if (camera == "Canon")
	{
		CGPothkaMainWinDlg *pDlg = this;
		Canon = AfxBeginThread(CanonContrl, pDlg, 0, 0, 0, NULL);
		pDlg->mocoLog.StatusOut(Info, "���ܾ�ͷ�����߳̿���");
	}
	GetDlgItem(IDC_CameraList)->EnableWindow(FALSE);

	//�Զ������ϴιر�ʱ�켣	
	//ImportTrack();

}

//���ܾ�ͷ�����߳�
UINT __cdecl CanonContrl(LPVOID lpParamter)
{
	WORD FocusValue, ZoomValue;
	CGPothkaMainWinDlg* pCanonDlg = (CGPothkaMainWinDlg*)lpParamter;

	//��ͷ��ʼ��
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


//ң�˿��ƾ�ͷ�߳�
UINT __cdecl camControl(LPVOID lpParamter)
{
	WORD FocusValue, ZoomValue;
	CGPothkaMainWinDlg* mlg = (CGPothkaMainWinDlg*)lpParamter;
	
	//��ͷ��ʼ��
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
	 //TODO: �ڴ���ӿؼ�֪ͨ����������
	if (softLock == 1)
	{
		MessageBox("�����������״̬��");
		return;
	}

	if(gRunState.Suspend==true)
	{
		  gRunState.Suspend=false;
		
		  GetDlgItem(IDC_btnSuspend)->SetWindowTextA("��ͣ");

		  GetDlgItem(IDC_btnRestorationMotion)->EnableWindow(TRUE);

		  mocoLog.StatusOut(Info,"�����ͣ");
	}
	else
	{
		gRunState.Suspend=true;
		
		GetDlgItem(IDC_btnSuspend)->SetWindowTextA("ȡ����ͣ");

		GetDlgItem(IDC_btnRestorationMotion)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnStartRepeatMotion)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnSingleMotion)->EnableWindow(FALSE);
		mocoLog.StatusOut(Info, "�����ͣȡ��");
	}
}

//�����¹켣
void CGPothkaMainWinDlg::Onbtnnewframeindex()
{
	// TODO:  �ڴ���������������
	if (softLock == 1)
	{
		MessageBox("�����������״̬��");
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
		mocoLog.StatusOut(Info, "�����¹켣 %s", indexNewtrack);
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

			char c_logInfo[100] = "�����켣��";
			strcat_s(c_logInfo, 100, indexNewtrack.GetBuffer(0));
			
		}
	}
	else
	{	//����ʧ��
		char c_logInfo[100] = "ʧ�ܴ����켣��";
		strcat_s(c_logInfo, 100, indexNewtrack.GetBuffer(0));
		
	}

}

//����켣ʱ�Զ������¹켣
void CGPothkaMainWinDlg::Onbtnnewframeindex(CString dir)
{
	// TODO:  �ڴ���������������
	frameNum = 0;
	totalPage = 0;
	currentPage = 0;
	loadIndex = 1;
	if (m_CAppWorkPlant.m_TrackRepeatModule.AddNewTrackToIndexTrack(dir.GetBuffer(0)) == true){
		mocoLog.StatusOut(Info, "����켣�����¹켣 %s", dir);
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
			
			char c_logInfo[100] = "�����켣��";
			strcat_s(c_logInfo, 100, dir.GetBuffer(0));
		}
	}
	else
	{	//����ʧ��
		char c_logInfo[100] = "ʧ�ܴ����켣��";
		strcat_s(c_logInfo, 100, dir.GetBuffer(0));
	}
}

//ɾ���켣
void CGPothkaMainWinDlg::Onbtndeleteframeindex()
{
	// TODO:  �ڴ���������������
	if (softLock == 1)
	{
		MessageBox("�����������״̬��");
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
		mocoLog.StatusOut(Info, "ɾ���켣 %s", indexNewtrack);
		m_cbxframesIndex.DeleteString(nIndex);

		m_cbxframesIndex.SetWindowTextA("");

		char c_logInfo[100] = "�ɹ�ɾ���켣��";
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
		char c_logInfo[100] = "ʧ��ɾ���켣��";
		strcat_s(c_logInfo, 100, indexNewtrack);
		
	}
}



//ѡ��켣����
void CGPothkaMainWinDlg::OnCbnSelchangecbxFramesIndex()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	mocoLog.StatusOut(Info, "�л��켣");
	CString strCBText;
	int nIndex = m_cbxframesIndex.GetCurSel();
	m_cbxframesIndex.GetLBText(nIndex,strCBText);
	char* s_trackIndex=strCBText.GetBuffer(0);
	m_CAppWorkPlant.m_TrackRepeatModule.m_TrackIndex=s_trackIndex;  //���ùؼ�֡����

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

//ѡ��ؼ�֡
void CGPothkaMainWinDlg::OnLvnItemchangedlistkeyframes(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	if(( pNMLV->uChanged&LVIF_STATE)&&(pNMLV-> uNewState&LVIS_SELECTED)&&!(pNMLV->uOldState & LVIS_FOCUSED))
	{
		int a=pNMLV->iItem;
		m_CAppWorkPlant.m_TrackRepeatModule.m_keyFrameIndex=a;
	}
}

//����µĹؼ�֡
void CGPothkaMainWinDlg::OnBnClickedbtnaddNewkeyframe()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (softLock == 1)
	{
		MessageBox("�����������״̬��");
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

	//��ǰѡ��Ĺؼ�֡
    POSITION pos=m_listKeyFrames.GetFirstSelectedItemPosition();
    int m_nIndex = m_listKeyFrames.GetNextSelectedItem(pos); 
	if(s_trackIndex=="")
	{
		char c_logInfo[100]="ʧ������µĹؼ�֡��";
		strcat_s(c_logInfo,100,"���ȴ����켣��������");
		return;
	}

	if(m_nIndex<0)//��ӹؼ�֡
	{
		if (frameNum == 0)
		{
			////�½����ڴ��ͼƬ���ļ���
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
			//�ɼ�ͼƬ
			mocoLog.StatusOut(Info, "��ӹؼ�֡");
			int cout = 0;
			m_selectedDevice->m_SaveFrameFlag = true;
			while (m_selectedDevice->m_SaveFrameFlag == true&&(cout<10))
			{
				cout++;
				Sleep(10);
			}
			
			char c_logInfo[100]="�ɹ�����µĹؼ�֡��";
			strcat_s(c_logInfo,100,"����");
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
			char c_logInfo[100]="ʧ������µĹؼ�֡��";
			strcat_s(c_logInfo,100,"����");
			
		}
	}
	else //����ؼ�֡
	{
		CString csPath = "D:\\TEST\\";
		csPath += str;
		picPath = csPath;

		if(true==m_CAppWorkPlant.m_TrackRepeatModule.InsertKeyFrame(gCurrentRobotLocation,s_trackIndex,m_nIndex,waitTime,partSpeed))
		{
			mocoLog.StatusOut(Info, "����ؼ�֡");
			char c_logInfo[100]="�ɹ������µĹؼ�֡��";
			strcat_s(c_logInfo,100,"����");
			
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
			char c_logInfo[100]="ʧ�ܲ����µĹؼ�֡��";
			strcat_s(c_logInfo,100,"����");
		
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


//ɾ���ؼ�֡
void CGPothkaMainWinDlg::Onbtndeleteframe()
{
	// TODO:  �ڴ���������������
	if (softLock == 1)
	{
		MessageBox("�����������״̬��");
		return;
	}

	CString strCBText;
	m_cbxframesIndex.GetWindowTextA(strCBText);
	char* s_trackIndex = strCBText.GetBuffer(0);

	//��ǰѡ��Ĺؼ�֡
	POSITION pos = m_listKeyFrames.GetFirstSelectedItemPosition();
	int m_nIndex = m_listKeyFrames.GetNextSelectedItem(pos);
	if (s_trackIndex == "")
	{
		char c_logInfo[100] = "ʧ�ܵ�ɾ���ؼ�֡��";
		strcat_s(c_logInfo, 100, "���ȴ����켣��������");
		
		return;
	}

	if (m_nIndex == -1)
	{
		m_nIndex = 0;
	}

	if (m_CAppWorkPlant.m_TrackRepeatModule.DeleteKeyFrame(m_nIndex + currentPage*frameNumEverPage, s_trackIndex))
	{
		mocoLog.StatusOut(Info, "ɾ���ؼ�֡");
		char c_logInfo[100] = "�ɹ�ɾ���ؼ�֡��";
		strcat_s(c_logInfo, 100, "����");
		
	}
	else{
		char c_logInfo[100] = "ʧ��ɾ���ؼ�֡��";
		strcat_s(c_logInfo, 100, "����");
		
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

	//������С����ʱ��
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

//���¹ؼ�֡
void CGPothkaMainWinDlg::OnBnClickedbtnUpdateframekeyframe()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (softLock == 1)
	{
		MessageBox("�����������״̬��");
		return;
	}

	//��ǰѡ��Ĺ켣����
	CString strCBText;
	m_cbxframesIndex.GetWindowTextA(strCBText);
	char* s_trackIndex=strCBText.GetBuffer(0);

	CString str;
	int nIndex = m_cbxframesIndex.GetCurSel();
	m_cbxframesIndex.GetLBText(nIndex, str);
	string indexNewtrack = str.GetBuffer(0);

	//��ǰѡ��Ĺؼ�֡
	POSITION pos=m_listKeyFrames.GetFirstSelectedItemPosition();
	int m_nIndex = m_listKeyFrames.GetNextSelectedItem(pos); 
	if(s_trackIndex=="")
	{
		char c_logInfo[100]="ʧ�ܵĸ��¹ؼ�֡��";
		strcat_s(c_logInfo,100,"���ȴ����켣��������");
		//SendLogToWindows(this->m_hWnd,c_logInfo);
		return;
	}

	float waitTime=0;
	double partSpeed=0;

	waitTime = wTime;
	partSpeed = 1;

	if (true == m_CAppWorkPlant.m_TrackRepeatModule.UpdateKeyFrame(gCurrentRobotLocation, m_nIndex + currentPage*frameNumEverPage, s_trackIndex, waitTime, partSpeed))
	{
		mocoLog.StatusOut(Info, "���¹ؼ�֡");
		char c_logInfo[100]="�ɹ����¹ؼ�֡��";
		strcat_s(c_logInfo,100,"����");
	}else
	{

	}

	//����ͼƬ
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

//�����켣
void CGPothkaMainWinDlg::Onbtnoutputtrack()
{
	// TODO:  �ڴ���������������
	if (softLock == 1)
	{
		MessageBox("�����������״̬��");
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
	FileDlg.m_ofn.lpstrInitialDir = "����:\\";
	if (FileDlg.DoModal() == IDOK)
	{
		mocoLog.StatusOut(Info, "�����켣 %s",strCBText);
		saveFullPath = FileDlg.GetPathName();
		m_CAppWorkPlant.m_TrackRepeatModule.SaveKeyFramesToLocal(saveFullPath.GetBuffer(0), strCBText.GetBuffer(0));
		return;
	}
}

//����켣
void CGPothkaMainWinDlg::Onbtnimporttrack()
{
	// TODO:  �ڴ���������������
	if (softLock == 1)
	{
		MessageBox("�����������״̬��");
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
		
		mocoLog.StatusOut(Info, "����켣 %s", strCBText);
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

//������ʱ�Զ����ع켣
void CGPothkaMainWinDlg::ImportTrack()
{
	// TODO:  �ڴ���������������
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
		MessageBox("�Ĺ켣�Զ�����ʧ�ܣ�");
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
		mocoLog.StatusOut(Info, "�����Զ����ع켣 %s", strCBText);
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


//�˶�����
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
		//���ö�ʱ������
		LARGE_INTEGER lageinter;
		LONGLONG CountBegin, CountEnd;
		double diFreq, diTime;
		//��ȡλ����Ϣ
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

			//����û�û������ʱ�䣬��ͷ�˶�ʱ����ݻ������˶����м���
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
		//���ö�ʱ������
		LARGE_INTEGER lageinter;
		LONGLONG CountBegin, CountEnd;
		double diFreq, diTime;
		//��ȡλ����Ϣ
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


//�����˶�
void CGPothkaMainWinDlg::OnBnClickedbtnSinglemotion()
{
	if (softLock == 1)
	{
		MessageBox("�����������״̬��");
		return;
	}

	GetDlgItem(IDC_btnStartRepeatMotion)->EnableWindow(FALSE);
	
	m_CAppWorkPlant.m_LocusRunCommand.clear();
	
	//��ǰѡ��Ĺؼ�֡
	POSITION pos=m_listKeyFrames.GetFirstSelectedItemPosition();
	int m_nIndex = m_listKeyFrames.GetNextSelectedItem(pos); 

	if(m_nIndex<0) 
	{				
		MessageBox("ִ�е����˶�ǰ����ѡ��ؼ�֡��");
		return;
	}
	else if (frameIndex == m_nIndex)
	{
		MessageBox("��ѡ�������ؼ�֡��");
		return;
	}

	frameIndex = m_nIndex;

	singleIndex = m_nIndex;
	mocoLog.StatusOut(Info, "�����˶�");
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_CAppWorkPlant.m_TrackRepeatModule.ObtainSingleMotionCommandData(
		gCurrentRobotLocation,
		m_CAppWorkPlant.m_TrackRepeatModule.m_keyFrameIndex + currentPage*frameNumEverPage,
		m_CAppWorkPlant.m_TrackRepeatModule.m_TrackIndex,&m_CAppWorkPlant.m_PresetSendDataColl
		);
	m_CAppWorkPlant.isPresetCommand=true;

	CGPothkaMainWinDlg *pDlg = this;
	camSing = AfxBeginThread(camSingle, pDlg, 0, 0, 0, NULL);

}

//��λ�˶�
void CGPothkaMainWinDlg::OnBnClickedbtnRestorationmotion()
{
	if (softLock == 1)
	{
		MessageBox("�����������״̬��");
		return;
	}

	if (frameNum == 0)
	{
		return;
	}
	
	softControl = 1;
	
	WORD zom;
	frameIndex = 0;
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_CAppWorkPlant.m_LocusRunCommand.clear();

	CString strCBText;
	int nIndex = m_cbxframesIndex.GetCurSel();
	m_cbxframesIndex.GetLBText(nIndex, strCBText);
	char* s_trackIndex = strCBText.GetBuffer(0);
	m_CAppWorkPlant.m_TrackRepeatModule.m_TrackIndex = s_trackIndex;  //���ùؼ�֡����

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

	mocoLog.StatusOut(Info, "��λ�˶�");
	//�����˸�λ
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

		//���ö�ʱ������
		LARGE_INTEGER lageinter;
		LONGLONG CountBegin, CountEnd;
		double diFreq, diTime;
		//��ȡλ����Ϣ
		vector<KeyFrame>::iterator KeyFraVecItor = itor->second.begin();
		float FocusValue, ZoomValue, Time, TimeFrequency;
		float CurrentFocusPos = KeyFraVecItor->Focus;//Focus��ǰλ��
		float CurrentZoomPos = KeyFraVecItor->Zoom;//Zoom��ǰλ��

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

		//��ͣ��ͷ���ظ��˶�Ҳֹͣ
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

		//���ö�ʱ������
		LARGE_INTEGER lageinter;
		LONGLONG CountBegin, CountEnd;
		double diFreq, diTime;
		//��ȡλ����Ϣ
		vector<KeyFrame>::iterator KeyFraVecItor = itor->second.begin();
		float FocusValue, ZoomValue, Time, TimeFrequency;
		float CurrentFocusPos = KeyFraVecItor->Focus;//Focus��ǰλ��
		float CurrentZoomPos = KeyFraVecItor->Zoom;//Zoom��ǰλ��

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

		//��ͣ��ͷ���ظ��˶�Ҳֹͣ
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

//�켣�ظ�
void CGPothkaMainWinDlg::OnBnClickedbtnStartrepeatmotion()
{
	//TODO: �ڴ���ӿؼ�֪ͨ����������
	if (softLock == 1)
	{
		MessageBox("�����������״̬��");
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
	m_CAppWorkPlant.m_TrackRepeatModule.m_TrackIndex=s_trackIndex;  //���ùؼ�֡����
	m_listKeyFrames.DeleteAllItems();

	mocoLog.StatusOut(Info, "�����˶�");
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

//��ҳ���¹ؼ�֡
void CGPothkaMainWinDlg::pageKeyFrames(){

	CString strCBText;
	char* s_trackIndex;
	int nIndex = m_cbxframesIndex.GetCurSel();

	m_cbxframesIndex.GetLBText(nIndex, strCBText);
	s_trackIndex = strCBText.GetBuffer(0);
	m_CAppWorkPlant.m_TrackRepeatModule.m_TrackIndex = s_trackIndex;  //���ùؼ�֡����
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

//ˢ��ͼƬ
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

	int j = 0;//�ؼ�֡�б����
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

	//����ͼƬ
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
	
    j = 0;//�ؼ�֡�б����
	while (j < frameNumEverPage * currentPage)
	{
		j++;
		it++;
	}

	for (int n = 0; n < temp; n++) {
		CString temp = it->name.c_str();

		strTemp.Format(_T("����:%s  ����ʱ��:%.1fs  ��Сʱ��:%.2fs �ȴ�ʱ��:%ds"), temp, it->FrameRunTime, it->FrameMinTime,it->WaitTime);

		m_listKeyFrames.InsertItem(k, strTemp, n);
		k++;
		it++;
	}
	ImgLit.Detach();
	mocoLog.StatusOut(Info, "ˢ��ͼƬ %s", strCBText);
	return ;

}

//����켣ʱˢ��ͼƬ
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

	//����ͼƬ
	for (int n = frameNumEverPage * currentPage; n < ImgNum; n++)
	{
		int j = 0;//�ؼ�֡�б����
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
		int j = 0;//�ؼ�֡�б����
		while (j < frameNumEverPage * currentPage)
		{
			j++;
			it++;
		}

		CString temp = it->name.c_str();

		strTemp.Format(_T("��λ:%s  ����ʱ��:%.1fs  ��Сʱ��:%.2fs �ȴ�ʱ��:%ds"), temp, it->FrameRunTime, it->FrameMinTime,it->WaitTime);

		m_listKeyFrames.InsertItem(k, strTemp, n);
		k++;
		it++;
	}
	ImgLit.Detach();
	mocoLog.StatusOut(Info, "�켣����ˢ��ͼƬ %s", strCBText);
	return;

}

//ǰһҳ���¹ؼ�֡
void CGPothkaMainWinDlg::prePageKeyFrames(){

	CString strCBText;
	char* s_trackIndex;
	int nIndex = m_cbxframesIndex.GetCurSel();

	m_cbxframesIndex.GetLBText(nIndex, strCBText);
	s_trackIndex = strCBText.GetBuffer(0);
	m_CAppWorkPlant.m_TrackRepeatModule.m_TrackIndex = s_trackIndex;  //���ùؼ�֡����
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

//��һҳ�ؼ�֡����
void CGPothkaMainWinDlg::nextPageKeyFrames(){

	CString strCBText;
	char* s_trackIndex;
	int nIndex = m_cbxframesIndex.GetCurSel();

	m_cbxframesIndex.GetLBText(nIndex, strCBText);
	s_trackIndex = strCBText.GetBuffer(0);
	m_CAppWorkPlant.m_TrackRepeatModule.m_TrackIndex = s_trackIndex;  //���ùؼ�֡����
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

LRESULT CGPothkaMainWinDlg::StickEvent(WPARAM wParam, LPARAM lParam) //ң�а�ť�¼�������������
{
	int a = wParam;
	mocoLog.StatusOut(Info, "��ݹ��� %d", a);
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
	mocoLog.StatusOut(Info, "��λ�ж� %d", a);
	switch (a)
	{
	case 0:
		State.SetWindowTextA("������");
		break;
	case 1:
		State.SetWindowTextA("���- ��������λ��");
		break;
	case 2:
		State.SetWindowTextA("���+ ��������λ��");
		break;
	case 3:
		State.SetWindowTextA("A1- ��������λ��");
		break;
	case 4:
		State.SetWindowTextA("A1+ ��������λ��");
		break;
	case 5:
		State.SetWindowTextA("A2- ��������λ��");
		break;
	case 6:
		State.SetWindowTextA("A2+ ��������λ��");
		break;
	case 7:
		State.SetWindowTextA("A3- ��������λ��");
		break;
	case 8:
		State.SetWindowTextA("A3+ ��������λ��");
		break;
	case 9:
		State.SetWindowTextA("A4- ��������λ��");
		break;
	case 10:
		State.SetWindowTextA("A4+ ��������λ��");
		break;
	case 11:
		State.SetWindowTextA("A5- ��������λ��");
		break;
	case 12:
		State.SetWindowTextA("A5+ ��������λ��");
		break;
	case 13:
		State.SetWindowTextA("A6- ��������λ��");
		break;
	case 14:
		State.SetWindowTextA("A6+ ��������λ��");
		break;
	

	default:
		break;
	}

	return 0;
}

void CGPothkaMainWinDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
}

void CGPothkaMainWinDlg::OnCancel()
{
	CDialogEx::OnCancel();
}


void CGPothkaMainWinDlg::OnBnClickedbtncalibration()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
   
	//У������
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
	
	m_dlg->mocoLog.StatusOut(Info, "����Freed�����߳̿���");

	CWnd* h_d = AfxGetApp()->GetMainWnd();

	WORD zom, foc;
	
	LARGE_INTEGER Liter;
	LONGLONG Q1, Q2;
	double dfreq, dFtim;

	//�򿪽���ͬ���źŴ���
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//�������㲢����Freed���߳�
	CGPothkaMainWinDlg *pDlg = this;
	sendF = AfxBeginThread(sendFreed,pDlg,0,0,0,NULL);

}

void CGPothkaMainWinDlg::OnBnClickedbtninitfreed()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	mocoLog.StatusOut(Info, "Freed���ڳ�ʼ��");
	Freed_Init(1);
	stopThread = 1;
}

void CGPothkaMainWinDlg::Onbtnkeyframetimeset()
{
	// TODO:  �ڴ���������������
	mocoLog.StatusOut(Info, "���ùؼ�֡ʱ��");
	if (softLock == 1)
	{
		MessageBox("�����������״̬��");
		return;
	}

	POSITION pos = m_listKeyFrames.GetFirstSelectedItemPosition();
	keyFrameIndex = m_listKeyFrames.GetNextSelectedItem(pos);
	if (keyFrameIndex<0)
	{
		MessageBox ("����ѡ��ؼ�֡��");
		return;
	}
	else if (keyFrameIndex + currentPage*frameNumEverPage == 0)
	{
		MessageBox("��ӵ�2���ؼ�֡��ʼ����");
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
		MessageBox("�ùؼ�֡��������ʱ��С����С����ʱ�䣬���������ã�");
		GetDlgItem(IDC_btnRestorationMotion)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_btnRestorationMotion)->EnableWindow(TRUE);
	}
	pageImages();
}

//�޸Ĺؼ�֡����
void CGPothkaMainWinDlg::OnbtnkeyframeNameSet()
{
	mocoLog.StatusOut(Info, "�޸Ĺؼ�֡����");
	if (softLock == 1)
	{
		MessageBox("�����������״̬��");
		return;
	}

	POSITION pos = m_listKeyFrames.GetFirstSelectedItemPosition();
	keyFrameIndex = m_listKeyFrames.GetNextSelectedItem(pos);
	if (keyFrameIndex<0)
	{
		MessageBox("����ѡ��ؼ�֡��");
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	mocoLog.StatusOut(Info, "ǰһҳ");
	if (softLock == 1)
	{
		MessageBox("�����������״̬��");
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	mocoLog.StatusOut(Info, "��һҳ");
	if (softLock == 1)
	{
		MessageBox("�����������״̬��");
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	mocoLog.StatusOut(Info, "��ʿ��ͷ��ʼ��");
	CString pComPort="COM4";
	UINT pBautRate = CBR_38400;
	bool ini=true;
	int b = m_Lens_Fujinon.InitComPort(pComPort, pBautRate);
	ini=m_Lens_Fujinon.IsConnect();
	if (ini == false)
	{
		MessageBox("��ͷ���ڳ�ʼ��ʧ��,���������Ƿ���ȷ��");
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	mocoLog.StatusOut(Info, "���ܾ�ͷ��ʼ��");
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
		MessageBox(_T("��ʼ��ʧ��"));
	}
}


void CGPothkaMainWinDlg::OnBnClickedbtnsetswitch4()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_Lens_Fujinon.SetSwitch4Host(0xFC);
}


void CGPothkaMainWinDlg::Onbtncameracontrol()
{
	// TODO:  �ڴ���������������
	mocoLog.StatusOut(Info, "������Ʋ˵�");
	if (softLock == 1)
	{
		MessageBox("�����������״̬��");
		return;
	}

	if (m_CAppWorkPlant.isPresetCommand == true)
	{
		MessageBox("��������˶�״̬��");
		return;
	}

	CameraControlDlg Dlg;
	softControl = 1;
	Dlg.DoModal();
	softControl = 0;
}


void CGPothkaMainWinDlg::Onbtnspeedcontrol()
{
	// TODO:  �ڴ���������������
	mocoLog.StatusOut(Info, "�ٶȿ��Ʋ˵�");
	if (softLock == 1)
	{
		MessageBox("�����������״̬��");
		return;
	}

	SpeedSetDlg Dlg;
	Dlg.DoModal();
}

void CGPothkaMainWinDlg::Onbtnrolllock()
{
	mocoLog.StatusOut(Info, "Roll����");
	if (softLock == 1)
	{
		MessageBox("�����������״̬��");
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
	mocoLog.StatusOut(Info, "Roll����");
	if (softLock == 1)
	{
		MessageBox("�����������״̬��");
		return;
	}

	GetMenu()->GetSubMenu(1)->EnableMenuItem(3, MF_BYPOSITION | MF_ENABLED);
	GetMenu()->GetSubMenu(1)->EnableMenuItem(4, MF_BYPOSITION | MF_DISABLED);
	rollLock = 0;
}

void CGPothkaMainWinDlg::Onbtnrobotlock()
{
	mocoLog.StatusOut(Info, "�����˿�������");
	if (softLock == 1)
	{
		MessageBox("�����������״̬��");
		return;
	}

	GetMenu()->GetSubMenu(1)->EnableMenuItem(0, MF_BYPOSITION | MF_DISABLED);
	GetMenu()->GetSubMenu(1)->EnableMenuItem(1, MF_BYPOSITION | MF_ENABLED);

	GetMenu()->GetSubMenu(3)->EnableMenuItem(0, MF_BYPOSITION | MF_DISABLED);

	robotLock = 1;
}
void CGPothkaMainWinDlg::Onbtnrobotunlock()
{
	mocoLog.StatusOut(Info, "�����˿��ƽ���");
	if (softLock == 1)
	{
		MessageBox("�����������״̬��");
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
	mocoLog.StatusOut(Info, "��ͷ��������");
	if (softLock == 1)
	{
		MessageBox("�����������״̬��");
		return;
	}

	GetMenu()->GetSubMenu(1)->EnableMenuItem(6, MF_BYPOSITION | MF_DISABLED);
	GetMenu()->GetSubMenu(1)->EnableMenuItem(7, MF_BYPOSITION | MF_ENABLED);

	GetMenu()->GetSubMenu(3)->EnableMenuItem(0, MF_BYPOSITION | MF_DISABLED);

	cameraLock = 1;
}
void CGPothkaMainWinDlg::Onbtncameraunlock()
{
	mocoLog.StatusOut(Info, "��ͷ���ƽ���");
	if (softLock == 1)
	{
		MessageBox("�����������״̬��");
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
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	mocoLog.StatusOut(Info, "�ػ��Զ�����켣 %s", strCBText);
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
	// TODO:  �ڴ���������������
	MessageBox("�汾�޸����ڣ�2017/3/18");
}


void CGPothkaMainWinDlg::OnbtnSoftLock()
{
	// TODO:  �ڴ���������������
	mocoLog.StatusOut(Info, "�������");
	softLock = 1;
	GetMenu()->GetSubMenu(1)->EnableMenuItem(9, MF_BYPOSITION | MF_DISABLED);
	GetMenu()->GetSubMenu(1)->EnableMenuItem(10, MF_BYPOSITION | MF_ENABLED);
}


void CGPothkaMainWinDlg::OnbtnSoftUnlock()
{
	// TODO:  �ڴ���������������
	mocoLog.StatusOut(Info, "�������");
	softLock = 0;
	GetMenu()->GetSubMenu(1)->EnableMenuItem(9, MF_BYPOSITION | MF_ENABLED);
	GetMenu()->GetSubMenu(1)->EnableMenuItem(10, MF_BYPOSITION | MF_DISABLED);
}
