#include "Physics3DTemplate.h"
#include <rpg3D/gw/entity/module/physics3D/Physics3DModule.h>
#include <rpg3D/gw/config/physics/PhysicsCollissionConfig.h>
#include <worldGame3D/gw/entity/template/EntityTemplate.h>
#include <graphics/visual3d/drawable/voxelsGrid/tiling/TilingInformation.h>

using namespace rpg3D;

// rpg3D::PhysicsCollissionConfig*;
std::string Physics3DTemplate::DATA_KEY_extraInteractionFilters = "physics3D_extraInteractionFilters";

Physics3DTemplate::Physics3DTemplate(
	rpg3D::PhysicsCollissionConfig* interactionFilters,
	CfgPActorController* pActorControllerCfg)
    : super("physics3D", IPhysics3DModule::TYPE),
	interactionFilters(interactionFilters),
	pActorControllerCfg(pActorControllerCfg)
{
    //void
}

IEntityModule* Physics3DTemplate::createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) {
	Physics3DTemplate* t = dynamic_cast<Physics3DTemplate*>(_t);
    if(!t) {
        throw LogicException(LOC);
    }
	
	if (t->interactionFilters == nullptr) {
		throw LogicException(LOC);
	}

	int groupsBitMap = t->interactionFilters->groups;
	int anyInteraction_maskBitMap = t->interactionFilters->anyInteraction_mask;
	int hardImpact_maskBitMap = t->interactionFilters->hardImpacts_mask;

	std::shared_ptr<graphics::TilingInformation> tilingInformation = nullptr;

	if (extraData != nullptr) {
		std::shared_ptr<base::IWrappedValue> val_extraGroupsBitMap = extraData->getDataOptional(DATA_KEY_extraInteractionFilters);
		if (val_extraGroupsBitMap != nullptr) {
			rpg3D::PhysicsCollissionConfig* extraInteractionFilters = val_extraGroupsBitMap->getDirectAs_Custom<rpg3D::PhysicsCollissionConfig*>(-1);

			groupsBitMap = groupsBitMap | extraInteractionFilters->groups;
			anyInteraction_maskBitMap = anyInteraction_maskBitMap | extraInteractionFilters->anyInteraction_mask;
			hardImpact_maskBitMap = hardImpact_maskBitMap | extraInteractionFilters->hardImpacts_mask;
		}

		std::shared_ptr<base::IWrappedValue> wVal_tilingInformation = extraData->getDataOptional(EntityTemplate::ExtraDataID_tilingInformation);
		if (wVal_tilingInformation != nullptr) {
			tilingInformation = wVal_tilingInformation->getDirectAs_CustomB<std::shared_ptr<graphics::TilingInformation>>("graphics::TilingInformation");
		}
	}

	Physics3DModule* mod = new Physics3DModule(
		entity, t,
		groupsBitMap,
		anyInteraction_maskBitMap, hardImpact_maskBitMap
	);
	if (tilingInformation != nullptr) {
		mod->setTilingInformation(tilingInformation);
	}

    return mod;
}

Physics3DTemplate::~Physics3DTemplate() {
    //void
}
