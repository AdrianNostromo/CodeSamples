#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/module/EntityModule.h>
#include "ICustomCubesCannonTurretModule.h"

class IDrawableNode3D;
namespace base {
	namespace audio {
		class ISound;
	};
};
namespace worldGame3D {
	class IActionsModule;
	class IVisual3DModule;
};
namespace rpg3D {
	class EffectPayload;
};
namespace towerDefense_a_v1 {
	class CustomCubesCannonTurretTemplate;
};

namespace towerDefense_a_v1 {
class CCCTM_Base : public EntityModule, virtual public ICustomCubesCannonTurretModule {priv typedef EntityModule super; pub dCtor(CCCTM_Base);
	pub static int ActionGroup_AimToTarget;

	pub CustomCubesCannonTurretTemplate* t;

	prot base::audio::ISound* activationSound;

	prot worldGame3D::IActionsModule* actions = nullptr;
	priv worldGame3D::IVisual3DModule* visual3D = nullptr;

	prot IDrawableNode3D* groundBase = nullptr;
	prot IDrawableNode3D* turret = nullptr;
	prot IDrawableNode3D* barrel = nullptr;
	prot IDrawableNode3D* projectile = nullptr;

	prot std::string projectileTemplate_namespace = "";
	prot std::string projectileTemplate_id = "";
	prot ArrayList<std::shared_ptr<rpg3D::EffectPayload>> projectilePayloadsList{};

	pub CCCTM_Base(
		IWorldEntity* entity,
		CustomCubesCannonTurretTemplate* t,
		base::audio::ISound* activationSound);
	prot void createBMain() override;

	pub void setProjectileTemplateId(std::string& projectileTemplate_namespace, std::string& projectileTemplate_id) final;
	pub void addProjectilePayload(std::shared_ptr<rpg3D::EffectPayload> payload) final;

	pub ~CCCTM_Base() override;
};
};
