#pragma once

#include <base/gh.h>
#include <string>
#include "Quaternion.h"
#include <sstream>

class Matrix4;
class Quaternion;

class Vector3 {
	pub typedef float type;

	pub static Vector3 X;
	pub static Vector3 Y;
	pub static Vector3 Z;
	pub static Vector3 Zero;

	pub type x = 0.0f;
	pub type y = 0.0f;
	pub type z = 0.0f;

	// Note. The copy constructor is initialised manually below.
	//Vector3(Vector3 const& src) = delete;
	pub Vector3(Vector3 &&) = default;
	pub Vector3& operator=(Vector3 const&) = delete;
	pub Vector3& operator=(Vector3 &&) = default;

	pub explicit Vector3();
	// This the same as the copy constructor.
	pub explicit Vector3(Vector3 const& src);
	pub explicit Vector3(Vector3 const* src);
	pub explicit Vector3(type x, type y, type z);
	pub explicit Vector3(type x, type y, type z, bool normalize);
	pub explicit Vector3(type xyz);
	pub explicit Vector3(std::string& val);

	pub Vector3* set(type x, type y, type z);
	pub Vector3* set(const Vector3& vector);
	pub Vector3* set(const Vector3* vector);

	pub Vector3* add(const Vector3& vector);
	pub Vector3* add(const Vector3* vector);
	pub Vector3* add(type x, type y, type z);

	pub Vector3* sub(const Vector3* a_vec);
	pub Vector3* sub(const Vector3& a_vec);
	pub Vector3* sub(type x, type y, type z);
	pub Vector3* sub(type value);
	
	pub type len();
	pub type len2();

	pub Vector3* scl(type scalar);
	pub Vector3* scl(const Vector3& other);
	pub Vector3* scl(type vx, type vy, type vz);
	
	pub Vector3* crs(const Vector3& vector);
	pub Vector3* crs(type x, type y, type z);

	pub Vector3* nor();
	pub Vector3* clipLength(type maxLength);

	pub type distTo(const Vector3& vector);

	pub static type dot(type x1, type y1, type z1, type x2, type y2, type z2);
	pub type dot(Vector3& vector);

/** Multiplies this vector by the given matrix dividing by w, assuming the fourth (w) component of the vector is 1. This is
 * mostly used to project/unproject vectors via a perspective projection matrix.
 *
 * @param matrix The matrix.
 * @return This vector for chaining */
	pub Vector3* prj(Matrix4& matrix);

    /** Apply rotation to this vector.
     *
     * @param matrix The matrix.
     * @return This vector for chaining */
	pub Vector3* mul(Quaternion& quat);
	pub Vector3* mul(type val);
	pub Vector3* mul(type x, type y, type z);

	pub Vector3* lerp(Vector3& target, type alpha);

	pub bool equals(const Vector3& o);
	pub bool equals(const Vector3* o);
	pub bool equals(type x, type y, type z);
	pub bool equals(type x, type y, type z, type roundingError);

	/** @return The euclidean length */
	pub static type len(const type x, const type y, const type z);

	pub static type dst2(const type x1, const type y1, const type z1, const type x2, const type y2, const type z2);
	pub type dst2(Vector3& point);
	pub type dst2(type x, type y, type z);

	pub void toString(std::stringstream& ss, bool includeTypePrefix);

};
