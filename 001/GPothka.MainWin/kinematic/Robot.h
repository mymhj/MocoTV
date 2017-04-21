#pragma once
#include "transmatrix.h"
#include <iostream>
#include <string>
#include <fstream>
#include <stack>
#include <vector>
#include <map>

using namespace std;

enum CHANNELNAME { XPOSITION, YPOSITION, ZPOSITION, XROTATION, YROTATION, ZROTATION };// Motion type
enum POSITIVEDIREC { ClockWise, AntiClockWise, Right, Left, NODEFINE };               // 旋转关节的正向（ClockWise, AntiClockWise），平移关节的正向（Right, Left）
class CJoint 
{
private:
	string _name;
	double _offsetx, _offsety, _offsetz;//关节坐标系原点在父关节坐标系中的坐标值
	CHANNELNAME	_channel;
	POSITIVEDIREC _positivedirec; //机器人厂家定义关节的正方向
	
	//旋转对应弧度，平移对应米
	double _dlimit;         //运动范围的下限
	double _ulimit;         //运动范围的下限
	double _position;       //关节当前位置
	CTransMatrix _trans;    //关节相对父关节坐标系的变换矩阵

	CJoint*	_son;           //关节的孩子，最多有一个孩子

public:
	CJoint(string name):_name(name),_positivedirec(NODEFINE),_offsetx(0),_offsety(0),_offsetz(0),_dlimit(0),_ulimit(0),_position(0),_son(NULL){}

	// assign operator
	CJoint& operator =(const CJoint& joint)
	{
		_name = joint._name;

		_offsetx = joint._offsetx;
		_offsety = joint._offsety;
		_offsetz = joint._offsetz;

		_channel = joint._channel;
		
		_positivedirec = joint._positivedirec;

		_dlimit = joint._dlimit;
		_ulimit = joint._ulimit;

		_position = joint._position;

		_trans = joint._trans;

		// 指向孩子的指针为空，需要通过函数SetSon重新赋值
		_son = NULL;

		return *this;
	}


	string GetName(void){ return _name; }
	
	//读取成员数据
	CHANNELNAME GetChannelName(void){ return _channel; }
	POSITIVEDIREC GetPositiveDirec(void) { return _positivedirec;}
	CVector3 GetOffset(void){ return CVector3(_offsetx, _offsety, _offsetz); }
    double GetPosition(void){ return _position; }
	double GetDownLimit(void){ return _dlimit; }
	double GetUpLimit(void){ return _ulimit; }
	CJoint* GetSon(void){ return _son; } 
	CTransMatrix GetTransMatrix(void){ return _trans; }

	//设置成员数据
	void SetName(string name){ _name = name; }
	void SetChannelName(CHANNELNAME channel){ _channel = channel;}
	void SetPositiveDirec(POSITIVEDIREC positivedirec) { _positivedirec = positivedirec; }
	void SetOffset(CVector3 offset){ _offsetx = offset.GetX(); _offsety = offset.GetY(); _offsetz = offset.GetZ();}
   	void SetDownLimit(double limit){ _dlimit = limit;}
	void SetUpLimit(double limit){ _ulimit = limit;}
	void SetSon(CJoint* son){ _son = son; }


	bool ComputeTransMatrix(void);   //根据关节当前位置和关节运动类型，计算关节的变换矩阵_trans
	bool setPosition( double pos );  //设置关节的位置数据
};

class Robot
{	
private:
	string _version; //机器人模型文件的版本号
	string _model;   //机器人类型
	
	//File stream
	ifstream _infile;
	ofstream _outfile;

	//Used for parse the skeleton part 
	vector<CJoint *>	_index;

	//关节名称到关节指针在_index中下标的映射表
	map<string, int>    _name2index;

	//Store the data
	CJoint *			_figure; //指向根关节		
	
	//Get a token from _infile 
	string GetToken();

	//Release the memory allocate for store joint's data
	void Release();

	//Parse the data file
	bool Parse();

	//Generate the robot file 
	void Generate();
	void GenerateHelper( int level, CJoint *joint );
	
	//read data of a joint
	bool ReadJoint( CJoint *joint);

public:
	Robot(void);
	~Robot(void);

	Robot& operator =(const Robot& robot) ;    // assign operator

	// Import & Export
	bool Import( string fname );
	bool Export( string fname );

	//建立关节名称与其在_index位置（下标索引）之间的映射表
	void BuildingMap(void);

	//设置各个关节的运动数据，并更新变换矩阵
	bool SetJointsMotionData( map<string, double> jointData );

	//计算从坐标系coor1到坐标系coor2的坐标变换,如果coord1或者coord2 为 'world'，其表示的是世界坐标系
	bool TransMatrixBetweenTwoCoordinates( string coor1, string coor2, CTransMatrix& matrix);

	//获取机器人关节的列表，其中关节顺序的排列是[根关节,子关节,子子关节,...]
	void GetJointsList(vector<CJoint>& jointslist);
};

