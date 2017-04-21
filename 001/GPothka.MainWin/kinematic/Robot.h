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
enum POSITIVEDIREC { ClockWise, AntiClockWise, Right, Left, NODEFINE };               // ��ת�ؽڵ�����ClockWise, AntiClockWise����ƽ�ƹؽڵ�����Right, Left��
class CJoint 
{
private:
	string _name;
	double _offsetx, _offsety, _offsetz;//�ؽ�����ϵԭ���ڸ��ؽ�����ϵ�е�����ֵ
	CHANNELNAME	_channel;
	POSITIVEDIREC _positivedirec; //�����˳��Ҷ���ؽڵ�������
	
	//��ת��Ӧ���ȣ�ƽ�ƶ�Ӧ��
	double _dlimit;         //�˶���Χ������
	double _ulimit;         //�˶���Χ������
	double _position;       //�ؽڵ�ǰλ��
	CTransMatrix _trans;    //�ؽ���Ը��ؽ�����ϵ�ı任����

	CJoint*	_son;           //�ؽڵĺ��ӣ������һ������

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

		// ָ���ӵ�ָ��Ϊ�գ���Ҫͨ������SetSon���¸�ֵ
		_son = NULL;

		return *this;
	}


	string GetName(void){ return _name; }
	
	//��ȡ��Ա����
	CHANNELNAME GetChannelName(void){ return _channel; }
	POSITIVEDIREC GetPositiveDirec(void) { return _positivedirec;}
	CVector3 GetOffset(void){ return CVector3(_offsetx, _offsety, _offsetz); }
    double GetPosition(void){ return _position; }
	double GetDownLimit(void){ return _dlimit; }
	double GetUpLimit(void){ return _ulimit; }
	CJoint* GetSon(void){ return _son; } 
	CTransMatrix GetTransMatrix(void){ return _trans; }

	//���ó�Ա����
	void SetName(string name){ _name = name; }
	void SetChannelName(CHANNELNAME channel){ _channel = channel;}
	void SetPositiveDirec(POSITIVEDIREC positivedirec) { _positivedirec = positivedirec; }
	void SetOffset(CVector3 offset){ _offsetx = offset.GetX(); _offsety = offset.GetY(); _offsetz = offset.GetZ();}
   	void SetDownLimit(double limit){ _dlimit = limit;}
	void SetUpLimit(double limit){ _ulimit = limit;}
	void SetSon(CJoint* son){ _son = son; }


	bool ComputeTransMatrix(void);   //���ݹؽڵ�ǰλ�ú͹ؽ��˶����ͣ�����ؽڵı任����_trans
	bool setPosition( double pos );  //���ùؽڵ�λ������
};

class Robot
{	
private:
	string _version; //������ģ���ļ��İ汾��
	string _model;   //����������
	
	//File stream
	ifstream _infile;
	ofstream _outfile;

	//Used for parse the skeleton part 
	vector<CJoint *>	_index;

	//�ؽ����Ƶ��ؽ�ָ����_index���±��ӳ���
	map<string, int>    _name2index;

	//Store the data
	CJoint *			_figure; //ָ����ؽ�		
	
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

	//�����ؽ�����������_indexλ�ã��±�������֮���ӳ���
	void BuildingMap(void);

	//���ø����ؽڵ��˶����ݣ������±任����
	bool SetJointsMotionData( map<string, double> jointData );

	//���������ϵcoor1������ϵcoor2������任,���coord1����coord2 Ϊ 'world'�����ʾ������������ϵ
	bool TransMatrixBetweenTwoCoordinates( string coor1, string coor2, CTransMatrix& matrix);

	//��ȡ�����˹ؽڵ��б����йؽ�˳���������[���ؽ�,�ӹؽ�,���ӹؽ�,...]
	void GetJointsList(vector<CJoint>& jointslist);
};

