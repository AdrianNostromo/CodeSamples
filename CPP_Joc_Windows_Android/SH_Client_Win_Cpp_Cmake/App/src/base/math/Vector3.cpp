#include "Vector3.h"
#include "Matrix4.h"
#include <base/math/Math.h>
#include <iomanip>
#include <base/util/StringUtil.h>
#include <base/exceptions/LogicException.h>

Vector3 Vector3::X(1.0f, 0.0f, 0.0f);
Vector3 Vector3::Y(0.0f, 1.0f, 0.0f);
Vector3 Vector3::Z(0.0f, 0.0f, 1.0f);
Vector3 Vector3::Zero(0.0f, 0.0f, 0.0f);

Vector3::Vector3() {
    set(0.0f, 0.0f, 0.0f);
}

Vector3::Vector3(Vector3 const& src) {
	set(src.x, src.y, src.z);
}

Vector3::Vector3(Vector3 const* src) {
	set(src->x, src->y, src->z);
}

Vector3::Vector3(type x, type y, type z) {
	set(x, y, z);
}

Vector3::Vector3(type x, type y, type z, bool normalize) {
	set(x, y, z);

	if (normalize) {
		nor();
	}
}

Vector3::Vector3(type xyz) {
	set(xyz, xyz, xyz);
}

Vector3::Vector3(std::string& val) {
	int cPartIndex = 0;
	std::shared_ptr<ArrayList<std::string>> partsList = StringUtil::split(val, ":", true);
	
	if (partsList->count() == 4) {
		// Check type value;
		std::string& typeName = partsList->getReference(cPartIndex++);
		if (typeName != "Vector3") {
			throw LogicException(LOC);
		}
	} else if (partsList->count() == 3) {
		//void
	}else {
		throw LogicException(LOC);
	}

	float valX = StringUtil::parseFloat(partsList->getReference(cPartIndex++));
	float valY = StringUtil::parseFloat(partsList->getReference(cPartIndex++));
	float valZ = StringUtil::parseFloat(partsList->getReference(cPartIndex++));

	this->x = valX;
	this->y = valY;
	this->z = valZ;
}

Vector3* Vector3::set(type x, type y, type z) {
	this->x = x;
	this->y = y;
	this->z = z;
	return this;
}

Vector3* Vector3::set(const Vector3& vector) {
	return this->set(vector.x, vector.y, vector.z);
}

Vector3* Vector3::set(const Vector3* vector) {
	return this->set(vector->x, vector->y, vector->z);
}

Vector3* Vector3::add(const Vector3& vector) {
	return add(vector.x, vector.y, vector.z);
}

Vector3* Vector3::add(const Vector3* vector) {
	return add(vector->x, vector->y, vector->z);
}

Vector3* Vector3::add(type x, type y, type z) {
	return set(this->x + x, this->y + y, this->z + z);
}

Vector3* Vector3::sub(const Vector3* a_vec) {
	return sub(a_vec->x, a_vec->y, a_vec->z);
}

Vector3* Vector3::sub(const Vector3& a_vec) {
	return sub(a_vec.x, a_vec.y, a_vec.z);
}

Vector3* Vector3::sub(type x, type y, type z) {
	return set(this->x - x, this->y - y, this->z - z);
}

Vector3* Vector3::sub(type value) {
	return set(this->x - value, this->y - value, this->z - value);
}

Vector3::type Vector3::len() {
	return Vector3::len(x, y, z);
}

Vector3::type Vector3::len2() {
	return x * x + y * y + z * z;
}

Vector3* Vector3::scl(type scalar) {
	return this->set(this->x * scalar, this->y * scalar, this->z * scalar);
}

Vector3* Vector3::scl(const Vector3& other) {
	return this->set(x * other.x, y * other.y, z * other.z);
}

Vector3* Vector3::scl(type vx, type vy, type vz) {
	return this->set(this->x * vx, this->y * vy, this->z * vz);
}

Vector3* Vector3::crs(const Vector3& vector) {
	return set(y * vector.z - z * vector.y, z * vector.x - x * vector.z, x * vector.y - y * vector.x);
}

Vector3* Vector3::crs(type x, type y, type z) {
	return set(this->y * z - this->z * y, this->z * x - this->x * z, this->x * y - this->y * x);
}

Vector3* Vector3::nor() {
	const type len2 = this->len2();
	if (len2 == 0.0f || len2 == 1.0f) {
		return this;
	}

	return scl(1.0f / (type) Math::sqrt(len2));
}

