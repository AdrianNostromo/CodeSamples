#pragma once

#include <base/gh.h>
#include "Vector1Int.h"

class Bounds1DInt {
    pub typedef int type;

    pub Vector1Int min;
    pub Vector1Int max;

    // Note. The copy constructor is initialised manually below.
    //pub Bounds1DInt(Bounds1DInt const&) = delete;
    pub Bounds1DInt(Bounds1DInt &&) = default;
    pub Bounds1DInt& operator=(Bounds1DInt const&) = delete;
    pub Bounds1DInt& operator=(Bounds1DInt &&) = default;

    pub explicit Bounds1DInt();
    pub explicit Bounds1DInt(Bounds1DInt const& src);
    pub explicit Bounds1DInt(Vector1Int min, Vector1Int max);
    pub explicit Bounds1DInt(type xMin, type xMax);

    pub void clear();

    pub type getLenX();
    pub void setLenX(type lenX);

    pub type getArea();

    // Array mode behaves the same as the bounds check in an array.
    pub bool contains(type x);
    pub bool contains(Vector1Int& p);
    pub bool contains(Vector1Int* p);

    pub Bounds1DInt* set(Bounds1DInt const& src);
    pub Bounds1DInt* set(Vector1Int const& min, Vector1Int const& max);
    pub Bounds1DInt* set(type xMin, type xMax);

    pub Bounds1DInt* intersect(Bounds1DInt& o);
    pub Bounds1DInt* intersect(const type xMin, const type xMax);
    
    pub Bounds1DInt* unionOther(Bounds1DInt& o);
    pub Bounds1DInt* unionOther(const type xMin, const type xMax);

    pub bool equals(Bounds1DInt& o);
    pub bool equals(Bounds1DInt* o);

    pub ~Bounds1DInt();
};
