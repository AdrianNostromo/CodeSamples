#pragma once

#include "IContainer3D.h"
#include <base/math/Vector3.h>
#include <base/math/Quaternion.h>

class IStage3D;

class IDrawable3D : virtual public IContainer3D {
public:
	virtual ~IDrawable3D() {};
};
