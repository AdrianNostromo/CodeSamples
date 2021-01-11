#pragma once

#include <base/gh.h>

class Matrix4;
class Vector3;
class Quaternion;

namespace worldGame3D {
class IPositional3DHandler {
	pub virtual void setMainIsPositionalHandler(bool isMainPositionalHandler) = 0;

	pub virtual Vector3* getPos() = 0;
	pub virtual float getX() = 0;
	pub virtual float getY() = 0;
	pub virtual float getZ() = 0;
	pub virtual void setPos(float x, float y, float z) = 0;
	pub virtual void setPos(Vector3& pos) = 0;
	pub virtual void setX(float x) = 0;
	pub virtual void setY(float y) = 0;
	pub virtual void setZ(float z) = 0;

	pub virtual Quaternion* getRot() = 0;
	pub virtual void setRot(Quaternion& rot) = 0;
	pub virtual void setRot(Vector3& axis, float degrees) = 0;
	pub virtual void setRotEulerZYX(Vector3& axis, float degrees) = 0;
	
	// This is a z_rotation but it rotates only a speciffic pBody if multi-pBody physics3D is used.
	pub virtual void setCardinalRotation(float degrees) = 0;

	pub virtual Vector3* getScale() = 0;
	pub virtual void setScale(float scale) = 0;

    pub virtual ~IPositional3DHandler() = default;
};
};
