#pragma once


#include <assert.h>
#include "Robot.h"

enum SOFTWARE { MAYA, MAX };// software type

struct CameraPose
{
public:
	double m_Focus;          // ���������
	double m_Tx, m_Ty, m_Tz; // ƽ�Ƶ���������
	double m_Ex, m_Ey, m_Ez; // ��ת������������ŷ����
	
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
	
	//���û�����ģ��
	void SetRobotModel( Robot& robot )
	{
		m_Robot = robot;
	}
	
	/*���û����˵Ĺؽ��˶����ݣ��ļ�inFile��ʧ�ܣ����߶������ݲ�ƥ�䣬����false�����򷵻�true

	  inFile���ļ���ʽ������Ϊ��Ǩ�ͷ�����������ݸ�ʽ������Ҫ�޸ģ�����ȥ��E2��ȥ��X��Y��Z��A��B��C�����ݣ�
	  -----------------------------------------------------------------------------------------
	  %% KUKA�����������ļ� 1.0�汾
      %% �˶����ݵ�λ��ƽ�ƣ����ף�����ת���ȣ���
      %% ���Ӽ�ϵͳ�ڲ�������λԼ���� ���ȣ��ף�����ת�����ȣ���������ת��ʱע�ⵥλ�任
      %% VERSION     �汾��ʶ��
      %% NumFrames   �˶�������֡��
      %% ���ݸ�ʽ���壺ʱ���  A1  A2  A3  A4  A5  A6  E1
	  
	  ���磺
      VERSION  1.0
      NumFrames  100  
      4215659280 -18.7571 -38.0038 29.3743 166.0869 62.7735 41.7330 1094.5068 
	  -----------------------------------------------------------------------------------------
	*/
	bool SetRobotMotionData( string inFile );

	//ͨ���ַ����ֽ��ȡ�������˶�����
	bool ExportTrajectory::SetRobotMotionDataFromString(string robotA1A6Data);

	//���յ�����������ͣ��������������̬����
	bool CalculateCameraPose( SOFTWARE type, CameraPose& camPose);

	/*���յ�����������ͣ����ļ���д��ת������������̬����
	  num�ĺ����ǵ���֡��: num > 0 �����numС�������ļ���������֡������ô��ǰnum֡���������̬���ݵ��������������ļ�������֡����������
	  noise�ĺ����Ǹ��Ӹ��˶����ݵ���������λΪ�ȣ�Ŀǰ����E1���ƽ���˶����������
	*/
	bool Export( SOFTWARE type, string fileName, int num,double noise = 0.0  );

	/*�����Ӽ�������ֲ�����ϵ��԰��Ӽ���������ϵ��ֵд���ļ�
	  �ļ���ʽ
	  -----------------------------------------------------------------------------------------
	  ��һ�У��������������������ϵ�е�����ֵ ��X Y Z
      �ڶ��У���ת����ĵ�3�У��ֲ�����ϵ��Z������������ϵ��ͶӰ��
      �����У���ת����ĵ�1�У��ֲ�����ϵ��X������������ϵ��ͶӰ��
	  �����У�***************************************************
	  -----------------------------------------------------------------------------------------

	  num�ĺ����ǵ���֡��: num > 0 �����numС�������ļ���������֡������ô��ǰnum֡���������̬���ݵ��������������ļ�������֡����������
	  noise�ĺ����Ǹ��Ӹ��˶����ݵ���������λΪ�ȣ�Ŀǰ����E1���ƽ���˶����������
	
	 �ú�����Ϊ�����ã�ͨ��OPGL��MAYA֮��ĶԱȣ����жϳ������ȷ�ԣ�
	*/
	bool ExportCamerapose( string fileName, int num, double noise = 0.0 );

	//����������ֲ�����ϵԭ�������̨�ֲ�����ϵ�ĳ�ʼ�任����
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

	// ����KUKA�������˶����ݵ���֡��
	int GetMotionSize( void )
	{
		assert( m_Times.size() > 0 );
		return m_Times.size();
	}

	// freq:���²�ֵ��Ƶ�ʣ�����25֡/��
	bool Interpolation( double freq );

private:
	// ���������ģ��
	Robot m_Robot; 
	
	//���������KUKA�������˶�����
	vector<long long> m_Times;
	vector<double> m_A1, m_A2, m_A3, m_A4, m_A5, m_A6, m_E1;
    
	// ������ֲ�����ϵԭ�������̨�ֲ�����ϵ�ĳ�ʼ�任����
	CTransMatrix m_CameraInitialPose;


	// y(x) = ( ( y1 - y0 )/( x1 - x0 ) ) * ( x - x0 ) + y0
	double _Interpolation( double x0, double x1, double y0, double y1, double x )
	{
		assert( x1 > x0 );
		assert( x0 <= x && x1 >= x );
		return ( ( y1 - y0 )/( x1 - x0 ) ) * ( x - x0 ) + y0;
	}
};

