#include "Quaternion.h"
#include "Matrix4.h"
#include "Vector3.h"
#include <base/math/Math.h>

Quaternion Quaternion::tmp1{ 0, 0, 0, 0 };
Quaternion Quaternion::tmp2{ 0, 0, 0, 0 };

Quaternion::Quaternion(Quaternion const& other) {
	set(other);
}

Quaternion::Quaternion() {
	idt();
}

Quaternion::Quaternion(float x, float y, float z, float w) {
	this->set(x, y, z, w);
}

Quaternion::Quaternion(Vector3& axis, float angle) {
	this->set(axis, angle);
}

Quaternion* Quaternion::idt() {
	return this->set(0, 0, 0, 1);
}

Quaternion* Quaternion::nor() {
	float len = len2();
	if (len != 0.f && !Math::isEqual(len, 1.0f)) {
		len = Math::sqrt(len);
		w /= len;
		x /= len;
		y /= len;
		z /= len;
	}

	return this;
}

Quaternion* Quaternion::inverse() {
	conjugate();
	scale(1.0f / len2());

	return this;
}

Quaternion* Quaternion::set(float x, float y, float z, float w) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;

	return this;
}

Quaternion* Quaternion::set(Quaternion const& quaternion) {
	return this->set(quaternion.x, quaternion.y, quaternion.z, quaternion.w);
}

Quaternion* Quaternion::set(const Quaternion* quaternion) {
	return this->set(quaternion->x, quaternion->y, quaternion->z, quaternion->w);
}

Quaternion* Quaternion::set(const Vector3& axis, float angleD) {
	return setFromAxis(axis.x, axis.y, axis.z, angleD);
}

Quaternion* Quaternion::setFromMatrix (bool normalizeAxes, Matrix4* matrix) {
	return setFromAxes(normalizeAxes, matrix->val[Matrix4::M00], matrix->val[Matrix4::M01], matrix->val[Matrix4::M02],
	                   matrix->val[Matrix4::M10], matrix->val[Matrix4::M11], matrix->val[Matrix4::M12], matrix->val[Matrix4::M20],
	                   matrix->val[Matrix4::M21], matrix->val[Matrix4::M22]);
}

Quaternion* Quaternion::setFromMatrix (Matrix4* matrix) {
	return setFromMatrix(false, matrix);
}

Quaternion* Quaternion::setFromAxis(const float x, const float y, const float z, const float degrees) {
	return setFromAxisRad(x, y, z, degrees * Math::degreesToRadians);
}

Quaternion* Quaternion::setFromAxisRad(const float x, const float y, const float z, const float radians) {
	float d = Vector3::len(x, y, z);
	if (d == 0.0f) return idt();
	d = 1.0f / d;

	float l_ang = radians < 0 ? Math::PIx2_F - Math::fmod(-radians, Math::PIx2_F) : Math::fmod(radians, Math::PIx2_F);
	float l_sin = Math::sinRad(l_ang / 2);
	float l_cos = Math::cosRad(l_ang / 2);
	return this->set(d * x * l_sin, d * y * l_sin, d * z * l_sin, l_cos)->nor();
}

Quaternion* Quaternion::setFromAxes(float xx, float xy, float xz, float yx, float yy, float yz, float zx, float zy, float zz) {
	return setFromAxes(false, xx, xy, xz, yx, yy, yz, zx, zy, zz);
}

