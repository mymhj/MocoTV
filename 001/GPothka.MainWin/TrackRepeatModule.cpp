#include "stdafx.h"
#include "TrackRepeatModule.h"
#include <algorithm>
#include <fstream>
#include<iomanip>
#include <vector>
#include <xfunctional>
#include <d3dx9math.h>
#include "CubicSplineInterpolation.h"
#include <sstream>


using namespace std;

map<string,string> CTrackRepeatModule::m_TrackDataPath;

//关键帧集合仓库
map<string,vector<KeyFrame>> CTrackRepeatModule::m_TrackKeyFrameDepository;

//关键帧集合在仓库中的索引
string CTrackRepeatModule::m_TrackIndex;

//当前聚焦的关键帧索引
int CTrackRepeatModule::m_keyFrameIndex;

//重复运动各帧前后修改指令数目
float CTrackRepeatModule::commandNum=20;

//关键帧集合
vector<KeyFrame> CTrackRepeatModule::m_KeyFrameColl;

//轨迹重复指令针集合
vector<char*> CTrackRepeatModule::m_TrackRepeatCommandData;

//复位指令集合
vector<char*> CTrackRepeatModule::m_RestorationCommandData;

//单步指令集合
vector<char*> CTrackRepeatModule::m_SingleStepCommandData;


CTrackRepeatModule::CTrackRepeatModule(void)
{

}

CTrackRepeatModule::~CTrackRepeatModule(void)
{

}


//获得指定轨迹索引的的关键帧集合
vector<KeyFrame> CTrackRepeatModule::GetKeyFrameCollToIndexTrack(string  indexTrack){

	map<string,vector<KeyFrame>>::iterator itor;
	itor=m_TrackKeyFrameDepository.find(indexTrack);
	if(itor != m_TrackKeyFrameDepository.end()){
		return itor->second;
	}
	else {
		//没有找到
	}
	return itor->second;
}

//创建一个新的轨迹关键帧集合
bool CTrackRepeatModule::AddNewTrackToIndexTrack(string  indexTrack){
	vector<KeyFrame> keyFrameColl;
	keyFrameColl.clear();
	m_TrackKeyFrameDepository.insert(make_pair(indexTrack,keyFrameColl));
	return true;
}

//删除指定索引的轨迹关键帧集合
bool CTrackRepeatModule::DeleteTrackToIndexTrack(string indexTrack){

	map<string,vector<KeyFrame>>::iterator itor;
	
	//itor=find_if(m_TrackKeyFrameDepository.begin,m_TrackKeyFrameDepository.end(),bind2nd(ptr_fun(search),indexTrack));
	itor=m_TrackKeyFrameDepository.find(indexTrack);
	//map<char*,vector<KeyFrame>>::const_iterator finditer=find_if

	if(itor != m_TrackKeyFrameDepository.end()){
		m_TrackKeyFrameDepository.erase(itor);
		return true;
	}
	else {
		//没有找到
		return false;
	}
}

//添加关键帧
bool CTrackRepeatModule::AddKeyFrame(robotlocation rbLocation,string  indexTrack){

	map<string,vector<KeyFrame>>::iterator itor;
	itor=m_TrackKeyFrameDepository.find(indexTrack);
	if(itor != m_TrackKeyFrameDepository.end()){
		KeyFrame keyFrame;
		keyFrame.A1=gCurrentRobotLocation.A1;
		keyFrame.A2=gCurrentRobotLocation.A2;
		keyFrame.A3=gCurrentRobotLocation.A3;
		keyFrame.A4=gCurrentRobotLocation.A4;
		keyFrame.A5=gCurrentRobotLocation.A5;
		keyFrame.A6=gCurrentRobotLocation.A6;
		keyFrame.E1=gCurrentRobotLocation.E1;
		itor->second.push_back(keyFrame);
		return true;
	}
	else {
		//没有找到
		return false;
	}
}

//添加关键帧
bool CTrackRepeatModule::AddKeyFrame(robotlocation rbLocation,string  indexTrack,int waitTime,float partSpeed){
	
	map<string,vector<KeyFrame>>::iterator itor;
	itor=m_TrackKeyFrameDepository.find(indexTrack);

	if(itor != m_TrackKeyFrameDepository.end()){
		
		KeyFrame keyFrame;
		memset(&keyFrame,0,sizeof(keyFrame));
		
		keyFrame.IPOC=rbLocation.IPOC;
		keyFrame.A1=rbLocation.A1;
		keyFrame.A2=rbLocation.A2;
		keyFrame.A3=rbLocation.A3;
		keyFrame.A4=rbLocation.A4;
		keyFrame.A5=rbLocation.A5;
		keyFrame.A6=rbLocation.A6;
		keyFrame.E1=rbLocation.E1;

		keyFrame.X = rbLocation.X;
		keyFrame.Y = rbLocation.Y;
		keyFrame.Z = rbLocation.Z;
		keyFrame.A = rbLocation.A;
		keyFrame.B = rbLocation.B;
		keyFrame.C = rbLocation.C;
	
		keyFrame.WaitTime=waitTime;
		keyFrame.PartSpeed=partSpeed;

		keyFrame.Focus = Focus;
		keyFrame.Zoom = Zoom;

		stringstream ss;
		ss << keyFrame.IPOC;
		ss >> keyFrame.name;
		
		picName.Format("%d", keyFrame.IPOC) ;
		keyFrame.name = picName;

		itor->second.push_back(keyFrame);

		return true;
	}
	else {
		//没有找到
		return false;
	}
}

//删除 指定索引的关键帧
bool CTrackRepeatModule::DeleteKeyFrame(int keyFrameIndex,string  indexTrack){
	map<string,vector<KeyFrame>>::iterator itor;
	itor=m_TrackKeyFrameDepository.find(indexTrack);
	if(itor != m_TrackKeyFrameDepository.end()){
		vector<KeyFrame>::iterator it=itor->second.begin()+keyFrameIndex;
		itor->second.erase(it);
		return true;
	}
	else {
		//没有找到
		return false;
	}
}

//更新 更新指定索引的关键帧
bool CTrackRepeatModule::UpdateKeyFrame(robotlocation rbLocation,int keyFrameIndex,string  indexTrack){
	map<string,vector<KeyFrame>>::iterator itor;
	itor=m_TrackKeyFrameDepository.find(indexTrack);
	if(itor != m_TrackKeyFrameDepository.end()){
		vector<KeyFrame> *m_KeyFrameColl=&itor->second;
		if(keyFrameIndex>=0)
		{
			KeyFrame* keyFrame=&(*m_KeyFrameColl)[keyFrameIndex];
			keyFrame->A1=gCurrentRobotLocation.A1;
			keyFrame->A2=gCurrentRobotLocation.A2;
			keyFrame->A3=gCurrentRobotLocation.A3;
			keyFrame->A4=gCurrentRobotLocation.A4;
			keyFrame->A5=gCurrentRobotLocation.A5;
			keyFrame->A6=gCurrentRobotLocation.A6;
			keyFrame->E1=gCurrentRobotLocation.E1;
			return true;
		}
		else
		{
			return false;}
	}
	else {
		//没有找到
		return false;
	}
}

//更新 更新指定索引的关键帧
bool CTrackRepeatModule::UpdateKeyFrame(robotlocation rbLocation,int keyFrameIndex,string  indexTrack,int waitTime,float partSpeed ){
	map<string,vector<KeyFrame>>::iterator itor;
	itor=m_TrackKeyFrameDepository.find(indexTrack);
	if(itor != m_TrackKeyFrameDepository.end()){
		vector<KeyFrame> *m_KeyFrameColl=&itor->second;
		if(keyFrameIndex>=0)
		{
			KeyFrame* keyFrame=&(*m_KeyFrameColl)[keyFrameIndex];
			keyFrame->A1=gCurrentRobotLocation.A1;
			keyFrame->A2=gCurrentRobotLocation.A2;
			keyFrame->A3=gCurrentRobotLocation.A3;
			keyFrame->A4=gCurrentRobotLocation.A4;
			keyFrame->A5=gCurrentRobotLocation.A5;
			keyFrame->A6=gCurrentRobotLocation.A6;
			keyFrame->E1=gCurrentRobotLocation.E1;

			keyFrame->X = gCurrentRobotLocation.X;
			keyFrame->Y = gCurrentRobotLocation.Y;
			keyFrame->Z = gCurrentRobotLocation.Z;
			keyFrame->A = gCurrentRobotLocation.A;
			keyFrame->B = gCurrentRobotLocation.B;
			keyFrame->C = gCurrentRobotLocation.C;

			keyFrame->WaitTime=waitTime;
			keyFrame->PartSpeed=partSpeed;

			keyFrame->Zoom = Zoom;
			keyFrame->Focus = Focus;

			stringstream ss;
			ss << keyFrame->IPOC;
			ss >> keyFrame->name;

			picName.Format("%d", keyFrame->IPOC);
			keyFrame->name = picName;

			return true;
		}
		else
		{
			return false;
		}
	}
	else {
		//没有找到
		return false;
	}
}

