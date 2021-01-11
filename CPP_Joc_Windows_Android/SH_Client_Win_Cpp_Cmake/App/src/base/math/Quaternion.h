#pragma once

class Matrix4;
class Vector3;

class Quaternion {
public:
	float x;
	float y;
	float z;
	float w;

private:
	static Quaternion tmp1;
	static Quaternion tmp2;

public:
    explicit Quaternion(Quaternion const&);
	Quaternion(Quaternion &&) = default;
	Quaternion& operator=(Quaternion const&) = delete;
	Quaternion& operator=(Quaternion &&) = default;
	explicit Quaternion();
	/** Constructor, sets the four components of the quaternion.
	* @param x The x-component
	* @param y The y-component
	* @param z The z-component
	* @param w The w-component */
	explicit Quaternion(float x, float y, float z, float w);
	explicit Quaternion(Vector3& axis, float angle);

	/** Sets the quaternion to an identity Quaternion
	* @return this quaternion for chaining */
	Quaternion* idt();

	/** Normalizes this quaternion to unit length
	* @return the quaternion for chaining */
	Quaternion* nor();

	Quaternion* inverse();

	Quaternion* set(float x, float y, float z, float w);
	Quaternion* set(Quaternion const& quaternion);
	Quaternion* set(const Quaternion* quaternion);
	/** Sets the quaternion components from the given axis and angle around that axis.
	*
	* @param axis The axis
	* @param angle The angle in degrees
	* @return This quaternion for chaining. */
	Quaternion* set(const Vector3& axis, float angleD);

	/** Sets the Quaternion from the given matrix, optionally removing any scaling. */
	Quaternion* setFromMatrix (bool normalizeAxes, Matrix4* matrix);

	/** Sets the Quaternion from the given rotation matrix, which must not contain scaling. */
	Quaternion* setFromMatrix (Matrix4* matrix);

	/** Sets the quaternion components from the given axis and angle around that axis.
	* @param x X direction of the axis
	* @param y Y direction of the axis
	* @param z Z direction of the axis
	* @param degrees The angle in degrees
	* @return This quaternion for chaining. */
	Quaternion* setFromAxis(const float x, const float y, const float z, const float degrees);
	/** Sets the quaternion components from the given axis and angle around that axis.
	* @param x X direction of the axis
	* @param y Y direction of the axis
	* @param z Z direction of the axis
	* @param radians The angle in radians
	* @return This quaternion for chaining. */
	Quaternion* setFromAxisRad(const float x, const float y, const float z, const float radians);

	/** <p>
	 * Sets the Quaternion from the given x-, y- and z-axis which have to be orthonormal.
	 * </p>
	 *
	 * <p>
	 * Taken from Bones framework for JPCT, see http://www.aptalkarga.com/bones/ which in turn took it from Graphics Gem code at
	 * ftp://ftp.cis.upenn.edu/pub/graphics/shoemake/quatut.ps.Z.
	 * </p>
	 *
	 * @param xx x-axis x-coordinate
	 * @param xy x-axis y-coordinate
	 * @param xz x-axis z-coordinate
	 * @param yx y-axis x-coordinate
	 * @param yy y-axis y-coordinate
	 * @param yz y-axis z-coordinate
	 * @param zx z-axis x-coordinate
	 * @param zy z-axis y-coordinate
	 * @param zz z-axis z-coordinate */
	Quaternion* setFromAxes(float xx, float xy, float xz, float yx, float yy, float yz, float zx, float zy, float zz);

	/** <p>
	 * Sets the Quaternion from the given x-, y- and z-axis.
	 * </p>
	 *
	 * <p>
	 * Taken from Bones framework for JPCT, see http://www.aptalkarga.com/bones/ which in turn took it from Graphics Gem code at
	 * ftp://ftp.cis.upenn.edu/pub/graphics/shoemake/quatut.ps.Z.
	 * </p>
	 *
	 * @param normalizeAxes whether to normalize the axes (necessary when they contain scaling)
	 * @param xx x-axis x-coordinate
	 * @param xy x-axis y-coordinate
	 * @param xz x-axis z-coordinate
	 * @param yx y-axis x-coordinate
	 * @param yy y-axis y-coordinate
	 * @param yz y-axis z-coordinate
	 * @param zx z-axis x-coordinate
	 * @param zy z-axis y-coordinate
	 * @param zz z-axis z-coordinate */
	Quaternion* setFromAxes(bool normalizeAxes, float xx, float xy, float xz, float yx, float yy, float yz, float zx, float zy, float zz);

	// This is for Z up;
	void getEulerAnglesZYXDeg(float& yawZDeg, float& pitchYDeg, float& rollXDeg);
	// This is for Z up;
	void getEulerAnglesZYXRad(float& yawZRad, float& pitchYRad, float& rollXRad);

	// This is for Z up;
	/**@brief Set the quaternion using euler angles
   * @param yaw Angle around Z
   * @param pitch Angle around Y
   * @param roll Angle around X */
	Quaternion* setEulerAnglesZYXDeg(float yawZDeg, float pitchYDeg, float rollXDeg);
	// This is for Z up;
	/**@brief Set the quaternion using euler angles
   * @param yaw Angle around Z
   * @param pitch Angle around Y
   * @param roll Angle around X */
	Quaternion* setEulerAnglesZYXRad(float yawZRad, float pitchYRad, float rollXRad);

	Quaternion* scale(const float s);

    Quaternion* mul(const Quaternion& other);

    /** Multiplies this quaternion with another one in the form of this = other * this
	 *
	 * @param other Quaternion to multiply with
	 * @return This quaternion for chaining */
    Quaternion* mulLeft(Quaternion& other);

    Vector3* transform(Vector3* v);

	static float len2(const float x, const float y, const float z, const float w);
	/** @return the length of this quaternion without square root */
	float len2();

    /** Conjugate the quaternion.
     *
     * @return This quaternion for chaining */
    Quaternion* conjugate();

	bool equals(Quaternion* obj);
	bool equals(Quaternion& obj);
	/** Fills a 4x4 matrix with the rotation matrix represented by this quaternion.
	*
	* @param matrix Matrix to fill */
	void toMatrix(float* matrix);

    float getAngleAroundRad(const float axisX, const float axisY, const float axisZ);
    float getAngleAroundRad(const Vector3& axis);
    float getAngleAround(const float axisX, const float axisY, const float axisZ);
    float getAngleAround(const Vector3& axis);

	Quaternion* slerp(Quaternion& end, float alpha);

	~Quaternion();
};
