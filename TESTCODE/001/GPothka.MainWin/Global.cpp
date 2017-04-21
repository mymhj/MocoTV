#include "stdafx.h"

//整体速度基准
float gWholeSpeed=0.3;//1.0
//摇杆控制末端速度
float sWholeSpeed = 0.3;
//摇杆控制轨道速度
float sWholeSpeedE1=0.3;
//A1轴最小步长。
float gA1Step=0.1;//0.4
//A2轴最小步长。
float gA2Step=0.1;//0.2
//A3轴最小步长。
float gA3Step=0.1;//0.2
//A4轴最小步长。
float gA4Step=0.6;//0.6
//A5轴最小步长。
float gA5Step=0.6;//0.6
//A6轴最小步长。
float gA6Step=0.6;//0,6
//E1轴最小步长。
float gE1Step=2.0;
////E1轴最小步长。
//float gE1Step=1.875;

//double cameraX=70;
//double cameraY=0;
//double cameraZ=-180;

double cameraX = 0;
double cameraY = 0;
double cameraZ = 0;

//角度弧度转换系数
double angleToRad=3.1415926/180.0;

//光心坐标
extern double cameraLocationX=0;
extern double cameraLocationY=0;
extern double cameraLocationZ=0;
extern double cameraPan=0;
extern double cameraTilt=0;
extern double cameraRoll=0;

//用于结束线程
int stopThread = 1;

RunState gRunState;

//当前执行的指令
robotRunCommand  gCurrentRunCommand;

//当前机器人位置
robotlocation gCurrentRobotLocation;

//发送数据包数量
long gSendDataPackageCount;

//接受数据包数量
long gReceivedDataPackageCount;

//采集卡添加设备
IDeckLink* g_deckLink = NULL;

//记录当前发送到了第几帧。
int gPresetDataSendPostioin;

//关键帧名称
CString keyframeName="";

//用于设置关键帧运行时间
int keyFrameIndex = 0;
float runTime = 0;

//关键帧等待时间
float wTime=0;

//轨迹重复时用于记录上一帧之间各轴运行步长,带符号
extern double lastE1Step=0;
extern double lastA1Step=0;
extern double lastA2Step=0;
extern double lastA3Step=0;
extern double lastA4Step=0;
extern double lastA5Step=0;
extern double lastA6Step=0;

//判断设置时间是否大于最小时间
int runTimeOK = 0;

//判断单步运动时是否选择下一关键帧
int frameIndex=0;

//镜头数据
WORD Focus =0;
WORD Zoom = 0;

//总页数
int totalPage=0;

//当前页数 
int currentPage=0;

//关键帧数量
int frameNum=0;

//当前页关键帧数量
int frameNumCurrentPage=0;

//每页关键帧限制数量
int frameNumEverPage=20; 

//用于判断遥感控制各轴是否超过阈值
float LocationE1=0;
float LocationA1=0;
float LocationA2=0;
float LocationA3=0;
float LocationA4=0;
float LocationA5=0;
float LocationA6=0;

//减速系数
float k = 1;

//发给机器人的运动偏移量
float cmd_X = 0;
float cmd_Y = 0;
float cmd_Z = 0;
double cmd_A = 0;
double cmd_B = 0;
double cmd_C = 0;

//镜头选择
CString camera = "";

//软件控制机器人
int softControl = 0;

//单步运动关键帧索引
int singleIndex = 0;

//遥杆控制镜头
int focCon = 0;
int zomCon = 0;
int focVal = 0;
int zomVal = 0;

//Roll锁
int rollLock = 0;

//机器人锁
int robotLock=0;

//摄像机锁
extern int cameraLock=0;

//图片路径和名称
extern CString picPath="";
extern CString picName="";

//记录超阈值时的状态
extern int tempState=0;

//判断当前轨迹是否是导入轨迹
extern int loadIndex=0;

//记录导入轨迹时图片文件夹位置
extern CString fileDirt="";

//用于判断机器人是否复位
extern int restoration=0;

//用于记录摇杆按键防止按键按住，多次触发
extern int btnPresh=0;

//记录E1步长，用于控制停止时缓冲
extern float cmdE1=0;

//软件锁定
extern int softLock=0;

//E1位置
extern double kk = -1305.58;


////包括轨迹姿态数据
//vector<string> gLocusTrackData;