Quaternion* Quaternion::setFromAxes(bool normalizeAxes, float xx, float xy, float xz, float yx, float yy, float yz, float zx, float zy, float zz) {
	if (normalizeAxes) {
		const float lx = 1.0f / Vector3::len(xx, xy, xz);
		const float ly = 1.0f / Vector3::len(yx, yy, yz);
		const float lz = 1.0f / Vector3::len(zx, zy, zz);
		xx *= lx;
		xy *= lx;
		xz *= lx;
		yx *= ly;
		yy *= ly;
		yz *= ly;
		zx *= lz;
		zy *= lz;
		zz *= lz;
	}
	// the trace is the sum of the diagonal elements; see
	// http://mathworld.wolfram.com/MatrixTrace.html
	const float t = xx + yy + zz;

	// we protect the division by s by ensuring that s>=1
	if (t >= 0) { // |w| >= .5
		float s = (float)Math::sqrt(t + 1); // |s|>=1 ...
		w = 0.5f * s;
		s = 0.5f / s; // so this division isn't bad
		x = (zy - yz) * s;
		y = (xz - zx) * s;
		z = (yx - xy) * s;
	} else if ((xx > yy) && (xx > zz)) {
		float s = (float)Math::sqrt(1.0f + xx - yy - zz); // |s|>=1
		x = s * 0.5f; // |x| >= .5
		s = 0.5f / s;
		y = (yx + xy) * s;
		z = (xz + zx) * s;
		w = (zy - yz) * s;
	} else if (yy > zz) {
		float s = (float)Math::sqrt(1.0f + yy - xx - zz); // |s|>=1
		y = s * 0.5f; // |y| >= .5
		s = 0.5f / s;
		x = (yx + xy) * s;
		z = (zy + yz) * s;
		w = (xz - zx) * s;
	} else {
		float s = (float)Math::sqrt(1.0f + zz - xx - yy); // |s|>=1
		z = s * 0.5f; // |z| >= .5
		s = 0.5f / s;
		x = (xz + zx) * s;
		y = (zy + yz) * s;
		w = (yx - xy) * s;
	}

	return this;
}

void Quaternion::getEulerAnglesZYXDeg(float& yawZDeg, float& pitchYDeg, float& rollXDeg) {
	float yawZRad;
	float pitchYRad;
	float rollXRad;

	getEulerAnglesZYXRad(yawZRad, pitchYRad, rollXRad);

	yawZDeg = yawZRad * Math::radiansToDegrees;
	pitchYDeg = pitchYRad * Math::radiansToDegrees;
	rollXDeg = rollXRad * Math::radiansToDegrees;
}

void Quaternion::getEulerAnglesZYXRad(float& yawZRad, float& pitchYRad, float& rollXRad) {
	float squ;
	float sqx;
	float sqy;
	float sqz;
	float sarg;
	sqx = x * x;
	sqy = y * y;
	sqz = z * z;
	squ = w * w;
	sarg = -2.0f * (x * z - w * y);

	// If the pitch angle is PI/2 or -PI/2, we can only compute
	// the sum roll + yaw.  However, any combination that gives
	// the right sum will produce the correct orientation, so we
	// set rollX = 0 and compute yawZ.
	if (sarg <= -0.99999f) {
		pitchYRad = -0.5f * Math::PI_F;
		rollXRad = 0;
		yawZRad = 2.0f * Math::atan2(x, -y);
	} else if (sarg >= 0.99999f) {
		pitchYRad = 0.5f * Math::PI_F;
		rollXRad = 0;
		yawZRad = 2.0f * Math::atan2(-x, y);
	} else {
		pitchYRad = Math::asin(sarg);
		rollXRad = Math::atan2(2 * (y * z + w * x), squ - sqx - sqy + sqz);
		yawZRad = Math::atan2(2 * (x * y + w * z), squ + sqx - sqy - sqz);
	}
}

/**@brief Set the quaternion using Euler angles
   * @param yaw Angle around Y
   * @param pitch Angle around X
   * @param roll Angle around Z */
Quaternion* Quaternion::setEulerAnglesZYXDeg(float yawZDeg, float pitchYDeg, float rollXDeg) {
	return setEulerAnglesZYXRad(yawZDeg * Math::degreesToRadians, pitchYDeg * Math::degreesToRadians, rollXDeg * Math::degreesToRadians);
}

/**@brief Set the quaternion using Euler angles
   * @param yaw Angle around Y
   * @param pitch Angle around X
   * @param roll Angle around Z */
