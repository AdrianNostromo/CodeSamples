#include "Matrix4.h"
#include <cstring>
#include <base/math/Math.h>
#include "Vector3.h"
#include "Quaternion.h"

unsigned int Matrix4::M00 = 0;
unsigned int Matrix4::M01 = 4;
unsigned int Matrix4::M02 = 8;
unsigned int Matrix4::M03 = 12;
unsigned int Matrix4::M10 = 1;
unsigned int Matrix4::M11 = 5;
unsigned int Matrix4::M12 = 9;
unsigned int Matrix4::M13 = 13;
unsigned int Matrix4::M20 = 2;
unsigned int Matrix4::M21 = 6;
unsigned int Matrix4::M22 = 10;
unsigned int Matrix4::M23 = 14;
unsigned int Matrix4::M30 = 3;
unsigned int Matrix4::M31 = 7;
unsigned int Matrix4::M32 = 11;
unsigned int Matrix4::M33 = 15;

Vector3 Matrix4::tmpVec{};
Matrix4 Matrix4::tmpMat{};

Vector3 Matrix4::l_vez{};
Vector3 Matrix4::l_vex{};
Vector3 Matrix4::l_vey{};

float Matrix4::tmp[16];

Matrix4::Matrix4() {
	idt();
}

Matrix4::Matrix4(const Matrix4* src) {
	set(src);
}

Matrix4::Matrix4(const Matrix4& src) {
	set(src);
}

void Matrix4::idt() {
	val[M00] = 1.0f;
	val[M01] = 0.0f;
	val[M02] = 0.0f;
	val[M03] = 0.0f;

	val[M10] = 0.0f;
	val[M11] = 1.0f;
	val[M12] = 0.0f;
	val[M13] = 0.0f;

	val[M20] = 0.0f;
	val[M21] = 0.0f;
	val[M22] = 1.0f;
	val[M23] = 0.0f;

	val[M30] = 0.0f;
	val[M31] = 0.0f;
	val[M32] = 0.0f;
	val[M33] = 1.0f;
}

Matrix4* Matrix4::setToOrtho2D(float x, float y, float width, float height) {
	return setToOrtho(x, x + width, y, y + height, 0.0f, 1.0f);
}

Matrix4* Matrix4::setToOrtho(float left, float right, float bottom, float top, float near, float far) {
	idt();
	float x_orth = 2.0f / (right - left);
	float y_orth = 2.0f / (top - bottom);
	float z_orth = -2.0f / (far - near);

	float tx = -(right + left) / (right - left);
	float ty = -(top + bottom) / (top - bottom);
	float tz = -(far + near) / (far - near);

	val[M00] = x_orth;
	val[M10] = 0;
	val[M20] = 0;
	val[M30] = 0;
	val[M01] = 0;
	val[M11] = y_orth;
	val[M21] = 0;
	val[M31] = 0;
	val[M02] = 0;
	val[M12] = 0;
	val[M22] = z_orth;
	val[M32] = 0;
	val[M03] = tx;
	val[M13] = ty;
	val[M23] = tz;
	val[M33] = 1;

    return this;
}

Matrix4* Matrix4::setToProjection(float near, float far, float fovy, float aspectRatio) {
    idt();
    float l_fd = (float)(1.0 / Math::tanRad((fovy * (Math::PI_F / 180.0f)) / 2.0f));
    float l_a1 = (far + near) / (near - far);
    float l_a2 = (2 * far * near) / (near - far);
    val[M00] = l_fd / aspectRatio;
    val[M10] = 0;
    val[M20] = 0;
    val[M30] = 0;
    val[M01] = 0;
    val[M11] = l_fd;
    val[M21] = 0;
    val[M31] = 0;
    val[M02] = 0;
    val[M12] = 0;
    val[M22] = l_a1;
    val[M32] = -1;
    val[M03] = 0;
    val[M13] = 0;
    val[M23] = l_a2;
    val[M33] = 0;

    return this;
}

