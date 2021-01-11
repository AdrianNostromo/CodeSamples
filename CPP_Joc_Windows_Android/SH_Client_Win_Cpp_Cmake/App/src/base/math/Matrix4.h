#pragma once

#include <base/gh.h>
#include <base/math/Affine2.h>

class Vector3;
class Quaternion;

// This is a type wrapper class and should not contain virtual functions (a vTable).
// This should have the same size as the wrapped type.
class Matrix4 final {
public:
	/** XX: Typically the unrotated X component for scaling, also the cosine of the angle when rotated on the Y and/or Z axis. On
	* Vector3 multiplication this value is multiplied with the source X component and added to the target X component. */
	static unsigned int M00;
	/** XY: Typically the negative sine of the angle when rotated on the Z axis. On Vector3 multiplication this value is multiplied
	* with the source Y component and added to the target X component. */
	static unsigned int M01;
	/** XZ: Typically the sine of the angle when rotated on the Y axis. On Vector3 multiplication this value is multiplied with the
	* source Z component and added to the target X component. */
	static unsigned int M02;
	/** XW: Typically the translation of the X component. On Vector3 multiplication this value is added to the target X component. */
	static unsigned int M03;
	/** YX: Typically the sine of the angle when rotated on the Z axis. On Vector3 multiplication this value is multiplied with the
	* source X component and added to the target Y component. */
	static unsigned int M10;
	/** YY: Typically the unrotated Y component for scaling, also the cosine of the angle when rotated on the X and/or Z axis. On
	* Vector3 multiplication this value is multiplied with the source Y component and added to the target Y component. */
	static unsigned int M11;
	/** YZ: Typically the negative sine of the angle when rotated on the X axis. On Vector3 multiplication this value is multiplied
	* with the source Z component and added to the target Y component. */
	static unsigned int M12;
	/** YW: Typically the translation of the Y component. On Vector3 multiplication this value is added to the target Y component. */
	static unsigned int M13;
	/** ZX: Typically the negative sine of the angle when rotated on the Y axis. On Vector3 multiplication this value is multiplied
	* with the source X component and added to the target Z component. */
	static unsigned int M20;
	/** ZY: Typical the sine of the angle when rotated on the X axis. On Vector3 multiplication this value is multiplied with the
	* source Y component and added to the target Z component. */
	static unsigned int M21;
	/** ZZ: Typically the unrotated Z component for scaling, also the cosine of the angle when rotated on the X and/or Y axis. On
	* Vector3 multiplication this value is multiplied with the source Z component and added to the target Z component. */
	static unsigned int M22;
	/** ZW: Typically the translation of the Z component. On Vector3 multiplication this value is added to the target Z component. */
	static unsigned int M23;
	/** WX: Typically the value zero. On Vector3 multiplication this value is ignored. */
	static unsigned int M30;
	/** WY: Typically the value zero. On Vector3 multiplication this value is ignored. */
	static unsigned int M31;
	/** WZ: Typically the value zero. On Vector3 multiplication this value is ignored. */
	static unsigned int M32;
	/** WW: Typically the value one. On Vector3 multiplication this value is ignored. */
	static unsigned int M33;

	float val[16];

private:
	static Vector3 tmpVec;
	static Matrix4 tmpMat;

	static Vector3 l_vez;
	static Vector3 l_vex;
	static Vector3 l_vey;

	static float tmp[16];

public:
	// Manually implemented below.
	// Matrix4(Matrix4 const&) = delete;
	Matrix4(Matrix4 &&) = default;
	Matrix4& operator=(Matrix4 const&) = delete;
	Matrix4& operator=(Matrix4 &&) = default;

	explicit Matrix4();
	explicit Matrix4(const Matrix4* src);
	explicit Matrix4(const Matrix4& src);

	void idt();
    Matrix4* setToOrtho2D(float x, float y, float width, float height);
    Matrix4* setToOrtho(float left, float right, float bottom, float top, float near, float far);

