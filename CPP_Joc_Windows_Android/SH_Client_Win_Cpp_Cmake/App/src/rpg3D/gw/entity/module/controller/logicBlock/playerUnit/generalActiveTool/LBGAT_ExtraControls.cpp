#include "LBGAT_ExtraControls.h"
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <rpg3D/gw/entity/template/userControlled/UserControlledTemplate.h>
#include <rpg3D/gw/entity/module/userControlled/IUserControlledModule.h>
#include <base/math/Math.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <base/log/GlobalAccessLogHandler.h>
#include <rpg3D/gw/entity/module/controller/logicBlock/util/WrappedTool.h>
#include <rpg3D/gw/entity/module/userControlled/util/WorldOrEntityInteractionEvent.h>
#include <worldGame3D/gw/event/GWEvent.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/config/ToolConfigBase.h>
#include <rpg3D/gw/entity/template/ToolsHandlerTemplate.h>
#include <rpg3D/gw/entity/module/toolsHandler/IToolsHandlerModule.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/groups/IToolMovement.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/groups/IToolTurn.h>

using namespace rpg3D;
using namespace rpg3D::playerUnit;

LBGAT_ExtraControls::LBGAT_ExtraControls()
	: super()
{
	//void
}

void LBGAT_ExtraControls::onToolHooked(std::shared_ptr<util::WrappedTool> wTool) {
	super::onToolHooked(wTool);

	//void
}

bool LBGAT_ExtraControls::shouldManageConnectTool(std::shared_ptr<util::WrappedTool> wTool) {
	if (!super::shouldManageConnectTool(wTool)) {
		return false;
	}

	if (wTool->tool->getConfig()->isUserSelectable) {
		return true;
	} else {
		// Auto tool.
		if (isExtraAutoToolsEnabled || (wTool->tool->getGroupID() == rpg3D::IToolMovement::GroupID || wTool->tool->getGroupID() == rpg3D::IToolTurn::GroupID)) {
			return true;
		}
	}

	return false;
}

void LBGAT_ExtraControls::onIsActiveChanged(bool isActive_new, worldGame3D::IGameWorldGameParts* gwGameParts) {
	super::onIsActiveChanged(isActive_new, gwGameParts);

	if (!isActive_new) {
		localAutoListenersList.clear();
	} else {
		if (toolsHandler == nullptr) {
			toolsHandler = entity->getComponentAs<rpg3D::IToolsHandlerModule*>(true/*mustExist*/);
		}
	
		// Don't use the setter because there are no tools hooked here.
		isExtraAutoToolsEnabled = toolsHandler->getIsAutomaticToolsEnabled();

		e->addEventListener(
			rpg3D::IToolsHandlerModule::EVENT_toolsHandler_nonUserSelectableToolsActivation_disabled->type, rpg3D::IToolsHandlerModule::EVENT_toolsHandler_nonUserSelectableToolsActivation_enabled->type,
			std::bind(&LBGAT_ExtraControls::onEvent, this, std::placeholders::_1, std::placeholders::_2),
			localAutoListenersList
		);
	}
}

void LBGAT_ExtraControls::onEvent(IEventListener& eventListener, base::EntityEvent& event) {
	if (event.type == rpg3D::IToolsHandlerModule::EVENT_toolsHandler_nonUserSelectableToolsActivation_disabled->type) {
		setIsExtraAutoToolsEnabled(false);
	} else if (event.type == rpg3D::IToolsHandlerModule::EVENT_toolsHandler_nonUserSelectableToolsActivation_enabled->type) {
		setIsExtraAutoToolsEnabled(true);
	}

	else {
		throw LogicException(LOC);
	}
}

void LBGAT_ExtraControls::setIsExtraAutoToolsEnabled(bool isExtraAutoToolsEnabled) {
	if (this->isExtraAutoToolsEnabled == isExtraAutoToolsEnabled) {
		return;
	}

	this->isExtraAutoToolsEnabled = isExtraAutoToolsEnabled;

	syncToolsFilters();
}

LBGAT_ExtraControls::~LBGAT_ExtraControls() {
	//void
}
