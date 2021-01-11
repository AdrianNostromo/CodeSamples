#include "LBGTBAT_TurnMonitor.h"
#include <rpg3D/gw/turnBasedPlay/IGPTurnBasedPlay.h>
#include <worldGame3D/gw/event/GWEvent.h>
#include <worldGame3D/gw/IGameWorldGameParts.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <base/log/GlobalAccessLogHandler.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/groups/IToolMovement.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/groups/IToolWeapon.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/playerUnit/moveTurnGridTurnBased/IToolMTGridTurnBased.h>
#include <rpg3D/gw/grid2DContent/IGPGrid2DContent.h>
#include <rpg3D/gw/entity/module/grid2DContent/IGrid2DContentModule.h>
#include <rpg3D/gw/entity/module/toolsHandler/IToolsHandlerModule.h>
#include <rpg3D/gw/turnBasedPlay/util/ITurnBasedInteractor.h>
#include <rpg3D/gw/entity/module/TeamModule.h>

using namespace rpg3D;
using namespace rpg3D::playerUnit;

LBGTBAT_TurnMonitor::LBGTBAT_TurnMonitor()
	: super()
{
	//void
}

bool LBGTBAT_TurnMonitor::shouldManageConnectTool(std::shared_ptr<util::WrappedTool> wTool) {
	if (!super::shouldManageConnectTool(wTool)) {
		return false;
	}
	
	if (turnBasedInteractor == nullptr/*not_connected_to_turn_based_gp*/ 
		|| turnBasedInteractor->getCurrentActionType() == ITurnBasedInteractor::None/*no_action_requested*/
		|| turnBasedInteractor->getIsCurrentActionFinshed()/*requested_action_already_finished*/)
	{
		return false;
	}

	if (turnBasedInteractor->getCurrentActionType() == ITurnBasedInteractor::ActionType::None) {
		//void
	}
	
	else if (turnBasedInteractor->getCurrentActionType() == ITurnBasedInteractor::ActionType::AnyAction) {
		if (startedActionType == ITurnBasedInteractor::ActionType::None) {
			if (wTool->tool->getGroupID() == rpg3D::IToolMovement::GroupID || wTool->tool->getGroupID() == rpg3D::IToolWeapon::GroupID) {
				return true;
			}
		} else if(startedActionType == ITurnBasedInteractor::ActionType::Movement) {
			if (wTool->tool->getGroupID() == rpg3D::IToolMovement::GroupID) {
				return true;
			}
		} else if(startedActionType == ITurnBasedInteractor::ActionType::Ability) {
			if (wTool->tool->getGroupID() == rpg3D::IToolWeapon::GroupID) {
				return true;
			}
		} else {
			// Unhandled action type.
			throw LogicException(LOC);
		}
	}
	
	else if (turnBasedInteractor->getCurrentActionType() == ITurnBasedInteractor::ActionType::Movement) {
		if (wTool->tool->getGroupID() == rpg3D::IToolMovement::GroupID) {
			return true;
		}
	}
	
	else if (turnBasedInteractor->getCurrentActionType() == ITurnBasedInteractor::ActionType::Ability) {
		if (wTool->tool->getGroupID() == rpg3D::IToolWeapon::GroupID) {
			return true;
		}
	}
	
	else {
		// Unhandled action type.
		throw LogicException(LOC);
	}

	return false;
}

void LBGTBAT_TurnMonitor::onEntityChanged() {
	super::onEntityChanged();

	if (e == nullptr) {
		//void
	} else {
		//void
	}
}

