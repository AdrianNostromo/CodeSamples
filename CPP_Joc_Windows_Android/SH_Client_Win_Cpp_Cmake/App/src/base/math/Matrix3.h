#pragma once

class Matrix4;

class Matrix3 {
public:
	static unsigned int M00;
	static unsigned int M01;
	static unsigned int M02;
	static unsigned int M10;
	static unsigned int M11;
	static unsigned int M12;
	static unsigned int M20;
	static unsigned int M21;
	static unsigned int M22;

	static float tmp[9];

	float val[9];
	
public:
	Matrix3();
	Matrix3(Matrix4& mat);

	Matrix3* idt();
	
	Matrix3* set(Matrix3& mat);
	Matrix3* set(Matrix3* mat);

	Matrix3* set(Matrix4& mat);
	
	// These are currently prefixed to avoid the inverse,transpose bug for 3d when calculating the normal. That should be done in mat4.
	Matrix3* inv();
	Matrix3* transpose();
	
	float det();
	
	~Matrix3();

};

#include <base/math/Matrix4.h>
