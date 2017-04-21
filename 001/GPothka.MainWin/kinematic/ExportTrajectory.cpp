#include "stdafx.h"
#include "ExportTrajectory.h"
#include <iosfwd>
#include <sstream>

ExportTrajectory::ExportTrajectory(void)
{
}


ExportTrajectory::~ExportTrajectory(void)
{
}

bool ExportTrajectory::SetRobotMotionData( string inFile )
{
	ifstream infile(inFile );
	if( !infile.is_open() )
	{
		assert(0);
		return false;
	}
	
	//清空数据存储区
	m_Times.clear();
	m_A1.clear();
	m_A2.clear();
	m_A3.clear();
	m_A4.clear();
	m_A5.clear();
	m_A6.clear();
	m_E1.clear();


	string container;  
	// 读入注释行
	while( true )
	{
		infile >> container;
		if( container != "%%" ) 
		{
			break;
		}
		else
		{
			//infile.getline();
			std::getline(infile, container );
			container.clear();
		}
	}

	// 读入版本信息
	double version = -1;
	if( container != "VERSION")
	{
		assert(0);
		return false;
	}
	infile >> version;

	// 读入总帧数
	long long numFrames = 0;
	infile >> container;
	if( container != "NumFrames")
	{
		assert(0);
		return false;
	}
	infile >> numFrames;
	
	// 读入运动数据
	long long time;
	double motionData; 
	for( long long i = 0; i < numFrames; ++i )
	{
		if( infile.eof() == true )
		{
			assert(0);
			return false;
		}

		infile >> time;
		m_Times.push_back( time );

		infile >> motionData;
		m_A1.push_back( motionData );

		infile >> motionData;
		m_A2.push_back( motionData );

		infile >> motionData;
		m_A3.push_back( motionData );

		infile >> motionData;
		m_A4.push_back( motionData );

		infile >> motionData;
		m_A5.push_back( motionData );

		infile >> motionData;
		m_A6.push_back( motionData );

		infile >> motionData;
		m_E1.push_back( motionData );
	}

	return true;
}

bool ExportTrajectory::SetRobotMotionDataFromString(string robotA1A6Data)
{
	std::stringstream rStream(robotA1A6Data);

	//清空数据存储区
	m_Times.clear();
	m_A1.clear();
	m_A2.clear();
	m_A3.clear();
	m_A4.clear();
	m_A5.clear();
	m_A6.clear();
	m_E1.clear();


	string container;  
	// 读入注释行
	while( true )
	{
		rStream >> container;
		if( container != "%%" ) 
		{
			break;
		}
		else
		{
			//infile.getline();
			std::getline(rStream, container );
			container.clear();
		}
	}

	//读入版本信息
		double version = -1;
	if( container != "VERSION")
	{
		assert(0);
		return false;
	}
	rStream >> version;

	// 读入总帧数
	long long numFrames = 0;
	rStream >> container;
	if( container != "NumFrames")
	{
		assert(0);
		return false;
	}
	rStream >> numFrames;

	// 读入运动数据
	long long time;
	double motionData; 
	for( long long i = 0; i < numFrames; ++i )
	{
		if( rStream.eof() == true )
		{
			assert(0);
			return false;
		}

		rStream >> time;
		m_Times.push_back( time );

		rStream >> motionData;
		m_A1.push_back( motionData );

		rStream >> motionData;
		m_A2.push_back( motionData );

		rStream >> motionData;
		m_A3.push_back( motionData );

		rStream >> motionData;
		m_A4.push_back( motionData );

		rStream >> motionData;
		m_A5.push_back( motionData );

		rStream >> motionData;
		m_A6.push_back( motionData );

		rStream >> motionData;
		m_E1.push_back( motionData );
	}

	return true;
}

