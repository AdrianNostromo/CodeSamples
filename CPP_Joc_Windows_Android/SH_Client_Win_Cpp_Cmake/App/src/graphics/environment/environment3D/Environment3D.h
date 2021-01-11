#pragma once

#include <base/gh.h>
#include "E3D_Cinematic.h"

namespace graphics {
class Environment3D : public E3D_Cinematic {priv typedef E3D_Cinematic super;pub dCtor(Environment3D);
    pub explicit Environment3D();
	
    pub ~Environment3D() override;
};
};