void LBGTBAT_TurnMonitor::onIsActiveChanged(bool isActive_new, worldGame3D::IGameWorldGameParts* gwGameParts) {
	super::onIsActiveChanged(isActive_new, gwGameParts);

	if (!isActive_new) {
		startedActionType = ITurnBasedInteractor::ActionType::None;

		if (turnBasedInteractor != nullptr) {
			turnBasedInteractor->unregister();
			turnBasedInteractor = nullptr;
		}

		localAutoListenersList.clear();

		grid2DContent = nullptr;
		team = nullptr;
	} else {
		if (grid2DContent == nullptr) {
			grid2DContent = entity->getComponentAs<IGrid2DContentModule*>(true/*mustExist*/);
			team = entity->getComponentAs<rpg3D::ITeamModule*>(true/*mustExist*/);
		}

		if (getIsZoneGamePartsReady()) {
			if (gpGridTurnBased == nullptr) {
				gpGridTurnBased = gwGameParts->getComponentAs<rpg3D::IGPTurnBasedPlay*>(true/*mustExist*/);
			}
			if (gpGrid2DContent == nullptr) {
				gpGrid2DContent = gwGameParts->getComponentAs<rpg3D::IGPGrid2DContent*>(false/*mustExist*/);
			}

			//GlobalAccessLogHandler::devLogHandler->post_debug("1 active turn state %i", gpGridTurnBased->getTurnState());

			e->addEventListener(
				rpg3D::ITool::EVENT_tool_triggerPointOfNoReturn_start->type, rpg3D::ITool::EVENT_tool_extendedActivationFinished->type,
				std::bind(&LBGTBAT_TurnMonitor::onEvent, this, std::placeholders::_1, std::placeholders::_2),
				localAutoListenersList
			);
			e->addEventListener(
				rpg3D::playerUnit::IToolMTGridTurnBased::EVENT_gridMovement_skipped->type, rpg3D::playerUnit::IToolMTGridTurnBased::EVENT_gridMovement_started->type, rpg3D::playerUnit::IToolMTGridTurnBased::EVENT_gridMovement_finished->type,
				std::bind(&LBGTBAT_TurnMonitor::onEvent, this, std::placeholders::_1, std::placeholders::_2),
				localAutoListenersList
			);
			
			// Add a listener for gpGridTurnBased state change.
			// This is required here because using the baked_isActive locgic will disable it when waiting for the event should occur.
			if (turnBasedInteractor != nullptr) {
				// Already registered.
				throw LogicException(LOC);
			}
			startedActionType = ITurnBasedInteractor::ActionType::None;
			turnBasedInteractor = gpGridTurnBased->registerTurnBasedEntity(this);
		} else {
			startedActionType = ITurnBasedInteractor::ActionType::None;
		}
	}
}

void LBGTBAT_TurnMonitor::onEvent(IEventListener& eventListener, base::EntityEvent& event) {
	if (event.type == rpg3D::ITool::EVENT_tool_triggerPointOfNoReturn_start->type) {
		if (startedActionType != ITurnBasedInteractor::ActionType::None) {
			throw LogicException(LOC);
		}

		startedActionType = ITurnBasedInteractor::ActionType::Ability;

		onActiveActionInfoChanged();
	} else if (event.type == rpg3D::ITool::EVENT_tool_extendedActivationFinished->type) {
		if (startedActionType != ITurnBasedInteractor::ActionType::Ability) {
			//asd_1;// entering the teleporter with a sword swing in progress, this will occur.
			throw LogicException(LOC);
		}

		turnBasedInteractor->currentActionFinished(startedActionType);

		onActiveActionInfoChanged();
	}
	
	else if (event.type == rpg3D::playerUnit::IToolMTGridTurnBased::EVENT_gridMovement_skipped->type) {
		if (startedActionType != ITurnBasedInteractor::ActionType::None) {
			throw LogicException(LOC);
		}
		
		turnBasedInteractor->currentActionFinished(startedActionType);
	} else if (event.type == rpg3D::playerUnit::IToolMTGridTurnBased::EVENT_gridMovement_started->type) {
		if (startedActionType != ITurnBasedInteractor::ActionType::None) {
			throw LogicException(LOC);
		}

		startedActionType = ITurnBasedInteractor::ActionType::Movement;

		onActiveActionInfoChanged();
	} else if (event.type == rpg3D::playerUnit::IToolMTGridTurnBased::EVENT_gridMovement_finished->type) {
		if (startedActionType != ITurnBasedInteractor::ActionType::Movement) {
			throw LogicException(LOC);
		}

		turnBasedInteractor->currentActionFinished(startedActionType);

		onActiveActionInfoChanged();
	}
	
	else {
		throw LogicException(LOC);
	}
}

