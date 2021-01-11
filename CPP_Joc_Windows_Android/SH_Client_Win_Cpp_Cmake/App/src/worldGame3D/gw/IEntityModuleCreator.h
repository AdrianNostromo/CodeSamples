#pragma once

#include <graphics/visual3d/drawable/IContainer3D.h>

namespace base {
	class IGameWorld;
};

class IEntityModuleCreator {
public:
    virtual IContainer3D* getLayerGameWorld3D() = 0;
	virtual base::IGameWorld* getGW() = 0;

	virtual ~IEntityModuleCreator() = default;
};
