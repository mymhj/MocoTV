#pragma once
#include "vector3d.h"
#include "RotMatrix.h"


class CTransMatrix
{
private:
	            
	//CTransMatrix = |   R(3*3)    T(3*1)  |
	//               |  0  0  0      1     |


	//CTransMatrix的逆矩阵 = |   R^(Tran)(3*3)    -R^(Tran)(3*3)*T(3*1)  |
	//                        |   0   0      0                      1     |

	//CTransMatrix不支持装置运算，因为平移矢量：T(3*1)被限定为列矢量

	/* CTransMatrix仅支持左乘方式的坐标变换: 空间中的一个点P，P在世界坐标系中的齐次坐标记为P_w，在局部坐标系中的齐次坐标记为P_L，
	                                         将P_L转化为P_w的变换矩阵记为T，则P_w、P_L、T之间的关系是：P_w  = T * P_L
	*/

	//当R(3*3)为单位矩阵I(3*3)，T为零矢量时，CTransMatrix为单位矩阵I(4*4)
	
	RotMatrix   m_R;
	CVector3    m_T;
	
public:
	// 构造函数
	CTransMatrix(void);
	CTransMatrix( const CTransMatrix &operand );
 	~CTransMatrix(void);

	//运算符重载
	CTransMatrix  operator * ( const CTransMatrix& operand );
	CTransMatrix& operator = ( const CTransMatrix& operand );		
	CVector3 CTransMatrix::operator * ( CVector3& vect ); //计算矢量vec在该坐标变换下的新坐标
	
	//设置成单位阵I(4*4)	
	void SetToIdentity(void);
    
	//设置平移矢量T(3*1)
	void SetTranslate( CVector3 &trans );

	//返回平移矢量T(3*1)
	CVector3 GetTranslate(void);

	//设置旋转矩阵R(3*3)
	void SetRotate( RotMatrix &rot );

	//返回旋转矩阵R(3*3)
	RotMatrix GetRotate(void);

	//合法：旋转矩阵R(3*3)为正交矩阵，否则：非法
	bool Legal(void);

	//设置第row行,第col列的元素值
	void SetElement( int row, int col, double value );
	
	//返还第row行,第col列的元素值
	double GetElement( int row, int col );

	//根据欧拉角旋转顺序计算三个欧拉角，欧拉角为弧度,且绕动轴旋转
	 CVector3 ToEulerAngle( ROTATIONTYPE rtype );

    //根据欧拉角计算变换矩阵
	 void FromEulerAngle( ROTATIONTYPE rtype,  CVector3& eulerAngles );
	
	//计算变换矩阵的逆矩阵
	CTransMatrix GetInverse();
};