Matrix4* Matrix4::setToProjection(float left, float right, float bottom, float top, float near, float far) {
    float x = 2.0f * near / (right - left);
    float y = 2.0f * near / (top - bottom);
    float a = (right + left) / (right - left);
    float b = (top + bottom) / (top - bottom);
    float l_a1 = (far + near) / (near - far);
    float l_a2 = (2 * far * near) / (near - far);
    val[M00] = x;
    val[M10] = 0;
    val[M20] = 0;
    val[M30] = 0;
    val[M01] = 0;
    val[M11] = y;
    val[M21] = 0;
    val[M31] = 0;
    val[M02] = a;
    val[M12] = b;
    val[M22] = l_a1;
    val[M32] = -1;
    val[M03] = 0;
    val[M13] = 0;
    val[M23] = l_a2;
    val[M33] = 0;

    return this;
}

void Matrix4::set(Affine2& affine) {
	val[M00] = affine.m00;
	val[M10] = affine.m10;
	val[M20] = 0;
	val[M30] = 0;
	val[M01] = affine.m01;
	val[M11] = affine.m11;
	val[M21] = 0;
	val[M31] = 0;
	val[M02] = 0;
	val[M12] = 0;
	val[M22] = 1;
	val[M32] = 0;
	val[M03] = affine.m02;
	val[M13] = affine.m12;
	val[M23] = 0;
	val[M33] = 1;
}

Matrix4* Matrix4::set(const Matrix4* matrix) {
	return set(matrix->val);
}

Matrix4* Matrix4::set(const Matrix4& matrix) {
	return set(matrix.val);
}

Matrix4* Matrix4::set(const float* values /*16 floats*/) {
	memcpy(this->val, values, 16 * sizeof(float));

	return this;
}

Matrix4* Matrix4::translate(float x, float y, float z) {
	tmp[M00] = 1;
	tmp[M01] = 0;
	tmp[M02] = 0;
	tmp[M03] = x;
	tmp[M10] = 0;
	tmp[M11] = 1;
	tmp[M12] = 0;
	tmp[M13] = y;
	tmp[M20] = 0;
	tmp[M21] = 0;
	tmp[M22] = 1;
	tmp[M23] = z;
	tmp[M30] = 0;
	tmp[M31] = 0;
	tmp[M32] = 0;
	tmp[M33] = 1;

	Mul(val, tmp);

	return this;
}

Matrix4* Matrix4::translate(Vector3& translation) {
	return translate(translation.x, translation.y, translation.z);
}

Matrix4* Matrix4::scale(float scaleX, float scaleY, float scaleZ) {
	tmp[M00] = scaleX;
	tmp[M01] = 0;
	tmp[M02] = 0;
	tmp[M03] = 0;
	tmp[M10] = 0;
	tmp[M11] = scaleY;
	tmp[M12] = 0;
	tmp[M13] = 0;
	tmp[M20] = 0;
	tmp[M21] = 0;
	tmp[M22] = scaleZ;
	tmp[M23] = 0;
	tmp[M30] = 0;
	tmp[M31] = 0;
	tmp[M32] = 0;
	tmp[M33] = 1;

	Mul(val, tmp);
	return this;
}

Matrix4* Matrix4::rotate(Quaternion& rotation) {
	rotation.toMatrix(tmp);
	Mul(val, tmp);
	return this;
}

Matrix4* Matrix4::setToLookAt(const Vector3& direction, const Vector3& up) {
	l_vez.set(direction)->nor();
	l_vex.set(direction)->nor();
	l_vex.crs(up)->nor();
	l_vey.set(l_vex)->crs(l_vez)->nor();
	idt();
	val[M00] = l_vex.x;
	val[M01] = l_vex.y;
	val[M02] = l_vex.z;
	val[M10] = l_vey.x;
	val[M11] = l_vey.y;
	val[M12] = l_vey.z;
	val[M20] = -l_vez.x;
	val[M21] = -l_vez.y;
	val[M22] = -l_vez.z;

	return this;
}