//更新运行时间
bool CTrackRepeatModule::UpdataKeyTime(int keyFrameIndex, string  indexTrack, float runTime,float waitTime){
	map<string, vector<KeyFrame>>::iterator itor;
	itor = m_TrackKeyFrameDepository.find(indexTrack);
	if (itor != m_TrackKeyFrameDepository.end()){
		vector<KeyFrame> *m_KeyFrameColl = &itor->second;
		if (keyFrameIndex >= 0)
		{
			KeyFrame* keyFrame = &(*m_KeyFrameColl)[keyFrameIndex];
			keyFrame->FrameRunTime = runTime;
			keyFrame->WaitTime = waitTime;
			if (keyFrame->FrameRunTime >= keyFrame->FrameMinTime || keyFrame->FrameRunTime == 0)
			{
				runTimeOK = 1;
			}
			else 
			{
				runTimeOK = 0;
			}

			return true;
		}
		else
		{
			return false;
		}
	}
	else {
		//没有找到
		return false;
	}
}

//更新关键帧名称
bool CTrackRepeatModule::UpdataKeyName(int keyFrameIndex, string  indexTrack, CString name){
	map<string, vector<KeyFrame>>::iterator itor;
	itor = m_TrackKeyFrameDepository.find(indexTrack);
	if (itor != m_TrackKeyFrameDepository.end()){
		vector<KeyFrame> *m_KeyFrameColl = &itor->second;
		if (keyFrameIndex >= 0)
		{
			KeyFrame* keyFrame = &(*m_KeyFrameColl)[keyFrameIndex];
	       
			keyFrame->name = name.GetBuffer(0);

			return true;
		}
		else
		{
			return false;
		}
	}
	else {
		//没有找到
		return false;
	}
}

//保存 关键帧到本地
bool CTrackRepeatModule::SaveKeyFramesToLocal(string saveFullPath,string indexTrack){
	vector<KeyFrame> KeyFrameColl;
	map<string,vector<KeyFrame>>::iterator it = this->m_TrackKeyFrameDepository.find(indexTrack);
	if(it == this->m_TrackKeyFrameDepository.end()) {
		return false;
	}
	else { //找到
		KeyFrameColl=it->second;
	}
	//ofstream outfile(saveFullPath, ios::app);
	ofstream outfile(saveFullPath, ios::out);
	if( !outfile.is_open() )
	{
		Sleep(100);
	}

	outfile<<"IPOC 局部速度 等待时间 E1 A1 A2 A3 A4 A5 A6 X Y Z A B C RunTime MinTime Zoom Focus 名称\n";
	char* txtkeyframeColl=(char*)malloc(2048);
	memset(txtkeyframeColl,0,sizeof(txtkeyframeColl));
	int keyframeSum=0;
	for(vector<KeyFrame>::iterator iter=KeyFrameColl.begin();iter!=KeyFrameColl.end();iter++){
		sprintf_s(txtkeyframeColl,2048,
			"%u %.4f %u %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %d %d %s\n",
			iter->IPOC, iter->PartSpeed, iter->WaitTime, iter->E1, iter->A1, iter->A2, iter->A3, iter->A4, iter->A5, iter->A6, iter->X, iter->Y, iter->Z, iter->A, iter->B, iter->C, iter->FrameRunTime, iter->FrameMinTime, (int)iter->Zoom, (int)iter->Focus, iter->name.c_str());
		outfile<<txtkeyframeColl;
		memset(txtkeyframeColl,0,sizeof(txtkeyframeColl));
	}
	free(txtkeyframeColl);
	outfile.clear();
	outfile.close();
	return true;
}

//导入 本地关键帧
bool CTrackRepeatModule::LoadLocalKeyFrames(char* filefullPath,string indexTrack){
	frameNum = 0;
	vector<KeyFrame> *KeyFrameColl;
	map<string,vector<KeyFrame>>::iterator it = this->m_TrackKeyFrameDepository.find(indexTrack);
	if(it == this->m_TrackKeyFrameDepository.end()) {
		return false;
	}
	else { //找到
		KeyFrameColl=&it->second;
	}
	FILE *fp;
	char* str_line=(char*)malloc(2048);
	errno_t err=fopen_s( &fp,filefullPath, "r" );
	if(err==0)
	{
		fgets(str_line, 2048, fp);
		memset(&str_line, 0, sizeof(*str_line));

		//加载前清空缓存
		KeyFrameColl->erase(KeyFrameColl->begin(),KeyFrameColl->end());

		while (fgets(str_line, 2048, fp) != NULL)
		{
			KeyFrame keyframe;
			int a, b;
			long int IPOC = 0;

			sscanf_s(str_line, "%d %f %d %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %d %d\n",
				&IPOC, &keyframe.PartSpeed, &keyframe.WaitTime, &keyframe.E1, &keyframe.A1, &keyframe.A2, &keyframe.A3, &keyframe.A4, &keyframe.A5, &keyframe.A6, &keyframe.X, &keyframe.Y, &keyframe.Z, &keyframe.A, &keyframe.B, &keyframe.C, &keyframe.FrameRunTime, &keyframe.FrameMinTime, &a, &b);

			keyframe.IPOC = IPOC;

			char name[1024];
			sscanf_s(str_line, "%*s%*s%*s%*s%*s%*s%*s%*s%*s%*s%*s%*s%*s%*s%*s%*s%*s%*s%*s%*s%s", name, _countof(name));
			keyframe.name = name;

			keyframe.Zoom = a;
			keyframe.Focus = b;

			KeyFrameColl->push_back(keyframe);

			memset(&str_line, 0, sizeof(*str_line));
			frameNum++;
		}
		//free(str_line);
		if (!fp)
		{
			fclose(fp);
		}
	 return true;	 
	}
	
	return false;
}

//单步 单步运动
bool CTrackRepeatModule::ObtainSingleMotionCommandData(robotlocation rbLocation,int keyFrameIndex,string indexTrack,vector<robotRunCommand> *CommandColl){
	map<string,vector<KeyFrame>>::iterator itor;
	itor=m_TrackKeyFrameDepository.find(indexTrack);
	if(itor != m_TrackKeyFrameDepository.end()){
		vector<KeyFrame> *m_KeyFrameColl=&itor->second;
		if(keyFrameIndex>=0)
		{
			KeyFrame startFrame;
			startFrame.A1=gCurrentRobotLocation.A1;
			startFrame.A2=gCurrentRobotLocation.A2;
			startFrame.A3=gCurrentRobotLocation.A3;
			startFrame.A4=gCurrentRobotLocation.A4;
			startFrame.A5=gCurrentRobotLocation.A5;
			startFrame.A6=gCurrentRobotLocation.A6;
			startFrame.E1=gCurrentRobotLocation.E1;

			double temp = setFrameRunTime(startFrame,(*m_KeyFrameColl)[keyFrameIndex]);
			(*m_KeyFrameColl)[keyFrameIndex].kk = (temp - (int)temp) * 10;
			//(*m_KeyFrameColl)[keyFrameIndex].kk = 1;
			(*m_KeyFrameColl)[keyFrameIndex].FrameMinTime = (int)temp / 1000.0;

			vector<robotRunCommand> sendDataCollec=CalculateCommandFrameFromTwoKeyFrame4(startFrame,(*m_KeyFrameColl)[keyFrameIndex],commandNum);
			CommandColl->insert(CommandColl->end(),sendDataCollec.begin(),sendDataCollec.end());
			return true;
		}
		else
		{
			//不存在目标关键帧
			return false;
		}
	}
	else {
		//没有找到
		return false;
	}

	return true;
}

//复位  复位运动
bool CTrackRepeatModule::ObtainRestorationMotionCommandData(robotlocation rbLocation,string  indexTrack,vector<robotRunCommand> *CommandColl){
	map<string,vector<KeyFrame>>::iterator itor;
	itor=m_TrackKeyFrameDepository.find(indexTrack);
	if(itor != m_TrackKeyFrameDepository.end()){
		if(itor->second.size()>0)
		{
			KeyFrame startFrame;
			startFrame.A1=gCurrentRobotLocation.A1;
			startFrame.A2=gCurrentRobotLocation.A2;
			startFrame.A3=gCurrentRobotLocation.A3;
			startFrame.A4=gCurrentRobotLocation.A4;
			startFrame.A5=gCurrentRobotLocation.A5;
			startFrame.A6=gCurrentRobotLocation.A6;
			startFrame.E1=gCurrentRobotLocation.E1;
			vector<robotRunCommand> sendDataCollec = CalculateCommandFrameFromTwoKeyFrame4(startFrame, itor->second[0], commandNum);
			CommandColl->insert(CommandColl->end(),sendDataCollec.begin(),sendDataCollec.end());
			return true;
		}
	}
	else {
		//没有找到
		return false;
	}

	return true;

}

