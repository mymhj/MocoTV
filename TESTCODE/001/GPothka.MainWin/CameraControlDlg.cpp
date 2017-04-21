// CameraControlDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GPothka.MainWin.h"
#include "CameraControlDlg.h"
#include "afxdialogex.h"
#include "GPothka.MainWinDlg.h"


// CameraControlDlg 对话框

IMPLEMENT_DYNAMIC(CameraControlDlg, CDialogEx)

CameraControlDlg::CameraControlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CameraControlDlg::IDD, pParent)
{

}

CameraControlDlg::~CameraControlDlg()
{
}

void CameraControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_sldFoc, sldFoc);
	DDX_Control(pDX, IDC_sldZom, sldZom);
}


BEGIN_MESSAGE_MAP(CameraControlDlg, CDialogEx)
	ON_BN_CLICKED(IDC_btnGetControl, &CameraControlDlg::OnBnClickedbtngetcontrol)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


BOOL CameraControlDlg::OnInitDialog()
{
	// 将“关于...”菜单项添加到系统菜单中。
	CDialogEx::OnInitDialog();

	if (camera == "Fujinon")
	{
		this->sldFoc.SetRange(MIN_FOCUS, MAX_FOCUS);
		this->sldZom.SetRange(MIN_ZOOM, MAX_ZOOM);
		GetDlgItem(IDC_sldFoc)->EnableWindow(TRUE);
		GetDlgItem(IDC_sldZom)->EnableWindow(TRUE);
		GetDlgItem(IDC_focEdit)->EnableWindow(TRUE);
		GetDlgItem(IDC_zomEdit)->EnableWindow(TRUE);
	}
	if (camera == "Canon")
	{
		this->sldFoc.SetRange(0, 60000);
		this->sldZom.SetRange(0, 60000);
		GetDlgItem(IDC_sldFoc)->EnableWindow(TRUE);
		GetDlgItem(IDC_sldZom)->EnableWindow(TRUE);
		GetDlgItem(IDC_focEdit)->EnableWindow(TRUE);
		GetDlgItem(IDC_zomEdit)->EnableWindow(TRUE);
	}

	if (rollLock == 1)
	{
		GetDlgItem(IDC_btnPanPlus)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnPanMinus)->EnableWindow(FALSE);
		
		GetDlgItem(IDC_btnRollPlus)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnRollMinus)->EnableWindow(FALSE);
	}
	else if (rollLock == 0)
	{
		GetDlgItem(IDC_btnPanPlus)->EnableWindow(TRUE);
		GetDlgItem(IDC_btnPanMinus)->EnableWindow(TRUE);

		GetDlgItem(IDC_btnRollPlus)->EnableWindow(TRUE);
		GetDlgItem(IDC_btnRollMinus)->EnableWindow(TRUE);
	}

	OnBnClickedbtngetcontrol();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// CameraControlDlg 消息处理程序


void CameraControlDlg::OnBnClickedbtngetcontrol()
{
	// TODO:  在此添加控件通知处理程序代码
	HWND hWnd = ::FindWindow(NULL, _T("MOCO-TV"));      //得到对话框的句柄
	CGPothkaMainWinDlg* pWnd = (CGPothkaMainWinDlg*)CGPothkaMainWinDlg::FromHandle(hWnd); //由句柄得到对话框的对象指针
	
	if (camera == "Fujinon")
	{
		pWnd->m_Lens_Fujinon.SetSwitch4Host(0xFC);

		WORD FocVal, ZomVal;
		pWnd->m_Lens_Fujinon.GetLensMotorFocus(FocVal);
		pWnd->m_Lens_Fujinon.GetLensMotorZoom(ZomVal);

		CString str;
		str.Format(_T("%d"), FocVal);
		GetDlgItem(IDC_focEdit)->SetWindowText(str);
		str.Format(_T("%d"), ZomVal);
		GetDlgItem(IDC_zomEdit)->SetWindowText(str);

		this->sldFoc.SetPos(FocVal);
		this->sldZom.SetPos(ZomVal);
	}
	else if (camera == "Canon")
	{
		WORD FocVal, ZomVal;
		
		FocVal = Focus;
		ZomVal = Zoom;

		CString str;
		str.Format(_T("%d"), FocVal);
		GetDlgItem(IDC_focEdit)->SetWindowText(str);
		str.Format(_T("%d"), ZomVal);
		GetDlgItem(IDC_zomEdit)->SetWindowText(str);

		this->sldFoc.SetPos(FocVal);
		this->sldZom.SetPos(ZomVal);
	}

}