Matrix4* Matrix4::setToLookAt(const Vector3& position, const Vector3& target, const Vector3& up) {
	tmpVec.set(target)->sub(position);
	setToLookAt(tmpVec, up);
	mul(tmpMat.setToTranslation(-position.x, -position.y, -position.z));

	return this;
}

Matrix4* Matrix4::setToTranslation(const Vector3& vector) {
	idt();
	val[M03] = vector.x;
	val[M13] = vector.y;
	val[M23] = vector.z;
	return this;
}

Matrix4* Matrix4::setToTranslation(float x, float y, float z) {
	idt();
	val[M03] = x;
	val[M13] = y;
	val[M23] = z;
	return this;
}

Matrix4* Matrix4::set(Vector3& position, Quaternion& orientation, Vector3& scale) {
	return set(position.x, position.y, position.z, orientation.x, orientation.y, orientation.z, orientation.w, scale.x,
		scale.y, scale.z);
}

Matrix4* Matrix4::set(
	float translationX, float translationY, float translationZ,
	float quaternionX, float quaternionY, float quaternionZ, float quaternionW,
	float scaleX, float scaleY, float scaleZ)
{
	const float xs = quaternionX * 2.0f, ys = quaternionY * 2.0f, zs = quaternionZ * 2.0f;
	const float wx = quaternionW * xs, wy = quaternionW * ys, wz = quaternionW * zs;
	const float xx = quaternionX * xs, xy = quaternionX * ys, xz = quaternionX * zs;
	const float yy = quaternionY * ys, yz = quaternionY * zs, zz = quaternionZ * zs;

	val[M00] = scaleX * (1.0f - (yy + zz));
	val[M01] = scaleY * (xy - wz);
	val[M02] = scaleZ * (xz + wy);
	val[M03] = translationX;

	val[M10] = scaleX * (xy + wz);
	val[M11] = scaleY * (1.0f - (xx + zz));
	val[M12] = scaleZ * (yz - wx);
	val[M13] = translationY;

	val[M20] = scaleX * (xz - wy);
	val[M21] = scaleY * (yz + wx);
	val[M22] = scaleZ * (1.0f - (xx + yy));
	val[M23] = translationZ;

	val[M30] = 0.f;
	val[M31] = 0.f;
	val[M32] = 0.f;
	val[M33] = 1.0f;

	return this;
}

Matrix4* Matrix4::mul(Matrix4& matrix) {
	Mul(val, matrix.val);
	return this;
}

Matrix4* Matrix4::mul(Matrix4* matrix) {
	Mul(val, (*matrix).val);
	return this;
}

bool Matrix4::_inv() {
	return Inv(val);
}

Matrix4* Matrix4::invUnchecked() {
	Inv(val);

	return this;
}

Matrix4* Matrix4::transpose() {
	tmp[M00] = val[M00];
	tmp[M01] = val[M10];
	tmp[M02] = val[M20];
	tmp[M03] = val[M30];
	tmp[M10] = val[M01];
	tmp[M11] = val[M11];
	tmp[M12] = val[M21];
	tmp[M13] = val[M31];
	tmp[M20] = val[M02];
	tmp[M21] = val[M12];
	tmp[M22] = val[M22];
	tmp[M23] = val[M32];
	tmp[M30] = val[M03];
	tmp[M31] = val[M13];
	tmp[M32] = val[M23];
	tmp[M33] = val[M33];

	return set(tmp);
}

Vector3* Matrix4::getTranslation(Vector3& position) {
    position.x = val[M03];
    position.y = val[M13];
    position.z = val[M23];
    return &position;
}

