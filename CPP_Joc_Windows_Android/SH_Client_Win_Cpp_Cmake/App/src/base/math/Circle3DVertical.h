#pragma once

#include <base/gh.h>
#include "Vector3.h"

class Circle3DVertical {pub dCtor(Circle3DVertical);
	pub Vector3 pos{};

	pub float radius = 0.0f;

	pub explicit Circle3DVertical();

	pub bool contains(float x, float y, float z);
	pub bool contains(Vector3& p);
	pub bool contains(Vector3* p);

	pub virtual ~Circle3DVertical();
};