bool ExportTrajectory::ExportCamerapose( string fileName, int num, double noise )
{
	assert( num > 0 );

	// 打开输出文件
	ofstream outfile( fileName );
	if( !outfile.is_open() )
	{
		assert(0);
		return false;
	}

	int size = m_Times.size();		
	map<string, double> jointData;
	string bk = " ";
	
	// 修改输出帧数
	if ( num > 0 &&  num < size )
	{
		size = num; // 将前num帧的摄像机姿态数据导出
	}

	// 计算奥视佳摄像机相对奥视佳世界坐标系的变换矩阵
	for( int i = 0; i < size; ++i )
	{
		jointData["E1"] = m_E1[i]/1000.0;               // 毫米 --> 米  ：KUKA系统默认的长度单位是毫米

		jointData["A1"] = ( m_A1[i] + noise ) * ( 3.1415 / 180.0 ); // 度   --> 弧度：KUKA系统默认的角度单位是度
		jointData["A2"] = ( m_A2[i] + noise ) * ( 3.1415 / 180.0 ); // 度   --> 弧度：KUKA系统默认的角度单位是度
		jointData["A3"] = ( m_A3[i] + noise ) * ( 3.1415 / 180.0 ); // 度   --> 弧度：KUKA系统默认的角度单位是度
		jointData["A4"] = ( m_A4[i] + noise ) * ( 3.1415 / 180.0 ); // 度   --> 弧度：KUKA系统默认的角度单位是度
		jointData["A5"] = ( m_A5[i] + noise ) * ( 3.1415 / 180.0 ); // 度   --> 弧度：KUKA系统默认的角度单位是度
		jointData["A6"] = ( m_A6[i] + noise ) * ( 3.1415 / 180.0 ); // 度   --> 弧度：KUKA系统默认的角度单位是度
				
		// 设置机器人的关节运动数据，更新旋转矩阵
		bool isleagl = m_Robot.SetJointsMotionData( jointData );
		if( isleagl == false )
		{
			assert( 0 );
			return false;
		}

		// 计算奥视佳摄像机坐标系到奥视佳世界坐标系的变换矩阵
		CTransMatrix tran;
			 
		// 计算A6轴局部坐标系相对于世界坐标系的变换
		m_Robot.TransMatrixBetweenTwoCoordinates( "A6", "world", tran );
			 
		// 加上奥视佳摄像机局部坐标系相对于A6轴局部坐标系的变换
		tran = tran * m_CameraInitialPose;

		// 将摄像机光心位置写入文件
		CVector3 translate = tran.GetTranslate();
		outfile << translate.GetX()*1000.0 << bk 
			    << translate.GetY()*1000.0 << bk 
				<< translate.GetZ()*1000.0 << endl; 
		
		// 将摄像机光轴方向的单位射线写入文件：旋转矩阵的第3列（局部坐标系的Z轴在世界坐标系上投影）
		RotMatrix rot = tran.GetRotate();
		outfile << rot.GetElement( 0, 2 ) << bk
			    << rot.GetElement( 1, 2 ) << bk
			    << rot.GetElement( 2, 2 ) << endl;

		// 将摄像机head方向的单位射线写入文件：旋转矩阵的第1列（局部坐标系的X轴在世界坐标系上投影）
		outfile << rot.GetElement( 0, 0 ) << bk
			    << rot.GetElement( 1, 0 ) << bk
			    << rot.GetElement( 2, 0 ) << endl;

		outfile << "***********************************"<< endl;
	}

	return true;
}

