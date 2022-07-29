#pragma once

class cMatrix; 

class cVector3
{
public :
	cVector3(); 
	~cVector3(); 

	float x, y, z; 

	cVector3(float x, float y, float z);

	void Print(); 

	bool operator==(cVector3& vec); 
	bool operator!=(cVector3& vec);
	cVector3 operator+(cVector3& vec); 
	cVector3 operator-(const cVector3& vec) const ;
	cVector3 operator*(float f);
	cVector3 operator/(float f);

	static float Dot(cVector3& v1, cVector3& v2); 
	static cVector3 Cross(cVector3& v1, cVector3& v2);
	float Length(); 
	cVector3 Normalize(); 

	static cVector3	TransformCoord(cVector3& v, cMatrix& mat); // : point
	static cVector3	TransformNormal(cVector3& v, cMatrix& mat); // : vector

};

