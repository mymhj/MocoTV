#pragma once
#include <vector>
#include <map>



//轨迹重复对象
class CTrackRepeatModule
{
public:
	CTrackRepeatModule(void);
	~CTrackRepeatModule(void);

	
	//关键帧集合仓库
	static map<string,vector<KeyFrame>> m_TrackKeyFrameDepository;

	//关键帧集合在仓库中的索引
	static string m_TrackIndex;

	//当前聚焦的关键帧索引
	static int m_keyFrameIndex;

	
	//重复运动各帧前后修改指令数目
	static float commandNum;

	//关键帧集合
	static vector<KeyFrame> m_KeyFrameColl;

	//轨迹重复指令针集合
	static vector<char*> m_TrackRepeatCommandData;

	//复位指令集合
	static vector<char*> m_RestorationCommandData;

	//单步指令集合
	static vector<char*> m_SingleStepCommandData;


	//获得指定轨迹索引的的关键帧集合
	vector<KeyFrame> GetKeyFrameCollToIndexTrack(string indexTrack);

	//创建一个新的轨迹关键帧集合
	bool AddNewTrackToIndexTrack(string indexTrack);

	//删除指定索引的轨迹关键帧集合
	bool DeleteTrackToIndexTrack(string indexTrack);

	//添加  关键帧
	bool AddKeyFrame(robotlocation rbLocation,string indexTrack);
	bool AddKeyFrame(robotlocation rbLocation,string indexTrack,int waitTime,float partSpeed);

	//插入  关键帧
	bool InsertKeyFrame(robotlocation rbLocation, string indexTrack, int m_nIndex, int waitTime, float partSpeed);

	//删除 指定索引的关键帧
	bool DeleteKeyFrame(int keyFrameIndex,string indexTrack);

	//更新 更新指定索引的关键帧
	bool UpdateKeyFrame(robotlocation rbLocation,int keyFrameIndex,string indexTrack);
	bool UpdateKeyFrame(robotlocation rbLocation,int keyFrameIndex,string indexTrack,int waitTime,float partSpeed);
	bool UpdataKeyTime(int keyFrameIndex,string indexTrack,float runTime,float waitTime);

	//更新 更新指定索引的关键帧名称
	bool UpdataKeyName(int keyFrameIndex, string indexTrack, CString name);

	//导入本地关键帧
	bool LoadLocalKeyFrames(char* filePath,string keyFrameCollIndex);

	//导出  关键帧序列
	bool SaveKeyFramesToLocal(string saveFullPath,string keyFrameCollIndex);

	//单步 获得单步运行指令
	bool ObtainSingleMotionCommandData(robotlocation rbLocation,int keyFrameIndex,string indexTrack,vector<robotRunCommand> *CommandColl);

	//复位 获得复位运动指令
	bool ObtainRestorationMotionCommandData(robotlocation rbLocation,string indexTrack,vector<robotRunCommand> *CommandColl);

	//重复  获取轨迹重复指令
	bool ObtainRepeatMotionCommandData(string indexTrack,vector<robotRunCommand> *CommandColl);

	//Roll锁定镜头调水平
	bool ObtainRollLockMotionCommandData(robotlocation rbLocation,vector<robotRunCommand> *CommandColl);

	//镜头复位和轨迹重复
	//WORD ObtainCameraRestorationMotionCommandData(string indexTrack);
	//bool ObtainCameraRepeatorationMotionCommandData(string indexTrack);
	
	//CR曲线
	bool ObtainCubicSplineCommandData(string indexTrack);
	bool SendCubicSplineCommandData(string indexTrack, vector<robotRunCommand> *CommandColl);

	//三次B样条曲线
	bool ObtainCubicBSplineCommandData(string indexTrack,vector<robotRunCommand> *CommandColl);


	///通过两个关帧针计算机器人运动指令
	vector<robotRunCommand> CalculateCommandFrameFromTwoKeyFrame3(KeyFrame startFrame,KeyFrame endFrame);
	vector<robotRunCommand> CalculateCommandFrameFromTwoKeyFrame4(KeyFrame startFrame,KeyFrame endFrame,float comdnum);

	//优化后轨迹重复时关键帧之间的差值算法
	vector<robotRunCommand> CalculateCommandFrameFromTwoKeyFrame5(KeyFrame startFrame, KeyFrame endFrame,bool lastFrame);//lastFrame用于判断是够是轨迹最后一帧

	//设置关键帧运行时间
	double setFrameRunTime(KeyFrame startFrame,KeyFrame endFrame);
	//计算每帧运行时间
	int frameRunTime(KeyFrame startFrame,KeyFrame endFrame);
	//计算每帧运行最小时间
	bool frameMinTime(string indexTrack);

	//计算摄像机光心位置
	void CalculateCameraLocation();

	void test();

	//虚拟相机校准
	void camCalibration(double x,double y,double z,double pan,double tilt,double roll);

	bool SpeedBuffer(vector<robotRunCommand>* sendDataEntityColl, int Sum);

	static map<string,string> m_TrackDataPath;
	
	GP_Lens_Fujinon m_Lens_Fujinon;
};

