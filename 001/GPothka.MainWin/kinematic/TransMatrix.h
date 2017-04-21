#pragma once
#include "vector3d.h"
#include "RotMatrix.h"


class CTransMatrix
{
private:
	            
	//CTransMatrix = |   R(3*3)    T(3*1)  |
	//               |  0  0  0      1     |


	//CTransMatrix������� = |   R^(Tran)(3*3)    -R^(Tran)(3*3)*T(3*1)  |
	//                        |   0   0      0                      1     |

	//CTransMatrix��֧��װ�����㣬��Ϊƽ��ʸ����T(3*1)���޶�Ϊ��ʸ��

	/* CTransMatrix��֧����˷�ʽ������任: �ռ��е�һ����P��P����������ϵ�е���������ΪP_w���ھֲ�����ϵ�е���������ΪP_L��
	                                         ��P_Lת��ΪP_w�ı任�����ΪT����P_w��P_L��T֮��Ĺ�ϵ�ǣ�P_w  = T * P_L
	*/

	//��R(3*3)Ϊ��λ����I(3*3)��TΪ��ʸ��ʱ��CTransMatrixΪ��λ����I(4*4)
	
	RotMatrix   m_R;
	CVector3    m_T;
	
public:
	// ���캯��
	CTransMatrix(void);
	CTransMatrix( const CTransMatrix &operand );
 	~CTransMatrix(void);

	//���������
	CTransMatrix  operator * ( const CTransMatrix& operand );
	CTransMatrix& operator = ( const CTransMatrix& operand );		
	CVector3 CTransMatrix::operator * ( CVector3& vect ); //����ʸ��vec�ڸ�����任�µ�������
	
	//���óɵ�λ��I(4*4)	
	void SetToIdentity(void);
    
	//����ƽ��ʸ��T(3*1)
	void SetTranslate( CVector3 &trans );

	//����ƽ��ʸ��T(3*1)
	CVector3 GetTranslate(void);

	//������ת����R(3*3)
	void SetRotate( RotMatrix &rot );

	//������ת����R(3*3)
	RotMatrix GetRotate(void);

	//�Ϸ�����ת����R(3*3)Ϊ�������󣬷��򣺷Ƿ�
	bool Legal(void);

	//���õ�row��,��col�е�Ԫ��ֵ
	void SetElement( int row, int col, double value );
	
	//������row��,��col�е�Ԫ��ֵ
	double GetElement( int row, int col );

	//����ŷ������ת˳���������ŷ���ǣ�ŷ����Ϊ����,���ƶ�����ת
	 CVector3 ToEulerAngle( ROTATIONTYPE rtype );

    //����ŷ���Ǽ���任����
	 void FromEulerAngle( ROTATIONTYPE rtype,  CVector3& eulerAngles );
	
	//����任����������
	CTransMatrix GetInverse();
};
