#pragma once

#include "vector3d.h"

//欧拉角旋转顺序模式集合
enum ROTATIONTYPE{ XYZ,ZYX };

class RotMatrix
{	
private:
	double m_data[3][3];

public:
	//构造函数
	RotMatrix(void);
	RotMatrix( const RotMatrix& operand );
	//根据指定的欧拉角旋转顺序和欧拉角（弧度表示），计算对应的旋转矩阵
	RotMatrix( ROTATIONTYPE rtype, CVector3 eulerAngles );
	~RotMatrix(void);
	
	//运算符重载
	RotMatrix& operator = ( const RotMatrix& operand );
	RotMatrix  operator * ( const RotMatrix& operand );
	RotMatrix  operator * ( const double operand );
	CVector3 RotMatrix::operator * ( const CVector3& vect );//计算新矢量 RotMatrix*vec
	//此运算仅限于调试时使用，因为两个旋转矩阵的和是没有几何与物理意义的
	RotMatrix operator + ( const RotMatrix& operand );
	
	//返回矩阵的转置
	RotMatrix GetTranspose(void);
   
	//设置成单位阵I(3*3)	
	void SetToIdentity(void);
	
	//是否为正交矩阵
	bool isOrthogonal(void);
		
	//设置第row行,第col列的元素值
	void SetElement( int row, int col, double value );
	
	//返还第row行,第col列的元素值
	double GetElement( int row, int col );
	
	//静态成员函数
	/*计算绕X轴旋转x弧度后的3*3旋转矩阵
				[ 1,  0,   0 ]
			Rx =[ 0, cx, -sx ]
				[ 0, sx,  cx ]		
	*/
	static RotMatrix GetRotMatrixAboutXAxis( double x );

	/*计算绕Y轴旋转y弧度后的3*3旋转矩阵
				[  cy, 0, sy ]
			Ry =[   0, 1,  0 ]
				[ -sy, 0, cy ]
	*/
	static RotMatrix GetRotMatrixAboutYAxis( double y );

	/*计算绕Z轴旋转z弧度后的3*3旋转矩阵
				[ cz, -sz, 0 ]
			Rz =[ sz,  cz, 0 ]
				[  0,   0, 1 ]
	*/	
	static RotMatrix GetRotMatrixAboutZAxis( double z );

	/*根据指定的欧拉角旋转顺序将旋转矩阵分解成3个欧拉角（弧度表示），
	  旋转矩阵是“左乘算子”：空间中的一个点P，P在世界坐标系中的坐标记为P_w，在局部坐标系中的坐标记为P_L，将P_L转化为P_w的变换矩阵记为T，
	  则P_w、P_L、T之间的关系是：P_w  = T * P_L

	  W：世界坐标系
	  L: 局部坐标系

	  欧拉角含义的解释（以XYZ的旋转顺序为例）：

	     绕W的X轴旋转     绕W'的Y轴旋转      绕W''的Z轴旋转
	  W -------------->W'--------------->W''---------------> L   （绕动轴旋转）
	*/
	CVector3 ToEulerAngle( ROTATIONTYPE rtype );

	/*根据指定的欧拉角旋转顺序将旋转矩阵分解成3个欧拉角（弧度表示）
	  旋转矩阵是“右乘算子”：空间中的一个动点P，P未运动前在世界坐标系中的坐标记为P_1，经过绕定轴旋转后在世界坐标中的坐标记为P_2，将P_1转化为P_2的变换矩阵记为T，
	  则P_1、P_2、T之间的关系是：P_2  = P_1 * T 
	  
	  欧拉角含义的解释（以XYZ的旋转顺序为例）：

	               绕W的X轴旋转          绕W的Y轴旋转          绕W的Z轴旋转
	  点P（P_1）------------------>点P'--------------->点P''---------------> P_2 （绕定轴旋转）

	  如果采用坐标系运动的观点看，两个坐标系（世界坐标系和运动坐标系）开始的时候是重合的，然后运动坐标系绕W的X轴旋转，再绕W的Y轴旋转，再绕W的Z轴旋转，
	  此时我们观察运动坐标系上一点P，该点在运动坐标系中的坐标值记为P_1，在世界坐标系中的坐标值记为P_2，则P_1、P_2之间的关系是：P_2  = P_1 * T，T = Rmx * Rmy * Rmz。
	  Rmx，Rmy，Rmz 的表达式参见cpp文件。
	  
	  该函数是按照Maya对于旋转矩阵的解释编写的，关于欧拉角的解释和旋转顺序的解释参考：http://download.autodesk.com/global/docs/maya2014/en_us/Nodes/transform.html
	*/
	CVector3 ToEulerAngleByRight( ROTATIONTYPE rtype );
};

