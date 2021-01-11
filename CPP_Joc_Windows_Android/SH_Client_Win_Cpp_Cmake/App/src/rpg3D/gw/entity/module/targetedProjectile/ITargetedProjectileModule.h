#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/module/IEntityModule.h>
#include <memory>
#include <base/list/ArrayList.h>
#include <base/math/Vector3.h>

class IWorldEntity;
namespace rpg3D {
	class EffectPayload;
};

namespace rpg3D {
class ITargetedProjectileModule : public virtual IEntityModule {priv typedef IComponent super;
	pub static int TYPE;
	DEC_componentBasics();

	pub virtual void initTargetEntity(
		IWorldEntity* targetEntity) = 0;

	pub virtual void addPayload(std::shared_ptr<rpg3D::EffectPayload> payload) = 0;
	pub virtual void addPayloadsList(ArrayList<std::shared_ptr<rpg3D::EffectPayload>>* payloadsList) = 0;

	pub ~ITargetedProjectileModule() override = default;
};
};