/** Gets the rotation of this matrix.
 * @param rotation The {@link Quaternion} to receive the rotation
 * @param normalizeAxes True to normalize the axes, necessary when the matrix might also include scaling.
 * @return The provided {@link Quaternion} for chaining. */
Quaternion* Matrix4::getRotation(Quaternion& rotation, bool normalizeAxes) {
	return rotation.setFromMatrix(normalizeAxes, this);
}

/** Gets the rotation of this matrix.
 * @param rotation The {@link Quaternion} to receive the rotation
 * @return The provided {@link Quaternion} for chaining. */
Quaternion* Matrix4::getRotation(Quaternion& rotation) {
	return rotation.setFromMatrix(this);
}

/** @return the squared scale factor on the X axis */
float Matrix4::getScaleXSquared() {
	return val[Matrix4::M00] * val[Matrix4::M00] + val[Matrix4::M01] * val[Matrix4::M01] + val[Matrix4::M02] * val[Matrix4::M02];
}

/** @return the squared scale factor on the Y axis */
float Matrix4::getScaleYSquared() {
	return val[Matrix4::M10] * val[Matrix4::M10] + val[Matrix4::M11] * val[Matrix4::M11] + val[Matrix4::M12] * val[Matrix4::M12];
}

/** @return the squared scale factor on the Z axis */
float Matrix4::getScaleZSquared() {
	return val[Matrix4::M20] * val[Matrix4::M20] + val[Matrix4::M21] * val[Matrix4::M21] + val[Matrix4::M22] * val[Matrix4::M22];
}

/** @return the scale factor on the X axis (non-negative) */
float Matrix4::getScaleX() {
	return (Math::isZero(val[Matrix4::M01]) && Math::isZero(val[Matrix4::M02])) ? Math::abs(val[Matrix4::M00])
		: Math::sqrt(getScaleXSquared());
}

/** @return the scale factor on the Y axis (non-negative) */
float Matrix4::getScaleY() {
	return (Math::isZero(val[Matrix4::M10]) && Math::isZero(val[Matrix4::M12])) ? Math::abs(val[Matrix4::M11])
		: Math::sqrt(getScaleYSquared());
}

/** @return the scale factor on the X axis (non-negative) */
float Matrix4::getScaleZ() {
	return (Math::isZero(val[Matrix4::M20]) && Math::isZero(val[Matrix4::M21])) ? Math::abs(val[Matrix4::M22])
		: Math::sqrt(getScaleZSquared());
}

/** @param scale The vector which will receive the (non-negative) scale components on each axis.
 * @return The provided vector for chaining. */
Vector3* Matrix4::getScale(Vector3& scale) {
	return scale.set(getScaleX(), getScaleY(), getScaleZ());
}

bool Matrix4::equals(Matrix4& o) {
	if(val[M00] != o.val[M00]
	|| val[M01] != o.val[M01]
	|| val[M02] != o.val[M02]
	|| val[M03] != o.val[M03]
	|| val[M10] != o.val[M10]
	|| val[M11] != o.val[M11]
	|| val[M12] != o.val[M12]
	|| val[M13] != o.val[M13]
	|| val[M20] != o.val[M20]
	|| val[M21] != o.val[M21]
	|| val[M22] != o.val[M22]
	|| val[M23] != o.val[M23]
	|| val[M30] != o.val[M30]
	|| val[M31] != o.val[M31]
	|| val[M32] != o.val[M32]
	|| val[M33] != o.val[M33])
	{
		return false;
	}

	return true;
}

