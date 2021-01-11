#include "T_Selected.h"
#include <rpg3D/gw/entity/template/toolshandler/util/ToolSlotConfig.h>
#include <rpg3D/gw/entity/module/toolsHandler/IToolsHandlerModule.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <rpg3D/gw/entity/template/ToolsHandlerTemplate.h>
#include <base/exceptions/LogicException.h>

using namespace rpg3D;

T_Selected::T_Selected(ToolConfigBase* config)
	: super(config)
{
	//void
}

IWorldEntity* T_Selected::getEntity() {
    return entity;
}

void T_Selected::setEntity(IWorldEntity* entity) {
    if (this->entity == entity) {
        return;
    }

    onEntityChangePre();

    this->entity = entity;

    onEntityChanged();
}

void T_Selected::onEntityChangePre() {
    //void
}

void T_Selected::onEntityChanged() {
    if (entity != nullptr) {
        toolsHandler = entity->getComponentAs<rpg3D::IToolsHandlerModule*>(true/*mustExist*/);
    } else {
        toolsHandler = nullptr;
    }
}

bool T_Selected::getIsToolSelected() {
    return isToolSelected;
}

ToolSlotConfig* T_Selected::getSelectedToolSlot() {
    return selectedToolSlot;
}

int T_Selected::getSelectedToolSlotIndex() {
    return selectedToolSlotIndex;
}

void T_Selected::selectedStateChanged(bool isToolSelected, int toolSlotIndex, ToolSlotConfig* toolSlot, worldGame3D::IGameWorldGameParts* gwGamePartsOptional) {
    if (this->isToolSelected == isToolSelected) {
        return;
    }

    this->isToolSelected = isToolSelected;
    this->selectedToolSlotIndex = toolSlotIndex;
    this->selectedToolSlot = toolSlot;

    _onSelectedStateChanged(this->isToolSelected, gwGamePartsOptional);
}

void T_Selected::_onSelectedStateChanged(bool isToolSelected, worldGame3D::IGameWorldGameParts* gwGamePartsOptional) {
    //void
}

ToolSlotConfig* T_Selected::peekSelectedToolSlot() {
    return selectedToolSlot;
}

T_Selected::~T_Selected() {
	//void
}