//重复  重复运动
bool CTrackRepeatModule::ObtainRepeatMotionCommandData(string indexTrack,vector<robotRunCommand> *CommandColl){

	map<string,vector<KeyFrame>>::iterator itor;
	itor=m_TrackKeyFrameDepository.find(indexTrack);

	//用于判断是否是最后一帧
	bool last=FALSE;

	if(itor != m_TrackKeyFrameDepository.end()){
		if(itor->second.size()>1)
		{
			for(int i=0;i<itor->second.size()-1;i++)
			{
				
				//重新计算局部速度和运行最小时间
				double temp=setFrameRunTime(itor->second[i],itor->second[i+1]);
				itor->second[i+1].kk=(temp-(int)temp)*10;
				itor->second[i+1].FrameMinTime=(int)temp/1000.0;

				if (i == (itor->second.size() - 2))
				{
					last = TRUE;
				}

				//vector<robotRunCommand> sendDataCollec=CalculateCommandFrameFromTwoKeyFrame4(itor->second[i],itor->second[i+1],commandNum);
				vector<robotRunCommand> sendDataCollec = CalculateCommandFrameFromTwoKeyFrame5(itor->second[i], itor->second[i + 1],last);

				//SpeedBuffer(&sendDataCollec, 30);
				CommandColl->insert(CommandColl->end(),sendDataCollec.begin(),sendDataCollec.end());
			}
			return true;
		}
	}
	else {
		//没有找到
		return false;
	}

	return true;
}

////镜头复位
//WORD CTrackRepeatModule::ObtainCameraRestorationMotionCommandData(string indexTrack)
//{
//	WORD zoomValue;
//	map<string, vector<KeyFrame>>::iterator itor;
//	itor = m_TrackKeyFrameDepository.find(indexTrack);
//	zoomValue = itor->second[0].Zoom;
//	return zoomValue;
//}

//镜头重复
//bool CTrackRepeatModule::ObtainCameraRepeatorationMotionCommandData(string indexTrack)
//{
//	map<string, vector<KeyFrame>>::iterator itor;
//	itor = m_TrackKeyFrameDepository.find(indexTrack);
//
//	return true;
//}







///通过两个关帧针计算机器人运动指令
vector<robotRunCommand> CTrackRepeatModule::CalculateCommandFrameFromTwoKeyFrame3(KeyFrame startFrame,KeyFrame endFrame){

vector<robotRunCommand> m_sendDataEntityColl;

	double A1Speed=gA1Step*gWholeSpeed*endFrame.PartSpeed*0.3;//当前A1到A6的步长
	double A2Speed=gA2Step*gWholeSpeed*endFrame.PartSpeed*0.3;//当前A1到A6的步长
	double A3Speed=gA3Step*gWholeSpeed*endFrame.PartSpeed*0.3;//当前A1到A6的步长
	double A4Speed=gA4Step*gWholeSpeed*endFrame.PartSpeed*0.3;//当前A1到A6的步长
	double A5Speed=gA5Step*gWholeSpeed*endFrame.PartSpeed*0.3;//当前A1到A6的步长
	double A6Speed=gA6Step*gWholeSpeed*endFrame.PartSpeed*0.3;//当前A1到A6的步长
	double E1Speed=gE1Step*gWholeSpeed*endFrame.PartSpeed;//E1轴的步长

	//计算两个关键帧之间各个轴的差。
	double diffA1=endFrame.A1-startFrame.A1;
	double diffA2=endFrame.A2-startFrame.A2;
	double diffA3=endFrame.A3-startFrame.A3;
	double diffA4=endFrame.A4-startFrame.A4;
	double diffA5=endFrame.A5-startFrame.A5;
	double diffA6=endFrame.A6-startFrame.A6;
	double diffE1=endFrame.E1-startFrame.E1;

	//计算各轴的差的绝对值
	double absA1=abs(diffA1);
	double absA2=abs(diffA2);
	double absA3=abs(diffA3);
	double absA4=abs(diffA4);
	double absA5=abs(diffA5);
	double absA6=abs(diffA6);
	double absE1=abs(diffE1);


	//计算各轴值的符号
	double signA1=absA1==0?1.0000:diffA1/absA1;
	double signA2=absA2==0?1.0000:diffA2/absA2;
	double signA3=absA3==0?1.0000:diffA3/absA3;
	double signA4=absA4==0?1.0000:diffA4/absA4;
	double signA5=absA5==0?1.0000:diffA5/absA5;
	double signA6=absA6==0?1.0000:diffA6/absA6;
	double signE1=absE1==0?1.0000:diffE1/absE1;


	//按预设帧数。
	double A1Frame=absA1/A1Speed;
	double A2Frame=absA2/A2Speed;
	double A3Frame=absA3/A3Speed;
	double A4Frame=absA4/A4Speed;
	double A5Frame=absA5/A5Speed;
	double A6Frame=absA6/A6Speed;
	double E1Frame=absE1/E1Speed;

	//计算最大的帧数,以确定指令个数
	double maxFrame=0;
	vector<double> absValue;
	absValue.push_back(A1Frame);
	absValue.push_back(A2Frame);
	absValue.push_back(A3Frame);
	absValue.push_back(A4Frame);
	absValue.push_back(A5Frame);
	absValue.push_back(A6Frame);
	absValue.push_back(E1Frame);
	std::sort(absValue.begin(),absValue.end());
	maxFrame=absValue[6];

	//设置步长缓冲指令数可变
	float comdnum=10*gWholeSpeed;

	double A1Step=absA1/maxFrame<0.00001?0.0000:absA1/maxFrame;
	double A2Step=absA2/maxFrame<0.00001?0.0000:absA2/maxFrame;
	double A3Step=absA3/maxFrame<0.00001?0.0000:absA3/maxFrame;
	double A4Step=absA4/maxFrame<0.00001?0.0000:absA4/maxFrame;
	double A5Step=absA5/maxFrame<0.00001?0.0000:absA5/maxFrame;
	double A6Step=absA6/maxFrame<0.00001?0.0000:absA6/maxFrame;
	double E1Step=absE1/maxFrame<0.0001?0.0000:absE1/maxFrame;

	//最后一个指令E1步长补偿
	double lastStepnum=absE1-E1Step*maxFrame;

	//计算总丢失步长
	double totallostE1=0;

	//计算每一帧丢失步长
	double loststepE1=0;

	//计算总补偿步长
	double compsatestepE1;

	for(int j=0;j<comdnum;j++)
	{
		loststepE1=abs(E1Step*signE1-(E1Step*signE1*(j+1))/comdnum);
		totallostE1+=loststepE1;
	}

	//开始结束阶段步长丢失基本相等
	totallostE1=2*totallostE1+lastStepnum;
	compsatestepE1=totallostE1/(maxFrame-2*comdnum);


	for(int i=0;i<maxFrame;i++)
	{
		robotRunCommand sendCommand;
		memset(&sendCommand,0,sizeof(sendCommand));	

		sendCommand.A1=A1Step*signA1;
		sendCommand.A2=A2Step*signA2;
		sendCommand.A3=A3Step*signA3;
		sendCommand.A4=A4Step*signA4;
		sendCommand.A5=A5Step*signA5;
		sendCommand.A6=A6Step*signA6;
		//sendCommand.E1=E1Step*signE1;

		//运动开始阶段速度缓冲
		if(i<comdnum)
		{
			float tempnum1;
			tempnum1=(i+1)/comdnum;
			sendCommand.E1=E1Step*signE1*tempnum1;
		}
		//运动结束速阶段度缓冲
		else if(i>=(maxFrame-comdnum))
		{
			float tempnum2;
			tempnum2=(maxFrame-i)/comdnum;
			sendCommand.E1=E1Step*signE1*tempnum2;
		}
		else
		{
			sendCommand.E1=(E1Step+compsatestepE1)*signE1;
		}
        m_sendDataEntityColl.push_back(sendCommand);
	}

	robotRunCommand endSendCommand;
	memset(&endSendCommand,0,sizeof(endSendCommand));	
	endSendCommand.A1=(absA1-A1Step*maxFrame)*signA1;
	endSendCommand.A2=(absA2-A2Step*maxFrame)*signA2;
	endSendCommand.A3=(absA3-A3Step*maxFrame)*signA3;
	endSendCommand.A4=(absA4-A4Step*maxFrame)*signA4;
	endSendCommand.A5=(absA5-A5Step*maxFrame)*signA5;
	endSendCommand.A6=(absA6-A6Step*maxFrame)*signA6;
	//endSendCommand.E1=(absE1-E1Step*maxFrame)*signE1;
	endSendCommand.E1=0;

	m_sendDataEntityColl.push_back(endSendCommand);

	int WaitSum=endFrame.WaitTime*1000/6;
	for(int i=0;i<WaitSum;i++)
	{
		robotRunCommand endSendCommand;
		memset(&endSendCommand,0,sizeof(endSendCommand));	
		m_sendDataEntityColl.push_back(endSendCommand);
	}
	return m_sendDataEntityColl;
}

