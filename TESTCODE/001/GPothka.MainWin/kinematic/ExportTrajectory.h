#pragma once


#include <assert.h>
#include "Robot.h"

enum SOFTWARE { MAYA, MAX };// software type

struct CameraPose
{
public:
	double m_Focus;          // 摄像机焦距
	double m_Tx, m_Ty, m_Tz; // 平移的三个分量
	double m_Ex, m_Ey, m_Ez; // 旋转的三个分量：欧拉角
	
	CameraPose(void)
	{
		m_Tx = 0;
		m_Ty = 0;
		m_Tz = 0;
		m_Ex = 0;
		m_Ey = 0;
		m_Ez = 0;
		
		m_Focus = 0;
	}

	void SetPose( double tx, double ty, double tz, double ex, double ey, double ez, double focus )
	{
		m_Tx = tx;
		m_Ty = ty;
		m_Tz = tz;
		m_Ex = ex;
		m_Ey = ey;
		m_Ez = ez;

		m_Focus = focus;
	}
};

class ExportTrajectory
{
public:
	ExportTrajectory(void);
	~ExportTrajectory(void);
	
	//设置机器人模型
	void SetRobotModel( Robot& robot )
	{
		m_Robot = robot;
	}
	
	/*设置机器人的关节运动数据，文件inFile打开失败，或者读入数据不匹配，返回false，否则返回true

	  inFile的文件格式（这是为了迁就冯慧箐的输出数据格式，后续要修改，例如去掉E2、去掉X、Y、Z、A、B、C等数据）
	  -----------------------------------------------------------------------------------------
	  %% KUKA机器人数据文件 1.0版本
      %% 运动数据单位：平移（毫米）；旋转（度）；
      %% 奥视佳系统内部度量单位约定： 长度（米）；旋转（弧度）；在数据转换时注意单位变换
      %% VERSION     版本标识符
      %% NumFrames   运动数据总帧数
      %% 数据格式定义：时间戳  A1  A2  A3  A4  A5  A6  E1
	  
	  例如：
      VERSION  1.0
      NumFrames  100  
      4215659280 -18.7571 -38.0038 29.3743 166.0869 62.7735 41.7330 1094.5068 
	  -----------------------------------------------------------------------------------------
	*/
	bool SetRobotMotionData( string inFile );

	//通过字符串分解获取机器人运动数据
	bool ExportTrajectory::SetRobotMotionDataFromString(string robotA1A6Data);

	//按照导出软件的类型，计算摄像机的姿态数据
	bool CalculateCameraPose( SOFTWARE type, CameraPose& camPose);

	/*按照导出软件的类型，向文件中写入转换后的摄像机姿态数据
	  num的含义是导出帧数: num > 0 ，如果num小于输入文件所包含的帧数，那么将前num帧的摄像机姿态数据导出，否则将输入文件的所有帧数均导出。
	  noise的含义是附加给运动数据的噪声，单位为度，目前对于E1轴的平移运动不添加噪声
	*/
	bool Export( SOFTWARE type, string fileName, int num,double noise = 0.0  );

	/*将奥视佳摄像机局部坐标系相对奥视佳世界坐标系的值写入文件
	  文件格式
	  -----------------------------------------------------------------------------------------
	  第一行：摄像机光心在世界坐标系中的坐标值 ：X Y Z
      第二行：旋转矩阵的第3列（局部坐标系的Z轴在世界坐标系上投影）
      第三行：旋转矩阵的第1列（局部坐标系的X轴在世界坐标系上投影）
	  第四行：***************************************************
	  -----------------------------------------------------------------------------------------

	  num的含义是导出帧数: num > 0 ，如果num小于输入文件所包含的帧数，那么将前num帧的摄像机姿态数据导出，否则将输入文件的所有帧数均导出。
	  noise的含义是附加给运动数据的噪声，单位为度，目前对于E1轴的平移运动不添加噪声
	
	 该函数是为调试用（通过OPGL与MAYA之间的对比，以判断程序的正确性）
	*/
	bool ExportCamerapose( string fileName, int num, double noise = 0.0 );

	//设置摄像机局部坐标系原点相对云台局部坐标系的初始变换矩阵
	bool SetCameraInitialPose( CTransMatrix pose )
	{
		if( pose.GetRotate().isOrthogonal() == true ) 
		{
			m_CameraInitialPose = pose;
			return true;
		}
		else
		{
			assert(0);
			return false;
		}
	}

	// 返回KUKA机器人运动数据的总帧数
	int GetMotionSize( void )
	{
		assert( m_Times.size() > 0 );
		return m_Times.size();
	}

	// freq:重新插值的频率，例如25帧/秒
	bool Interpolation( double freq );

private:
	// 保存机器人模型
	Robot m_Robot; 
	
	//保存输入的KUKA机器人运动数据
	vector<long long> m_Times;
	vector<double> m_A1, m_A2, m_A3, m_A4, m_A5, m_A6, m_E1;
    
	// 摄像机局部坐标系原点相对云台局部坐标系的初始变换矩阵
	CTransMatrix m_CameraInitialPose;


	// y(x) = ( ( y1 - y0 )/( x1 - x0 ) ) * ( x - x0 ) + y0
	double _Interpolation( double x0, double x1, double y0, double y1, double x )
	{
		assert( x1 > x0 );
		assert( x0 <= x && x1 >= x );
		return ( ( y1 - y0 )/( x1 - x0 ) ) * ( x - x0 ) + y0;
	}
};