void LBGTBAT_TurnMonitor::onActiveActionInfoChanged() {
	if (turnBasedInteractor == nullptr) {
		gpGrid2DContent->hidePathableGCellsIndicator();
	}
	
	else if (turnBasedInteractor->getCurrentActionType() == ITurnBasedInteractor::ActionType::None) {
		gpGrid2DContent->hidePathableGCellsIndicator();
	}

	else if (turnBasedInteractor->getCurrentActionType() == ITurnBasedInteractor::ActionType::AnyAction) {
		if (startedActionType == ITurnBasedInteractor::ActionType::None) {
			// Only when starting a _Pre action, increment this counter. This way, it will be the same until the tool activation finishes.
			toolsHandler->incrementTurnBasedActionStateCounter();

			// Show grid.
			gpGrid2DContent->showPathableGCellsIndicator(grid2DContent->getGPosRef(), 1/*drawRadius*/, false/*showStartGPosAsValidDestination*/);
		} else {
			gpGrid2DContent->hidePathableGCellsIndicator();
		}
	}
	
	else if (turnBasedInteractor->getCurrentActionType() == ITurnBasedInteractor::ActionType::Movement) {
		if (startedActionType == ITurnBasedInteractor::ActionType::None) {
			// Only when starting a _Pre action, increment this counter. This way, it will be the same until the tool activation finishes.
			toolsHandler->incrementTurnBasedActionStateCounter();

			// Show grid.
			gpGrid2DContent->showPathableGCellsIndicator(grid2DContent->getGPosRef(), 1/*drawRadius*/, false/*showStartGPosAsValidDestination*/);
		} else {
			gpGrid2DContent->hidePathableGCellsIndicator();
		}
	}
	
	else if (turnBasedInteractor->getCurrentActionType() == ITurnBasedInteractor::ActionType::Ability) {
		if (startedActionType == ITurnBasedInteractor::ActionType::None) {
			// Only when starting a _Pre action, increment this counter. This way, it will be the same until the tool activation finishes.
			toolsHandler->incrementTurnBasedActionStateCounter();
		} else {
			//void
		}
	}
	
	else {
		// Unahndled action type.
		throw LogicException(LOC);
	}

	syncToolsFilters();
}

void LBGTBAT_TurnMonitor::connectToGameParts(worldGame3D::IGameWorldGameParts* gwGameParts) {
	if (getIsActive()) {
		if (gpGridTurnBased == nullptr) {
			gpGridTurnBased = gwGameParts->getComponentAs<rpg3D::IGPTurnBasedPlay*>(true/*mustExist*/);
		}
		if (gpGrid2DContent == nullptr) {
			gpGrid2DContent = gwGameParts->getComponentAs<rpg3D::IGPGrid2DContent*>(false/*mustExist*/);
		}

		//GlobalAccessLogHandler::devLogHandler->post_debug("2 active turn state %i", gpGridTurnBased->getTurnState());

		e->addEventListener(
			rpg3D::ITool::EVENT_tool_triggerPointOfNoReturn_start->type, rpg3D::ITool::EVENT_tool_extendedActivationFinished->type,
			std::bind(&LBGTBAT_TurnMonitor::onEvent, this, std::placeholders::_1, std::placeholders::_2),
			localAutoListenersList
		);
		e->addEventListener(
			rpg3D::playerUnit::IToolMTGridTurnBased::EVENT_gridMovement_skipped->type, rpg3D::playerUnit::IToolMTGridTurnBased::EVENT_gridMovement_started->type, rpg3D::playerUnit::IToolMTGridTurnBased::EVENT_gridMovement_finished->type,
			std::bind(&LBGTBAT_TurnMonitor::onEvent, this, std::placeholders::_1, std::placeholders::_2),
			localAutoListenersList
		);
		
		// Add a listener for gpGridTurnBased state change.
		// This is required here because using the baked_isActive locgic will disable it when waiting for the event should occur.
		startedActionType = ITurnBasedInteractor::ActionType::None;
		turnBasedInteractor = gpGridTurnBased->registerTurnBasedEntity(this);
	}

	// Call after because the supers call an extended function that is in this class.
	super::connectToGameParts(gwGameParts);
}

void LBGTBAT_TurnMonitor::disconnectFromGamePartsPre() {
	startedActionType = ITurnBasedInteractor::ActionType::None;

	if (turnBasedInteractor != nullptr) {
		turnBasedInteractor->unregister();
		turnBasedInteractor = nullptr;
	}
	localAutoListenersList.clear();

	gpGridTurnBased = nullptr;
	gpGrid2DContent = nullptr;

	super::disconnectFromGamePartsPre();
}

int LBGTBAT_TurnMonitor::getTeam() {
	if (team == nullptr) {
		// This should never occur.
		throw LogicException(LOC);
	}

	return team->getTeamIndex();
}

void LBGTBAT_TurnMonitor::onCurrentActionChanged() {
	startedActionType = ITurnBasedInteractor::ActionType::None;

	onActiveActionInfoChanged();
}

LBGTBAT_TurnMonitor::~LBGTBAT_TurnMonitor() {
	//void
}
