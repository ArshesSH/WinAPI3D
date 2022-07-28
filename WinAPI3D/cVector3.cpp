#include "framework.h"
#include "cVector3.h"
#include "cMatrix.h"

#define EPSILON		0.0001f

cVector3::cVector3()
	: x(0.0f) , y(0.0f), z(0.0f)
{
}

cVector3::~cVector3()
{
}


cVector3::cVector3(float _x, float _y, float _z)
	: x(_x) , y(_y) , z(_z)
{
}

void cVector3::Print()
{
	printf("Vector : ( %8.2f , %8.2f , %8.2f ) \n", x, y, z); 
}

bool cVector3::operator==(cVector3 & vec)
{
	if (x - EPSILON <= vec.x && vec.x <= x + EPSILON &&
		y - EPSILON <= vec.y && vec.y <= y + EPSILON &&
		z - EPSILON <= vec.z && vec.z <= z + EPSILON)
	{
		return true; 
	}
	return false;
}

bool cVector3::operator!=(cVector3 & vec)
{
	return !(*this == vec);  
}

cVector3 cVector3::operator+(cVector3 & vec)
{
	cVector3 v; 
	v.x = x + vec.x; 
	v.y = y + vec.y;
	v.z = z + vec.z;
	return v;
}

cVector3 cVector3::operator-(cVector3 & vec)
{
	cVector3 v;
	v.x = x - vec.x;
	v.y = y - vec.y;
	v.z = z - vec.z;
	return v;
}

cVector3 cVector3::operator*(float f)
{
	cVector3 v;
	v.x = x * f; 
	v.y = y * f;
	v.z = z * f;
	return v;
}

cVector3 cVector3::operator/(float f)
{
	cVector3 v;
	v.x = x / f;
	v.y = y / f;
	v.z = z / f;
	return v;
}

float cVector3::Dot(cVector3 & v1, cVector3 & v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; 
}

cVector3 cVector3::Cross(cVector3 & v1, cVector3 & v2)
{
	cVector3 v; 
	v.x = v1.y * v2.z - v1.z * v2.y; 
	v.y = v1.z * v2.x - v1.x * v2.z;
	v.z = v1.x * v2.y - v1.y * v2.x;
	return v; 
}

float cVector3::Length()
{
	return (float)sqrt(x*x + y * y + z * z); 
}

cVector3 cVector3::Normalize()
{
	float len = Length(); 
	cVector3 v;
	v.x = x / len; 
	v.y = y / len; 
	v.z = z / len; 
	return v; 
}

cVector3 cVector3::TransformCoord(cVector3 & v, cMatrix & mat)
{
	cVector3	vRet; 
	vRet.x = v.x * mat[0][0] + v.y * mat[1][0] + v.z * mat[2][0] + 1.0f * mat[3][0]; 
	vRet.y = v.x * mat[0][1] + v.y * mat[1][1] + v.z * mat[2][1] + 1.0f * mat[3][1];
	vRet.z = v.x * mat[0][2] + v.y * mat[1][2] + v.z * mat[2][2] + 1.0f * mat[3][2];
	float w = v.x * mat[0][3] + v.y * mat[1][3] + v.z * mat[2][3] + 1.0f * mat[3][3];

	if (w < -EPSILON || w > EPSILON)
	{
		vRet.x /= w; 
		vRet.y /= w;
		vRet.z /= w;
		//w /= w;
	}
	
	return vRet; 
}

cVector3 cVector3::TransformNormal(cVector3 & v, cMatrix & mat)
{
	cVector3	vRet;
	vRet.x = v.x * mat[0][0] + v.y * mat[1][0] + v.z * mat[2][0] ;
	vRet.y = v.x * mat[0][1] + v.y * mat[1][1] + v.z * mat[2][1] ;
	vRet.z = v.x * mat[0][2] + v.y * mat[1][2] + v.z * mat[2][2] ;
	   	 
	return vRet;
}