    /** Sets the matrix to a projection matrix with a near- and far plane, a field of view in degrees and an aspect ratio. Note that
     * the field of view specified is the angle in degrees for the height, the field of view for the width will be calculated
     * according to the aspect ratio.
     *
     * @param near The near plane
     * @param far The far plane
     * @param fovy The field of view of the height in degrees
     * @param aspectRatio The "width over height" aspect ratio
     * @return This matrix for the purpose of chaining methods together. */
    Matrix4* setToProjection(float near, float far, float fovy, float aspectRatio);
    /** Sets the matrix to a projection matrix with a near/far plane, and left, bottom, right and top specifying the points on the
	 * near plane that are mapped to the lower left and upper right corners of the viewport. This allows to create projection
	 * matrix with off-center vanishing point.
	 *
	 * @param left
	 * @param right
	 * @param bottom
	 * @param top
	 * @param near The near plane
	 * @param far The far plane
	 * @return This matrix for the purpose of chaining methods together. */
    Matrix4* setToProjection(float left, float right, float bottom, float top, float near, float far);

	void set(Affine2& affine);
	Matrix4* set(const Matrix4* matrix);
	Matrix4* set(const Matrix4& matrix);
	Matrix4* set(const float* values);

	/** Postmultiplies this matrix by a translation matrix. Postmultiplication is also used by OpenGL ES' 1.x
	* glTranslate/glRotate/glScale.
	* @param x Translation in the x-axis.
	* @param y Translation in the y-axis.
	* @param z Translation in the z-axis.
	* @return This matrix for the purpose of chaining methods together. */
	Matrix4* translate(float x, float y, float z);

    Matrix4* translate(Vector3& translation);

	/** Postmultiplies this matrix with a scale matrix. Postmultiplication is also used by OpenGL ES' 1.x
	* glTranslate/glRotate/glScale.
	* @param scaleX The scale in the x-axis.
	* @param scaleY The scale in the y-axis.
	* @param scaleZ The scale in the z-axis.
	* @return This matrix for the purpose of chaining methods together. */
	Matrix4* scale(float scaleX, float scaleY, float scaleZ);

	/** Postmultiplies this matrix with a (counter-clockwise) rotation matrix. Postmultiplication is also used by OpenGL ES' 1.x
	* glTranslate/glRotate/glScale.
	*
	* @param rotation
	* @return This matrix for the purpose of chaining methods together. */
	Matrix4* rotate(Quaternion& rotation);

	Matrix4* setToLookAt(const Vector3& direction, const Vector3& up);
	Matrix4* setToLookAt(const Vector3& position, const Vector3& target, const Vector3& up);
	
	Matrix4* setToTranslation(const Vector3& vector);
	Matrix4* setToTranslation(float x, float y, float z);

	Matrix4* set(Vector3 & position, Quaternion & orientation, Vector3 & scale);
	Matrix4* set(
		float translationX, float translationY, float translationZ, 
		float quaternionX, float quaternionY, float quaternionZ, float quaternionW,
		float scaleX, float scaleY, float scaleZ);

	Matrix4* mul(Matrix4& matrix);
	Matrix4* mul(Matrix4* matrix);

	pub bool _inv();
	pub Matrix4* invUnchecked();
	pub Matrix4* transpose();

    Vector3* getTranslation (Vector3& position);

	pub Quaternion* getRotation(Quaternion& rotation, bool normalizeAxes);
	pub Quaternion* getRotation(Quaternion& rotation);

	pub float getScaleXSquared();
	pub float getScaleYSquared();
	pub float getScaleZSquared();

	pub float getScaleX();
	pub float getScaleY();
	pub float getScaleZ();
	
	pub Vector3* getScale(Vector3& scale);

	pub bool equals(Matrix4& o);

	static void Mul(float* mata, float* matb);
    static bool Inv(float* values);

    static void prj(float* mat, float* vecs, int offset, int numVecs, int stride);

    // Note. The destructor is not virtual because it creates a vTable that is not needed because this is a type wrapper class.
	~Matrix4();
private:
    static inline float matrix4_det(float* val);
    static inline bool matrix4_inv(float* val);
    static inline void matrix4_proj(float* mat, float* vec);
};
