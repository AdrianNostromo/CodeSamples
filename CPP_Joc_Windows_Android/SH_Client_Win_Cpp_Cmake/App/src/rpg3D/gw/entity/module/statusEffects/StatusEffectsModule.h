#pragma once

#include <worldGame3D/gw/entity/util/EntityRemoveReason.h>
#include "SEMActions.h"

namespace rpg3D {
class StatusEffectsModule : public SEMActions {
private: typedef SEMActions super;
public:
	StatusEffectsModule(StatusEffectsModule const&) = delete;
    StatusEffectsModule(StatusEffectsModule &&) = default;
    StatusEffectsModule& operator=(StatusEffectsModule const&) = delete;
    StatusEffectsModule& operator=(StatusEffectsModule &&) = default;

    explicit StatusEffectsModule(
	    IWorldEntity* entity,
	    StatusEffectsTemplate* t);

	~StatusEffectsModule() override;
};
};