bool ExportTrajectory::Export( SOFTWARE type, string fileName, int num, double noise )
{
	vector<CameraPose> CameraPoseList;
	CameraPose cpose;
	int size = m_Times.size();		
	map<string, double> jointData;

	// 修改输出帧数
	if ( num > 0 &&  num < size )
	{
		size = num; // 将前num帧的摄像机姿态数据导出
	}

	switch(type)
	{
	case MAYA:
		{	
			// 计算摄像机在MAYA类型下的姿态数据
			for( int i = 0; i < size; ++i )
			{
				jointData["E1"] = m_E1[i]/1000.0;               // 毫米 --> 米  ：KUKA系统默认的长度单位是毫米

				jointData["A1"] = ( m_A1[i] + noise ) * ( 3.1415 / 180.0 ); // 度   --> 弧度：KUKA系统默认的角度单位是度
		        jointData["A2"] = ( m_A2[i] + noise ) * ( 3.1415 / 180.0 ); // 度   --> 弧度：KUKA系统默认的角度单位是度
		        jointData["A3"] = ( m_A3[i] + noise ) * ( 3.1415 / 180.0 ); // 度   --> 弧度：KUKA系统默认的角度单位是度
		        jointData["A4"] = ( m_A4[i] + noise ) * ( 3.1415 / 180.0 ); // 度   --> 弧度：KUKA系统默认的角度单位是度
		        jointData["A5"] = ( m_A5[i] + noise ) * ( 3.1415 / 180.0 ); // 度   --> 弧度：KUKA系统默认的角度单位是度
		        jointData["A6"] = ( m_A6[i] + noise ) * ( 3.1415 / 180.0 ); // 度   --> 弧度：KUKA系统默认的角度单位是度


				//jointData["A1"] = m_A1[i] * ( 3.1415 / 180.0 ); // 度   --> 弧度：KUKA系统默认的角度单位是度
				//jointData["A2"] = m_A2[i] * ( 3.1415 / 180.0 ); // 度   --> 弧度：KUKA系统默认的角度单位是度
				//jointData["A3"] = m_A3[i] * ( 3.1415 / 180.0 ); // 度   --> 弧度：KUKA系统默认的角度单位是度
				//jointData["A4"] = m_A4[i] * ( 3.1415 / 180.0 ); // 度   --> 弧度：KUKA系统默认的角度单位是度
				//jointData["A5"] = m_A5[i] * ( 3.1415 / 180.0 ); // 度   --> 弧度：KUKA系统默认的角度单位是度
				//jointData["A6"] = m_A6[i] * ( 3.1415 / 180.0 ); // 度   --> 弧度：KUKA系统默认的角度单位是度
				
				// 设置机器人的关节运动数据，更新旋转矩阵
				bool isleagl = m_Robot.SetJointsMotionData( jointData );
				if( isleagl == false )
				{
					assert( 0 );
					return false;
				}
				
				// 计算摄像机姿态数据
				CalculateCameraPose( type, cpose );

				cpose.m_Focus = 20; // 这里暂时默认为20毫米
				
				// 保存摄像机姿态数据
				CameraPoseList.push_back( cpose );
			}
			
			// 打开输出文件
			ofstream outfile( fileName );
			if( !outfile.is_open() )
			{
				assert(0);
				return false;
			}

			string bk = " ";
	        //文件头 
	        outfile << "requires maya \"2009\";" << endl;
	        outfile << "currentUnit -l centimeter -a degree -t pal;" << endl; // 长度单位：厘米，角度单位：度
            outfile << "createNode transform -n \"OthkaCameraNode\";" << endl;
	        outfile << "createNode camera -n \"OthkaCamera\" -p \"OthkaCameraNode\";" << endl;

			//写入焦距数据
	        outfile << "createNode animCurveTU -n \"CameraFocalLengthCurve\";" << endl;
	        outfile << "    setAttr -s " << size << " \".ktv[0:" << ( size - 1 ) << "]\" ";
	        for( int i = 0; i < size; ++i )
			{
				outfile << bk << ( i + 1 ) << bk << CameraPoseList[i].m_Focus;
			}
	        outfile << ";" << endl;
	      
			//写入摄像机X方向位移数据
			outfile << "createNode animCurveTL -n \"CameraPositionXCurve\";" << endl;
			outfile << "    setAttr -s " << size << " \".ktv[0:" << ( size - 1 ) << "]\" ";
			for( int i = 0; i < size; ++i )
			{
				outfile << bk << ( i + 1 ) << bk << ( CameraPoseList[i].m_Tx * 100 ); // 米-->厘米
			}
			outfile << ";"<<endl;
	   
			//写入摄像机Y方向位移数据
			outfile << "createNode animCurveTL -n \"CameraPositionYCurve\";" << endl;
			outfile << "    setAttr -s " << size << " \".ktv[0:" << ( size - 1 ) << "]\" ";
			for( int i = 0; i < size; ++i )
			{
				outfile << bk << ( i + 1 ) << bk << ( CameraPoseList[i].m_Ty * 100 ); // 米-->厘米
			}
			outfile << ";"<<endl;

			//写入摄像机Z方向位移数据
			outfile << "createNode animCurveTL -n \"CameraPositionZCurve\";" << endl;
			outfile << "    setAttr -s " << size << " \".ktv[0:" << ( size - 1 ) << "]\" ";
			for( int i = 0; i < size; ++i )
			{
				outfile << bk << ( i + 1 ) << bk << ( CameraPoseList[i].m_Tz * 100 ); // 米-->厘米
			}
			outfile << ";"<<endl;

			//写入摄像机绕X轴的旋转数据
			outfile << "createNode animCurveTA -n \"CameraAngleXCurve\";" << endl;
			outfile << "    setAttr -s " << size << " \".ktv[0:" << ( size - 1 ) << "]\" ";
			for( int i = 0; i < size; ++i )
			{
				outfile << bk << ( i + 1 ) << bk << ( CameraPoseList[i].m_Ex * (180/3.1415) );// 弧度-->度
			}
			outfile << ";" << endl;

			//写入摄像机绕Y轴的旋转数据
			outfile << "createNode animCurveTA -n \"CameraAngleYCurve\";" << endl;
			outfile << "    setAttr -s " << size << " \".ktv[0:" << ( size - 1 ) << "]\" ";
			for( int i = 0; i < size; ++i )
			{
				outfile << bk << ( i + 1 ) << bk << ( CameraPoseList[i].m_Ey * (180/3.1415) );// 弧度-->度
			}
			outfile << ";" << endl;


			//写入摄像机绕Z轴的旋转数据
			outfile << "createNode animCurveTA -n \"CameraAngleZCurve\";" << endl;
			outfile << "    setAttr -s " << size << " \".ktv[0:" << ( size - 1 ) << "]\" ";
			for( int i = 0; i < size; ++i )
			{
				outfile << bk << ( i + 1 ) << bk << ( CameraPoseList[i].m_Ez * (180/3.1415) );// 弧度-->度
			}
			outfile << ";" << endl;


			outfile << "connectAttr \"CameraFocalLengthCurve.o\" \"OthkaCamera.fl\";" << endl;
			outfile << "connectAttr \"CameraPositionXCurve.o\" \"OthkaCameraNode.tx\";" << endl;
			outfile << "connectAttr \"CameraPositionYCurve.o\" \"OthkaCameraNode.ty\";" << endl;
			outfile << "connectAttr \"CameraPositionZCurve.o\" \"OthkaCameraNode.tz\";" << endl;
			outfile << "connectAttr \"CameraAngleXCurve.o\" \"OthkaCameraNode.rx\";" << endl;
			outfile << "connectAttr \"CameraAngleYCurve.o\" \"OthkaCameraNode.ry\";" << endl;
			outfile << "connectAttr \"CameraAngleZCurve.o\" \"OthkaCameraNode.rz\";" << endl;
			break;
		}
	default:
		{
			assert(0);
			return false;
		}
	}


	return true;
}

