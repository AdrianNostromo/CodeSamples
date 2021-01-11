#pragma once

#include <base/gh.h>
#include "Vector2Int.h"

class Bounds2DInt {
    pub typedef int type;

    pub Vector2Int min;
    pub Vector2Int max;

    // Note. The copy constructor is initialised manually below.
    //pub Bounds2DInt(Bounds2DInt const&) = delete;
    pub Bounds2DInt(Bounds2DInt &&) = default;
    pub Bounds2DInt& operator=(Bounds2DInt const&) = delete;
    pub Bounds2DInt& operator=(Bounds2DInt &&) = default;

    pub explicit Bounds2DInt();
    pub explicit Bounds2DInt(Bounds2DInt const& src);
    pub explicit Bounds2DInt(Vector2Int min, Vector2Int max);
    pub explicit Bounds2DInt(
		type xMin, type xMax,
		type yMin, type yMax);

    pub void clear();

    pub type getLenX();
    pub void setLenX(type lenX);

    pub type getLenY();
    pub void setLenY(type lenY);

    pub type getArea();

    // Array mode behaves the same as the bounds check in an array.
    pub bool contains(type x, type y);
    pub bool contains(Vector2Int& p);
    pub bool contains(Vector2Int* p);

    pub Bounds2DInt* set(Bounds2DInt const& src);
    pub Bounds2DInt* set(Vector2Int const& min, Vector2Int const& max);
    pub Bounds2DInt* set(
		type xMin, type xMax,
		type yMin, type yMax);

    pub Bounds2DInt* unionA(
		const type xMin, const type xMax,
		const type yMin, const type yMax);

    pub Bounds2DInt* intersect(Bounds2DInt& o);
    pub Bounds2DInt* intersect(
        const type xMin, const type xMax,
        const type yMin, const type yMax);
    
    pub Bounds2DInt* unionOther(Bounds2DInt& o);
    pub Bounds2DInt* unionOther(
        const type xMin, const type xMax,
        const type yMin, const type yMax);

    pub bool equals(Bounds2DInt& o);
    pub bool equals(Bounds2DInt* o);

    pub ~Bounds2DInt();
};
