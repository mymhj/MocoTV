#include "stdafx.h"

//�����ٶȻ�׼
float gWholeSpeed=0.3;//1.0
//ҡ�˿���ĩ���ٶ�
float sWholeSpeed = 0.3;
//ҡ�˿��ƹ���ٶ�
float sWholeSpeedE1=0.3;
//A1����С������
float gA1Step=0.1;//0.4
//A2����С������
float gA2Step=0.1;//0.2
//A3����С������
float gA3Step=0.1;//0.2
//A4����С������
float gA4Step=0.6;//0.6
//A5����С������
float gA5Step=0.6;//0.6
//A6����С������
float gA6Step=0.6;//0,6
//E1����С������
float gE1Step=2.0;
////E1����С������
//float gE1Step=1.875;

//double cameraX=70;
//double cameraY=0;
//double cameraZ=-180;

double cameraX = 0;
double cameraY = 0;
double cameraZ = 0;

//�ǶȻ���ת��ϵ��
double angleToRad=3.1415926/180.0;

//��������
extern double cameraLocationX=0;
extern double cameraLocationY=0;
extern double cameraLocationZ=0;
extern double cameraPan=0;
extern double cameraTilt=0;
extern double cameraRoll=0;

//���ڽ����߳�
int stopThread = 1;

RunState gRunState;

//��ǰִ�е�ָ��
robotRunCommand  gCurrentRunCommand;

//��ǰ������λ��
robotlocation gCurrentRobotLocation;

//�������ݰ�����
long gSendDataPackageCount;

//�������ݰ�����
long gReceivedDataPackageCount;

//�ɼ�������豸
IDeckLink* g_deckLink = NULL;

//��¼��ǰ���͵��˵ڼ�֡��
int gPresetDataSendPostioin;

//�ؼ�֡����
CString keyframeName="";

//�������ùؼ�֡����ʱ��
int keyFrameIndex = 0;
float runTime = 0;

//�ؼ�֡�ȴ�ʱ��
float wTime=0;

//�켣�ظ�ʱ���ڼ�¼��һ֮֡��������в���,������
extern double lastE1Step=0;
extern double lastA1Step=0;
extern double lastA2Step=0;
extern double lastA3Step=0;
extern double lastA4Step=0;
extern double lastA5Step=0;
extern double lastA6Step=0;

//�ж�����ʱ���Ƿ������Сʱ��
int runTimeOK = 0;

//�жϵ����˶�ʱ�Ƿ�ѡ����һ�ؼ�֡
int frameIndex=0;

//��ͷ����
WORD Focus =0;
WORD Zoom = 0;

//��ҳ��
int totalPage=0;

//��ǰҳ�� 
int currentPage=0;

//�ؼ�֡����
int frameNum=0;

//��ǰҳ�ؼ�֡����
int frameNumCurrentPage=0;

//ÿҳ�ؼ�֡��������
int frameNumEverPage=20; 

//�����ж�ң�п��Ƹ����Ƿ񳬹���ֵ
float LocationE1=0;
float LocationA1=0;
float LocationA2=0;
float LocationA3=0;
float LocationA4=0;
float LocationA5=0;
float LocationA6=0;

//����ϵ��
float k = 1;

//���������˵��˶�ƫ����
float cmd_X = 0;
float cmd_Y = 0;
float cmd_Z = 0;
double cmd_A = 0;
double cmd_B = 0;
double cmd_C = 0;

//��ͷѡ��
CString camera = "";

//������ƻ�����
int softControl = 0;

//�����˶��ؼ�֡����
int singleIndex = 0;

//ң�˿��ƾ�ͷ
int focCon = 0;
int zomCon = 0;
int focVal = 0;
int zomVal = 0;

//Roll��
int rollLock = 0;

//��������
int robotLock=0;

//�������
extern int cameraLock=0;

//ͼƬ·��������
extern CString picPath="";
extern CString picName="";

//��¼����ֵʱ��״̬
extern int tempState=0;

//�жϵ�ǰ�켣�Ƿ��ǵ���켣
extern int loadIndex=0;

//��¼����켣ʱͼƬ�ļ���λ��
extern CString fileDirt="";

//�����жϻ������Ƿ�λ
extern int restoration=0;

//���ڼ�¼ҡ�˰�����ֹ������ס����δ���
extern int btnPresh=0;

//��¼E1���������ڿ���ֹͣʱ����
extern float cmdE1=0;

//�������
extern int softLock=0;

//E1λ��
extern double kk = -1305.58;


////�����켣��̬����
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

//��õ�ǰʱ����ַ�����ʽ��"%02d:%02d:%02d"
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

//ͨ����Ϣ��windows���巢����־��Ϣ
void SendLogToWindows(HWND hWnd,const char * strInfo, ...){

	if(!strInfo)
		return;
	char prefix[1024]={0};
	char pTemp[1024] = {0};
	strcpy_s(prefix, NowTimeByChar());
	//��־ǰ׺
	strcat_s(prefix," Info: ");
	va_list arg_ptr = NULL;
	va_start(arg_ptr, strInfo);
	vsprintf_s(pTemp,sizeof(pTemp), strInfo, arg_ptr);
	strcat_s(prefix, pTemp);
	va_end(arg_ptr);
	::SendMessage(hWnd,WM_USERLOGGRR,0,(LPARAM)&prefix); //ͬ��
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

