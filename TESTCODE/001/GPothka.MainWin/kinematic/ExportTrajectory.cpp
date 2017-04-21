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
	
	//������ݴ洢��
	m_Times.clear();
	m_A1.clear();
	m_A2.clear();
	m_A3.clear();
	m_A4.clear();
	m_A5.clear();
	m_A6.clear();
	m_E1.clear();


	string container;  
	// ����ע����
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

	// ����汾��Ϣ
	double version = -1;
	if( container != "VERSION")
	{
		assert(0);
		return false;
	}
	infile >> version;

	// ������֡��
	long long numFrames = 0;
	infile >> container;
	if( container != "NumFrames")
	{
		assert(0);
		return false;
	}
	infile >> numFrames;
	
	// �����˶�����
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

	//������ݴ洢��
	m_Times.clear();
	m_A1.clear();
	m_A2.clear();
	m_A3.clear();
	m_A4.clear();
	m_A5.clear();
	m_A6.clear();
	m_E1.clear();


	string container;  
	// ����ע����
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

	//����汾��Ϣ
		double version = -1;
	if( container != "VERSION")
	{
		assert(0);
		return false;
	}
	rStream >> version;

	// ������֡��
	long long numFrames = 0;
	rStream >> container;
	if( container != "NumFrames")
	{
		assert(0);
		return false;
	}
	rStream >> numFrames;

	// �����˶�����
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

	// ������ļ�
	ofstream outfile( fileName );
	if( !outfile.is_open() )
	{
		assert(0);
		return false;
	}

	int size = m_Times.size();		
	map<string, double> jointData;
	string bk = " ";
	
	// �޸����֡��
	if ( num > 0 &&  num < size )
	{
		size = num; // ��ǰnum֡���������̬���ݵ���
	}

	// ������Ӽ��������԰��Ӽ���������ϵ�ı任����
	for( int i = 0; i < size; ++i )
	{
		jointData["E1"] = m_E1[i]/1000.0;               // ���� --> ��  ��KUKAϵͳĬ�ϵĳ��ȵ�λ�Ǻ���

		jointData["A1"] = ( m_A1[i] + noise ) * ( 3.1415 / 180.0 ); // ��   --> ���ȣ�KUKAϵͳĬ�ϵĽǶȵ�λ�Ƕ�
		jointData["A2"] = ( m_A2[i] + noise ) * ( 3.1415 / 180.0 ); // ��   --> ���ȣ�KUKAϵͳĬ�ϵĽǶȵ�λ�Ƕ�
		jointData["A3"] = ( m_A3[i] + noise ) * ( 3.1415 / 180.0 ); // ��   --> ���ȣ�KUKAϵͳĬ�ϵĽǶȵ�λ�Ƕ�
		jointData["A4"] = ( m_A4[i] + noise ) * ( 3.1415 / 180.0 ); // ��   --> ���ȣ�KUKAϵͳĬ�ϵĽǶȵ�λ�Ƕ�
		jointData["A5"] = ( m_A5[i] + noise ) * ( 3.1415 / 180.0 ); // ��   --> ���ȣ�KUKAϵͳĬ�ϵĽǶȵ�λ�Ƕ�
		jointData["A6"] = ( m_A6[i] + noise ) * ( 3.1415 / 180.0 ); // ��   --> ���ȣ�KUKAϵͳĬ�ϵĽǶȵ�λ�Ƕ�
				
		// ���û����˵Ĺؽ��˶����ݣ�������ת����
		bool isleagl = m_Robot.SetJointsMotionData( jointData );
		if( isleagl == false )
		{
			assert( 0 );
			return false;
		}

		// ������Ӽ����������ϵ�����Ӽ���������ϵ�ı任����
		CTransMatrix tran;
			 
		// ����A6��ֲ�����ϵ�������������ϵ�ı任
		m_Robot.TransMatrixBetweenTwoCoordinates( "A6", "world", tran );
			 
		// ���ϰ��Ӽ�������ֲ�����ϵ�����A6��ֲ�����ϵ�ı任
		tran = tran * m_CameraInitialPose;

		// �����������λ��д���ļ�
		CVector3 translate = tran.GetTranslate();
		outfile << translate.GetX()*1000.0 << bk 
			    << translate.GetY()*1000.0 << bk 
				<< translate.GetZ()*1000.0 << endl; 
		
		// ����������᷽��ĵ�λ����д���ļ�����ת����ĵ�3�У��ֲ�����ϵ��Z������������ϵ��ͶӰ��
		RotMatrix rot = tran.GetRotate();
		outfile << rot.GetElement( 0, 2 ) << bk
			    << rot.GetElement( 1, 2 ) << bk
			    << rot.GetElement( 2, 2 ) << endl;

		// �������head����ĵ�λ����д���ļ�����ת����ĵ�1�У��ֲ�����ϵ��X������������ϵ��ͶӰ��
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

	// �޸����֡��
	if ( num > 0 &&  num < size )
	{
		size = num; // ��ǰnum֡���������̬���ݵ���
	}

	switch(type)
	{
	case MAYA:
		{	
			// �����������MAYA�����µ���̬����
			for( int i = 0; i < size; ++i )
			{
				jointData["E1"] = m_E1[i]/1000.0;               // ���� --> ��  ��KUKAϵͳĬ�ϵĳ��ȵ�λ�Ǻ���

				jointData["A1"] = ( m_A1[i] + noise ) * ( 3.1415 / 180.0 ); // ��   --> ���ȣ�KUKAϵͳĬ�ϵĽǶȵ�λ�Ƕ�
		        jointData["A2"] = ( m_A2[i] + noise ) * ( 3.1415 / 180.0 ); // ��   --> ���ȣ�KUKAϵͳĬ�ϵĽǶȵ�λ�Ƕ�
		        jointData["A3"] = ( m_A3[i] + noise ) * ( 3.1415 / 180.0 ); // ��   --> ���ȣ�KUKAϵͳĬ�ϵĽǶȵ�λ�Ƕ�
		        jointData["A4"] = ( m_A4[i] + noise ) * ( 3.1415 / 180.0 ); // ��   --> ���ȣ�KUKAϵͳĬ�ϵĽǶȵ�λ�Ƕ�
		        jointData["A5"] = ( m_A5[i] + noise ) * ( 3.1415 / 180.0 ); // ��   --> ���ȣ�KUKAϵͳĬ�ϵĽǶȵ�λ�Ƕ�
		        jointData["A6"] = ( m_A6[i] + noise ) * ( 3.1415 / 180.0 ); // ��   --> ���ȣ�KUKAϵͳĬ�ϵĽǶȵ�λ�Ƕ�


				//jointData["A1"] = m_A1[i] * ( 3.1415 / 180.0 ); // ��   --> ���ȣ�KUKAϵͳĬ�ϵĽǶȵ�λ�Ƕ�
				//jointData["A2"] = m_A2[i] * ( 3.1415 / 180.0 ); // ��   --> ���ȣ�KUKAϵͳĬ�ϵĽǶȵ�λ�Ƕ�
				//jointData["A3"] = m_A3[i] * ( 3.1415 / 180.0 ); // ��   --> ���ȣ�KUKAϵͳĬ�ϵĽǶȵ�λ�Ƕ�
				//jointData["A4"] = m_A4[i] * ( 3.1415 / 180.0 ); // ��   --> ���ȣ�KUKAϵͳĬ�ϵĽǶȵ�λ�Ƕ�
				//jointData["A5"] = m_A5[i] * ( 3.1415 / 180.0 ); // ��   --> ���ȣ�KUKAϵͳĬ�ϵĽǶȵ�λ�Ƕ�
				//jointData["A6"] = m_A6[i] * ( 3.1415 / 180.0 ); // ��   --> ���ȣ�KUKAϵͳĬ�ϵĽǶȵ�λ�Ƕ�
				
				// ���û����˵Ĺؽ��˶����ݣ�������ת����
				bool isleagl = m_Robot.SetJointsMotionData( jointData );
				if( isleagl == false )
				{
					assert( 0 );
					return false;
				}
				
				// �����������̬����
				CalculateCameraPose( type, cpose );

				cpose.m_Focus = 20; // ������ʱĬ��Ϊ20����
				
				// �����������̬����
				CameraPoseList.push_back( cpose );
			}
			
			// ������ļ�
			ofstream outfile( fileName );
			if( !outfile.is_open() )
			{
				assert(0);
				return false;
			}

			string bk = " ";
	        //�ļ�ͷ 
	        outfile << "requires maya \"2009\";" << endl;
	        outfile << "currentUnit -l centimeter -a degree -t pal;" << endl; // ���ȵ�λ�����ף��Ƕȵ�λ����
            outfile << "createNode transform -n \"OthkaCameraNode\";" << endl;
	        outfile << "createNode camera -n \"OthkaCamera\" -p \"OthkaCameraNode\";" << endl;

			//д�뽹������
	        outfile << "createNode animCurveTU -n \"CameraFocalLengthCurve\";" << endl;
	        outfile << "    setAttr -s " << size << " \".ktv[0:" << ( size - 1 ) << "]\" ";
	        for( int i = 0; i < size; ++i )
			{
				outfile << bk << ( i + 1 ) << bk << CameraPoseList[i].m_Focus;
			}
	        outfile << ";" << endl;
	      
			//д�������X����λ������
			outfile << "createNode animCurveTL -n \"CameraPositionXCurve\";" << endl;
			outfile << "    setAttr -s " << size << " \".ktv[0:" << ( size - 1 ) << "]\" ";
			for( int i = 0; i < size; ++i )
			{
				outfile << bk << ( i + 1 ) << bk << ( CameraPoseList[i].m_Tx * 100 ); // ��-->����
			}
			outfile << ";"<<endl;
	   
			//д�������Y����λ������
			outfile << "createNode animCurveTL -n \"CameraPositionYCurve\";" << endl;
			outfile << "    setAttr -s " << size << " \".ktv[0:" << ( size - 1 ) << "]\" ";
			for( int i = 0; i < size; ++i )
			{
				outfile << bk << ( i + 1 ) << bk << ( CameraPoseList[i].m_Ty * 100 ); // ��-->����
			}
			outfile << ";"<<endl;

			//д�������Z����λ������
			outfile << "createNode animCurveTL -n \"CameraPositionZCurve\";" << endl;
			outfile << "    setAttr -s " << size << " \".ktv[0:" << ( size - 1 ) << "]\" ";
			for( int i = 0; i < size; ++i )
			{
				outfile << bk << ( i + 1 ) << bk << ( CameraPoseList[i].m_Tz * 100 ); // ��-->����
			}
			outfile << ";"<<endl;

			//д���������X�����ת����
			outfile << "createNode animCurveTA -n \"CameraAngleXCurve\";" << endl;
			outfile << "    setAttr -s " << size << " \".ktv[0:" << ( size - 1 ) << "]\" ";
			for( int i = 0; i < size; ++i )
			{
				outfile << bk << ( i + 1 ) << bk << ( CameraPoseList[i].m_Ex * (180/3.1415) );// ����-->��
			}
			outfile << ";" << endl;

			//д���������Y�����ת����
			outfile << "createNode animCurveTA -n \"CameraAngleYCurve\";" << endl;
			outfile << "    setAttr -s " << size << " \".ktv[0:" << ( size - 1 ) << "]\" ";
			for( int i = 0; i < size; ++i )
			{
				outfile << bk << ( i + 1 ) << bk << ( CameraPoseList[i].m_Ey * (180/3.1415) );// ����-->��
			}
			outfile << ";" << endl;


			//д���������Z�����ת����
			outfile << "createNode animCurveTA -n \"CameraAngleZCurve\";" << endl;
			outfile << "    setAttr -s " << size << " \".ktv[0:" << ( size - 1 ) << "]\" ";
			for( int i = 0; i < size; ++i )
			{
				outfile << bk << ( i + 1 ) << bk << ( CameraPoseList[i].m_Ez * (180/3.1415) );// ����-->��
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

// ���յ�����������ͣ��������������̬����
bool ExportTrajectory::CalculateCameraPose( SOFTWARE type, CameraPose& camPose )
{
	switch(type)
	{
	case MAYA:
		 {
			 // �Ĳ����㷨
			 
			 // step 1: ����Maya���������ϵ�����Ӽ����������ϵ�ı任����
			 RotMatrix rot;
			 rot.SetElement(0,0,0);  rot.SetElement(0,1,1);  rot.SetElement(0,2,0);
			 rot.SetElement(1,0,1);  rot.SetElement(1,1,0);  rot.SetElement(1,2,0);
			 rot.SetElement(2,0,0);  rot.SetElement(2,1,0);  rot.SetElement(2,2,-1);
			 
			 CVector3 vec;         // ��ʸ��

			 CTransMatrix tran1;
			 tran1.SetRotate(rot);    // ������ת����
			 tran1.SetTranslate(vec); // ������ƽ��ʸ����ע����֤һ��
			  
			 // step 2: ������Ӽ����������ϵ�����Ӽ���������ϵ�ı任����
			 CTransMatrix tran2;
			 
			 // ����A6��ֲ�����ϵ�������������ϵ�ı任
			 m_Robot.TransMatrixBetweenTwoCoordinates("A6","world",tran2);
			 
			 // ���ϰ��Ӽ�������ֲ�����ϵ�����A6��ֲ�����ϵ�ı任
			 tran2 = tran2*m_CameraInitialPose;

			 // step 3: ������Ӽ���������ϵ��Maya��������ϵ�ı任����
			 rot.SetElement(0,0,1);  rot.SetElement(0,1,0);  rot.SetElement(0,2,0);
			 rot.SetElement(1,0,0);  rot.SetElement(1,1,0);  rot.SetElement(1,2,1);
			 rot.SetElement(2,0,0);  rot.SetElement(2,1,-1);  rot.SetElement(2,2,0);

			 CTransMatrix tran3;
			 tran3.SetRotate(rot);    // ������ת����
			 tran3.SetTranslate(vec); // ������ƽ��ʸ����ע����֤һ��

			 // step 4: ����Maya���������ϵ��Maya��������ϵ�ı任���� 
			 CTransMatrix tran4;
			 tran4 = tran3 * tran2 * tran1;

			 // ����CTransMatrix��֧��ת�����㣬��˽���ת����RotMatrix��m_R����tran4�г�ȡ������Ȼ������ת������
			 rot = tran4.GetRotate();
			 rot = rot.GetTranspose();
			
			 
			 // ��������ĳ������ݣ�����ŷ����
			 CVector3 euler = rot.ToEulerAngleByRight( XYZ );

			 // ��������ĳ�������
			 camPose.m_Ex = euler.GetX();
			 camPose.m_Ey = euler.GetY();
			 camPose.m_Ez = euler.GetZ();

			 // ���������λ������
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
	// ���²�ֵ��Ƶ�ʱ������0
	if( freq <=0 )
	{
		assert( true );
		return false;
	}

	// ԭʼ�˶����ݳ��ȱ������0
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
		//assert( times[i]- times[0] == 6*i ); // ȷ��֮֡���ʱ������6����
		m_Times.push_back( times[i]- times[0] );     // ��ʼʱ�̹���
		m_Times[i] = 2 * m_Times[i];         // ֮֡���ʱ����������12����
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
	double  step = 1000 / freq; // �������Ժ���Ϊ��λ
	for( int i = 0; ( i * step ) <= uplimit; ++i )
	{
		int  interval_index = -1;
		
		// Ѱ�Ҳ�ֵ����
		for( int j = 0; j < size - 1; ++j )
		{
			if( times[j] <= i * step && times[j+1] >= i * step)
			{
				interval_index = j;
				break;
			}
		}

		// Ѱ�Ҳ�ֵ����ʧ�ܣ���������
		if( interval_index == -1 ) 
		{
			assert( true );
			return false;
		}

		m_Times.push_back( ( long long )( i * step ) ); // ��¼����ʱ�̣�����Ϊ��λ,ȡ��

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