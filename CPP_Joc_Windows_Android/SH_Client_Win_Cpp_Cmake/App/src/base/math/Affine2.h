#ifndef Affine2_H
#define Affine2_H

#include <base/math/Vector2.h>

class Affine2 {
private:
public:
    // -               -
    // | m00, m01, m02 |
    // | m10, m11, m12 |
    // | m20, m21, m22 |
    // -               -

    float m00 = 1;
    float m01 = 0;
    float m02 = 0;

    float m10 = 0;
    float m11 = 1;
    float m12 = 0;

    Affine2();

    Vector2* getTranslation(Vector2& position);

    Affine2* idt();
    Affine2* set (Affine2& other);
    Affine2* inv();
    Affine2* mul (Affine2& other);
    float det();

    Affine2* translate (float x, float y);
    Affine2* scale (float scaleX, float scaleY);
    Affine2* rotate (float degrees);

    void applyTo(Vector2& p);

};

#endif