Quaternion* Quaternion::setEulerAnglesZYXRad(float yawZRad, float pitchYRad, float rollXRad) {
	float halfYaw = yawZRad * 0.5f;
	float halfPitch = pitchYRad * 0.5f;
	float halfRoll = rollXRad * 0.5f;
	float cosYaw = Math::cos(halfYaw);
	float sinYaw = Math::sin(halfYaw);
	float cosPitch = Math::cos(halfPitch);
	float sinPitch = Math::sin(halfPitch);
	float cosRoll = Math::cos(halfRoll);
	float sinRoll = Math::sin(halfRoll);
	
	x = sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw;
	y = cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw;
	z = cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw;
	w = cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw;

	return this;
}

Quaternion* Quaternion::scale(const float s) {
	x *= s;
	y *= s;
	z *= s;
	w *= s;
	
	return this;
}

/** Multiplies this quaternion with another one in the form of this = this * other
 * 
 * @param other Quaternion to multiply with
 * @return This quaternion for chaining */
Quaternion* Quaternion::mul(const Quaternion& other) {
    const float newX = this->w * other.x + this->x * other.w + this->y * other.z - this->z * other.y;
    const float newY = this->w * other.y + this->y * other.w + this->z * other.x - this->x * other.z;
    const float newZ = this->w * other.z + this->z * other.w + this->x * other.y - this->y * other.x;
    const float newW = this->w * other.w - this->x * other.x - this->y * other.y - this->z * other.z;
    this->x = newX;
    this->y = newY;
    this->z = newZ;
    this->w = newW;
    return this;
}

Quaternion* Quaternion::mulLeft(Quaternion& other) {
    const float newX = other.w * this->x + other.x * this->w + other.y * this->z - other.z * this->y;
    const float newY = other.w * this->y + other.y * this->w + other.z * this->x - other.x * this->z;
    const float newZ = other.w * this->z + other.z * this->w + other.x * this->y - other.y * this->x;
    const float newW = other.w * this->w - other.x * this->x - other.y * this->y - other.z * this->z;
    this->x = newX;
    this->y = newY;
    this->z = newZ;
    this->w = newW;
    return this;
}

Vector3* Quaternion::transform(Vector3* v) {
    tmp2.set(this);
    tmp2.conjugate();
    tmp1.set(v->x, v->y, v->z, 0);
    tmp2.mulLeft(tmp1)->mulLeft(*this);

    v->x = tmp2.x;
    v->y = tmp2.y;
    v->z = tmp2.z;
    return v;
}

float Quaternion::len2(const float x, const float y, const float z, const float w) {
    return x * x + y * y + z * z + w * w;
}

float Quaternion::len2() {
	return x * x + y * y + z * z + w * w;
}

Quaternion* Quaternion::conjugate() {
    x = -x;
    y = -y;
    z = -z;

    return this;
}

bool Quaternion::equals(Quaternion* obj) {
	if (obj == nullptr || this->x != obj->x || this->y != obj->y || this->z != obj->z || this->w != obj->w) {
		return false;
	}

	return true;
}

bool Quaternion::equals(Quaternion& obj) {
	return equals(&obj);
}

void Quaternion::toMatrix(float* matrix) {
	const float xx = x * x;
	const float xy = x * y;
	const float xz = x * z;
	const float xw = x * w;
	const float yy = y * y;
	const float yz = y * z;
	const float yw = y * w;
	const float zz = z * z;
	const float zw = z * w;
	// Set matrix from quaternion
	matrix[Matrix4::M00] = 1 - 2 * (yy + zz);
	matrix[Matrix4::M01] = 2 * (xy - zw);
	matrix[Matrix4::M02] = 2 * (xz + yw);
	matrix[Matrix4::M03] = 0;
	matrix[Matrix4::M10] = 2 * (xy + zw);
	matrix[Matrix4::M11] = 1 - 2 * (xx + zz);
	matrix[Matrix4::M12] = 2 * (yz - xw);
	matrix[Matrix4::M13] = 0;
	matrix[Matrix4::M20] = 2 * (xz - yw);
	matrix[Matrix4::M21] = 2 * (yz + xw);
	matrix[Matrix4::M22] = 1 - 2 * (xx + yy);
	matrix[Matrix4::M23] = 0;
	matrix[Matrix4::M30] = 0;
	matrix[Matrix4::M31] = 0;
	matrix[Matrix4::M32] = 0;
	matrix[Matrix4::M33] = 1;
}

