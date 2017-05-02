#pragma once
#include "stdafx.h"
#include "BaseDataType.h"
#include <map>
#include "GP_Lens_Fujinon.h"
#include "DeckLinkAPI_h.h"

#define WM_USERLOGGRR WM_USER+2000
#define JOYRANGMAX 1200  //摇杆轴范围最大值
#define JOYRANGMIN -1200 //摇杆轴范围最小值
#define JOYAXSISTEP 1200 //摇杆轴最小分区，对应机器人最小步长  ///。
#define AXSTEP  0.001;


//整体速度基准
extern float gWholeSpeed ;
//摇杆控制末端速度
extern float sWholeSpeed;
//摇杆控制轨道速度
extern float sWholeSpeedE1;
//A1轴最小步长。
extern float gA1Step ;
//A2轴最小步长。
extern float gA2Step ;
//A3轴最小步长。
extern float gA3Step ;
//A4轴最小步长。
extern float gA4Step ;
//A5轴最小步长。
extern float gA5Step ;
//A6轴最小步长。
extern float gA6Step ;
//E1轴最小步长。
extern float gE1Step ;

//摄像机相对于云台的偏移量
extern double cameraX;
extern double cameraY;
extern double cameraZ;

//角度转弧度系数
extern double angleToRad;

//摄像机光心坐标
extern double cameraLocationX;
extern double cameraLocationY;
extern double cameraLocationZ;
extern double cameraPan;
extern double cameraTilt;
extern double cameraRoll;

extern int stopThread;

//系统控制状态机
extern RunState gRunState;
//运动控制指令字符串。
extern char *gRunCommand_format_str;

extern char *gRobotLocation_format_str;

//当前执行的指令
extern robotRunCommand  gCurrentRunCommand;

//当前机器人位置
extern robotlocation gCurrentRobotLocation;

//获的当前时间的字符串格式。
char *NowTimeByChar();

//日志发送函数。
void SendLogToWindows(HWND hWnd,const char * strInfo, ...);

//视频采集卡设备变量
extern IDeckLink* g_deckLink;

//字符串查找函数
int rfind(char*source ,char* match);

map<string,limits> GetLimit( string robotModel );

//关键帧名称
extern CString keyframeName;

//关键帧运行时间
extern int keyFrameIndex;
extern float runTime;

//关键帧等待时机
extern float wTime;

//轨迹重复时用于记录上一帧之间各轴运行步长,带符号
extern double lastE1Step;
extern double lastA1Step;
extern double lastA2Step;
extern double lastA3Step;
extern double lastA4Step;
extern double lastA5Step;
extern double lastA6Step;

//镜头数据
extern WORD Focus;
extern WORD Zoom;

//判断设置时间是否大于最小时间
extern int runTimeOK;

//判断单步运动时是否选择下一关键帧
extern int frameIndex;

//总页数
extern int totalPage;

//当前页数 
extern int currentPage;

//关键帧总量
extern int frameNum;

//当前页关键帧数量
extern int frameNumCurrentPage;

//每页关键帧限制数量
extern int frameNumEverPage;

//用于判断遥感控制各轴是否超过阈值
extern float LocationE1;
extern float LocationA1;
extern float LocationA2;
extern float LocationA3;
extern float LocationA4;
extern float LocationA5;
extern float LocationA6;

//减速系数
extern float k;

//发给机器人的运动偏移量
extern float cmd_X, cmd_Y, cmd_Z;
extern double cmd_A, cmd_B, cmd_C;

//镜头选择
extern CString camera;

//软件控制机器人
extern int softControl;

//单步运动关键帧索引
extern int singleIndex;

//遥杆控制镜头
extern int focCon;
extern int zomCon;
extern int focVal;
extern int zomVal;

//Roll锁
extern int rollLock;

//机器人锁
extern int robotLock;

//摄像机锁
extern int cameraLock;

//图片路径和名称
extern CString picPath;
extern CString picName;


//记录超阈值时的状态
extern int tempState;

//判断当前轨迹是否是导入轨迹
extern int loadIndex;

//记录导入轨迹时图片文件夹位置
extern CString fileDirt;

//用于判断机器人是否复位
extern int restoration;

//用于记录摇杆按键防止按键按住，多次触发
extern int btnPresh;

//记录E1步长，用于控制停止时缓冲
extern float cmdE1;

//软件锁定
extern int softLock;

//E1位置
extern double kk;
