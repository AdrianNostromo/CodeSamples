#pragma once

#include <worldGame3D/gw/entity/module/EntityModule.h>
#include <rpg3D/gw/entity/template/statusEffects/StatusEffectsTemplate.h>
#include "IStatusEffectsModule.h"

namespace rpg3D {
class SEMBase : public EntityModule, virtual public IStatusEffectsModule {
private: typedef EntityModule super;
public:
	StatusEffectsTemplate* t;

public:
    SEMBase(SEMBase const&) = delete;
    SEMBase(SEMBase &&) = default;
    SEMBase& operator=(SEMBase const&) = delete;
    SEMBase& operator=(SEMBase &&) = default;

    explicit SEMBase(
	    IWorldEntity* entity,
	    StatusEffectsTemplate* t);

    ~SEMBase() override;
};
};
