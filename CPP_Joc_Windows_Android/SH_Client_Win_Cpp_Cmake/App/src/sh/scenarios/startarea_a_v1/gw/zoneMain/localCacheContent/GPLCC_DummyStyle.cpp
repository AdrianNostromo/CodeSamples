#include "GPLCC_DummyStyle.h"
#include <base/map/ExtraData.h>
#include <sh/scenarios/startarea_a_v1/gw/config/entity/EntityIds.h>
#include <rpg3D/gw/config/entity/unit/Templates_Unit_PlayerUnitDummy_A_V1.h>
#include <rpg3D/gw/config/entity/unit/UnitTemplates_PlayerA.h>
#include <worldGame3D/gw/entity/module/visual3D/Visual3DModule.h>
#include <graphics/visual3d/drawable/VoxelGridDrawable3D.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <rpg3D/gw/entity/module/TeamModule.h>
#include <sh/scenarios/startarea_a_v1/gw/zoneMain/localCacheContent/IGamePartLocalCacheContent.h>
#include <rpg3D/gw/config/team/TeamsIndexesList.h>
#include <rpg3D/scenariohandler/customisedContentHandler/ICustomisedContentHandler.h>
#include <rpg3D/gw/customisedContent/IGamePartCustomisedContent.h>
#include <worldGame3D/gw/main/IGamePartMain.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>

using namespace startarea_a_v1;

GPLCC_DummyStyle::GPLCC_DummyStyle(base::IGameWorld* gw)
	: super(gw)
{
	gw->addEventListener(
		rpg3D::ICustomisedContentHandler::GWEvent_PlayerUnitMaterialsAndVoxelsGrid_loaded->type,
		std::bind(&GPLCC_DummyStyle::onGWEvent_PlayerUnitMaterialsAndVoxelsGrid_loaded, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
}

void GPLCC_DummyStyle::onGWEvent_PlayerUnitMaterialsAndVoxelsGrid_loaded(IEventListener& eventListener, base::GWEvent& event) {
	IWorldEntity* entity = gw->getGPMain()->getEntityWithInstanceId(*EntityIds::player_hero_dummy);
	if (entity == nullptr) {
		return;
	}

	rpg3D::IGamePartCustomisedContent* gpCustomisedContent = gw->getComponentAs<rpg3D::IGamePartCustomisedContent*>(true/*mustExist*/);
	std::shared_ptr<rpg3D::ICustomisedContentHandler::EntityMaterialsAndVoxelsGrid> unitMaterialsAndVoxelsGrid = gpCustomisedContent->getCustomisedContentHandler()->getPlayerUnitMaterialsAndVoxelsGrid();

	worldGame3D::IVisual3DModule* entity_visual3D = entity->getComponentAs<worldGame3D::IVisual3DModule*>(true/*mustExist*/);
	VoxelGridDrawable3D* entity_editingVoxelsVisual = entity_visual3D->getDrawableMustExistAs< VoxelGridDrawable3D*>("_VoxelGridGroup_");
	entity_editingVoxelsVisual->getVoxelsGrid()->setMaterialsListAndVoxelsGridCloneMode(
		unitMaterialsAndVoxelsGrid->materialsList,
		unitMaterialsAndVoxelsGrid->voxelGrid
	);
}

void GPLCC_DummyStyle::onPersistentEntitiesLoadFinished() {
	super::onPersistentEntitiesLoadFinished();

	// The dummy entity was not found in local cache, create a new one.
	IWorldEntity* player_hero_dummy = gw->getGPMain()->getEntityWithInstanceId(*EntityIds::player_hero_dummy);
	if (player_hero_dummy == nullptr) {
		std::shared_ptr<ExtraData> extraData = std::make_shared<ExtraData>();
		
		rpg3D::IGamePartCustomisedContent* gpCustomisedContent = gw->getComponentAs<rpg3D::IGamePartCustomisedContent*>(true/*mustExist*/);
		std::shared_ptr<rpg3D::ICustomisedContentHandler::EntityMaterialsAndVoxelsGrid> unitMaterialsAndVoxelsGrid = gpCustomisedContent->getCustomisedContentHandler()->getPlayerUnitMaterialsAndVoxelsGrid();
		extraData->setData(rpg3D::UnitTemplates_PlayerA::DATA_KEY_voxelGridZXY, base::IWrappedValue::new_Custom<Array3D<signed char>*>(unitMaterialsAndVoxelsGrid->voxelGrid, -1));
		extraData->setData(rpg3D::UnitTemplates_PlayerA::DATA_KEY_materials, base::IWrappedValue::new_Custom<ArrayList<Visual3DModelMaterial*>*>(unitMaterialsAndVoxelsGrid->materialsList, -1));

		IWorldEntity* platformCreatureCreator = gw->getGPMain()->getEntityWithInstanceId(*EntityIds::platformCreatureCreator);
		if (platformCreatureCreator == nullptr) {
			throw LogicException(LOC);
		}
		Vector3 pos{ platformCreatureCreator->getPos() };

		float angZDeg = -135.0f;

		Quaternion rot{ Vector3::Z, angZDeg };
		player_hero_dummy = gw->getGPMain()->createEntity(
			rpg3D::Templates_Unit_PlayerUnitDummy_A_V1::getNew(
				rpg3D::Templates_Unit_PlayerUnitDummy_A_V1::default_voxelSize, rpg3D::Templates_Unit_PlayerUnitDummy_A_V1::default_voxelGridOffset,
				rpg3D::Templates_Unit_PlayerUnitDummy_A_V1::default_fixedSizeAABBToushShape_min, rpg3D::Templates_Unit_PlayerUnitDummy_A_V1::default_fixedSizeAABBToushShape_max,
				rpg3D::Templates_Unit_PlayerUnitDummy_A_V1::default_stackable_haulOffsetZM
			),
			pos,
			&rot/*rot*/, nullptr/*cardinalRotation*/,
			EntityIds::player_hero_dummy/*instanceId*/,
			0/*ownerPlayerId*/,
			extraData,
			true/*isZoneOwnedEntity*/, nullptr/*manualContentGroupsList*/, nullptr/*creatorEntity*/,
			[](IWorldEntity* cfgEntity) -> void {
				rpg3D::ITeamModule* entity_team = cfgEntity->getComponentAs<rpg3D::ITeamModule*>(true/*mustExist*/);
				entity_team->setTeamIndex(rpg3D::TeamsIndexesList::PLAYER_UNIT);
			}
		);
		pushPersistentEntity(player_hero_dummy, false);
	}
}

GPLCC_DummyStyle::~GPLCC_DummyStyle() {
	//void
}