vector<robotRunCommand> CTrackRepeatModule::CalculateCommandFrameFromTwoKeyFrame4(KeyFrame startFrame,KeyFrame endFrame,float comdnum){

	vector<robotRunCommand> m_sendDataEntityColl;

	float dd;
	dd = 2*endFrame.kk / gWholeSpeed;
	if (dd==0||endFrame.kk == 1)
	{
		dd = 1.0;
	}
	
	double A1Speed = gA1Step*gWholeSpeed*endFrame.PartSpeed*dd;//当前A1到A6的步长
	double A2Speed = gA2Step*gWholeSpeed*endFrame.PartSpeed*dd;//当前A1到A6的步长
	double A3Speed = gA3Step*gWholeSpeed*endFrame.PartSpeed*dd;//当前A1到A6的步长
	double A4Speed = gA4Step*gWholeSpeed*endFrame.PartSpeed*dd;//当前A1到A6的步长
	double A5Speed = gA5Step*gWholeSpeed*endFrame.PartSpeed*dd;//当前A1到A6的步长
	double A6Speed = gA6Step*gWholeSpeed*endFrame.PartSpeed*dd;//当前A1到A6的步长
	double E1Speed = gE1Step*gWholeSpeed*endFrame.PartSpeed*dd;//E1轴的步长

	//计算两个关键帧之间各个轴的差。
	double diffA1=endFrame.A1-startFrame.A1;
	double diffA2=endFrame.A2-startFrame.A2;
	double diffA3=endFrame.A3-startFrame.A3;
	double diffA4=endFrame.A4-startFrame.A4;
	double diffA5=endFrame.A5-startFrame.A5;
	double diffA6=endFrame.A6-startFrame.A6;
	double diffE1=endFrame.E1-startFrame.E1;

	//计算各轴的差的绝对值
	double absA1=abs(diffA1);
	double absA2=abs(diffA2);
	double absA3=abs(diffA3);
	double absA4=abs(diffA4);
	double absA5=abs(diffA5);
	double absA6=abs(diffA6);
	double absE1=abs(diffE1);


	//计算各轴值的符号
	double signA1=absA1==0?1.0000:diffA1/absA1;
	double signA2=absA2==0?1.0000:diffA2/absA2;
	double signA3=absA3==0?1.0000:diffA3/absA3;
	double signA4=absA4==0?1.0000:diffA4/absA4;
	double signA5=absA5==0?1.0000:diffA5/absA5;
	double signA6=absA6==0?1.0000:diffA6/absA6;
	double signE1=absE1==0?1.0000:diffE1/absE1;


	//按预设帧数。
	double A1Frame=absA1/A1Speed;
	double A2Frame=absA2/A2Speed;
	double A3Frame=absA3/A3Speed;
	double A4Frame=absA4/A4Speed;
	double A5Frame=absA5/A5Speed;
	double A6Frame=absA6/A6Speed;
	double E1Frame=absE1/E1Speed;

	//计算最大的帧数,以确定指令个数
	double maxFrame=0;
	vector<double> absValue;
	absValue.push_back(A1Frame);
	absValue.push_back(A2Frame);
	absValue.push_back(A3Frame);
	absValue.push_back(A4Frame);
	absValue.push_back(A5Frame);
	absValue.push_back(A6Frame);
	absValue.push_back(E1Frame);
	std::sort(absValue.begin(),absValue.end());
	maxFrame=absValue[6];
	if (maxFrame == 0)
	{
		maxFrame = 1;
	}

	//设置步长缓冲指令数可变
	comdnum=ceil(0.05*maxFrame)+3;//0.08

	double A1Step=absA1/maxFrame<0.00001?0.0000:absA1/maxFrame;
	double A2Step=absA2/maxFrame<0.00001?0.0000:absA2/maxFrame;
	double A3Step=absA3/maxFrame<0.00001?0.0000:absA3/maxFrame;
	double A4Step=absA4/maxFrame<0.00001?0.0000:absA4/maxFrame;
	double A5Step=absA5/maxFrame<0.00001?0.0000:absA5/maxFrame;
	double A6Step=absA6/maxFrame<0.00001?0.0000:absA6/maxFrame;
	double E1Step=absE1/maxFrame<0.0001?0.0000:absE1/maxFrame;

	//最后一个指令E1步长补偿
	double lastStepnum=absE1-E1Step*maxFrame;

	//计算总丢失步长
	double totallostA1=0;
	double totallostA2=0;
	double totallostA3=0;
	double totallostA4=0;
	double totallostA5=0;
	double totallostA6=0;
	double totallostE1=0;

	//计算每一帧丢失步长
	double loststepA1=0;
	double loststepA2=0;
	double loststepA3=0;
	double loststepA4=0;
	double loststepA5=0;
	double loststepA6=0;
	double loststepE1=0;

	//计算总补偿步长
	double compsatestepA1;
	double compsatestepA2;
	double compsatestepA3;
	double compsatestepA4;
	double compsatestepA5;
	double compsatestepA6;
	double compsatestepE1;

	for(int j=0;j<comdnum;j++)
	{
		loststepA1=abs(A1Step*signA1-(A1Step*signA1*(j+1))/comdnum);
		loststepA2=abs(A2Step*signA2-(A2Step*signA2*(j+1))/comdnum);
		loststepA3=abs(A3Step*signA3-(A3Step*signA3*(j+1))/comdnum);
		loststepA4=abs(A4Step*signA4-(A4Step*signA4*(j+1))/comdnum);
		loststepA5=abs(A5Step*signA5-(A5Step*signA5*(j+1))/comdnum);
		loststepA6=abs(A6Step*signA6-(A6Step*signA6*(j+1))/comdnum);
		loststepE1=abs(E1Step*signE1-(E1Step*signE1*(j+1))/comdnum);

		totallostA1+=loststepA1;
		totallostA2+=loststepA2;
		totallostA3+=loststepA3;
		totallostA4+=loststepA4;
		totallostA5+=loststepA5;
		totallostA6+=loststepA6;
		totallostE1+=loststepE1;
	}

	totallostA1=2*totallostA1;//开始结束阶段步长丢失基本相等
	totallostA2=2*totallostA2;
	totallostA3=2*totallostA3;
	totallostA4=2*totallostA4;
	totallostA5=2*totallostA5;
	totallostA6=2*totallostA6;
	totallostE1=2*totallostE1+lastStepnum;

	compsatestepA1=totallostA1/(maxFrame-2*comdnum);
	compsatestepA2=totallostA2/(maxFrame-2*comdnum);
	compsatestepA3=totallostA3/(maxFrame-2*comdnum);
	compsatestepA4=totallostA4/(maxFrame-2*comdnum);
	compsatestepA5=totallostA5/(maxFrame-2*comdnum);
	compsatestepA6=totallostA6/(maxFrame-2*comdnum);
	compsatestepE1=totallostE1/(maxFrame-2*comdnum);

	for(int i=0;i<maxFrame;i++)
	{
		robotRunCommand sendCommand;
		memset(&sendCommand,0,sizeof(sendCommand));	
		//sendCommand.A1=A1Step*signA1;
		//sendCommand.A2=A2Step*signA2;
		//sendCommand.A3=A3Step*signA3;
		//sendCommand.A4=A4Step*signA4;
		//sendCommand.A5=A5Step*signA5;
		//sendCommand.A6=A6Step*signA6;
		//sendCommand.E1=E1Step*signE1;

		//运动开始阶段速度缓冲
		if(i<comdnum)
		{
			float tempnum1;
			tempnum1=(i+1)/comdnum;

			sendCommand.A1=A1Step*signA1*tempnum1;
			sendCommand.A2=A2Step*signA2*tempnum1;
			sendCommand.A3=A3Step*signA3*tempnum1;
			sendCommand.A4=A4Step*signA4*tempnum1;
			sendCommand.A5=A5Step*signA5*tempnum1;
			sendCommand.A6=A6Step*signA6*tempnum1;
			sendCommand.E1=E1Step*signE1*tempnum1;
		}
		//运动结束速阶段度缓冲
		else if(i>=(maxFrame-comdnum))
		{
			float tempnum2;
			tempnum2=(maxFrame-i)/comdnum;

			sendCommand.A1=A1Step*signA1*tempnum2;
			sendCommand.A2=A2Step*signA2*tempnum2;
			sendCommand.A3=A3Step*signA3*tempnum2;
			sendCommand.A4=A4Step*signA4*tempnum2;
			sendCommand.A5=A5Step*signA5*tempnum2;
			sendCommand.A6=A6Step*signA6*tempnum2;
			sendCommand.E1=E1Step*signE1*tempnum2;
		}
		else
		{
			sendCommand.A1=(A1Step+compsatestepA1)*signA1;
			sendCommand.A2=(A2Step+compsatestepA2)*signA2;
			sendCommand.A3=(A3Step+compsatestepA3)*signA3;
			sendCommand.A4=(A4Step+compsatestepA4)*signA4;
			sendCommand.A5=(A5Step+compsatestepA5)*signA5;
			sendCommand.A6=(A6Step+compsatestepA6)*signA6;
			sendCommand.E1=(E1Step+compsatestepE1)*signE1;
		}
        m_sendDataEntityColl.push_back(sendCommand);
			
	}

	robotRunCommand endSendCommand;
	memset(&endSendCommand,0,sizeof(endSendCommand));	
	endSendCommand.A1=(absA1-A1Step*maxFrame)*signA1;
	endSendCommand.A2=(absA2-A2Step*maxFrame)*signA2;
	endSendCommand.A3=(absA3-A3Step*maxFrame)*signA3;
	endSendCommand.A4=(absA4-A4Step*maxFrame)*signA4;
	endSendCommand.A5=(absA5-A5Step*maxFrame)*signA5;
	endSendCommand.A6=(absA6-A6Step*maxFrame)*signA6;
	//endSendCommand.E1=(absE1-E1Step*maxFrame)*signE1;
	endSendCommand.E1=0;

	m_sendDataEntityColl.push_back(endSendCommand);

	int WaitSum=endFrame.WaitTime*1000/6;
	for(int i=0;i<WaitSum;i++)
	{
		robotRunCommand endSendCommand;
		memset(&endSendCommand,0,sizeof(endSendCommand));	
		m_sendDataEntityColl.push_back(endSendCommand);
	}
	return m_sendDataEntityColl;
}