BOOL CameraControlDlg::PreTranslateMessage(MSG* pMsg)
{
	HWND hWnd = ::FindWindow(NULL, _T("MOCO-TV"));      //得到对话框的句柄
	CGPothkaMainWinDlg* pWnd = (CGPothkaMainWinDlg*)CGPothkaMainWinDlg::FromHandle(hWnd); //由句柄得到对话框的对象指针

	float temp;

	if (pMsg->message == WM_LBUTTONDOWN)
	{
		if (pMsg->hwnd == GetDlgItem(IDC_btnXPlus)->m_hWnd)
		{
			//X+按钮按下
			softControl = 1;
			temp = gE1Step*sWholeSpeed*0.2;
			pWnd->m_CAppWorkPlant.m_RockerModule.caculate(temp, 0, 0, 0, 0, 0);
			gCurrentRunCommand.X = cmd_X;
			gCurrentRunCommand.Y = cmd_Y;
			gCurrentRunCommand.Z = cmd_Z;
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_btnXMinus)->m_hWnd)
		{
			//X-按钮按下
			softControl = 1;
			temp = -1*gE1Step*sWholeSpeed*0.2;
			pWnd->m_CAppWorkPlant.m_RockerModule.caculate(temp, 0, 0, 0, 0, 0);
			gCurrentRunCommand.X = cmd_X;
			gCurrentRunCommand.Y = cmd_Y;
			gCurrentRunCommand.Z = cmd_Z;

		}
		else if (pMsg->hwnd == GetDlgItem(IDC_btnYPlus)->m_hWnd)
		{
			//Y+按钮按下
			softControl = 1;
			temp = gE1Step*sWholeSpeed*0.2;
			pWnd->m_CAppWorkPlant.m_RockerModule.caculate(0, temp, 0, 0, 0, 0);
			gCurrentRunCommand.X = cmd_X;
			gCurrentRunCommand.Y = cmd_Y;
			gCurrentRunCommand.Z = cmd_Z;

		}
		else if (pMsg->hwnd == GetDlgItem(IDC_btnYMinus)->m_hWnd)
		{
			//Y-按钮按下
			softControl = 1;
			temp = -1 * gE1Step*sWholeSpeed*0.2;
			pWnd->m_CAppWorkPlant.m_RockerModule.caculate(0, temp, 0, 0, 0, 0);
			gCurrentRunCommand.X = cmd_X;
			gCurrentRunCommand.Y = cmd_Y;
			gCurrentRunCommand.Z = cmd_Z;

		}
		else if (pMsg->hwnd == GetDlgItem(IDC_btnZPlus)->m_hWnd)
		{
			//Z+按钮按下
			softControl = 1;
			temp = gE1Step*sWholeSpeed*0.2;
			pWnd->m_CAppWorkPlant.m_RockerModule.caculate(0, 0, temp, 0, 0, 0);
			gCurrentRunCommand.X = cmd_X;
			gCurrentRunCommand.Y = cmd_Y;
			gCurrentRunCommand.Z = cmd_Z;
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_btnZMinus)->m_hWnd)
		{
			//Z-按钮按下
			softControl = 1;
			temp = -1 * gE1Step*sWholeSpeed*0.2;
			pWnd->m_CAppWorkPlant.m_RockerModule.caculate(0, 0, temp, 0, 0, 0);
			gCurrentRunCommand.X = cmd_X;
			gCurrentRunCommand.Y = cmd_Y;
			gCurrentRunCommand.Z = cmd_Z;
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_btnPanPlus)->m_hWnd)
		{
			//Pan+按钮按下
			softControl = 1;
			temp = gE1Step*sWholeSpeed*0.05;
			pWnd->m_CAppWorkPlant.m_RockerModule.caculate(0, 0, 0, temp, 0, 0);
			gCurrentRunCommand.A = cmd_A;
			Sleep(12);
			gCurrentRunCommand.B = cmd_B;
			Sleep(12);
			gCurrentRunCommand.C = cmd_C;
			Sleep(12);
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_btnPanMinus)->m_hWnd)
		{
			//Pan-按钮按下
			softControl = 1;
			temp = -1 * gE1Step*sWholeSpeed*0.05;
			pWnd->m_CAppWorkPlant.m_RockerModule.caculate(0, 0, 0, temp, 0, 0);
			gCurrentRunCommand.A = cmd_A;
			gCurrentRunCommand.B = cmd_B;
			gCurrentRunCommand.C = cmd_C;

		}
		else if (pMsg->hwnd == GetDlgItem(IDC_btnTiltPlus)->m_hWnd)
		{
			//Tilt+按钮按下
			softControl = 1;
			temp = gE1Step*sWholeSpeed*0.05;
			pWnd->m_CAppWorkPlant.m_RockerModule.caculate(0, 0, 0, 0, temp, 0);
			gCurrentRunCommand.A = cmd_A;
			gCurrentRunCommand.B = cmd_B;
			gCurrentRunCommand.C = cmd_C;


		}
		else if (pMsg->hwnd == GetDlgItem(IDC_btnTiltMinus)->m_hWnd)
		{
			//Tilt-按钮按下
			softControl = 1;
			temp = -1 * gE1Step*sWholeSpeed*0.05;
			pWnd->m_CAppWorkPlant.m_RockerModule.caculate(0, 0, 0, 0, temp, 0);
			gCurrentRunCommand.A = cmd_A;
			gCurrentRunCommand.B = cmd_B;
			gCurrentRunCommand.C = cmd_C;

		}
		else if (pMsg->hwnd == GetDlgItem(IDC_btnRollPlus)->m_hWnd)
		{
			//Roll+按钮按下
			softControl = 1;
			temp = gE1Step*sWholeSpeed*0.05;
			pWnd->m_CAppWorkPlant.m_RockerModule.caculate(0, 0, 0, 0, 0, temp);
			gCurrentRunCommand.A = cmd_A;
			gCurrentRunCommand.B = cmd_B;
			gCurrentRunCommand.C = cmd_C;

		}
		else if (pMsg->hwnd == GetDlgItem(IDC_btnRollMinus)->m_hWnd)
		{
			//Roll-按钮按下
			softControl = 1;
			temp = -1 * gE1Step*sWholeSpeed*0.05;
			pWnd->m_CAppWorkPlant.m_RockerModule.caculate(0, 0, 0, 0, 0, temp);
			gCurrentRunCommand.A = cmd_A;
			gCurrentRunCommand.B = cmd_B;
			gCurrentRunCommand.C = cmd_C;

		}
		else if (pMsg->hwnd == GetDlgItem(IDC_btnA1Plus)->m_hWnd)
		{
			//A1+按钮按下
			softControl = 1;
			gCurrentRunCommand.A1 = 0.02;
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_btnA1Minus)->m_hWnd)
		{
			//A1-按钮按下
			softControl = 1;
			gCurrentRunCommand.A1 = -0.02;
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_btnA2Plus)->m_hWnd)
		{
			//A2+按钮按下
			softControl = 1;
			gCurrentRunCommand.A2 = 0.02;
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_btnA2Minus)->m_hWnd)
		{
			//A2-按钮按下
			softControl = 1;
			gCurrentRunCommand.A2 = -0.02;
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_btnA3Plus)->m_hWnd)
		{
			//A3+按钮按下
			softControl = 1;
			gCurrentRunCommand.A3 = 0.02;
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_btnA3Minus)->m_hWnd)
		{
			//A3-按钮按下
			softControl = 1;
			gCurrentRunCommand.A3 = -0.02;
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_btnA4Plus)->m_hWnd)
		{
			//A4+按钮按下
			softControl = 1;
			gCurrentRunCommand.A4 = 0.02;
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_btnA4Minus)->m_hWnd)
		{
			//A4-按钮按下
			softControl = 1;
			gCurrentRunCommand.A4 = -0.02;
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_btnA5Plus)->m_hWnd)
		{
			//A5+按钮按下
			softControl = 1;
			gCurrentRunCommand.A5 = 0.02;
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_btnA5Minus)->m_hWnd)
		{
			//A5-按钮按下
			softControl = 1;
			gCurrentRunCommand.A5 = -0.02;
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_btnA6Plus)->m_hWnd)
		{
			//A6+按钮按下
			softControl = 1;
			gCurrentRunCommand.A6 = 0.02;
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_btnA6Minus)->m_hWnd)
		{
			//A6-按钮按下
			softControl = 1;
			gCurrentRunCommand.A6 = -0.02;
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_btnE1Plus)->m_hWnd)
		{
			//轨道+按钮按下
			softControl = 1;
			temp = gE1Step*sWholeSpeedE1;
			
			for (float i = 0; i < temp;)
			{
				gCurrentRunCommand.E1 = i;
				Sleep(20);
				i = i + 0.02;
			}
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_btnE1Minus)->m_hWnd)
		{
			//轨道-按钮按下
			softControl = 1;
			temp = -1 * gE1Step*sWholeSpeedE1;
			for (float i = 0; i > temp;)
			{
				gCurrentRunCommand.E1 = i;
				Sleep(20);
				i = i - 0.02;
			}
			

		}
	}
	else if (pMsg->message == WM_LBUTTONUP)
	{
		if (pMsg->hwnd == GetDlgItem(IDC_btnXPlus)->m_hWnd || pMsg->hwnd == GetDlgItem(IDC_btnXMinus)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_btnYPlus)->m_hWnd || pMsg->hwnd == GetDlgItem(IDC_btnYMinus)->m_hWnd || 
			pMsg->hwnd == GetDlgItem(IDC_btnZPlus)->m_hWnd || pMsg->hwnd == GetDlgItem(IDC_btnZMinus)->m_hWnd || 
			pMsg->hwnd == GetDlgItem(IDC_btnPanPlus)->m_hWnd || pMsg->hwnd == GetDlgItem(IDC_btnPanMinus)->m_hWnd || 
			pMsg->hwnd == GetDlgItem(IDC_btnTiltPlus)->m_hWnd || pMsg->hwnd == GetDlgItem(IDC_btnTiltMinus)->m_hWnd || 
			pMsg->hwnd == GetDlgItem(IDC_btnRollPlus)->m_hWnd || pMsg->hwnd == GetDlgItem(IDC_btnRollMinus)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_btnE1Plus)->m_hWnd || pMsg->hwnd == GetDlgItem(IDC_btnE1Minus)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_btnA1Plus)->m_hWnd || pMsg->hwnd == GetDlgItem(IDC_btnA1Minus)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_btnA2Plus)->m_hWnd || pMsg->hwnd == GetDlgItem(IDC_btnA2Minus)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_btnA3Plus)->m_hWnd || pMsg->hwnd == GetDlgItem(IDC_btnA3Minus)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_btnA4Plus)->m_hWnd || pMsg->hwnd == GetDlgItem(IDC_btnA4Minus)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_btnA5Plus)->m_hWnd || pMsg->hwnd == GetDlgItem(IDC_btnA5Minus)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_btnA6Plus)->m_hWnd || pMsg->hwnd == GetDlgItem(IDC_btnA6Minus)->m_hWnd )

		{

			if (pMsg->hwnd == GetDlgItem(IDC_btnE1Plus)->m_hWnd)
			{
				while (gCurrentRunCommand.E1>0)
				{
					gCurrentRunCommand.E1 = gCurrentRunCommand.E1 - 0.02;
					if (gCurrentRunCommand.E1 < 0)
					{
						gCurrentRunCommand.E1 = 0;
					}
					Sleep(15);
				}

			}
			else if (pMsg->hwnd == GetDlgItem(IDC_btnE1Minus)->m_hWnd)
			{
				while (gCurrentRunCommand.E1<0)
				{
					gCurrentRunCommand.E1 = gCurrentRunCommand.E1 + 0.02;
					if (gCurrentRunCommand.E1 > 0)
					{
						gCurrentRunCommand.E1 = 0;
					}
					Sleep(15);
				}
			}

			softControl = 0;

			memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));

		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}



void CameraControlDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	WORD ZoomValue=0;
	WORD FocusValue=0;

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
	HWND hWnd = ::FindWindow(NULL, _T("MOCO-TV"));      //得到对话框的句柄
	CGPothkaMainWinDlg* pWnd = (CGPothkaMainWinDlg*)CGPothkaMainWinDlg::FromHandle(hWnd); //由句柄得到对话框的对象指针
	CString str, str1;
	Sleep(10);
	if (pScrollBar != NULL&&nSBCode == SB_THUMBTRACK)//拖拽的最终位置
	{
		// 强制转换成CSliderCtrl
		CSliderCtrl* pSlider = (CSliderCtrl*)pScrollBar;
		int p_int1 = 0;
		switch (pSlider->GetDlgCtrlID())
		{
		case IDC_sldFoc:
			p_int1 = pSlider->GetPos();
			
			if (camera == "Fujinon")
			{
				pWnd->m_Lens_Fujinon.SetLensMotorFocus(p_int1);
				Focus = p_int1;
			}			
			else if (camera == "Canon")
			{
				Focus = p_int1;
			}
			str.Format(_T("%d"), p_int1);
			GetDlgItem(IDC_focEdit)->SetWindowText(str);


			break;
		case IDC_sldZom:
			p_int1 = pSlider->GetPos();

			if (camera == "Fujinon")
			{
				pWnd->m_Lens_Fujinon.SetLensMotorZoom(p_int1);
				Zoom = p_int1;
			}
			else if (camera == "Canon")
			{
				Zoom = p_int1;
			}
			str.Format(_T("%d"), p_int1);
			GetDlgItem(IDC_zomEdit)->SetWindowText(str);


			break;

		default:
			break;
		}

	}

	if (camera == "Fujinon")
	{
		pWnd->m_Lens_Fujinon.GetLensMotorFocus(FocusValue);
		pWnd->m_Lens_Fujinon.GetLensMotorZoom(ZoomValue);
		Focus = FocusValue;
		Zoom = ZoomValue;
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