//char *gRunCommand_format_str="<Sen Type=\"KMC\" xmlns:xsi=\"http:\/\/www.w3.org/2001/XMLSchema-instance\" xsi:noNamespaceSchemaLocation=\"commanddoc.xsd\">\
//							 \n<Dat TaskType=\"b\">\
//							 \n<EStr>successfully!</EStr>\
//							 \n<RKorr X=\"%.4f\" Y=\"%.4f\" Z=\"%.4f\" A=\"%.4f\" B=\"%.4f\" C=\"%.4f\" />\
//							 \n<AKorr A1=\"%.4f\" A2=\"%.4f\" A3=\"%.4f\" A4=\"%.4f\" A5=\"%.4f\" A6=\"%.4f\" />\
//							 \n<EKorr E1=\"%.4f\" E2=\"%.4f\" />\
//							 \n<DiO>%u</DiO>\
//							 \n<IPOC>%u<\/IPOC>\
//							 \n</Dat>\
//							 \n</Sen>";

char* gRunCommand_format_str = "<Sen Type=\"ImFree\">\
							 \r\n<EStr>successfully!</EStr>\
							 \r\n<RKorr X=\"%.4f\" Y=\"%.4f\" Z=\"%.4f\" A=\"%.4f\" B=\"%.4f\" C=\"%.4f\"/>\r\n<AKorr A1=\"%.4f\" A2=\"%.4f\" A3=\"%.4f\" A4=\"%.4f\" A5=\"%.4f\" A6=\"%.4f\"/>\r\n<EKorr E1=\"%.4f\"/>\
							 \r\n<IPOC>%s</IPOC>\
							 \r\n</Sen>";


char *gRobotLocation_format_str="<Rob Type=\"KUKA\">\
								\r\n<RIst X=\"%f\" Y=\"%f\" Z=\"%f\" A=\"%f\" B=\"%f\" C=\"%f\" />\
								\r\n<RSol X=\"%f\" Y=\"%f\" Z=\"%f\" A=\"%f\" B=\"%f\" C=\"%f\" />\
								\r\n<AIPos A1=\"%f\" A2=\"%f\" A3=\"%f\" A4=\"%f\" A5=\"%f\" A6=\"%f\" />\
								\r\n<ASPos A1=\"%f\" A2=\"%f\" A3=\"%f\" A4=\"%f\" A5=\"%f\" A6=\"%f\" />\
								\r\n<EIPos E1=\"%f\" E2=\"%f\" E3=\"%f\" E4=\"%f\" E5=\"%f\" E6=\"%f\" />\
								\r\n<ESPos E1=\"%f\" E2=\"%f\" E3=\"%f\" E4=\"%f\" E5=\"%f\" E6=\"%f\" />\
								\r\n<Delay D=\"%u\" />\
								\r\n<IPOC>%u<IPOC>\
								\r\n</Rob>";

//char *gRobotLocation_format_str="<Rob Type=\"KUKA\">\
								\n<RIst X=\"%f\" Y=\"%f\" Z=\"%f\" A=\"%f\" B=\"%f\" C=\"%f\"/>\
								\n<AIPos A1=\"%f\" A2=\"%f\" A3=\"%f\" A4=\"%f\" A5=\"%f\" A6=\"%f\" />\
								\n<EIPos E1=\"%f\" E2=\"%f\" E3=\"%f\" E4=\"%f\" E5=\"%f\" E6=\"%f\"/>\
								\n<Delay D=\"%u\" />\
								\n<DiL>%f</DiL>\
								\n<Digout o1=\"%u\" o2=\"%u\" o3=\"%u\" />\
								\n<ST_Source>%f</ST_Source>\
								\n<FTC Fx=\"%f\" Fy=\"%f\" Fz=\"%f\" Tx=\"%f\" Ty=\"%f\" Tz=\"%f\" State=\"%u\" />\
								\n<IPOC>%u</IPOC>\
								\n</Rob>";



//char* ConvertRobotLocationTochar(robotlocation &rbLocation){
//
//	char * str="<Rob Type=\"KUKA\">\
//			   \n<RIst X=\"1458.8766\" Y=\"311.3011\" Z=\"1274.4641\" A=\"-175.6956\" B=\"0.7502\" C=\"2.6071\"/>\
//			   \n<IPOC>4211158252</IPOC>\
//			   \n</Rob>";
//	char * format_str="<Rob Type=\"KUKA\">\
//					  \n<RIst X=\"%f\" Y=\"%f\" Z=\"%f\" A=\"%f\" B=\"%f\" C=\"%f\"/>\
//					  \n<IPOC>%u</IPOC>\
//					  \n</Rob>";
//	char*  buff11=(char*)malloc(1000);
//	sprintf_s(buff11,500,format_str,rbLocation.RIstX,struct1.RIstY,struct1.RIstZ,struct1.RIstA,struct1.RIstB,struct1.RIstC,struct1.IPOC);
//	return NULL;
//}

