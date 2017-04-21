			
#pragma once
//-------------------------------------------------------------------------------
//
//  class CVector3
//
//-------------------------------------------------------------------------------

class CVector3
{
protected:
	double m_x, m_y, m_z; // the 3 components of the vector

public:
	CVector3();                                // default constructor
	CVector3(const CVector3& v);               // copy construtor
	CVector3(double x, double y, double z);    // construct a vector from 3 double number
	CVector3(double dwVec[]);                  // construct a vector from a array with 3 elements
	virtual ~CVector3();                       // destructor

	void   SetTo(double x, double y, double z);// set this vector by 3 doubles
	double GetX() const;                       // get the x component of this vector 
	double GetY() const;                       // get the y component of this vector 
	double GetZ() const;                       // get the z component of this vector
	void   GetData( double &x, double &y, double &z ) const { x = m_x, y = m_y, z = m_z; }
	double EuclideanNorms(void);
	void   Normalize();                        // make this vector satisfy: x*x+y*y+z*z == 1.0
   	virtual CVector3& operator =(const CVector3& vector) ;    // assign operator
	double & operator [](int i);

	// operators
	friend 
	CVector3 operator *(const double c, const CVector3& v);                 // numerical product
	friend 
	CVector3 operator *(const CVector3& v, const double c);                 // numerical product
	friend 
	double  operator *(const CVector3& vector1, const CVector3& vector2);   // dot product
	friend  
	CVector3 operator +(const CVector3& vector1, const CVector3& vector2);  // plus operator
	friend  
	CVector3 operator -(const CVector3& vector1, const CVector3& vector2);  // minus operator
	friend  
	CVector3 operator %(const CVector3& vector1, const CVector3& vector2);  // cross product
};            
