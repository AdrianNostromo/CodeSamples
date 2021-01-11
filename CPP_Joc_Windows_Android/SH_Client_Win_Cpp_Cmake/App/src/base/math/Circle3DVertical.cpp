#include <base/math/Math.h>
#include "Circle3DVertical.h"

Circle3DVertical::Circle3DVertical() {
	//void
}

bool Circle3DVertical::contains(float x, float y, float z) {
	float dist = Math::dist3D(
		pos.x, pos.y, pos.z,
		x, y, z
	);
	if(dist > radius) {
		return false;
	}

	return true;
}

bool Circle3DVertical::contains(Vector3& p) {
	return contains(p.x, p.y, p.z);
}

bool Circle3DVertical::contains(Vector3* p) {
	return contains(p->x, p->y, p->z);
}

Circle3DVertical::~Circle3DVertical() {
	//void
}
