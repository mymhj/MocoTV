#include "stdafx.h"
#include "transmatrix.h"
#include <assert.h>
#include "math.h"
#include <new>


CTransMatrix::CTransMatrix(void)
{
}

CTransMatrix::CTransMatrix( const CTransMatrix &operand ) 
{
	m_R = operand.m_R;
	m_T = operand.m_T; 
}

CTransMatrix::~CTransMatrix(void)
{

}

 //R = R1 * R2;
 //T = R1 * T2 + T1
CTransMatrix CTransMatrix::operator * ( const CTransMatrix& operand )
{
	CTransMatrix product;
	product.m_R = m_R * operand.m_R;
	product.m_T = m_R * operand.m_T + m_T;
	return product;
}

CTransMatrix& CTransMatrix::operator = ( const CTransMatrix& operand )
{
	m_R = operand.m_R;
	m_T = operand.m_T;
	return (*this);
}


void CTransMatrix::SetToIdentity()
{
	m_R.SetToIdentity();
	m_T.SetTo( 0, 0, 0 ); 
}


void CTransMatrix::SetTranslate( CVector3 &trans )
{
	m_T = trans;
}

CVector3  CTransMatrix::GetTranslate()
{
	return m_T;
}


void CTransMatrix::SetRotate( RotMatrix &rot )
{
	m_R = rot;
}

RotMatrix CTransMatrix::GetRotate( void )
{
	return m_R;
}


bool CTransMatrix::Legal(void)
{
	return m_R.isOrthogonal();
}


//设置第row行,第col列的元素值
void CTransMatrix::SetElement( int row, int col, double value )
{
	assert( row >= 0 && row <= 2 );
	assert( col >= 0 && col <= 3 );

	if ( col == 3 )
	{
		m_T[row] = value;
	}
	else
	{
		m_R.SetElement( row, col, value );
	}
}
	
//返还第row行,第col列的元素值
double CTransMatrix::GetElement( int row, int col )
{
	assert( row >= 0 && row <= 2 );
	assert( col >= 0 && col <= 3 );

	if ( col == 3 )
	{
		return m_T[row] ;
	}
	else
	{
		return m_R.GetElement( row, col );
	}
}

void CTransMatrix::FromEulerAngle( ROTATIONTYPE rtype,  CVector3& eulerAngles )
{
	//double x = eulerAngles[0];
	//double y = eulerAngles[1];
	//double z = eulerAngles[2];

	switch( rtype )
	{
	case XYZ:
		{
			// rotation matrix = Rx*Ry*Rz
			/*m_R = RotMatrix::GetRotMatrixAboutXAxis( x ) * 
				  RotMatrix::GetRotMatrixAboutYAxis( y ) * 
				  RotMatrix::GetRotMatrixAboutZAxis( z );*/
			m_R = RotMatrix( rtype, eulerAngles );
			m_T.SetTo( 0.0, 0.0, 0.0 );
			break;
		  }
	default:
		{
			assert(0);
		}
	}

	assert( Legal() );
}

CVector3 CTransMatrix::ToEulerAngle( ROTATIONTYPE rtype )
{
	return m_R.ToEulerAngle( rtype );
}

// R*vect + T
CVector3 CTransMatrix::operator * ( CVector3& vect )
{
	return m_R * vect + m_T;
}

// iR = R^(-1), iT = -1*R^(-1)*T
CTransMatrix CTransMatrix::GetInverse()
{
	//m_R必须为正交矩
	assert( Legal() );
	CTransMatrix inverse;
	
	//因为假设m_R为正交矩阵，因此m_R的逆等于转置
	inverse.m_R = m_R.GetTranspose();
	inverse.m_T = ( m_R.GetTranspose() * (-1) ) * m_T;

	return inverse;
}