void Matrix4::Mul(float* mata, float* matb) {
	tmp[M00] = mata[M00] * matb[M00] + mata[M01] * matb[M10] + mata[M02] * matb[M20] + mata[M03] * matb[M30];
	tmp[M01] = mata[M00] * matb[M01] + mata[M01] * matb[M11] + mata[M02] * matb[M21] + mata[M03] * matb[M31];
	tmp[M02] = mata[M00] * matb[M02] + mata[M01] * matb[M12] + mata[M02] * matb[M22] + mata[M03] * matb[M32];
	tmp[M03] = mata[M00] * matb[M03] + mata[M01] * matb[M13] + mata[M02] * matb[M23] + mata[M03] * matb[M33];
	tmp[M10] = mata[M10] * matb[M00] + mata[M11] * matb[M10] + mata[M12] * matb[M20] + mata[M13] * matb[M30];
	tmp[M11] = mata[M10] * matb[M01] + mata[M11] * matb[M11] + mata[M12] * matb[M21] + mata[M13] * matb[M31];
	tmp[M12] = mata[M10] * matb[M02] + mata[M11] * matb[M12] + mata[M12] * matb[M22] + mata[M13] * matb[M32];
	tmp[M13] = mata[M10] * matb[M03] + mata[M11] * matb[M13] + mata[M12] * matb[M23] + mata[M13] * matb[M33];
	tmp[M20] = mata[M20] * matb[M00] + mata[M21] * matb[M10] + mata[M22] * matb[M20] + mata[M23] * matb[M30];
	tmp[M21] = mata[M20] * matb[M01] + mata[M21] * matb[M11] + mata[M22] * matb[M21] + mata[M23] * matb[M31];
	tmp[M22] = mata[M20] * matb[M02] + mata[M21] * matb[M12] + mata[M22] * matb[M22] + mata[M23] * matb[M32];
	tmp[M23] = mata[M20] * matb[M03] + mata[M21] * matb[M13] + mata[M22] * matb[M23] + mata[M23] * matb[M33];
	tmp[M30] = mata[M30] * matb[M00] + mata[M31] * matb[M10] + mata[M32] * matb[M20] + mata[M33] * matb[M30];
	tmp[M31] = mata[M30] * matb[M01] + mata[M31] * matb[M11] + mata[M32] * matb[M21] + mata[M33] * matb[M31];
	tmp[M32] = mata[M30] * matb[M02] + mata[M31] * matb[M12] + mata[M32] * matb[M22] + mata[M33] * matb[M32];
	tmp[M33] = mata[M30] * matb[M03] + mata[M31] * matb[M13] + mata[M32] * matb[M23] + mata[M33] * matb[M33];
	memcpy(mata, tmp, sizeof(float) * 16);
}

bool Matrix4::Inv(float* values) {
    return matrix4_inv(values);
}

/** Multiplies the vectors with the given matrix, , performing a division by w. The matrix array is assumed to hold a 4x4 column
 * major matrix as you can get from {@link Matrix4#val}. The vectors array is assumed to hold 3-component vectors. Offset
 * specifies the offset into the array where the x-component of the first vector is located. The numVecs parameter specifies
 * the number of vectors stored in the vectors array. The stride parameter specifies the number of floats between subsequent
 * vectors and must be >= 3. This is the same as {@link Vector3#prj(Matrix4)} applied to multiple vectors.
 *
 * @param mat the matrix
 * @param vecs the vectors
 * @param offset the offset into the vectors array
 * @param numVecs the number of vectors
 * @param stride the stride between vectors in floats */
void Matrix4::prj(float* mat, float* vecs, int offset, int numVecs, int stride) {
    float *vecPtr = vecs + offset;
    for (int i = 0; i < numVecs; i++) {
        matrix4_proj(mat, vecPtr);
        vecPtr += stride;
    }
}