//优化后轨迹重复时关键帧之间的差值算法
vector<robotRunCommand> CTrackRepeatModule::CalculateCommandFrameFromTwoKeyFrame5(KeyFrame startFrame, KeyFrame endFrame, bool lastFrame){


	vector<robotRunCommand> m_sendDataEntityColl;

	float dd;
	dd = 1.6 * endFrame.kk / gWholeSpeed;
	if (dd == 0 || endFrame.kk == 1)
	{
		dd = 1.0;
	}

	double A1Speed = gA1Step*gWholeSpeed*endFrame.PartSpeed*dd;//当前A1到A6的步长
	double A2Speed = gA2Step*gWholeSpeed*endFrame.PartSpeed*dd;//当前A1到A6的步长
	double A3Speed = gA3Step*gWholeSpeed*endFrame.PartSpeed*dd;//当前A1到A6的步长
	double A4Speed = gA4Step*gWholeSpeed*endFrame.PartSpeed*dd;//当前A1到A6的步长
	double A5Speed = gA5Step*gWholeSpeed*endFrame.PartSpeed*dd;//当前A1到A6的步长
	double A6Speed = gA6Step*gWholeSpeed*endFrame.PartSpeed*dd;//当前A1到A6的步长
	double E1Speed = gE1Step*gWholeSpeed*endFrame.PartSpeed*dd;//E1轴的步长

	//计算两个关键帧之间各个轴的差。
	double diffA1 = endFrame.A1 - startFrame.A1;
	double diffA2 = endFrame.A2 - startFrame.A2;
	double diffA3 = endFrame.A3 - startFrame.A3;
	double diffA4 = endFrame.A4 - startFrame.A4;
	double diffA5 = endFrame.A5 - startFrame.A5;
	double diffA6 = endFrame.A6 - startFrame.A6;
	double diffE1 = endFrame.E1 - startFrame.E1;

	//计算各轴的差的绝对值
	double absA1 = abs(diffA1);
	double absA2 = abs(diffA2);
	double absA3 = abs(diffA3);
	double absA4 = abs(diffA4);
	double absA5 = abs(diffA5);
	double absA6 = abs(diffA6);
	double absE1 = abs(diffE1);


	//计算各轴值的符号
	double signA1 = absA1 == 0 ? 1.0000 : diffA1 / absA1;
	double signA2 = absA2 == 0 ? 1.0000 : diffA2 / absA2;
	double signA3 = absA3 == 0 ? 1.0000 : diffA3 / absA3;
	double signA4 = absA4 == 0 ? 1.0000 : diffA4 / absA4;
	double signA5 = absA5 == 0 ? 1.0000 : diffA5 / absA5;
	double signA6 = absA6 == 0 ? 1.0000 : diffA6 / absA6;
	double signE1 = absE1 == 0 ? 1.0000 : diffE1 / absE1;


	//按预设帧数。
	double A1Frame = absA1 / A1Speed;
	double A2Frame = absA2 / A2Speed;
	double A3Frame = absA3 / A3Speed;
	double A4Frame = absA4 / A4Speed;
	double A5Frame = absA5 / A5Speed;
	double A6Frame = absA6 / A6Speed;
	double E1Frame = absE1 / E1Speed;

	//计算最大的帧数,以确定指令个数
	double maxFrame = 0;
	vector<double> absValue;
	absValue.push_back(A1Frame);
	absValue.push_back(A2Frame);
	absValue.push_back(A3Frame);
	absValue.push_back(A4Frame);
	absValue.push_back(A5Frame);
	absValue.push_back(A6Frame);
	absValue.push_back(E1Frame);
	std::sort(absValue.begin(), absValue.end());
	maxFrame = absValue[6];
	if (maxFrame == 0)
	{
		maxFrame = 1;
	}

	double A1Step = absA1 / maxFrame<0.00001 ? 0.0000 : absA1 / maxFrame;
	double A2Step = absA2 / maxFrame<0.00001 ? 0.0000 : absA2 / maxFrame;
	double A3Step = absA3 / maxFrame<0.00001 ? 0.0000 : absA3 / maxFrame;
	double A4Step = absA4 / maxFrame<0.00001 ? 0.0000 : absA4 / maxFrame;
	double A5Step = absA5 / maxFrame<0.00001 ? 0.0000 : absA5 / maxFrame;
	double A6Step = absA6 / maxFrame<0.00001 ? 0.0000 : absA6 / maxFrame;
	double E1Step = absE1 / maxFrame<0.0001 ? 0.0000 : absE1 / maxFrame;

	//计算与上一帧步长的差
	double diffStepE1 = E1Step*signE1 - lastE1Step;
	double diffStepA1 = A1Step*signA1 - lastA1Step;
	double diffStepA2 = A2Step*signA2 - lastA2Step;
	double diffStepA3 = A3Step*signA3 - lastA3Step;
	double diffStepA4 = A4Step*signA4 - lastA4Step;
	double diffStepA5 = A5Step*signA5 - lastA5Step;
	double diffStepA6 = A6Step*signA6 - lastA6Step;

	//计算最大步长差
	double maxStep = 0;
	double stepValue[7];
	stepValue[0] = (fabs(diffStepE1));
	stepValue[1] = (fabs(diffStepA1));
	stepValue[2] = (fabs(diffStepA2));
	stepValue[3] = (fabs(diffStepA3));
	stepValue[4] = (fabs(diffStepA4));
	stepValue[5] = (fabs(diffStepA5));
	stepValue[6] = (fabs(diffStepA6));

	for (int k = 0; k < 7; k++)
	{
		if (stepValue[k] >= maxStep)
		{
			maxStep = stepValue[k];
		}
	}

	if (maxStep == 0)
	{
		maxStep = 0.001;
	}

	//设置步长缓冲指令数可变
	float comdnum, comdnumLastFrame;

	//两帧之间速度缓冲系数
	comdnum = ceil(10 * maxStep) + 30;//系数待测


	//计算总丢失步长
	double totallostA1 = 0;
	double totallostA2 = 0;
	double totallostA3 = 0;
	double totallostA4 = 0;
	double totallostA5 = 0;
	double totallostA6 = 0;
	double totallostE1 = 0;

	//计算每一帧丢失步长
	double loststepA1 = 0;
	double loststepA2 = 0;
	double loststepA3 = 0;
	double loststepA4 = 0;
	double loststepA5 = 0;
	double loststepA6 = 0;
	double loststepE1 = 0;

	//计算总补偿步长
	double compsatestepA1;
	double compsatestepA2;
	double compsatestepA3;
	double compsatestepA4;
	double compsatestepA5;
	double compsatestepA6;
	double compsatestepE1;

	if (lastFrame == FALSE&&endFrame.WaitTime==0)
	{

		for (int j = 0; j<comdnum; j++)
		{
			loststepA1 = diffStepA1 - (diffStepA1*(j + 1)) / comdnum;
			loststepA2 = diffStepA2 - (diffStepA2*(j + 1)) / comdnum;
			loststepA3 = diffStepA3 - (diffStepA3*(j + 1)) / comdnum;
			loststepA4 = diffStepA4 - (diffStepA4*(j + 1)) / comdnum;
			loststepA5 = diffStepA5 - (diffStepA5*(j + 1)) / comdnum;
			loststepA6 = diffStepA6 - (diffStepA6*(j + 1)) / comdnum;
			loststepE1 = diffStepE1 - (diffStepE1*(j + 1)) / comdnum;

			totallostA1 += loststepA1;
			totallostA2 += loststepA2;
			totallostA3 += loststepA3;
			totallostA4 += loststepA4;
			totallostA5 += loststepA5;
			totallostA6 += loststepA6;
			totallostE1 += loststepE1;
		}

		compsatestepA1 = totallostA1 / (maxFrame - comdnum-1);
		compsatestepA2 = totallostA2 / (maxFrame - comdnum-1);
		compsatestepA3 = totallostA3 / (maxFrame - comdnum-1);
		compsatestepA4 = totallostA4 / (maxFrame - comdnum-1);
		compsatestepA5 = totallostA5 / (maxFrame - comdnum-1);
		compsatestepA6 = totallostA6 / (maxFrame - comdnum-1);
		compsatestepE1 = totallostE1 / (maxFrame - comdnum-1);


		for (int i = 0; i<maxFrame-1; i++)
		{
			robotRunCommand sendCommand;
			memset(&sendCommand, 0, sizeof(sendCommand));

			//运动开始阶段速度缓冲
			if (i<comdnum)
			{
				float tempnum1;
				tempnum1 = (i + 1) / comdnum;

				sendCommand.A1 = lastA1Step + (diffStepA1*tempnum1);
				sendCommand.A2 = lastA2Step + (diffStepA2*tempnum1);
				sendCommand.A3 = lastA3Step + (diffStepA3*tempnum1);
				sendCommand.A4 = lastA4Step + (diffStepA4*tempnum1);
				sendCommand.A5 = lastA5Step + (diffStepA5*tempnum1);
				sendCommand.A6 = lastA6Step + (diffStepA6*tempnum1);
				sendCommand.E1 = lastE1Step + (diffStepE1*tempnum1);

				//kk = kk + sendCommand.E1;

			}
			else
			{
				sendCommand.A1 = (A1Step + compsatestepA1)*signA1;
				sendCommand.A2 = (A2Step + compsatestepA2)*signA2;
				sendCommand.A3 = (A3Step + compsatestepA3)*signA3;
				sendCommand.A4 = (A4Step + compsatestepA4)*signA4;
				sendCommand.A5 = (A5Step + compsatestepA5)*signA5;
				sendCommand.A6 = (A6Step + compsatestepA6)*signA6;
				sendCommand.E1 = (E1Step + compsatestepE1)*signE1;

				lastA1Step = sendCommand.A1;
				lastA2Step = sendCommand.A2;
				lastA3Step = sendCommand.A3;
				lastA4Step = sendCommand.A4;
				lastA5Step = sendCommand.A5;
				lastA6Step = sendCommand.A6;
				lastE1Step = sendCommand.E1;

				//kk = kk + sendCommand.E1;
			}

			m_sendDataEntityColl.push_back(sendCommand);
		}
	}

	//最后一帧结束时或者有等待时间时速度缓冲
	else if (lastFrame == TRUE||endFrame.WaitTime!=0)
	{
		//计算最大步长
		double Step = 0;
		double step[7];
		step[0] = (fabs(E1Step));
		step[1] = (fabs(A1Step));
		step[2] = (fabs(A2Step));
		step[3] = (fabs(A3Step));
		step[4] = (fabs(A4Step));
		step[5] = (fabs(A5Step));
		step[6] = (fabs(A6Step));

		for (int k = 0; k < 7; k++)
		{
			if (step[k] >= Step)
			{
				Step = step[k];
			}
		}
		
		
		//最后一帧结束速度缓冲帧数
		comdnumLastFrame = ceil(10*Step) + 30;

		for (int j = 0; j<comdnum; j++)
		{
			loststepA1 = diffStepA1 - (diffStepA1*(j + 1)) / comdnum;
			loststepA2 = diffStepA2 - (diffStepA2*(j + 1)) / comdnum;
			loststepA3 = diffStepA3 - (diffStepA3*(j + 1)) / comdnum;
			loststepA4 = diffStepA4 - (diffStepA4*(j + 1)) / comdnum;
			loststepA5 = diffStepA5 - (diffStepA5*(j + 1)) / comdnum;
			loststepA6 = diffStepA6 - (diffStepA6*(j + 1)) / comdnum;
			loststepE1 = diffStepE1 - (diffStepE1*(j + 1)) / comdnum;

			totallostA1 += loststepA1;
			totallostA2 += loststepA2;
			totallostA3 += loststepA3;
			totallostA4 += loststepA4;
			totallostA5 += loststepA5;
			totallostA6 += loststepA6;
			totallostE1 += loststepE1;
		}
		for (int j = 0; j<comdnumLastFrame; j++)
		{
			loststepA1 = abs(A1Step*signA1 - (A1Step*signA1*(j + 1)) / comdnumLastFrame);
			loststepA2 = abs(A2Step*signA2 - (A2Step*signA2*(j + 1)) / comdnumLastFrame);
			loststepA3 = abs(A3Step*signA3 - (A3Step*signA3*(j + 1)) / comdnumLastFrame);
			loststepA4 = abs(A4Step*signA4 - (A4Step*signA4*(j + 1)) / comdnumLastFrame);
			loststepA5 = abs(A5Step*signA5 - (A5Step*signA5*(j + 1)) / comdnumLastFrame);
			loststepA6 = abs(A6Step*signA6 - (A6Step*signA6*(j + 1)) / comdnumLastFrame);
			loststepE1 = abs(E1Step*signE1 - (E1Step*signE1*(j + 1)) / comdnumLastFrame);

			totallostA1 += loststepA1;
			totallostA2 += loststepA2;
			totallostA3 += loststepA3;
			totallostA4 += loststepA4;
			totallostA5 += loststepA5;
			totallostA6 += loststepA6;
			totallostE1 += loststepE1;
		}


		compsatestepA1 = totallostA1 / (maxFrame - comdnum - comdnumLastFrame-2);
		compsatestepA2 = totallostA2 / (maxFrame - comdnum - comdnumLastFrame-2);
		compsatestepA3 = totallostA3 / (maxFrame - comdnum - comdnumLastFrame-2);
		compsatestepA4 = totallostA4 / (maxFrame - comdnum - comdnumLastFrame-2);
		compsatestepA5 = totallostA5 / (maxFrame - comdnum - comdnumLastFrame-2);
		compsatestepA6 = totallostA6 / (maxFrame - comdnum - comdnumLastFrame-2);
		compsatestepE1 = totallostE1 / (maxFrame - comdnum - comdnumLastFrame-2);

		for (int i = 0; i<maxFrame-1; i++)
		{
			robotRunCommand sendCommand;
			memset(&sendCommand, 0, sizeof(sendCommand));

			//运动开始阶段速度缓冲
			if (i<comdnum)
			{
				float tempnum1;
				tempnum1 = (i + 1) / comdnum;

				sendCommand.A1 = lastA1Step + (diffStepA1*tempnum1);
				sendCommand.A2 = lastA2Step + (diffStepA2*tempnum1);
				sendCommand.A3 = lastA3Step + (diffStepA3*tempnum1);
				sendCommand.A4 = lastA4Step + (diffStepA4*tempnum1);
				sendCommand.A5 = lastA5Step + (diffStepA5*tempnum1);
				sendCommand.A6 = lastA6Step + (diffStepA6*tempnum1);
				sendCommand.E1 = lastE1Step + (diffStepE1*tempnum1);

				//kk = kk + sendCommand.E1;

			}
			else if (i <=(maxFrame - comdnumLastFrame-1))
			{
				sendCommand.A1 = (A1Step + compsatestepA1)*signA1;
				sendCommand.A2 = (A2Step + compsatestepA2)*signA2;
				sendCommand.A3 = (A3Step + compsatestepA3)*signA3;
				sendCommand.A4 = (A4Step + compsatestepA4)*signA4;
				sendCommand.A5 = (A5Step + compsatestepA5)*signA5;
				sendCommand.A6 = (A6Step + compsatestepA6)*signA6;
				sendCommand.E1 = (E1Step + compsatestepE1)*signE1;

				//kk = kk + sendCommand.E1;
			}
			else 
			{
				float tempnum2;
				tempnum2 = (maxFrame - i) / comdnumLastFrame;

				sendCommand.A1 = A1Step*signA1*tempnum2;
				sendCommand.A2 = A2Step*signA2*tempnum2;
				sendCommand.A3 = A3Step*signA3*tempnum2;
				sendCommand.A4 = A4Step*signA4*tempnum2;
				sendCommand.A5 = A5Step*signA5*tempnum2;
				sendCommand.A6 = A6Step*signA6*tempnum2;
				sendCommand.E1 = E1Step*signE1*tempnum2;

				//kk = kk + sendCommand.E1;

				lastA1Step = 0;
				lastA2Step = 0;
				lastA3Step = 0;
				lastA4Step = 0;
				lastA5Step = 0;
				lastA6Step = 0;
				lastE1Step = 0;

			}
			m_sendDataEntityColl.push_back(sendCommand);

		}
	}


	int WaitSum = endFrame.WaitTime * 1000 / 12;
	for (int i = 0; i<WaitSum; i++)
	{
		robotRunCommand endSendCommand;
		memset(&endSendCommand, 0, sizeof(endSendCommand));
		m_sendDataEntityColl.push_back(endSendCommand);
	}
	return m_sendDataEntityColl;
}

