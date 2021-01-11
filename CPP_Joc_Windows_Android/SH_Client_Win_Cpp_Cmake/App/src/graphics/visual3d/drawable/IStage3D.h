#pragma once

#include <graphics/visual/drawable/IStage.h>
#include "IContainer3D.h"
#include <graphics/visual/drawable/IStage.h>

class IStage3D : virtual public IStage, virtual public IContainer3D {
public:
	//void

    ~IStage3D() override {};
};