// 按照导出软件的类型，计算摄像机的姿态数据
bool ExportTrajectory::CalculateCameraPose( SOFTWARE type, CameraPose& camPose )
{
	switch(type)
	{
	case MAYA:
		 {
			 // 四步计算法
			 
			 // step 1: 计算Maya摄像机坐标系到奥视佳摄像机坐标系的变换矩阵
			 RotMatrix rot;
			 rot.SetElement(0,0,0);  rot.SetElement(0,1,1);  rot.SetElement(0,2,0);
			 rot.SetElement(1,0,1);  rot.SetElement(1,1,0);  rot.SetElement(1,2,0);
			 rot.SetElement(2,0,0);  rot.SetElement(2,1,0);  rot.SetElement(2,2,-1);
			 
			 CVector3 vec;         // 零矢量

			 CTransMatrix tran1;
			 tran1.SetRotate(rot);    // 设置旋转矩阵
			 tran1.SetTranslate(vec); // 设置零平移矢量，注意验证一下
			  
			 // step 2: 计算奥视佳摄像机坐标系到奥视佳世界坐标系的变换矩阵
			 CTransMatrix tran2;
			 
			 // 计算A6轴局部坐标系相对于世界坐标系的变换
			 m_Robot.TransMatrixBetweenTwoCoordinates("A6","world",tran2);
			 
			 // 加上奥视佳摄像机局部坐标系相对于A6轴局部坐标系的变换
			 tran2 = tran2*m_CameraInitialPose;

			 // step 3: 计算奥视佳世界坐标系到Maya世界坐标系的变换矩阵
			 rot.SetElement(0,0,1);  rot.SetElement(0,1,0);  rot.SetElement(0,2,0);
			 rot.SetElement(1,0,0);  rot.SetElement(1,1,0);  rot.SetElement(1,2,1);
			 rot.SetElement(2,0,0);  rot.SetElement(2,1,-1);  rot.SetElement(2,2,0);

			 CTransMatrix tran3;
			 tran3.SetRotate(rot);    // 设置旋转矩阵
			 tran3.SetTranslate(vec); // 设置零平移矢量，注意验证一下

			 // step 4: 计算Maya摄像机坐标系到Maya世界坐标系的变换矩阵 
			 CTransMatrix tran4;
			 tran4 = tran3 * tran2 * tran1;

			 // 由于CTransMatrix不支持转置运算，因此将旋转矩阵（RotMatrix：m_R）从tran4中抽取出来，然后再做转置运算
			 rot = tran4.GetRotate();
			 rot = rot.GetTranspose();
			
			 
			 // 计算相机的朝向数据：三个欧拉角
			 CVector3 euler = rot.ToEulerAngleByRight( XYZ );

			 // 设置相机的朝向数据
			 camPose.m_Ex = euler.GetX();
			 camPose.m_Ey = euler.GetY();
			 camPose.m_Ez = euler.GetZ();

			 // 设置相机的位移数据
			 CVector3 translate = tran4.GetTranslate();
			 camPose.m_Tx = translate.GetX();
			 camPose.m_Ty = translate.GetY();
			 camPose.m_Tz = translate.GetZ();

			 break;
		 }
	default:
		{
			assert(0);
			return false;
		}
	}
	return true;
}

