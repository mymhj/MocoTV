#pragma once
#include "stdafx.h"
#include "BaseDataType.h"
#include <map>
#include "GP_Lens_Fujinon.h"
#include "DeckLinkAPI_h.h"

#define WM_USERLOGGRR WM_USER+2000
#define JOYRANGMAX 1200  //ҡ���᷶Χ���ֵ
#define JOYRANGMIN -1200 //ҡ���᷶Χ��Сֵ
#define JOYAXSISTEP 1200 //ҡ������С��������Ӧ��������С����  ///��
#define AXSTEP  0.001;


//�����ٶȻ�׼
extern float gWholeSpeed ;
//ҡ�˿���ĩ���ٶ�
extern float sWholeSpeed;
//ҡ�˿��ƹ���ٶ�
extern float sWholeSpeedE1;
//A1����С������
extern float gA1Step ;
//A2����С������
extern float gA2Step ;
//A3����С������
extern float gA3Step ;
//A4����С������
extern float gA4Step ;
//A5����С������
extern float gA5Step ;
//A6����С������
extern float gA6Step ;
//E1����С������
extern float gE1Step ;

//������������̨��ƫ����
extern double cameraX;
extern double cameraY;
extern double cameraZ;

//�Ƕ�ת����ϵ��
extern double angleToRad;

//�������������
extern double cameraLocationX;
extern double cameraLocationY;
extern double cameraLocationZ;
extern double cameraPan;
extern double cameraTilt;
extern double cameraRoll;

extern int stopThread;

//ϵͳ����״̬��
extern RunState gRunState;
//�˶�����ָ���ַ�����
extern char *gRunCommand_format_str;

extern char *gRobotLocation_format_str;

//��ǰִ�е�ָ��
extern robotRunCommand  gCurrentRunCommand;

//��ǰ������λ��
extern robotlocation gCurrentRobotLocation;

//��ĵ�ǰʱ����ַ�����ʽ��
char *NowTimeByChar();

//��־���ͺ�����
void SendLogToWindows(HWND hWnd,const char * strInfo, ...);

//��Ƶ�ɼ����豸����
extern IDeckLink* g_deckLink;

//�ַ������Һ���
int rfind(char*source ,char* match);

map<string,limits> GetLimit( string robotModel );

//�ؼ�֡����
extern CString keyframeName;

//�ؼ�֡����ʱ��
extern int keyFrameIndex;
extern float runTime;

//�ؼ�֡�ȴ�ʱ��
extern float wTime;

//�켣�ظ�ʱ���ڼ�¼��һ֮֡��������в���,������
extern double lastE1Step;
extern double lastA1Step;
extern double lastA2Step;
extern double lastA3Step;
extern double lastA4Step;
extern double lastA5Step;
extern double lastA6Step;

//��ͷ����
extern WORD Focus;
extern WORD Zoom;

//�ж�����ʱ���Ƿ������Сʱ��
extern int runTimeOK;

//�жϵ����˶�ʱ�Ƿ�ѡ����һ�ؼ�֡
extern int frameIndex;

//��ҳ��
extern int totalPage;

//��ǰҳ�� 
extern int currentPage;

//�ؼ�֡����
extern int frameNum;

//��ǰҳ�ؼ�֡����
extern int frameNumCurrentPage;

//ÿҳ�ؼ�֡��������
extern int frameNumEverPage;

//�����ж�ң�п��Ƹ����Ƿ񳬹���ֵ
extern float LocationE1;
extern float LocationA1;
extern float LocationA2;
extern float LocationA3;
extern float LocationA4;
extern float LocationA5;
extern float LocationA6;

//����ϵ��
extern float k;

//���������˵��˶�ƫ����
extern float cmd_X, cmd_Y, cmd_Z;
extern double cmd_A, cmd_B, cmd_C;

//��ͷѡ��
extern CString camera;

//������ƻ�����
extern int softControl;

//�����˶��ؼ�֡����
extern int singleIndex;

//ң�˿��ƾ�ͷ
extern int focCon;
extern int zomCon;
extern int focVal;
extern int zomVal;

//Roll��
extern int rollLock;

//��������
extern int robotLock;

//�������
extern int cameraLock;

//ͼƬ·��������
extern CString picPath;
extern CString picName;


//��¼����ֵʱ��״̬
extern int tempState;

//�жϵ�ǰ�켣�Ƿ��ǵ���켣
extern int loadIndex;

//��¼����켣ʱͼƬ�ļ���λ��
extern CString fileDirt;

//�����жϻ������Ƿ�λ
extern int restoration;

//���ڼ�¼ҡ�˰�����ֹ������ס����δ���
extern int btnPresh;

//��¼E1���������ڿ���ֹͣʱ����
extern float cmdE1;

//�������
extern int softLock;

//E1λ��
extern double kk;
