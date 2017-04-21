#include "StdAfx.h"
#include "RotMatrix.h"
#include "math.h"
#include <assert.h>

RotMatrix::RotMatrix(void)
{
	memset( m_data, 0, 9*sizeof(double) );
}

RotMatrix::RotMatrix( const RotMatrix& operand )
{
	memcpy( m_data, operand.m_data, 9*sizeof(double) );
}

RotMatrix::~RotMatrix(void)
{
}

RotMatrix& RotMatrix::operator = ( const RotMatrix& operand )
{
	memcpy( m_data, operand.m_data, 9*sizeof(double) );
	return (*this);
}

RotMatrix RotMatrix::operator * ( const RotMatrix& operand )
{
	RotMatrix product;
	for( int i = 0; i < 3; ++i )
	{
		for( int j = 0; j < 3; ++j )
		{
			for( int k = 0; k < 3; ++k )
			{
				product.m_data[i][j] += m_data[i][k] * operand.m_data[k][j];
			}
		}
	}
	return product;
}

RotMatrix RotMatrix::operator * ( const double operand )
{
	RotMatrix product;
	for( int i = 0; i < 3; ++i )
	{
		for( int j = 0; j < 3; ++j )
		{
			product.m_data[i][j] = m_data[i][j] * operand;
		}
	}
	return product;
}

CVector3 RotMatrix::operator * ( const CVector3& vect )
{
	CVector3 product;
	double data[3];
	vect.GetData( data[0], data[1], data[2] );
	for( int i = 0; i < 3; ++i )
	{
		double tmp = 0;
		for( int j = 0; j < 3; ++j )
		{
			tmp += m_data[i][j] * data[j];
		}
		product[i] = tmp;
	}
	return product;
}

//此运算仅限于调试时使用，因为两个旋转矩阵的和是没有几何与物理意义的
RotMatrix RotMatrix::operator + ( const RotMatrix& operand )
{
	RotMatrix sum;
	for( int i = 0; i < 3; ++i )
	{
		for( int j = 0; j < 3; ++j )
		{
			sum.m_data[i][j] = m_data[i][j] + operand.m_data[i][j];
		}
	}
	return sum;
}

//矩阵转置
RotMatrix RotMatrix::GetTranspose(void)
{
	RotMatrix transp;
	for( int i = 0; i < 3; ++i )
	{
		for( int j = 0; j <3; ++j )
		{
			transp.m_data[i][j] = m_data[j][i];
		}
	}
	return transp;
}


void RotMatrix::SetToIdentity(void)
{
	memset( m_data, 0, 9*sizeof(double) );
	m_data[0][0] = m_data[1][1] = m_data[2][2] = 1.0;
}

bool RotMatrix::isOrthogonal(void)
{
	CVector3 row0, row1, row2;
	row0.SetTo( m_data[0][0], m_data[0][1], m_data[0][2] );
	row1.SetTo( m_data[1][0], m_data[1][1], m_data[1][2] );
	row2.SetTo( m_data[2][0], m_data[2][1], m_data[2][2] );

	double error = 1.0e-4;
	bool isOrth = ( abs(row0.EuclideanNorms()-1) < error ) &&
		           ( abs(row1.EuclideanNorms()-1) < error ) &&
		           ( abs(row2.EuclideanNorms()-1) < error ) &&
				   ( abs(row0*row1) < error ) &&
				   ( abs(row1*row2) < error ) &&
				   ( abs(row0*row2) < error );
	return isOrth;
}


void RotMatrix::SetElement( int row, int col, double value )
{
	assert( row >=0 && row <=2 );
	assert( col >=0 && col <=2 );
	m_data[row][col] = value;
}
	
double RotMatrix::GetElement( int row, int col )
{
	assert( row >=0 && row <=2 );
	assert( col >=0 && col <=2 );
	return m_data[row][col];
}


	
/*计算绕X轴旋转x弧度后的3*3旋转矩阵
				[ 1,  0,   0 ]
			Rx =[ 0, cx, -sx ]
				[ 0, sx,  cx ]		
*/
RotMatrix RotMatrix::GetRotMatrixAboutXAxis( double x )
{ 
	RotMatrix Rx;

	Rx.m_data[0][0] = 1;  Rx.m_data[0][1] = 0;       Rx.m_data[0][2] = 0;
	Rx.m_data[1][0] = 0;  Rx.m_data[1][1] = cos(x);  Rx.m_data[1][2] = -sin(x);
	Rx.m_data[2][0] = 0;  Rx.m_data[2][1] = sin(x);  Rx.m_data[2][2] =  cos(x);

	return Rx;
}

/*计算绕Y轴旋转y弧度后的3*3旋转矩阵
				[  cy, 0, sy ]
			Ry =[   0, 1,  0 ]
				[ -sy, 0, cy ]
 
*/
RotMatrix RotMatrix::GetRotMatrixAboutYAxis( double y )
{
	RotMatrix Ry;

	Ry.m_data[0][0] = cos(y);   Ry.m_data[0][1] = 0;  Ry.m_data[0][2] = sin(y);
	Ry.m_data[1][0] = 0;        Ry.m_data[1][1] = 1;  Ry.m_data[1][2] = 0;
	Ry.m_data[2][0] = -sin(y);  Ry.m_data[2][1] = 0;  Ry.m_data[2][2] = cos(y);

	return Ry;
}

/*计算绕Z轴旋转z弧度后的3*3旋转矩阵
				[ cz, -sz, 0]
			Rz =[ sz,  cz, 0]
				[  0,   0, 1]
*/	
RotMatrix RotMatrix::GetRotMatrixAboutZAxis( double z )	
{
	RotMatrix Rz;
	
	Rz.m_data[0][0] = cos(z);   Rz.m_data[0][1] = -sin(z);  Rz.m_data[0][2] = 0;
	Rz.m_data[1][0] = sin(z);   Rz.m_data[1][1] = cos(z);   Rz.m_data[1][2] = 0;
	Rz.m_data[2][0] = 0;        Rz.m_data[2][1] = 0;        Rz.m_data[2][2] = 1;
	
	return Rz;
}

