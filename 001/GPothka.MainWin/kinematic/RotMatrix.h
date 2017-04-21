#pragma once

#include "vector3d.h"

//ŷ������ת˳��ģʽ����
enum ROTATIONTYPE{ XYZ,ZYX };

class RotMatrix
{	
private:
	double m_data[3][3];

public:
	//���캯��
	RotMatrix(void);
	RotMatrix( const RotMatrix& operand );
	//����ָ����ŷ������ת˳���ŷ���ǣ����ȱ�ʾ���������Ӧ����ת����
	RotMatrix( ROTATIONTYPE rtype, CVector3 eulerAngles );
	~RotMatrix(void);
	
	//���������
	RotMatrix& operator = ( const RotMatrix& operand );
	RotMatrix  operator * ( const RotMatrix& operand );
	RotMatrix  operator * ( const double operand );
	CVector3 RotMatrix::operator * ( const CVector3& vect );//������ʸ�� RotMatrix*vec
	//����������ڵ���ʱʹ�ã���Ϊ������ת����ĺ���û�м��������������
	RotMatrix operator + ( const RotMatrix& operand );
	
	//���ؾ����ת��
	RotMatrix GetTranspose(void);
   
	//���óɵ�λ��I(3*3)	
	void SetToIdentity(void);
	
	//�Ƿ�Ϊ��������
	bool isOrthogonal(void);
		
	//���õ�row��,��col�е�Ԫ��ֵ
	void SetElement( int row, int col, double value );
	
	//������row��,��col�е�Ԫ��ֵ
	double GetElement( int row, int col );
	
	//��̬��Ա����
	/*������X����תx���Ⱥ��3*3��ת����
				[ 1,  0,   0 ]
			Rx =[ 0, cx, -sx ]
				[ 0, sx,  cx ]		
	*/
	static RotMatrix GetRotMatrixAboutXAxis( double x );

	/*������Y����תy���Ⱥ��3*3��ת����
				[  cy, 0, sy ]
			Ry =[   0, 1,  0 ]
				[ -sy, 0, cy ]
	*/
	static RotMatrix GetRotMatrixAboutYAxis( double y );

	/*������Z����תz���Ⱥ��3*3��ת����
				[ cz, -sz, 0 ]
			Rz =[ sz,  cz, 0 ]
				[  0,   0, 1 ]
	*/	
	static RotMatrix GetRotMatrixAboutZAxis( double z );

	/*����ָ����ŷ������ת˳����ת����ֽ��3��ŷ���ǣ����ȱ�ʾ����
	  ��ת�����ǡ�������ӡ����ռ��е�һ����P��P����������ϵ�е������ΪP_w���ھֲ�����ϵ�е������ΪP_L����P_Lת��ΪP_w�ı任�����ΪT��
	  ��P_w��P_L��T֮��Ĺ�ϵ�ǣ�P_w  = T * P_L

	  W����������ϵ
	  L: �ֲ�����ϵ

	  ŷ���Ǻ���Ľ��ͣ���XYZ����ת˳��Ϊ������

	     ��W��X����ת     ��W'��Y����ת      ��W''��Z����ת
	  W -------------->W'--------------->W''---------------> L   ���ƶ�����ת��
	*/
	CVector3 ToEulerAngle( ROTATIONTYPE rtype );

	/*����ָ����ŷ������ת˳����ת����ֽ��3��ŷ���ǣ����ȱ�ʾ��
	  ��ת�����ǡ��ҳ����ӡ����ռ��е�һ������P��Pδ�˶�ǰ����������ϵ�е������ΪP_1�������ƶ�����ת�������������е������ΪP_2����P_1ת��ΪP_2�ı任�����ΪT��
	  ��P_1��P_2��T֮��Ĺ�ϵ�ǣ�P_2  = P_1 * T 
	  
	  ŷ���Ǻ���Ľ��ͣ���XYZ����ת˳��Ϊ������

	               ��W��X����ת          ��W��Y����ת          ��W��Z����ת
	  ��P��P_1��------------------>��P'--------------->��P''---------------> P_2 ���ƶ�����ת��

	  �����������ϵ�˶��Ĺ۵㿴����������ϵ����������ϵ���˶�����ϵ����ʼ��ʱ�����غϵģ�Ȼ���˶�����ϵ��W��X����ת������W��Y����ת������W��Z����ת��
	  ��ʱ���ǹ۲��˶�����ϵ��һ��P���õ����˶�����ϵ�е�����ֵ��ΪP_1������������ϵ�е�����ֵ��ΪP_2����P_1��P_2֮��Ĺ�ϵ�ǣ�P_2  = P_1 * T��T = Rmx * Rmy * Rmz��
	  Rmx��Rmy��Rmz �ı��ʽ�μ�cpp�ļ���
	  
	  �ú����ǰ���Maya������ת����Ľ��ͱ�д�ģ�����ŷ���ǵĽ��ͺ���ת˳��Ľ��Ͳο���http://download.autodesk.com/global/docs/maya2014/en_us/Nodes/transform.html
	*/
	CVector3 ToEulerAngleByRight( ROTATIONTYPE rtype );
};

