// MyNewDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GPothka.MainWin.h"
#include "MyNewDlg.h"
#include "afxdialogex.h"
#include "GP_Lens_Fujinon.h"




// CMyNewDlg �Ի���

IMPLEMENT_DYNAMIC(CMyNewDlg, CDialogEx)

CMyNewDlg::CMyNewDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyNewDlg::IDD, pParent)
{

}

CMyNewDlg::~CMyNewDlg()
{
}

void CMyNewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_cmbPortNum, m_cbxSerialPort);
	DDX_Control(pDX, IDC_sldFocus, m_sldFocusMotorValue);
	DDX_Control(pDX, IDC_sldZoom, m_sldZoomMotorValue);
}


BEGIN_MESSAGE_MAP(CMyNewDlg, CDialogEx)
	ON_BN_CLICKED(IDC_btnInitPort, &CMyNewDlg::OnBnClickedbtninitport)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_btnSetSwitch4, &CMyNewDlg::OnBnClickedbtnsetswitch4)
END_MESSAGE_MAP()


// CMyNewDlg ��Ϣ�������


BOOL CMyNewDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	//��ӱ��ص����ϵĴ����б�
	GetComList_Reg(&m_cbxSerialPort);
	//���������������ķ�Χ
	this->m_sldFocusMotorValue.SetRange(MIN_FOCUS, MAX_FOCUS);
	this->m_sldZoomMotorValue.SetRange(MIN_ZOOM, MAX_ZOOM);


	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}


void CMyNewDlg::OnBnClickedbtninitport()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString pComPort;
	int Index=m_cbxSerialPort.GetCurSel();
	m_cbxSerialPort.GetLBText(Index, pComPort);
	UINT pBautRate = CBR_38400;
	int b = m_Lens_Fujinon.InitComPort(pComPort, pBautRate);
	m_Lens_Fujinon.IsConnect();
	m_Lens_Fujinon.SetSwitch4Host(0xFC);

	m_Lens_Fujinon.GetLensMotorZoom(ZoomResultValue);
	m_Lens_Fujinon.GetLensMotorFocus(FocusResultValue);
	Zoom = ZoomResultValue;
	Focus = FocusResultValue;

	CString str;
	str.Format(_T("%d"), ZoomResultValue);
	GetDlgItem(IDC_editZoom)->SetWindowText(str);
	str.Format(_T("%d"), FocusResultValue);
	GetDlgItem(IDC_editFocus)->SetWindowText(str);


	this->m_sldZoomMotorValue.SetPos(ZoomResultValue);
	this->m_sldFocusMotorValue.SetPos(FocusResultValue);

}


int CMyNewDlg::GetComList_Reg(CComboBox * CCombox)
{
	HKEY hkey;
	int result;
	int i = 0;

	CString strComName;//��������
	CString strDrName;//������ϸ����

	result = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		_T("Hardware\\DeviceMap\\SerialComm"),
		NULL,
		KEY_READ,
		&hkey);

	if (ERROR_SUCCESS == result)   //   �򿪴���ע���   
	{
		TCHAR portName[0x100], commName[0x100];
		DWORD dwLong, dwSize;

		CCombox->ResetContent();
		do
		{
			dwSize = sizeof(portName) / sizeof(TCHAR);
			dwLong = dwSize;
			result = RegEnumValue(hkey, i, portName, &dwLong, NULL, NULL, (LPBYTE)commName, &dwSize);
			if (ERROR_NO_MORE_ITEMS == result)
			{
				break;   //   commName���Ǵ�������"COM2"
			}
			strComName = commName;
			strDrName = portName;

			// ��������߿�ʼ���ҵ�һ��'\\'����ȡ����ַ����ĳ���
			int len = strDrName.ReverseFind('\\');
			// ��ȡ'\\'��ߵ��ַ���
			CString strFilePath = strDrName.Left(len + 1);
			// ��ȡ'\\'�ұߵ��ַ���
			CString fileName = strDrName.Right(strDrName.GetLength() - len - 1);
			fileName = strComName + _T(": ") + fileName;
			CCombox->AddString(strComName);
			i++;
		} while (1);

		RegCloseKey(hkey);
	}
	CCombox->SetCurSel(0);
	return i;
}

void CMyNewDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CString str, str1;
	Sleep(10);
	if (pScrollBar != NULL&&nSBCode == SB_THUMBTRACK)//��ק������λ��
	{
		// ǿ��ת����CSliderCtrl
		CSliderCtrl* pSlider = (CSliderCtrl*)pScrollBar;
		int p_int1 = 0;
		switch (pSlider->GetDlgCtrlID())
		{
		case IDC_sldFocus:
			p_int1 = pSlider->GetPos();
			m_Lens_Fujinon.SetLensMotorFocus(p_int1);
			str.Format(_T("%d"), p_int1);
			GetDlgItem(IDC_editFocus)->SetWindowText(str);

			m_Lens_Fujinon.GetLensMotorFocus(FocusResultValue);
			/*str1.Format(_T("%d"), ResultValue);*/
			


			break;
		case IDC_sldZoom:
			p_int1 = pSlider->GetPos();
			m_Lens_Fujinon.SetLensMotorZoom(p_int1);
			str.Format(_T("%d"), p_int1);
			GetDlgItem(IDC_editZoom)->SetWindowText(str);

			m_Lens_Fujinon.GetLensMotorZoom(ZoomResultValue);
			//str.Format(_T("%d"), ResultValue);
			



			break;

		default:
			break;
		}

	}

	Focus = FocusResultValue;
	Zoom = ZoomResultValue;
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CMyNewDlg::OnBnClickedbtnsetswitch4()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_Lens_Fujinon.SetSwitch4Host(0xFC);
}



