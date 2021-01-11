#pragma once

#include <base/gh.h>
#include <graphics/material/Material.h>
#include "IEnvironment.h"

namespace graphics {
class E_Base : public AttributesList, virtual public IEnvironment {priv typedef AttributesList super;pub dCtor(E_Base);
    priv bool isCreated = false;

    pub explicit E_Base();

    pub void reservedCreate();

    prot virtual void createMain();
    prot virtual void create();
    prot virtual void createPost();

    pub bool getIsCreated();

    pub void syncIfNeeded() override;

    pub ~E_Base() override;
};
};
