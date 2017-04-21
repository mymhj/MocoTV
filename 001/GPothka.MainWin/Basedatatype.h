#pragma once

//�������˶�����ָ�����ݽṹ
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

//������λ����Ϣ���ݽṹ
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
	double upLimit;   // ����
	double downLimit; // ����
};


//�ؼ�֡���ݽṹ
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


//����״̬-״̬��
typedef struct RunState{
	bool    normal;    //����
	bool    Suspend;   //��ͣ
	bool    WaitState; //�ȴ�
	bool    RestorationState;      //��λ״̬
	bool    SingleStepState;      //�����˶�
	bool    TrackRepeatState;     //�켣�ظ�
	RunState(){
		normal=false;
		Suspend=false;
		WaitState=false;
		RestorationState=false;
		SingleStepState=false;
		TrackRepeatState=false;
	};

};


//�ַ����Ƚ�
struct ltstr
{
	bool operator()(const char* s1, const char* s2) const
	{
		return strcmp(s1, s2) < 0;
	}
};


