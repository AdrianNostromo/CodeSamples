#pragma once

#include <base/gh.h>
#include "Vector3.h"

class Bounds3D {
    pub typedef float type;

public:
    Vector3 min;
    Vector3 max;

public:
    // Note. The copy constructor is initialised manually below.
    //Bounds3D(Bounds3D const&) = delete;
    Bounds3D(Bounds3D &&) = default;
    Bounds3D& operator=(Bounds3D const&) = delete;
    Bounds3D& operator=(Bounds3D &&) = default;

    explicit Bounds3D();
    explicit Bounds3D(Bounds3D const& src);
    explicit Bounds3D(Vector3 min, Vector3 max);

    pub void clear();

    type getLenX();
    type getLenY();
    type getLenZ();

    bool containsEdgeInclusive(type x, type y, type z);
    bool containsEdgeInclusive(Vector3& p);
    bool containsEdgeInclusive(Vector3* p);

    void set(Bounds3D const& src);
    void set(Vector3 const& min, Vector3 const& max);

    Bounds3D* unionPoint(
        const type x,
        const type y,
        const type z
    );

    ~Bounds3D();
};
