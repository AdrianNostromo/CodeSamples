#pragma once

#include <worldGame3D/gw/entity/module/EntityModule.h>
#include "IActionsModule.h"
#include <worldGame3D/gw/entity/template/actions/ActionsTemplate.h>

namespace worldGame3D {
class AMBase : public EntityModule, virtual public IActionsModule {
private: typedef EntityModule super;
protected:
	ActionsTemplate* t;

public:
    AMBase(AMBase const&) = delete;
    AMBase(AMBase &&) = default;
    AMBase& operator=(AMBase const&) = delete;
    AMBase& operator=(AMBase &&) = default;

    explicit AMBase(
	    IWorldEntity* entity,
	    ActionsTemplate* t);

    ~AMBase() override;
};
};
