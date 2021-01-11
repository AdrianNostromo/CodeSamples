#pragma once

#include <base/gh.h>
#include "Vector3Int.h"

class Bounds3DInt {
    pub typedef int type;

public:
    Vector3Int min;
    Vector3Int max;

public:
    // Note. The copy constructor is initialised manually below.
    //Bounds3DInt(Bounds3DInt const&) = delete;
    Bounds3DInt(Bounds3DInt &&) = default;
    Bounds3DInt& operator=(Bounds3DInt const&) = delete;
    Bounds3DInt& operator=(Bounds3DInt &&) = default;

    explicit Bounds3DInt();
    explicit Bounds3DInt(Bounds3DInt const& src);
    explicit Bounds3DInt(Vector3Int min, Vector3Int max);
    explicit Bounds3DInt(
		type xMin, type xMax,
		type yMin, type yMax,
		type zMin, type zMax);

    pub void clear();

    type getLenX();
    type getLenY();
    type getLenZ();

    bool containsEdgeInclusive(type x, type y, type z);
    bool containsEdgeInclusive(Vector3Int& p);
    bool containsEdgeInclusive(Vector3Int* p);

	Bounds3DInt* set(Bounds3DInt const& src);
	Bounds3DInt* set(Vector3Int const& min, Vector3Int const& max);
	Bounds3DInt* set(
		type xMin, type xMax,
		type yMin, type yMax,
		type zMin, type zMax);

    Bounds3DInt* unionA(
		const type xMin, const type xMax,
		const type yMin, const type yMax,
		const type zMin, const type zMax);

	Bounds3DInt* intersect(Bounds3DInt& o);

	bool equals(Bounds3DInt& o);
	bool equals(Bounds3DInt* o);

    ~Bounds3DInt();
};
