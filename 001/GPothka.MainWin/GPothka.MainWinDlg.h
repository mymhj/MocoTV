
//GPothka.MainWinDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "GP_Lens_Fujinon.h"
#include "GP_Lens_Canon.h"
#include "DeckLinkAPI_h.h"
#include "Resource.h"
#include "MocoLog.h"

#define WM_REFRESH_INPUT_STREAM_DATA_MESSAGE	(WM_APP + 1)
#define WM_SELECT_VIDEO_MODE_MESSAGE			(WM_APP + 2)
#define WM_ADD_DEVICE_MESSAGE					(WM_APP + 3)
#define WM_REMOVE_DEVICE_MESSAGE				(WM_APP + 4)

class DeckLinkDevice;
class DeckLinkDeviceDiscovery;

// CGPothkaMainWinDlg 对话框
class CGPothkaMainWinDlg : public CDialogEx
{
// 构造
public:
	CGPothkaMainWinDlg(CWnd* pParent = NULL);	// 标准构造函数

	// 对话框数据
	enum { IDD = IDD_GPOTHKAMAINWIN_DIALOG };


	DeckLinkDeviceDiscovery*	m_deckLinkDiscovery;
	DeckLinkDevice*				m_selectedDevice;

	CAppWorkPlant m_CAppWorkPlant;
	CListCtrl m_listKeyFrames;
	CSliderCtrl m_sdrPartSpeed;
	CSliderCtrl m_sdrWholeSpeed;
	CComboBox m_cbxframesIndex;
	CComboBox m_cbxcameraList;
	//Logger m_logger;

	void SyncKeyFrames();

	void pageKeyFrames();
	void prePageKeyFrames();
	void nextPageKeyFrames();

	void pageImages();
	void pageImages(CString filename);
	void prePageImages();
	void nextPageImages();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedbtnStartListenServer();
	afx_msg void Onbtnnewframeindex();
	void Onbtnnewframeindex(CString dir);//导入轨迹时自动创建新轨迹
	afx_msg void Onbtndeleteframeindex();
	afx_msg void OnBnClickedbtnaddNewkeyframe();
	afx_msg void OnBnClickedbtnDeletekeyframe();
	afx_msg void OnBnClickedbtnUpdateframekeyframe();
	afx_msg void Onbtnoutputtrack();
	afx_msg void Onbtnimporttrack();
	void ImportTrack();//程序开启时自动加载上次关闭时轨迹

	afx_msg void OnBnClickedbtnSinglemotion();
	afx_msg void OnBnClickedbtnRestorationmotion();
	afx_msg void OnBnClickedbtnStartrepeatmotion();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnCbnSelchangecbxFramesIndex();
	afx_msg void OnLvnItemchangedlistkeyframes(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnBnClickedbtnSuspend();

	afx_msg LRESULT TraceLogger(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserBtnEvent(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT StickEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT LimitState(WPARAM wParam, LPARAM lParam);

	bool SaveLocusTrackData(string saveFullPath);
	CEdit m_txtWaitTime;


	CEdit m_txtLogger;
	CStatic m_picVideo;

	CEdit calX;
	CEdit calY;
	CEdit calZ;
	CEdit calPan;
	CEdit calTilt;
	CEdit calRoll;

	CEdit page;
	CEdit State;
	
	afx_msg void OnNMCustomdrawsdrwholespeed(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnCancel();
	afx_msg void OnBnClickedbtncalibration();
	

private:
	bool cmotion_control;
public:
	bool get_cmotion_end()
	{
		return m_CAppWorkPlant.cmotion_end_repeat;
	}
	void set_cmotion_end_false()
	{
		m_CAppWorkPlant.cmotion_end_repeat=false;
	}
public:
	
private:
	
	CWinThread *sendF;
	friend UINT __cdecl sendFreed(LPVOID lpParamter);

	
public:
	afx_msg void OnEnChangeEditCmotionFps();
	CEdit m_cmotion_foc;
	CEdit m_cmotion_zoo;
	afx_msg void OnBnClickedButtonGetcmotion();

	afx_msg void OnBnClickedButtoninitmotion();
	afx_msg void OnBnClickedButtonstopmotion();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedRadioCmotionGet();
	
	afx_msg void OnBnClickedbtnsendcameradata();
	afx_msg void OnBnClickedbtninitfreed();
	afx_msg void OnBnClickedbtnstopfreed();

	CMenu m_Menu;
	afx_msg void setConfig();
	afx_msg void Onbtnkeyframetimeset();
	afx_msg void OnBnClickedbtnprepage();
	afx_msg void OnBnClickedbtnnextpage();

	afx_msg void OnBnClickedbtninitportcam();//富士镜头初始化
	afx_msg void OnBnClickedbtninitportcamCanon();//佳能镜头初始化


	afx_msg void OnBnClickedbtnsetswitch4();

	afx_msg void OnbtnkeyframeNameSet();//修改关键帧名称
	
	GP_Lens_Fujinon m_Lens_Fujinon;
	GP_Lens_Canon m_Lens_Canon;

	WORD ZoomResultValue, FocusResultValue;
	CSliderCtrl m_sldFocusMotorValue;
	CSliderCtrl m_sldZoomMotorValue;

	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	CWinThread *camRert;
	friend UINT __cdecl camRepeat(LPVOID lpParamter);

	CWinThread *camSing;
	friend UINT __cdecl camSingle(LPVOID lpParamter);

	CWinThread *camCon;
	friend UINT __cdecl camControl(LPVOID lpParamter);

	CWinThread *Canon;
	friend UINT __cdecl CanonContrl(LPVOID lpParamter);

	afx_msg void Onbtndeleteframe();
	afx_msg void Onbtncameracontrol();
	afx_msg void Onbtnspeedcontrol();

	afx_msg void Onbtnrolllock();
	afx_msg void Onbtnrollunlock();

	afx_msg void Onbtnrobotlock();
	afx_msg void Onbtnrobotunlock();

	afx_msg void Onbtncameralock();
	afx_msg void Onbtncameraunlock();
	afx_msg LRESULT	 OnAddDevice(WPARAM wParam, LPARAM lParam);
	
	afx_msg void OnClose();
	afx_msg void Onbtnhelp();
	afx_msg void OnbtnSoftLock();
	afx_msg void OnbtnSoftUnlock();

	MocoLog mocoLog;
};
