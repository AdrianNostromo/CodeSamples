#pragma once

#include <graphics/visual3d/drawable/IContainer3D.h>

class IRoot3D : virtual public IContainer3D {
public:
	virtual int getGlobalTransformPreupdateIndexCounter() = 0;
	virtual void incrementGlobalTransformPreupdateIndexCounter() = 0;

	~IRoot3D() override = default;
};
