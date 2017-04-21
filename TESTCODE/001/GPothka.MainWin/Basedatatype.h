#pragma once

//机器人运动控制指令数据结构
typedef struct robotRunCommand
{
	float X;
	float Y;
	float Z;
	float A;
	float B;
	float C;
	float A1;
	float A2;
	float A3;
	float A4;
	float A5;
	float A6;
	float E1;
	float E2;
	unsigned int Di0;
	unsigned int IPOC;
public:
	robotRunCommand()
	{
	  memset(this,0,sizeof(*this));
	}
 
};

//机器人位置信息数据结构
typedef struct robotlocation
{
	float X;
	float Y;
	float Z;
	float A;
	float B;
	float C;
	float A1;
	float A2;
	float A3;
	float A4;
	float A5;
	float A6;
	float E1;
	float E2;
	float E3;
	float E4;
	float E5;
	float E6;
	unsigned int Delay;
	unsigned int IPOC;

public:
	robotlocation(){
		memset(this, 0, sizeof(*this));
	}
};

typedef struct limits
{
	double upLimit;   // 上限
	double downLimit; // 下限
};


//关键帧数据结构
typedef struct  KeyFrame
{
	unsigned int IPOC;
	float X;
	float Y;
	float Z;
	float A;
	float B;
	float C;
	float A1;
	float A2;
	float A3;
	float A4;
	float A5;
	float A6;
	float E1;
	float E2;
	unsigned int WaitTime;
	float PartSpeed;

	string name;

	float FrameMinTime;
	float FrameRunTime;
	float kk;

	WORD Zoom;
	WORD Focus;

public:
	KeyFrame()
	{
		memset(this,0,sizeof(*this));
	}
};


//控制状态-状态机
typedef struct RunState{
	bool    normal;    //正常
	bool    Suspend;   //悬停
	bool    WaitState; //等待
	bool    RestorationState;      //复位状态
	bool    SingleStepState;      //单步运动
	bool    TrackRepeatState;     //轨迹重复
	RunState(){
		normal=false;
		Suspend=false;
		WaitState=false;
		RestorationState=false;
		SingleStepState=false;
		TrackRepeatState=false;
	};

};


//字符串比较
struct ltstr
{
	bool operator()(const char* s1, const char* s2) const
	{
		return strcmp(s1, s2) < 0;
	}
};