//根据指定的欧拉角旋转顺序将旋转矩阵分解成3个欧拉角，旋转矩阵是“左乘算子”
CVector3 RotMatrix::ToEulerAngle( ROTATIONTYPE rtype )
{
	/*	          
			[ 1,  0,   0 ]
		Rx =[ 0, cx, -sx ]
			[ 0, sx,  cx ]		
 
			[  cy, 0, sy ]
		Ry =[   0, 1,  0 ]
			[ -sy, 0, cy ]
 
			[ cz, -sz, 0 ]
		Rz =[ sz,  cz, 0 ]
			[  0,   0, 1 ]
    */

	assert( isOrthogonal() );

	CVector3 eulerAngles;
	switch( rtype )
	{
	case XYZ:
		{
			/*          
						 [            cy*cz,           -cy*sz,     sy ]
			Rx*Ry*Rz =   [ cx*sz + cz*sx*sy, cx*cz - sx*sy*sz, -cy*sx ]
						 [ sx*sz - cx*cz*sy, cz*sx + cx*sy*sz,  cx*cy ]


		    P_w  = T * P_L = Rx * Ry * Rz * P_L // 因为是绕动轴旋转，因此向按照反向顺序，以此把P_L转换成P_W''，然后在转换成P_W'，最后转换成P_w 
			*/

			double x = 0; 
			double y = 0; 
			double z = 0; 


			double sy = m_data[0][2];
			if ( sy <= -1.0  )		
			{ 
				y = -1.570796; // −pi /2
			} 
			else if ( sy >= 1.0  ) 
			{
				y = 1.570796; // pi /2
			}
			else 
			{
				y = asin(sy);
			}

			if( abs(sy) > 0.9999 )
			{				
				x = atan2( m_data[2][1], m_data[1][1] );
				z = 0;
			}
			else
			{
				x = atan2( -m_data[1][2], m_data[2][2] );
			    z = atan2( -m_data[0][1], m_data[0][0] );
			}
			eulerAngles[0] = x;
			eulerAngles[1] = y;
			eulerAngles[2] = z;
			break;
		}
	default:
		{
			assert(0);
		}
	}
	return eulerAngles;
}

//根据指定的欧拉角旋转顺序将旋转矩阵分解成3个欧拉角，旋转矩阵是“右乘算子”
CVector3 RotMatrix::ToEulerAngleByRight( ROTATIONTYPE rtype )
{
	/*	          
	     [ 1,   0,  0 ]
	Rmx =[ 0,  cx, sx ]
	     [ 0, -sx, cx ]		

		 [  cy, 0,-sy ]
	Rmy =[   0, 1,  0 ]
	     [  sy, 0, cy ]

	     [  cz, sz, 0 ]
	Rmz =[ -sz, cz, 0 ]
	     [  0,   0, 1 ]
	*/

	assert( isOrthogonal() );

	CVector3 eulerAngles;
	switch( rtype )
	{
	case XYZ:
		{
			/*          
							[            cy*cz,             cy*sz,    -sy ]
			Rmx*Rmy*Rmz =   [ sx*sy*cz - cx*sz,  sx*sy*sz + cx*cz,  sx*cy ]
							[ cx*sy*cz + sx*sz , cx*sy*sz - sx*cz,  cx*cy ]

			P_w  = P_L * T  =  P_L * Rmx * Rmy * Rmz  // 因为是绕定轴旋转，因此先把P_L转换成P_W''，然后在转换成P_W'，最后转换成P_w 

			*/
			double x = 0; // 绕X轴旋转角度（单位弧度）
			double y = 0; // 绕Y轴旋转角度（单位弧度）
			double z = 0; // 绕Z轴旋转角度（单位弧度）

			double negSy = m_data[0][2];
			if( negSy <= -1.0  )		
			{ 
				y = 1.570796; // pi /2
			} 
			else if( negSy >= 1.0  ) 
			{
				y = -1.570796; // -pi /2
			}
			else
			{
				y = asin( -1*negSy );
			}

			if( abs( negSy ) > 0.9999 ) // 处理死锁造成的自由度缺失的现象（参考文献1第280页）
			{				
				x = atan2( -m_data[2][1], m_data[1][1] );
				z = 0;
			}
			else
			{
				x = atan2( m_data[1][2], m_data[2][2] );
				z = atan2( m_data[0][1], m_data[0][0] );
			}
			eulerAngles[0] = x;
			eulerAngles[1] = y;
			eulerAngles[2] = z;

			break;
		}
	default:
		{
			assert(0);
		}
	}
	return eulerAngles;

}


// 根据指定的欧拉角旋转顺序和欧拉角（弧度表示），计算对应的旋转矩阵
RotMatrix::RotMatrix( ROTATIONTYPE rtype, CVector3 eulerAngles )
{
	double x = eulerAngles[0];
	double y = eulerAngles[1];
	double z = eulerAngles[2];

	switch( rtype )
	{
	case XYZ:
		{
			// rotation matrix = Rx*Ry*Rz
			RotMatrix rot = RotMatrix::GetRotMatrixAboutXAxis( x ) * 
				            RotMatrix::GetRotMatrixAboutYAxis( y ) * 
				            RotMatrix::GetRotMatrixAboutZAxis( z );
			memcpy( m_data, rot.m_data, 9*sizeof(double) );
			break;
		  }
	default:
		{
			assert(0);
		}
	}
}