inline float Matrix4::matrix4_det(float* val) {
    return val[M30] * val[M21] * val[M12] * val[M03] - val[M20] * val[M31] * val[M12] * val[M03] - val[M30] * val[M11]
            * val[M22] * val[M03] + val[M10] * val[M31] * val[M22] * val[M03] + val[M20] * val[M11] * val[M32] * val[M03] - val[M10]
            * val[M21] * val[M32] * val[M03] - val[M30] * val[M21] * val[M02] * val[M13] + val[M20] * val[M31] * val[M02] * val[M13]
            + val[M30] * val[M01] * val[M22] * val[M13] - val[M00] * val[M31] * val[M22] * val[M13] - val[M20] * val[M01] * val[M32]
            * val[M13] + val[M00] * val[M21] * val[M32] * val[M13] + val[M30] * val[M11] * val[M02] * val[M23] - val[M10] * val[M31]
            * val[M02] * val[M23] - val[M30] * val[M01] * val[M12] * val[M23] + val[M00] * val[M31] * val[M12] * val[M23] + val[M10]
            * val[M01] * val[M32] * val[M23] - val[M00] * val[M11] * val[M32] * val[M23] - val[M20] * val[M11] * val[M02] * val[M33]
            + val[M10] * val[M21] * val[M02] * val[M33] + val[M20] * val[M01] * val[M12] * val[M33] - val[M00] * val[M21] * val[M12]
            * val[M33] - val[M10] * val[M01] * val[M22] * val[M33] + val[M00] * val[M11] * val[M22] * val[M33];
}

