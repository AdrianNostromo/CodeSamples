#pragma once

#include <base/gh.h>
#include "E_Camera.h"

namespace graphics {
class Environment : public E_Camera {priv typedef E_Camera super;pub dCtor(Environment);
    pub explicit Environment();

    pub ~Environment() override;
};
};
