#pragma once

#include "AMActionsSync.h"

namespace worldGame3D {
class ActionsModule : public AMActionsSync {
private: typedef AMActionsSync super;
public:
	ActionsModule(ActionsModule const&) = delete;
    ActionsModule(ActionsModule &&) = default;
    ActionsModule& operator=(ActionsModule const&) = delete;
    ActionsModule& operator=(ActionsModule &&) = default;

    explicit ActionsModule(
	    IWorldEntity* entity,
	    ActionsTemplate* t);

	~ActionsModule() override;
};
};