///插入关键帧
bool CTrackRepeatModule::InsertKeyFrame(robotlocation rbLocation, string  indexTrack, int m_nIndex, int waitTime, float partSpeed)
{
	map<string,vector<KeyFrame>>::iterator itor;
	itor=m_TrackKeyFrameDepository.find(indexTrack);
	if(itor != m_TrackKeyFrameDepository.end()){
		KeyFrame keyFrame;
		keyFrame.A1=gCurrentRobotLocation.A1;
		keyFrame.A2=gCurrentRobotLocation.A2;
		keyFrame.A3=gCurrentRobotLocation.A3;
		keyFrame.A4=gCurrentRobotLocation.A4;
		keyFrame.A5=gCurrentRobotLocation.A5;
		keyFrame.A6=gCurrentRobotLocation.A6;
		keyFrame.E1=gCurrentRobotLocation.E1;

		keyFrame.X = gCurrentRobotLocation.X;
		keyFrame.Y = gCurrentRobotLocation.Y;
		keyFrame.Z = gCurrentRobotLocation.Z;
		keyFrame.A = gCurrentRobotLocation.A;
		keyFrame.B = gCurrentRobotLocation.B;
		keyFrame.C = gCurrentRobotLocation.C;

		keyFrame.WaitTime=waitTime;
		keyFrame.PartSpeed=partSpeed;

		keyFrame.Focus = Focus;
		keyFrame.Zoom = Zoom;

		stringstream ss;
		ss << keyFrame.IPOC;
		ss >> keyFrame.name;

		picName.Format("%d", keyFrame.IPOC);
		keyFrame.name = picName;

		itor->second.insert(itor->second.begin()+m_nIndex,keyFrame);
		return true;
	}
	else {
		//没有找到
		return false;
	}
}

