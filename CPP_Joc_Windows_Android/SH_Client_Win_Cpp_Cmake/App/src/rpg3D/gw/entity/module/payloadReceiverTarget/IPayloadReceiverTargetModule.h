#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/module/IEntityModule.h>
#include <rpg3D/gw/entity/template/payloadReceiverTarget/PayloadReceiverTargetTemplate.h>

namespace rpg3D {
class IProjectileModule;
class ITeamModule;
class ILivingEntityModule;

class IPayloadReceiverTargetModule : public virtual IEntityModule {priv typedef IComponent super;
	pub static int TYPE;
	DEC_componentBasics();

	pub virtual PayloadReceiverTargetTemplate* getTemplate() = 0;

	pub virtual ITeamModule* getTeam() = 0;
	pub virtual ILivingEntityModule* getLivingEntity() = 0;

	pub ~IPayloadReceiverTargetModule() override = default;
};
};
