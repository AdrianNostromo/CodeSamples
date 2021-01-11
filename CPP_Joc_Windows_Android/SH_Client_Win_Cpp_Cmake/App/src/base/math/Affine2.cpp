#include "Affine2.h"
#include <base/exceptions/LogicException.h>
#include <base/math/Math.h>

Affine2::Affine2() {
  //void
}

Vector2* Affine2::getTranslation (Vector2& position) {
  position.x = m02;
  position.y = m12;

  return &position;
}

Affine2* Affine2::idt() {
  m00 = 1;
  m01 = 0;
  m02 = 0;
  m10 = 0;
  m11 = 1;
  m12 = 0;
  
  return this;
}

Affine2* Affine2::set(Affine2& other) {
  m00 = other.m00;
  m01 = other.m01;
  m02 = other.m02;
  m10 = other.m10;
  m11 = other.m11;
  m12 = other.m12;

  return this;
}

/** Inverts this matrix given that the determinant is != 0.
 * @return This matrix for the purpose of chaining operations.
 * @throws GdxRuntimeException if the matrix is singular (not invertible) */
Affine2* Affine2::inv() {
    float det = this->det();
    if (det == 0) {
        // "Can't invert a singular affine matrix"
        throw LogicException(LOC);
    }

    float invDet = 1.0f / det;

    float tmp00 = m11;
    float tmp01 = -m01;
    float tmp02 = m01 * m12 - m11 * m02;
    float tmp10 = -m10;
    float tmp11 = m00;
    float tmp12 = m10 * m02 - m00 * m12;

    m00 = invDet * tmp00;
    m01 = invDet * tmp01;
    m02 = invDet * tmp02;
    m10 = invDet * tmp10;
    m11 = invDet * tmp11;
    m12 = invDet * tmp12;
    return this;
}

Affine2* Affine2::mul(Affine2& other) {
  float tmp00 = m00 * other.m00 + m01 * other.m10;
  float tmp01 = m00 * other.m01 + m01 * other.m11;
  float tmp02 = m00 * other.m02 + m01 * other.m12 + m02;
  float tmp10 = m10 * other.m00 + m11 * other.m10;
  float tmp11 = m10 * other.m01 + m11 * other.m11;
  float tmp12 = m10 * other.m02 + m11 * other.m12 + m12;

  m00 = tmp00;
  m01 = tmp01;
  m02 = tmp02;
  m10 = tmp10;
  m11 = tmp11;
  m12 = tmp12;
  
  return this;
}

/** Calculates the determinant of the matrix.
 * @return The determinant of this matrix. */
float Affine2::det() {
    return m00 * m11 - m01 * m10;
}

Affine2* Affine2::translate(float x, float y) {
  m02 += m00 * x + m01 * y;
  m12 += m10 * x + m11 * y;

  return this;
}

Affine2* Affine2::scale(float scaleX, float scaleY) {
  m00 *= scaleX;
  m01 *= scaleY;
  m10 *= scaleX;
  m11 *= scaleY;

  return this;
}

Affine2* Affine2::rotate(float degrees) {
  if (degrees == 0) return this;

  float cos = Math::cosDegToRad(degrees);
  float sin = Math::sinDegToRad(degrees);

  float tmp00 = m00 * cos + m01 * sin;
  float tmp01 = m00 * -sin + m01 * cos;
  float tmp10 = m10 * cos + m11 * sin;
  float tmp11 = m10 * -sin + m11 * cos;

  m00 = tmp00;
  m01 = tmp01;
  m10 = tmp10;
  m11 = tmp11;

  return this;
}

void Affine2::applyTo(Vector2& p) {
    float x = p.x;
    float y = p.y;
    p.x = m00 * x + m01 * y + m02;
    p.y = m10 * x + m11 * y + m12;
}
