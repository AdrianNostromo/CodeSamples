#pragma once

#include <base/gh.h>
#include <memory>
#include <worldGame3D/gw/entity/module/IEntityModule.h>
#include <base/math/Vector3.h>
#include <worldGame3D/gw/entity/event/EntityEvent.h>

class IWorldEntity;
namespace base {
	class WrappedWorldEntity;
};
namespace rpg3D {
	class EffectPayload;
};

namespace towerDefense_a_v1 {
class ICustomCubesCannonTurretModule : public virtual IEntityModule {priv typedef IComponent super;
	pub static int TYPE;
	DEC_componentBasics();

	pub static base::EntityEvent* EVENT_ReloadFinished;
	
	pub static base::EntityEvent* EVENT_ProjectileLaunched;
	pub static base::EntityEvent* EVENT_ProjectileLaunchCancelled_TargetLost;

	pub virtual void setProjectileTemplateId(std::string& projectileTemplate_namespace, std::string& projectileTemplate_id) = 0;
	pub virtual void addProjectilePayload(std::shared_ptr<rpg3D::EffectPayload> payload) = 0;

	pub virtual bool getIsProjectileLoaded() = 0;
	pub virtual bool isPositionInAimAnglesRange(Vector3& pos, float maxYawOffset, float maxPitchOffset) = 0;

	pub virtual void addAction_ActionReload() = 0;
	pub virtual void addAction_ActionSetAimTarget(IWorldEntity* aimTargetEnity) = 0;
	pub virtual void addAction_ActionFireProjectileAtTarget(IWorldEntity* attackTargetEnity) = 0;

    pub ~ICustomCubesCannonTurretModule() override = default;
};
};
