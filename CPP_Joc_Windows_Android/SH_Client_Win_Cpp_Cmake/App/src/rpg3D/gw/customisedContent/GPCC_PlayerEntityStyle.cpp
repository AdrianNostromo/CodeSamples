#include "GPCC_PlayerEntityStyle.h"
#include <worldGame3D/gw/IGameWorld.h>
#include <rpg3D/scenariohandler/customisedContentHandler/ICustomisedContentHandler.h>
#include <rpg3D/gw/config/entity/EntityIds.h>
#include <worldGame3D/gw/entity/module/visual3D/Visual3DModule.h>
#include <graphics/visual3d/drawable/VoxelGridDrawable3D.h>
#include <worldGame3D/gw/entity/template/visual3D/Visual3DTemplate.h>
#include <worldGame3D/gw/main/IGamePartMain.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>

using namespace rpg3D;

GPCC_PlayerEntityStyle::GPCC_PlayerEntityStyle(base::IGameWorld* gw)
	: super(gw)
{
	gw->addEventListener(
		rpg3D::ICustomisedContentHandler::GWEvent_PlayerUnitMaterialsAndVoxelsGrid_loaded->type,
		std::bind(&GPCC_PlayerEntityStyle::onGWEvent_PlayerUnitMaterialsAndVoxelsGrid_loaded, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
}

void GPCC_PlayerEntityStyle::onGWEvent_PlayerUnitMaterialsAndVoxelsGrid_loaded(IEventListener& eventListener, base::GWEvent& event) {
	IWorldEntity* entity = gw->getGPMain()->getEntityWithInstanceId(*EntityIds::playerUnit);
	if (entity == nullptr) {
		throw LogicException(LOC);
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

GPCC_PlayerEntityStyle::~GPCC_PlayerEntityStyle() {
	//void
}