/** Get the angle in radians of the rotation around the specified axis. The axis must be normalized.
 * @param axisX the x component of the normalized axis for which to get the angle
 * @param axisY the y component of the normalized axis for which to get the angle
 * @param axisZ the z component of the normalized axis for which to get the angle
 * @return the angle in radians of the rotation around the specified axis */
float Quaternion::getAngleAroundRad(const float axisX, const float axisY, const float axisZ) {
    const float d = Vector3::dot(this->x, this->y, this->z, axisX, axisY, axisZ);
    const float l2 = Quaternion::len2(axisX * d, axisY * d, axisZ * d, this->w);

    if(Math::isZero(l2)) {
        return 0.0f;
    }

    float f1 = d < 0 ? -this->w : this->w;
    float f2 = Math::sqrt(l2);
    float f3 = Math::clamp(
        (float)(f1 / f2), -1.0f, 1.0f
    );
    float f4 = (float)(2.0f * Math::acosRad(f3));
    return f4;
}

/** Get the angle in radians of the rotation around the specified axis. The axis must be normalized.
 * @param axis the normalized axis for which to get the angle
 * @return the angle in radians of the rotation around the specified axis */
float Quaternion::getAngleAroundRad(const Vector3& axis) {
    return getAngleAroundRad(axis.x, axis.y, axis.z);
}

/** Get the angle in degrees of the rotation around the specified axis. The axis must be normalized.
 * @param axisX the x component of the normalized axis for which to get the angle
 * @param axisY the y component of the normalized axis for which to get the angle
 * @param axisZ the z component of the normalized axis for which to get the angle
 * @return the angle in degrees of the rotation around the specified axis */
float Quaternion::getAngleAround(const float axisX, const float axisY, const float axisZ) {
    float v = getAngleAroundRad(axisX, axisY, axisZ) * Math::radiansToDegrees;

    return v;
}

/** Get the angle in degrees of the rotation around the specified axis. The axis must be normalized.
 * @param axis the normalized axis for which to get the angle
 * @return the angle in degrees of the rotation around the specified axis */
float Quaternion::getAngleAround(const Vector3& axis) {
    return getAngleAround(axis.x, axis.y, axis.z);
}

/** Spherical linear interpolation between this quaternion and the other quaternion, based on the alpha value in the range
	 * [0,1]. Taken from Bones framework for JPCT, see http://www.aptalkarga.com/bones/
	 * @param end the end quaternion
	 * @param alpha alpha in the range [0,1]
	 * @return this quaternion for chaining */
Quaternion* Quaternion::slerp(Quaternion& end, float alpha) {
	const float d = this->x * end.x + this->y * end.y + this->z * end.z + this->w * end.w;
	float absDot = d < 0.f ? -d : d;

	// Set the first and second scale for the interpolation
	float scale0 = 1.0f - alpha;
	float scale1 = alpha;

	// Check if the angle between the 2 quaternions was big enough to
	// warrant such calculations
	if ((1 - absDot) > 0.1) {// Get the angle between the 2 quaternions,
		// and then store the sin() of that angle
		const float angle = (float)Math::acosRad(absDot);
		const float invSinTheta = 1.0f / (float)Math::sinRad(angle);

		// Calculate the scale for q1 and q2, according to the angle and
		// it's sine value
		scale0 = ((float)Math::sinRad((1.0f - alpha) * angle) * invSinTheta);
		scale1 = ((float)Math::sinRad((alpha * angle)) * invSinTheta);
	}

	if (d < 0.f) scale1 = -scale1;

	// Calculate the x, y, z and w values for the quaternion by using a
	// special form of linear interpolation for quaternions.
	x = (scale0 * x) + (scale1 * end.x);
	y = (scale0 * y) + (scale1 * end.y);
	z = (scale0 * z) + (scale1 * end.z);
	w = (scale0 * w) + (scale1 * end.w);

	// Return the interpolated quaternion
	return this;
}

Quaternion::~Quaternion() {
	//void
}
