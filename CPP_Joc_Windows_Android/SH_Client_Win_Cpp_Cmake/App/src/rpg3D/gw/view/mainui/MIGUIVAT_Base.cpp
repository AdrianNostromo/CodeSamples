#include "MIGUIVAT_Base.h"
#include <rpg3D/gw/entity/template/ToolsHandlerTemplate.h>
#include <rpg3D/gw/entity/module/toolsHandler/IToolsHandlerModule.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>

using namespace rpg3D;

MIGUIVAT_Base::MIGUIVAT_Base(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, base::IGameWorld* gw)
	: super(app, viewItemConfigs, viewEaseDurationsSList, gw)
{
	//void
}

void MIGUIVAT_Base::setListenerToolExtraActivation(IListenerToolExtraActivation* listenerToolExtraActivation) {
	this->listenerToolExtraActivation = listenerToolExtraActivation;
}

void MIGUIVAT_Base::onSelectedFocusEntityChange_pre() {
	super::onSelectedFocusEntityChange_pre();

	if (selectedFocusEntity_toolsHandler != nullptr) {
		selectedFocusEntity_toolsHandler = nullptr;
	}
}

void MIGUIVAT_Base::onSelectedFocusEntityChanged(IWorldEntity* selectedFocusEntity, ArrayList<std::shared_ptr<IEventListener>>& selectedFocusEntity_autoListenersList) {
	super::onSelectedFocusEntityChanged(selectedFocusEntity, selectedFocusEntity_autoListenersList);

	if (selectedFocusEntity != nullptr) {
		selectedFocusEntity_toolsHandler = selectedFocusEntity->getComponentAs<IToolsHandlerModule*>(false/*mustExist*/);
	}
}

MIGUIVAT_Base::~MIGUIVAT_Base() {
	//void
}