Vector3* Vector3::clipLength(type maxLength) {
	type _len = len();
	if (_len > maxLength) {
		type mult = maxLength / _len;

		x *= mult;
		y *= mult;
		z *= mult;
	}

	return this;
}

Vector3::type Vector3::distTo(const Vector3& vector) {
	type dist = (type)Math::sqrt(
		(vector.x - x) * (vector.x - x) + (vector.y - y) * (vector.y - y) + (vector.z - z) * (vector.z - z) 
	);

	return dist;
}

/** @return The dot product between the two vectors */
Vector3::type Vector3::dot(type x1, type y1, type z1, type x2, type y2, type z2) {
    return x1 * x2 + y1 * y2 + z1 * z2;
}

Vector3::type Vector3::dot(Vector3& vector) {
    return x * vector.x + y * vector.y + z * vector.z;
}

Vector3* Vector3::prj(Matrix4& matrix) {
    type* l_mat = matrix.val;
    type l_w = 1.0f / (x * l_mat[Matrix4::M30] + y * l_mat[Matrix4::M31] + z * l_mat[Matrix4::M32] + l_mat[Matrix4::M33]);
    return this->set(
            (x * l_mat[Matrix4::M00] + y * l_mat[Matrix4::M01] + z * l_mat[Matrix4::M02] + l_mat[Matrix4::M03]) * l_w,
            (x * l_mat[Matrix4::M10] + y * l_mat[Matrix4::M11] + z * l_mat[Matrix4::M12] + l_mat[Matrix4::M13]) * l_w,
            (x * l_mat[Matrix4::M20] + y * l_mat[Matrix4::M21] + z * l_mat[Matrix4::M22] + l_mat[Matrix4::M23]) * l_w
    );
}

Vector3* Vector3::mul(Quaternion& quat) {
    return quat.transform(this);
}

Vector3* Vector3::mul(type val) {
    this->x *= val;
    this->y *= val;
    this->z *= val;

    return this;
}

Vector3* Vector3::mul(type x, type y, type z) {
    this->x *= x;
    this->y *= y;
    this->z *= z;

    return this;
}

Vector3* Vector3::lerp(Vector3& target, type alpha) {
	x += alpha * (target.x - x);
	y += alpha * (target.y - y);
	z += alpha * (target.z - z);
	return this;
}

bool Vector3::equals(const Vector3& o) {
    if(x != o.x
       || y != o.y
       || z != o.z)
    {
        return false;
    }

    return true;
}

bool Vector3::equals(const Vector3* o) {
	return equals(*o);
}

bool Vector3::equals(type x, type y, type z, type roundingError) {
	if (!Math::isEqual(this->x, x, roundingError)
		|| !Math::isEqual(this->y, y, roundingError)
		|| !Math::isEqual(this->z, z, roundingError))
	{
		return false;
	}

	return true;
}

bool Vector3::equals(type x, type y, type z) {
    if(this->x != x
       || this->y != y
       || this->z != z)
    {
        return false;
    }

    return true;
}

/** @return The euclidean length */
Vector3::type Vector3::len(const type x, const type y, const type z) {
	return Math::sqrt(x * x + y * y + z * z);
}

/** @return the squared distance between the given points */
Vector3::type Vector3::dst2(const type x1, const type y1, const type z1, const type x2, const type y2, const type z2) {
	const type a = x2 - x1;
	const type b = y2 - y1;
	const type c = z2 - z1;

	return a * a + b * b + c * c;
}

Vector3::type Vector3::dst2(Vector3& point) {
	const type a = point.x - x;
	const type b = point.y - y;
	const type c = point.z - z;

	return a * a + b * b + c * c;
}

/** Returns the squared distance between this point and the given point
 * @param x The x-component of the other point
 * @param y The y-component of the other point
 * @param z The z-component of the other point
 * @return The squared distance */
Vector3::type Vector3::dst2(type x, type y, type z) {
	const type a = x - this->x;
	const type b = y - this->y;
	const type c = z - this->z;

	return a * a + b * b + c * c;
}

void Vector3::toString(std::stringstream& ss, bool includeTypePrefix) {
	if (includeTypePrefix) {
		ss << "Vector3:";
	}

	// Set the max precission for floats.
	ss << std::setprecision(7) << x << ":" << y << ":" << z;
}