//获得当前时间的字符串格式；"%02d:%02d:%02d"
char *NowTimeByChar(){
	time_t curTime;
	tm pTimeInfo;
	time(&curTime);
	localtime_s(&pTimeInfo,&curTime);
	char temp[1024] = {0};
	sprintf_s(temp, "%02d:%02d:%02d", pTimeInfo.tm_hour, pTimeInfo.tm_min, pTimeInfo.tm_sec);
	char * pTemp = temp;
	return pTemp; 
}

//通过消息给windows窗体发送日志信息
void SendLogToWindows(HWND hWnd,const char * strInfo, ...){

	if(!strInfo)
		return;
	char prefix[1024]={0};
	char pTemp[1024] = {0};
	strcpy_s(prefix, NowTimeByChar());
	//日志前缀
	strcat_s(prefix," Info: ");
	va_list arg_ptr = NULL;
	va_start(arg_ptr, strInfo);
	vsprintf_s(pTemp,sizeof(pTemp), strInfo, arg_ptr);
	strcat_s(prefix, pTemp);
	va_end(arg_ptr);
	::SendMessage(hWnd,WM_USERLOGGRR,0,(LPARAM)&prefix); //同步
	arg_ptr = NULL;
}

int rfind(char*source ,char* match){ 
	int slen,mlen,i;    
	char *p,*q;    
	p =source;    
	q=match;    slen =strlen(source);  
	mlen =strlen(match);    
	i= slen -mlen;    for(;i>=0;i--)    
	{       
		if(strncmp(p+i,q,mlen)==0)      
		{            return i;        } 

	}    return -1;
}

map<string, limits> GetLimit( string robotModel)
{
	map<string, limits>  rLimit;
	if( robotModel == "Kuka16") 
	{
		limits jLimit;
		//jLimit.upLimit = 4.0;
		//jLimit.downLimit = 0.0;
		jLimit.upLimit = 3990.0;
		jLimit.downLimit = -3990.0;
		rLimit.insert( std::pair<string,limits>("E1", jLimit));

		jLimit.upLimit = 190.0;
		jLimit.downLimit = -170.0;
		rLimit.insert( std::pair<string,limits>("A1", jLimit));

		//jLimit.uplimit=0.0;
		jLimit.upLimit = 20.0;
		jLimit.downLimit = -120.0;
		rLimit.insert( std::pair<string,limits>("A2", jLimit));

		jLimit.upLimit = 140;
		jLimit.downLimit = -105.0;
		rLimit.insert( std::pair<string,limits>("A3", jLimit));

		jLimit.upLimit = 335.0;
		jLimit.downLimit = -335.0;
		rLimit.insert( std::pair<string,limits>("A4", jLimit));

		jLimit.upLimit = 85.0;
		jLimit.downLimit = -105.0;
		rLimit.insert( std::pair<string,limits>("A5", jLimit));

		jLimit.upLimit = 100.0;
		jLimit.downLimit = -80.0;
		rLimit.insert( std::pair<string,limits>("A6", jLimit));
	}
	else if( robotModel == "Kuka150" )
	{
		limits jLimit;
		jLimit.upLimit = 6000.0;
		jLimit.downLimit = -100.0;
		rLimit.insert( std::pair<string,limits>("E1", jLimit));

		jLimit.upLimit = 180.0;
		jLimit.downLimit = -180.0;
		rLimit.insert( std::pair<string,limits>("A1", jLimit));

		jLimit.upLimit = -10.0;
		jLimit.downLimit = -135.0;
		rLimit.insert( std::pair<string,limits>("A2", jLimit));

		jLimit.upLimit = 150;
		jLimit.downLimit = -115.0;
		rLimit.insert( std::pair<string,limits>("A3", jLimit));

		jLimit.upLimit = 345.0;
		jLimit.downLimit = -345.0;
		rLimit.insert( std::pair<string,limits>("A4", jLimit));

		jLimit.upLimit = 120.0;
		jLimit.downLimit = -120.0;
		rLimit.insert( std::pair<string,limits>("A5", jLimit));

		jLimit.upLimit = 345.0;
		jLimit.downLimit = -345.0;
		rLimit.insert( std::pair<string,limits>("A6", jLimit));
	}
	return rLimit;
}