bool ExportTrajectory::Interpolation( double freq )
{
	// 重新插值的频率必须大于0
	if( freq <=0 )
	{
		assert( true );
		return false;
	}

	// 原始运动数据长度必须大于0
	int size = m_Times.size();
	if( size <= 0 )
	{
		assert( true );
		return false;
	}

	vector<long long> times;
	times = m_Times;

	m_Times.clear();
	for( int i = 0; i < size; ++i )
	{
		//assert( times[i]- times[0] == 6*i ); // 确认帧之间的时间间隔是6毫秒
		m_Times.push_back( times[i]- times[0] );     // 起始时刻归零
		m_Times[i] = 2 * m_Times[i];         // 帧之间的时间间隔调整到12毫秒
	}

	times = m_Times;
	m_Times.clear();

	vector<double> A1, A2, A3, A4, A5, A6, E1;
	A1 = m_A1;
	A2 = m_A2;
	A3 = m_A3;
	A4 = m_A4;
	A5 = m_A5;
	A6 = m_A6;
	E1 = m_E1;

	m_A1.clear();
	m_A2.clear();
	m_A3.clear();
	m_A4.clear();
	m_A5.clear();
	m_A6.clear();
	m_E1.clear();
	
	long long uplimit = times[ times.size() - 1 ];
	double  step = 1000 / freq; // 步长，以毫秒为单位
	for( int i = 0; ( i * step ) <= uplimit; ++i )
	{
		int  interval_index = -1;
		
		// 寻找插值区间
		for( int j = 0; j < size - 1; ++j )
		{
			if( times[j] <= i * step && times[j+1] >= i * step)
			{
				interval_index = j;
				break;
			}
		}

		// 寻找插值区间失败，报错，返回
		if( interval_index == -1 ) 
		{
			assert( true );
			return false;
		}

		m_Times.push_back( ( long long )( i * step ) ); // 记录采用时刻，毫秒为单位,取整

		m_A1.push_back( _Interpolation( times[interval_index], times[interval_index + 1], A1[interval_index], A1[interval_index + 1], i * step ) );
		m_A2.push_back( _Interpolation( times[interval_index], times[interval_index + 1], A2[interval_index], A2[interval_index + 1], i * step ) );
		m_A3.push_back( _Interpolation( times[interval_index], times[interval_index + 1], A3[interval_index], A3[interval_index + 1], i * step ) );
		m_A4.push_back( _Interpolation( times[interval_index], times[interval_index + 1], A4[interval_index], A4[interval_index + 1], i * step ) );
		m_A5.push_back( _Interpolation( times[interval_index], times[interval_index + 1], A5[interval_index], A5[interval_index + 1], i * step ) );
		m_A6.push_back( _Interpolation( times[interval_index], times[interval_index + 1], A6[interval_index], A6[interval_index + 1], i * step ) );
		m_E1.push_back( _Interpolation( times[interval_index], times[interval_index + 1], E1[interval_index], E1[interval_index + 1], i * step ) );
	}

	return true;
}