/*float CTrackRepeatModule::CalculateCameraX(float x,float y,float z,float a,float b,float c)
{
	float result;
	result=cameraX*cos(a)*cos(b)+cameraY*(-sin(a)*cos(c)+cos(a)*sin(b)*sin(c))+cameraZ*(sin(a)*sin(c)+cos(a)*sin(b)*cos(c))+x;
	return result;

}

float CTrackRepeatModule::CalculateCameraY(float x,float y,float z,float a,float b,float c)
{
	float result;
	result=cameraX*sin(a)*cos(b)+cameraY*(cos(a)*cos(c)+sin(a)*sin(b)*sin(c))+cameraZ*(-cos(a)*sin(c)+sin(a)*sin(b)*cos(c))+y;
	return result;

}

float CTrackRepeatModule::CalculateCameraZ(float x,float y,float z,float a,float b,float c)
{
	float result;
	result=cameraX*(-sin(b))+cameraY*cos(b)*sin(c)+cameraZ*cos(b)*cos(c)+z;
	return result;

}*/

double CTrackRepeatModule::setFrameRunTime(KeyFrame startFrame,KeyFrame endFrame)
{
	
	double A1Speed=gA1Step*1.6*endFrame.PartSpeed;//当前A1到A6的步长
	double A2Speed = gA2Step*1.6*endFrame.PartSpeed;//当前A1到A6的步长
	double A3Speed = gA3Step*1.6*endFrame.PartSpeed;//当前A1到A6的步长
	double A4Speed = gA4Step*1.6*endFrame.PartSpeed;//当前A1到A6的步长
	double A5Speed = gA5Step*1.6*endFrame.PartSpeed;//当前A1到A6的步长
	double A6Speed = gA6Step*1.6*endFrame.PartSpeed;//当前A1到A6的步长
	double E1Speed = gE1Step*1.6*endFrame.PartSpeed;//E1轴的步长

	//计算两个关键帧之间各个轴的差。
	double diffA1=endFrame.A1-startFrame.A1;
	double diffA2=endFrame.A2-startFrame.A2;
	double diffA3=endFrame.A3-startFrame.A3;
	double diffA4=endFrame.A4-startFrame.A4;
	double diffA5=endFrame.A5-startFrame.A5;
	double diffA6=endFrame.A6-startFrame.A6;
	double diffE1=endFrame.E1-startFrame.E1;

	//计算各轴的差的绝对值
	double absA1=abs(diffA1);
	double absA2=abs(diffA2);
	double absA3=abs(diffA3);
	double absA4=abs(diffA4);
	double absA5=abs(diffA5);
	double absA6=abs(diffA6);
	double absE1=abs(diffE1);


	//计算各轴值的符号
	double signA1=absA1==0?1.0000:diffA1/absA1;
	double signA2=absA2==0?1.0000:diffA2/absA2;
	double signA3=absA3==0?1.0000:diffA3/absA3;
	double signA4=absA4==0?1.0000:diffA4/absA4;
	double signA5=absA5==0?1.0000:diffA5/absA5;
	double signA6=absA6==0?1.0000:diffA6/absA6;
	double signE1=absE1==0?1.0000:diffE1/absE1;


	//按预设帧数。
	double A1Frame=absA1/A1Speed;
	double A2Frame=absA2/A2Speed;
	double A3Frame=absA3/A3Speed;
	double A4Frame=absA4/A4Speed;
	double A5Frame=absA5/A5Speed;
	double A6Frame=absA6/A6Speed;
	double E1Frame=absE1/E1Speed;

	//计算最大的帧数,以确定指令个数
	double maxFrame=0;
	vector<double> absValue;
	absValue.push_back(A1Frame);
	absValue.push_back(A2Frame);
	absValue.push_back(A3Frame);
	absValue.push_back(A4Frame);
	absValue.push_back(A5Frame);
	absValue.push_back(A6Frame);
	absValue.push_back(E1Frame);
	std::sort(absValue.begin(),absValue.end());
	maxFrame=absValue[6];

	endFrame.FrameMinTime=((int)maxFrame*endFrame.PartSpeed)*0.012;

	if(endFrame.FrameRunTime>endFrame.FrameMinTime)
	{
		endFrame.kk=maxFrame/(endFrame.FrameRunTime/0.012);
	}
	else
	{
		endFrame.kk = 1.0;
	}
	

	return 1000*endFrame.FrameMinTime+0.1*endFrame.kk;
}

//计算每帧运行时间
int CTrackRepeatModule::frameRunTime(KeyFrame startFrame,KeyFrame endFrame)
{

	double A1Speed = gA1Step*gWholeSpeed*endFrame.PartSpeed;//当前A1到A6的步长
	double A2Speed = gA2Step*gWholeSpeed*endFrame.PartSpeed;//当前A1到A6的步长
	double A3Speed = gA3Step*gWholeSpeed*endFrame.PartSpeed;//当前A1到A6的步长
	double A4Speed = gA4Step*gWholeSpeed*endFrame.PartSpeed;//当前A1到A6的步长
	double A5Speed = gA5Step*gWholeSpeed*endFrame.PartSpeed;//当前A1到A6的步长
	double A6Speed = gA6Step*gWholeSpeed*endFrame.PartSpeed;//当前A1到A6的步长
	double E1Speed = gE1Step*gWholeSpeed*endFrame.PartSpeed;//E1轴的步长

	//计算两个关键帧之间各个轴的差。
	double diffA1=endFrame.A1-startFrame.A1;
	double diffA2=endFrame.A2-startFrame.A2;
	double diffA3=endFrame.A3-startFrame.A3;
	double diffA4=endFrame.A4-startFrame.A4;
	double diffA5=endFrame.A5-startFrame.A5;
	double diffA6=endFrame.A6-startFrame.A6;
	double diffE1=endFrame.E1-startFrame.E1;

	//计算各轴的差的绝对值
	double absA1=abs(diffA1);
	double absA2=abs(diffA2);
	double absA3=abs(diffA3);
	double absA4=abs(diffA4);
	double absA5=abs(diffA5);
	double absA6=abs(diffA6);
	double absE1=abs(diffE1);


	//计算各轴值的符号
	double signA1=absA1==0?1.0000:diffA1/absA1;
	double signA2=absA2==0?1.0000:diffA2/absA2;
	double signA3=absA3==0?1.0000:diffA3/absA3;
	double signA4=absA4==0?1.0000:diffA4/absA4;
	double signA5=absA5==0?1.0000:diffA5/absA5;
	double signA6=absA6==0?1.0000:diffA6/absA6;
	double signE1=absE1==0?1.0000:diffE1/absE1;


	//按预设帧数。
	double A1Frame=absA1/A1Speed;
	double A2Frame=absA2/A2Speed;
	double A3Frame=absA3/A3Speed;
	double A4Frame=absA4/A4Speed;
	double A5Frame=absA5/A5Speed;
	double A6Frame=absA6/A6Speed;
	double E1Frame=absE1/E1Speed;

	//计算最大的帧数,以确定指令个数
	double maxFrame=0;
	vector<double> absValue;
	absValue.push_back(A1Frame);
	absValue.push_back(A2Frame);
	absValue.push_back(A3Frame);
	absValue.push_back(A4Frame);
	absValue.push_back(A5Frame);
	absValue.push_back(A6Frame);
	absValue.push_back(E1Frame);
	std::sort(absValue.begin(),absValue.end());
	maxFrame=absValue[6];

	return maxFrame;
}

//计算每帧运行最小时间
bool CTrackRepeatModule::frameMinTime(string indexTrack){

	map<string, vector<KeyFrame>>::iterator itor;
	itor = m_TrackKeyFrameDepository.find(indexTrack);
	if (itor != m_TrackKeyFrameDepository.end()){
		if (itor->second.size()>1)
		{
			for (int i = 0; i<itor->second.size() - 1; i++)
			{

				//重新计算局部速度和运行最小时间
				double temp = setFrameRunTime(itor->second[i], itor->second[i + 1]);
				itor->second[i + 1].FrameMinTime =(int)temp / 1000.0;

			}
			return true;
		}
	}
	else {
		//没有找到
		return false;
	}

	return true;
}


