#pragma once

#include <vector>

class cVector3; 

class cMatrix
{
public : 
	cMatrix(); 
	~cMatrix(); 

private :
	class cRow
	{
	private : 
		std::vector<float> m_vecData; 
	public :
		cRow(); 
		cRow(int nDimension); 
		~cRow() {}
		void Resize(int nDimension); 
		float& operator[] (int nIndex); 
	};

	std::vector<cRow> m_vecData; 
public :
	cMatrix(int nDimension); 
	void Print(); 
	void Resize(int nDimension); 
	int Dimension(); 

	static cMatrix Identity(int nDimension); 

	cRow& operator[] (int nIndex); 

	bool operator==(cMatrix& mat); 
	bool operator!=(cMatrix& mat);
	cMatrix operator+(cMatrix& mat);
	cMatrix operator-(cMatrix& mat);
	cMatrix operator*(cMatrix& mat);
	cMatrix operator*(float f);

	cMatrix Inverse(OUT float& fDeterminant); 
	float Determinant(); 
	cMatrix Adjoint(); 
	cMatrix Transpose(); 
	float Cofactor(int nRow, int nCol); 
	float Minor(int nRow, int nCol); 


	static cMatrix Translation(float x, float y, float z); 
	static cMatrix Translation(cVector3& v);
	static cMatrix RotationX(float fAngle); 
	static cMatrix RotationY(float fAngle);
	static cMatrix RotationZ(float fAngle);
	static cMatrix View(cVector3& vEye, cVector3& vLookAt, cVector3& vUp); 
	static cMatrix Projection(float fFovY, float fAspect, float fNearZ, float fFarZ); 
	static cMatrix Viewport(float x, float y, float w, float h, float minz, float max); 
	static cMatrix Scale(float scale); 
};

