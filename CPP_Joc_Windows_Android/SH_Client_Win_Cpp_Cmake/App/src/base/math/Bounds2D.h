#pragma once

#include <base/gh.h>
#include <base/math/Vector2.h>

class Bounds2D {
    pub typedef float type;

    pub Vector2 min;
    pub Vector2 max;

    // Note. The copy constructor is initialised manually below.
    // Bounds2D(Bounds2D const& src) = delete;
    pub Bounds2D(Bounds2D&&) = default;
    pub Bounds2D& operator=(Bounds2D const&) = delete;
    pub Bounds2D& operator=(Bounds2D&&) = default;

    pub explicit Bounds2D();
    pub explicit Bounds2D(type xMin, type yMin, type xMax, type yMax);
    pub explicit Bounds2D(Bounds2D const& src);

    pub void clear();

    pub type getWidth();
    pub type getHeight();

    pub bool contains(type x, type y);
    pub bool containsEdgeInclusive(type x, type y);
    
    pub void set(type xMin, type yMin, type xMax, type yMax);
    pub void set(Bounds2D& src);

    pub Bounds2D* unionPoint(
        const type x,
        const type y);
	
    pub Bounds2D* unionBounds(
        const type xMin, const type xMax,
        const type yMin, const type yMax);

    pub bool equals(Bounds2D& other);
    pub bool equals(Bounds2D* other);

    pub ~Bounds2D();
};
