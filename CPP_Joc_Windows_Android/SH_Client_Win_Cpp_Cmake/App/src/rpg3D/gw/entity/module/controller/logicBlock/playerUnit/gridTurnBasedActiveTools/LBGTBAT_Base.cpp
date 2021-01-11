#include "LBGTBAT_Base.h"
#include <rpg3D/gw/entity/module/toolsHandler/IToolsHandlerModule.h>
#include <rpg3D/gw/entity/template/ToolsHandlerTemplate.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>

using namespace rpg3D;
using namespace rpg3D::playerUnit;

LBGTBAT_Base::LBGTBAT_Base()
	: super()
{
	//void
}

void LBGTBAT_Base::onIsActiveChanged(bool isActive_new, worldGame3D::IGameWorldGameParts* gwGameParts) {
	super::onIsActiveChanged(isActive_new, gwGameParts);

	if (!isActive_new) {
		if (toolsHandler != nullptr) {
			toolsHandler->setIsTurnBasedToolsModeActive(false);
		}
	} else {
		if (toolsHandler == nullptr) {
			toolsHandler = e->getComponentAs<rpg3D::IToolsHandlerModule*>(true/*mustExist*/);
		}

		toolsHandler->setIsTurnBasedToolsModeActive(true);
	}
}

LBGTBAT_Base::~LBGTBAT_Base() {
	//void
}