inline bool Matrix4::matrix4_inv(float* val) {
    float tmp[16];
    float l_det = matrix4_det(val);
    if (l_det == 0) {
        return false;
    }

    tmp[M00] = val[M12] * val[M23] * val[M31] - val[M13] * val[M22] * val[M31] + val[M13] * val[M21] * val[M32] - val[M11]
        * val[M23] * val[M32] - val[M12] * val[M21] * val[M33] + val[M11] * val[M22] * val[M33];
    tmp[M01] = val[M03] * val[M22] * val[M31] - val[M02] * val[M23] * val[M31] - val[M03] * val[M21] * val[M32] + val[M01]
        * val[M23] * val[M32] + val[M02] * val[M21] * val[M33] - val[M01] * val[M22] * val[M33];
    tmp[M02] = val[M02] * val[M13] * val[M31] - val[M03] * val[M12] * val[M31] + val[M03] * val[M11] * val[M32] - val[M01]
        * val[M13] * val[M32] - val[M02] * val[M11] * val[M33] + val[M01] * val[M12] * val[M33];
    tmp[M03] = val[M03] * val[M12] * val[M21] - val[M02] * val[M13] * val[M21] - val[M03] * val[M11] * val[M22] + val[M01]
        * val[M13] * val[M22] + val[M02] * val[M11] * val[M23] - val[M01] * val[M12] * val[M23];
    tmp[M10] = val[M13] * val[M22] * val[M30] - val[M12] * val[M23] * val[M30] - val[M13] * val[M20] * val[M32] + val[M10]
        * val[M23] * val[M32] + val[M12] * val[M20] * val[M33] - val[M10] * val[M22] * val[M33];
    tmp[M11] = val[M02] * val[M23] * val[M30] - val[M03] * val[M22] * val[M30] + val[M03] * val[M20] * val[M32] - val[M00]
        * val[M23] * val[M32] - val[M02] * val[M20] * val[M33] + val[M00] * val[M22] * val[M33];
    tmp[M12] = val[M03] * val[M12] * val[M30] - val[M02] * val[M13] * val[M30] - val[M03] * val[M10] * val[M32] + val[M00]
        * val[M13] * val[M32] + val[M02] * val[M10] * val[M33] - val[M00] * val[M12] * val[M33];
    tmp[M13] = val[M02] * val[M13] * val[M20] - val[M03] * val[M12] * val[M20] + val[M03] * val[M10] * val[M22] - val[M00]
        * val[M13] * val[M22] - val[M02] * val[M10] * val[M23] + val[M00] * val[M12] * val[M23];
    tmp[M20] = val[M11] * val[M23] * val[M30] - val[M13] * val[M21] * val[M30] + val[M13] * val[M20] * val[M31] - val[M10]
        * val[M23] * val[M31] - val[M11] * val[M20] * val[M33] + val[M10] * val[M21] * val[M33];
    tmp[M21] = val[M03] * val[M21] * val[M30] - val[M01] * val[M23] * val[M30] - val[M03] * val[M20] * val[M31] + val[M00]
        * val[M23] * val[M31] + val[M01] * val[M20] * val[M33] - val[M00] * val[M21] * val[M33];
    tmp[M22] = val[M01] * val[M13] * val[M30] - val[M03] * val[M11] * val[M30] + val[M03] * val[M10] * val[M31] - val[M00]
        * val[M13] * val[M31] - val[M01] * val[M10] * val[M33] + val[M00] * val[M11] * val[M33];
    tmp[M23] = val[M03] * val[M11] * val[M20] - val[M01] * val[M13] * val[M20] - val[M03] * val[M10] * val[M21] + val[M00]
        * val[M13] * val[M21] + val[M01] * val[M10] * val[M23] - val[M00] * val[M11] * val[M23];
    tmp[M30] = val[M12] * val[M21] * val[M30] - val[M11] * val[M22] * val[M30] - val[M12] * val[M20] * val[M31] + val[M10]
        * val[M22] * val[M31] + val[M11] * val[M20] * val[M32] - val[M10] * val[M21] * val[M32];
    tmp[M31] = val[M01] * val[M22] * val[M30] - val[M02] * val[M21] * val[M30] + val[M02] * val[M20] * val[M31] - val[M00]
        * val[M22] * val[M31] - val[M01] * val[M20] * val[M32] + val[M00] * val[M21] * val[M32];
    tmp[M32] = val[M02] * val[M11] * val[M30] - val[M01] * val[M12] * val[M30] - val[M02] * val[M10] * val[M31] + val[M00]
        * val[M12] * val[M31] + val[M01] * val[M10] * val[M32] - val[M00] * val[M11] * val[M32];
    tmp[M33] = val[M01] * val[M12] * val[M20] - val[M02] * val[M11] * val[M20] + val[M02] * val[M10] * val[M21] - val[M00]
        * val[M12] * val[M21] - val[M01] * val[M10] * val[M22] + val[M00] * val[M11] * val[M22];

    float inv_det = 1.0f / l_det;
    val[M00] = tmp[M00] * inv_det;
    val[M01] = tmp[M01] * inv_det;
    val[M02] = tmp[M02] * inv_det;
    val[M03] = tmp[M03] * inv_det;
    val[M10] = tmp[M10] * inv_det;
    val[M11] = tmp[M11] * inv_det;
    val[M12] = tmp[M12] * inv_det;
    val[M13] = tmp[M13] * inv_det;
    val[M20] = tmp[M20] * inv_det;
    val[M21] = tmp[M21] * inv_det;
    val[M22] = tmp[M22] * inv_det;
    val[M23] = tmp[M23] * inv_det;
    val[M30] = tmp[M30] * inv_det;
    val[M31] = tmp[M31] * inv_det;
    val[M32] = tmp[M32] * inv_det;
    val[M33] = tmp[M33] * inv_det;

    return true;
}

inline void Matrix4::matrix4_proj(float* mat, float* vec) {
    float inv_w = 1.0f / (vec[0] * mat[M30] + vec[1] * mat[M31] + vec[2] * mat[M32] + mat[M33]);
    float x = (vec[0] * mat[M00] + vec[1] * mat[M01] + vec[2] * mat[M02] + mat[M03]) * inv_w;
    float y = (vec[0] * mat[M10] + vec[1] * mat[M11] + vec[2] * mat[M12] + mat[M13]) * inv_w;
    float z = (vec[0] * mat[M20] + vec[1] * mat[M21] + vec[2] * mat[M22] + mat[M23]) * inv_w;
    vec[0] = x;
    vec[1] = y;
    vec[2] = z;
}

Matrix4::~Matrix4() {
	//void
}
