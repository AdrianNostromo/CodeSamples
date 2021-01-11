#pragma once

#include <worldGame3D/gw/entity/module/EntityModule.h>
#include "IPayloadReceiverTargetModule.h"
#include <base/list/ArrayList.h>
#include <rpg3D/gw/entity/template/payloadReceiverTarget/PayloadReceiverTargetTemplate.h>
#include <worldGame3D/gw/entity/module/EntityModule.h>

namespace rpg3D {
class ITeamModule;
class ILivingEntityModule;
class IProjectileModule;

class PayloadReceiverTargetModule : public EntityModule, virtual public IPayloadReceiverTargetModule {
private: typedef EntityModule super;
public:
	PayloadReceiverTargetTemplate* t;

	ITeamModule* team = nullptr;
	ILivingEntityModule* livingEntity = nullptr;

public:
	PayloadReceiverTargetModule(PayloadReceiverTargetModule const&) = delete;
    PayloadReceiverTargetModule(PayloadReceiverTargetModule &&) = default;
    PayloadReceiverTargetModule& operator=(PayloadReceiverTargetModule const&) = delete;
    PayloadReceiverTargetModule& operator=(PayloadReceiverTargetModule &&) = default;

    explicit PayloadReceiverTargetModule(
	    IWorldEntity* entity,
	    PayloadReceiverTargetTemplate* t);

	PayloadReceiverTargetTemplate* getTemplate() final;

	ITeamModule* getTeam() override;
	ILivingEntityModule* getLivingEntity() override;

	~PayloadReceiverTargetModule() override;
protected:
	void createBMain() override;

};
};
