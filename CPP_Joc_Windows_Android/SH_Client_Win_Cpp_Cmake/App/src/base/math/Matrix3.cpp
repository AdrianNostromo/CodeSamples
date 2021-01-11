#include "Matrix3.h"
#include <assert.h>

unsigned int Matrix3::M00 = 0;
unsigned int Matrix3::M01 = 3;
unsigned int Matrix3::M02 = 6;
unsigned int Matrix3::M10 = 1;
unsigned int Matrix3::M11 = 4;
unsigned int Matrix3::M12 = 7;
unsigned int Matrix3::M20 = 2;
unsigned int Matrix3::M21 = 5;
unsigned int Matrix3::M22 = 8;

float Matrix3::tmp[9];

Matrix3::Matrix3() {
	idt();
}

Matrix3::Matrix3(Matrix4& mat) {
	val[M00] = mat.val[Matrix4::M00];
	val[M10] = mat.val[Matrix4::M10];
	val[M20] = mat.val[Matrix4::M20];
	val[M01] = mat.val[Matrix4::M01];
	val[M11] = mat.val[Matrix4::M11];
	val[M21] = mat.val[Matrix4::M21];
	val[M02] = mat.val[Matrix4::M02];
	val[M12] = mat.val[Matrix4::M12];
	val[M22] = mat.val[Matrix4::M22];
}

Matrix3* Matrix3::idt() {
	val[M00] = 1;
	val[M10] = 0;
	val[M20] = 0;
	val[M01] = 0;
	val[M11] = 1;
	val[M21] = 0;
	val[M02] = 0;
	val[M12] = 0;
	val[M22] = 1;

	return this;
}

Matrix3* Matrix3::set(Matrix3& mat) {
	val[M00] = mat.val[Matrix3::M00];
	val[M10] = mat.val[Matrix3::M10];
	val[M20] = mat.val[Matrix3::M20];
	val[M01] = mat.val[Matrix3::M01];
	val[M11] = mat.val[Matrix3::M11];
	val[M21] = mat.val[Matrix3::M21];
	val[M02] = mat.val[Matrix3::M02];
	val[M12] = mat.val[Matrix3::M12];
	val[M22] = mat.val[Matrix3::M22];

	return this;
}

Matrix3* Matrix3::set(Matrix3* mat) {
	return set(*mat);
}

/** Sets this 3x3 matrix to the top left 3x3 corner of the provided 4x4 matrix.
* @param mat The matrix whose top left corner will be copied. This matrix will not be modified.
* @return This matrix for the purpose of chaining operations. */
Matrix3* Matrix3::set(Matrix4& mat) {
	val[M00] = mat.val[Matrix4::M00];
	val[M10] = mat.val[Matrix4::M10];
	val[M20] = mat.val[Matrix4::M20];
	val[M01] = mat.val[Matrix4::M01];
	val[M11] = mat.val[Matrix4::M11];
	val[M21] = mat.val[Matrix4::M21];
	val[M02] = mat.val[Matrix4::M02];
	val[M12] = mat.val[Matrix4::M12];
	val[M22] = mat.val[Matrix4::M22];

	return this;
}

/** Inverts this matrix given that the determinant is != 0.
* @return This matrix for the purpose of chaining operations.
* @throws GdxRuntimeException if the matrix is singular (not invertible) */
Matrix3* Matrix3::inv() {
	float det = this->det();
	if (det == 0) {
		assert(false);
	}

	float inv_det = 1.0f / det;

	tmp[M00] = val[M11] * val[M22] - val[M21] * val[M12];
	tmp[M10] = val[M20] * val[M12] - val[M10] * val[M22];
	tmp[M20] = val[M10] * val[M21] - val[M20] * val[M11];
	tmp[M01] = val[M21] * val[M02] - val[M01] * val[M22];
	tmp[M11] = val[M00] * val[M22] - val[M20] * val[M02];
	tmp[M21] = val[M20] * val[M01] - val[M00] * val[M21];
	tmp[M02] = val[M01] * val[M12] - val[M11] * val[M02];
	tmp[M12] = val[M10] * val[M02] - val[M00] * val[M12];
	tmp[M22] = val[M00] * val[M11] - val[M10] * val[M01];

	val[M00] = inv_det * tmp[M00];
	val[M10] = inv_det * tmp[M10];
	val[M20] = inv_det * tmp[M20];
	val[M01] = inv_det * tmp[M01];
	val[M11] = inv_det * tmp[M11];
	val[M21] = inv_det * tmp[M21];
	val[M02] = inv_det * tmp[M02];
	val[M12] = inv_det * tmp[M12];
	val[M22] = inv_det * tmp[M22];

	return this;
}

/** Transposes the current matrix.
* @return This matrix for the purpose of chaining methods together. */
Matrix3* Matrix3::transpose() {
	// Where MXY you do not have to change MXX
	float v01 = val[M10];
	float v02 = val[M20];
	float v10 = val[M01];
	float v12 = val[M21];
	float v20 = val[M02];
	float v21 = val[M12];
	val[M01] = v01;
	val[M02] = v02;
	val[M10] = v10;
	val[M12] = v12;
	val[M20] = v20;
	val[M21] = v21;
	return this;
}

/** @return The determinant of this matrix */
float Matrix3::det() {
	return val[M00] * val[M11] * val[M22] + val[M01] * val[M12] * val[M20] + val[M02] * val[M10] * val[M21] - val[M00]
		* val[M12] * val[M21] - val[M01] * val[M10] * val[M22] - val[M02] * val[M11] * val[M20];
}

Matrix3::~Matrix3() {
	//void
}
