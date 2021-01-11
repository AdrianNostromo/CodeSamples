#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/template/BasicModuleTemplate.h>
#include <base/math/Vector3.h>
#include <base/physics3D/config/shape/PhysicsShapeConfig.h>
#include <base/list/ArrayList.h>
#include <base/physics3D/config/body/PBodyConfig.h>
#include "constraint/PhysicsConstraintConfig.h"

namespace rpg3D {
	class PhysicsCollissionConfig;
	class CfgPActorController;

class Physics3DTemplate : public BasicModuleTemplate {priv typedef BasicModuleTemplate super;pub dCtor(Physics3DTemplate);
	// rpg3D::PhysicsCollissionConfig*;
	pub static std::string DATA_KEY_extraInteractionFilters;

	// These are used for any physics interaction.
	pub rpg3D::PhysicsCollissionConfig* interactionFilters;

	pub CfgPActorController* pActorControllerCfg;

	pub ArrayList<PBodyConfig> pBodiesConfigsList{};

	pub ArrayList<PhysicsConstraintConfig*> pConstraintsConfigsList{};

    pub Physics3DTemplate(
		rpg3D::PhysicsCollissionConfig* interactionFilters,
		CfgPActorController* pActorControllerCfg);
    
	pub IEntityModule* createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) override;
    
	pub ~Physics3DTemplate() override;
};
};
