#include "stdafx.h"
#include "math.h"
#include "vector.h"


//-------------------------------------------------------------------------------
// class CVector3
//-------------------------------------------------------------------------------

CVector3::CVector3()
//default constructor
{
	m_x = m_y = m_z = 0.0;
}

CVector3::CVector3(double x,double y,double z)
// construt a vector from 3 doubles
{
	m_x = x; m_y = y; m_z = z;
}

CVector3::CVector3(const CVector3& v)
// copy constructor
{
    m_x = v.GetX(); m_y = v.GetY(); m_z = v.GetZ();
}

CVector3::~CVector3()
{

}

double CVector3::GetX() const
{
	return m_x;
}

double CVector3::GetY() const
{
	return m_y;
}

double CVector3::GetZ() const
{ 
	return m_z;
}

void CVector3::SetTo(double x, double y, double z)
{
	m_x = x; m_y = y; m_z = z;
}

double CVector3::EuclideanNorms(void)
{ 
	return sqrt(m_x*m_x + m_y*m_y +m_z*m_z);
}

void CVector3::Normalize()
{
	// to normalize the vector
	double temp = m_x*m_x+m_y*m_y+m_z*m_z;
	if ( temp <= 0.000001 )
	{
		m_x = m_y = m_z = 0.0;
	}
	else 
	{
		temp = sqrt(temp);
		m_x = m_x /temp;
		m_y = m_y /temp;
		m_z = m_z /temp;
	}
}


CVector3::CVector3(double dwVec[])
{
	m_x = dwVec[0]; m_y = dwVec[1]; m_z = dwVec[2];
}

CVector3& CVector3::operator =(const CVector3& vect)
{
	m_x = vect.GetX();
	m_y = vect.GetY();
	m_z = vect.GetZ();
	return *this;
} 

double & CVector3::operator [](int i)
{
	switch(i)
	{
	case 0:
		return m_x;
	case 1:
		return m_y;
	case 2:
		return m_z;
	default: // overflow
		return m_x;
	}
}


CVector3 operator +(const CVector3& vector1, const CVector3& vector2) //why no CVector3::friend func
// plus 
{
	CVector3 v(vector1.m_x + vector2.m_x, 
		      vector1.m_y + vector2.m_y,
			  vector1.m_z + vector2.m_z);
	return v;
}

CVector3 operator -(const CVector3& vector1, const CVector3& vector2) 
// minus
{
	CVector3 v(vector1.m_x - vector2.m_x, 
		      vector1.m_y - vector2.m_y,
			  vector1.m_z - vector2.m_z);
	return v;
}

double operator *(const CVector3& vector1, const CVector3& vector2) 
// dot product
{
	return vector1.m_x*vector2.m_x
		  +vector1.m_y*vector2.m_y
		  +vector1.m_z*vector2.m_z;
}

CVector3 operator %(const CVector3& vector1, const CVector3& vector2)
// cross product
{
	double x, y, z, x1, y1, z1, x2, y2, z2;
	x1 = vector1.m_x; y1 = vector1.m_y; z1 = vector1.m_z;
	x2 = vector2.m_x; y2 = vector2.m_y; z2 = vector2.m_z;
	x  = y1*z2 - y2*z1 ; y  = z1*x2 - z2*x1 ; z  = x1*y2 - x2*y1;
	CVector3 v(x,y,z);
	return v;
}

CVector3 operator *(const double c, const CVector3& v)
// numeric product
{
	CVector3 v0(c*v.m_x, c*v.m_y, c*v.m_z);
	return v0;
}

CVector3 operator *(const CVector3& v, const double c)
// numeric product
{
	return c*v;
}