//计算摄像机光心位置
void CTrackRepeatModule::CalculateCameraLocation()
{
	D3DXMATRIX rotationX;
	D3DXMATRIX rotationY;
	D3DXMATRIX rotationZ;
	D3DXMATRIX rotation;
	D3DXMATRIX cameraRotation;
	D3DXMATRIX cameraOffset;
	D3DXMATRIX result;

	D3DXMATRIX toCamera;
	D3DXMATRIX a;
	D3DXMATRIX b;

	float AngleA,AngleB,AngleC;

	AngleA=gCurrentRobotLocation.A*angleToRad*(-1);
	AngleB=gCurrentRobotLocation.B*angleToRad*(-1);
	AngleC=gCurrentRobotLocation.C*angleToRad*(-1);

	//绕X轴旋转矩阵
	D3DXMatrixRotationX(&rotationX,AngleC);

	//绕Y轴
	D3DXMatrixRotationY(&rotationY,AngleB);

	//绕Z轴
	D3DXMatrixRotationZ(&rotationZ,AngleA);

	//计算光心到工具坐标系的旋转矩阵
	D3DXMatrixRotationX(&a,-90*angleToRad);
	D3DXMatrixRotationY(&b,90*angleToRad);
	toCamera=b*a;

	//计算工具坐标系到底座的旋转矩阵
	rotation=rotationZ*rotationY*rotationX;

	//摄像机相对于云台偏移量矩阵
	cameraOffset(0,0)=cameraX;
	cameraOffset(1,0)=cameraY;
	cameraOffset(2,0)=cameraZ;

    result=rotation*cameraOffset;

	D3DXVECTOR4 cameraLocation(result(0,0),result(1,0),result(2,0),1);
	D3DXVECTOR4 offset(gCurrentRobotLocation.X,gCurrentRobotLocation.Y,gCurrentRobotLocation.Z,1);
	cameraLocation=cameraLocation+offset;

	//计算光心到底座的旋转矩阵
	cameraRotation=rotation*toCamera;

	cameraTilt=atan2(cameraRotation(2,1),sqrt((cameraRotation(2,0)*cameraRotation(2,0))+(cameraRotation(2,2)*cameraRotation(2,2))));
	cameraRoll=atan2((-cameraRotation(0,1)/cos(cameraTilt)),(cameraRotation(1,1)/cos(cameraTilt)));
	cameraPan=atan2((-cameraRotation(2,0)/cos(cameraTilt)),(cameraRotation(2,2)/cos(cameraTilt)));

	cameraLocationX=cameraLocation.x;
	cameraLocationY=cameraLocation.y;
	cameraLocationZ=cameraLocation.z;

	cameraTilt=cameraTilt/angleToRad;
	cameraRoll=cameraRoll/angleToRad;
	cameraPan=cameraPan/angleToRad;

}

/*
void CTrackRepeatModule::test()
{
	float A1=-22.15*angleToRad*-1;
	float A2=-95.58*angleToRad*-1;
	float A3=100.57*angleToRad*-1;
	float A4=66.73*angleToRad*-1;
	float A5=51.22*angleToRad*-1;
	float A6=89.79*angleToRad*-1;
	float Ca=-56.88*angleToRad*-1;
	float Cb=-66.14*angleToRad*-1;
	float Cc=-153.26*angleToRad*-1;

	D3DXMATRIX resultabc;
	D3DXMATRIX result16;

	D3DXMATRIX MA1;
	D3DXMATRIX MA2;
	D3DXMATRIX MA3;
	D3DXMATRIX MA4;
	D3DXMATRIX MA5;
	D3DXMATRIX MA6;
	D3DXMATRIX Cx;
	D3DXMATRIX Cy;
	D3DXMATRIX Cz;
	D3DXMATRIX temp;


	D3DXMatrixRotationX(&Cx,Cc);
	D3DXMatrixRotationY(&Cy,Cb);
	D3DXMatrixRotationZ(&Cz,Ca);

	D3DXMatrixRotationZ(&MA1,-1*A1);
	D3DXMatrixRotationY(&MA2,A2);
	D3DXMatrixRotationY(&MA3,A3);
	D3DXMatrixRotationX(&MA4,-1*A4);
	D3DXMatrixRotationY(&MA5,A5);
	D3DXMatrixRotationX(&MA6,-1*A6);

	D3DXMatrixRotationY(&temp,-90*angleToRad);

	MA1(2,3)=675.0;
	MA2(0,3)=260.0;
	MA3(0,3)=680.0;
	MA4(0,3)=670.0;
	MA4(2,3)=-35.0;
	MA6(0,3)=115.0;
	temp(0,3)=43;


	resultabc=Cz*Cy*Cx;
	result16=MA1*MA2*MA3*MA4*MA5*MA6*temp;

	D3DXMATRIX temp1(0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,1);

	result16=result16*temp1;
	result16(0,0)*=(-1);
	result16(0,1)*=(-1);
	result16(1,2)*=(-1);
	result16(2,0)*=(-1);
	result16(2,1)*=(-1);

	int stop=99;
}
*/

void CTrackRepeatModule::camCalibration(double x,double y,double z,double pan,double tilt,double roll)
{
	CalculateCameraLocation();

	cameraLocationX=cameraLocationX+x;
	cameraLocationY=cameraLocationY+y;
	cameraLocationZ=cameraLocationZ+z;

	//计算光心旋转矩阵
	D3DXMATRIX Pan;
	D3DXMATRIX Tilt;
	D3DXMATRIX Roll;
	D3DXMATRIX Camera;
	D3DXMatrixRotationX(&Tilt,-1*cameraTilt);
	D3DXMatrixRotationY(&Pan,-1*cameraPan);
	D3DXMatrixRotationZ(&Roll,-1*cameraRoll);
	Camera=Roll*Tilt*Pan;

	//计算校正旋转矩阵
	D3DXMATRIX caliPan;
	D3DXMATRIX caliTilt;
	D3DXMATRIX caliRoll;
	D3DXMATRIX Calibration;
	D3DXMatrixRotationX(&caliTilt,-1*tilt*angleToRad);
	D3DXMatrixRotationY(&caliPan,-1*pan*angleToRad);
	D3DXMatrixRotationZ(&caliRoll,-1*roll*angleToRad);
	Calibration=caliRoll*caliTilt*caliPan;

	//计算偏移后的光心旋转矩阵
	D3DXMATRIX Result;
	Result=Camera*Calibration;

	if(Result(2,0)==0)
	{
		Result(2,0)+=0.0001;
	}
	if(Result(2,2)==0)
	{
		Result(2,2)+=0.0001;
	}
	if(Result(1,1)==0)
	{
		Result(1,1)+=0.0001;
	}

	cameraTilt=atan2(Result(2,1),sqrt((Result(2,0)*Result(2,0))+(Result(2,2)*Result(2,2))));
	cameraRoll=atan2((-Result(0,1)/cos(cameraTilt)),(Result(1,1)/cos(cameraTilt)));
	cameraPan=atan2((-Result(2,0)/cos(cameraTilt)),(Result(2,2)/cos(cameraTilt)));

}





bool CTrackRepeatModule::ObtainRollLockMotionCommandData(robotlocation rbLocation, vector<robotRunCommand> *CommandColl)
{
	//D3DXMATRIX rotationX;
	//D3DXMATRIX rotationY;
	//D3DXMATRIX rotationZ;
	//D3DXMATRIX rotation;
	//D3DXMATRIX cameraRotation;
	//D3DXMATRIX rotationInver;

	//D3DXMATRIX temp;

	//float AngleA, AngleB, AngleC;

	//AngleA = rbLocation.A*angleToRad*(-1);
	//AngleB = rbLocation.B*angleToRad*(-1);
	//AngleC = rbLocation.C*angleToRad*(-1);

	////绕X轴旋转矩阵
	//D3DXMatrixRotationX(&rotationX, AngleC);

	////绕Y轴
	//D3DXMatrixRotationY(&rotationY, AngleB);

	////绕Z轴
	//D3DXMatrixRotationZ(&rotationZ, AngleA);

	////计算工具坐标系到底座的旋转矩阵
	//rotation = rotationZ*rotationY*rotationX;
	//rotation(0, 3) = rbLocation.X;
	//rotation(1, 3) = rbLocation.Y;
	//rotation(2, 3) = rbLocation.Z;

	//D3DXMatrixRotationX(&temp, 0);
	//temp = rotationZ*rotationY*temp;

	//D3DXMatrixInverse(&rotationInver, 0, &rotation);

	//cameraRotation = temp*rotationInver;

	////ZYX顺序
	//cameraPan = atan2(cameraRotation(1, 0), cameraRotation(0, 0));
	//cameraRoll = atan2(cameraRotation(2, 1), cameraRotation(2, 2));
	//cameraTilt = atan2(-1 * cameraRotation(2, 0), cameraRotation(2, 2) / cos(cameraRoll));

	//cameraTilt = cameraTilt / angleToRad;//B
	//cameraRoll = cameraRoll / angleToRad;//C
	//cameraPan = cameraPan / angleToRad;//A
	
	KeyFrame startFrame,endFrame;
	startFrame.A1 = rbLocation.A1;
	startFrame.A2 = rbLocation.A2;
	startFrame.A3 = rbLocation.A3;
	startFrame.A4 = rbLocation.A4;
	startFrame.A5 = rbLocation.A5;
	startFrame.A6 = rbLocation.A6;
	startFrame.E1 = rbLocation.E1;

	//结束位置镜头水平
	endFrame.A1 = rbLocation.A1;
	endFrame.A2 = rbLocation.A2;
	endFrame.A3 = rbLocation.A3;
	endFrame.A4 = 0;
	endFrame.A5 = rbLocation.A5;
	endFrame.A6 = 0;
	endFrame.E1 = rbLocation.E1;
	endFrame.PartSpeed = 0.2;

	vector<robotRunCommand> sendDataCollec = CalculateCommandFrameFromTwoKeyFrame4(startFrame,endFrame, commandNum);
	CommandColl->insert(CommandColl->end(), sendDataCollec.begin(), sendDataCollec.end());

	return true;

}