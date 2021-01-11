#include "LBGridTurnBasedMobBrain.h"
#include <base/math/Math.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <rpg3D/gw/entity/module/movingEntity/MovingEntityModule.h>
#include <rpg3D/gw/entity/template/statusEffects/StatusEffectsTemplate.h>
#include <rpg3D/gw/entity/module/statusEffects/IStatusEffectsModule.h>
#include <rpg3D/gw/entity/module/statusEffects/statusEffect/config/CfgStatusEffectStun.h>
#include <rpg3D/gw/content/IGPContent.h>
#include <rpg3D/gw/entity/module/grid2DContent/IGrid2DContentModule.h>
#include <rpg3D/gw/grid2DContent/IGPGrid2DContent.h>
#include <rpg3D/gw/entity/module/grid2DContent/IGrid2DContentModule.h>
#include <rpg3D/gw/entity/module/TeamModule.h>
#include <rpg3D/gw/turnBasedPlay/IGPTurnBasedPlay.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/ITool.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/playerUnit/moveTurnGridTurnBased/IToolMTGridTurnBased.h>
#include <rpg3D/gw/entity/module/toolsHandler/IToolsHandlerModule.h>
#include <rpg3D/gw/entity/template/ToolsHandlerTemplate.h>
#include <rpg3D/gw/turnBasedPlay/util/ITurnBasedInteractor.h>
#include <rpg3D/gw/entity/module/movingEntity/IMovingEntityModule.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>

using namespace gridAdventure_a_v1;
using namespace gridAdventure_a_v1::GridBattleA;

LBGridTurnBasedMobBrain::LBGridTurnBasedMobBrain()
	: super(0.1f/*tickCooldownS*/)
{
	//void
}

void LBGridTurnBasedMobBrain::onEntityChanged() {
	super::onEntityChanged();

	if (e == nullptr) {
		//void
	} else {
		//void
	}
}

void LBGridTurnBasedMobBrain::onIsActiveChanged(bool isActive_new, worldGame3D::IGameWorldGameParts* gwGameParts) {
	super::onIsActiveChanged(isActive_new, gwGameParts);

	if (!isActive_new) {
		if (turnBasedInteractor != nullptr) {
			turnBasedInteractor->unregister();
			turnBasedInteractor = nullptr;
		}

		if (toolsHandler != nullptr) {
			toolsHandler->setIsTurnBasedToolsModeActive(false);
		}
		toolsHandler = nullptr;
		grid2DContent = nullptr;
		team = nullptr;
		movingEntity = nullptr;
	} else {
		if (toolsHandler == nullptr) {
			toolsHandler = e->getComponentAs<rpg3D::IToolsHandlerModule*>(true/*mustExist*/);
			grid2DContent = entity->getComponentAs<rpg3D::IGrid2DContentModule*>(true/*mustExist*/);
			team = entity->getComponentAs<rpg3D::ITeamModule*>(true/*mustExist*/);
			movingEntity = entity->getComponentAs<IMovingEntityModule*>(true/*mustExist*/);
		}

		toolsHandler->setIsTurnBasedToolsModeActive(true);

		if (getIsZoneGamePartsReady()) {
			if (gpGridTurnBased == nullptr) {
				gpGridTurnBased = gwGameParts->getComponentAs<rpg3D::IGPTurnBasedPlay*>(true/*mustExist*/);
			}
			if (gpGrid2DContent == nullptr) {
				gpGrid2DContent = gwGameParts->getComponentAs<rpg3D::IGPGrid2DContent*>(false/*mustExist*/);
			}

			//GlobalAccessLogHandler::devLogHandler->post_debug("1 active turn state %i", gpGridTurnBased->getTurnState());

			// Add a listener for gpGridTurnBased state change.
			// This is required here because using the baked_isActive locgic will disable it when waiting for the event should occur.
			if (turnBasedInteractor != nullptr) {
				// Already registered.
				throw LogicException(LOC);
			}
			turnBasedInteractor = gpGridTurnBased->registerTurnBasedEntity(this);
		} else {
			//void
		}
	}
}

void LBGridTurnBasedMobBrain::onActiveActionInfoChanged() {
	//void
}

void LBGridTurnBasedMobBrain::connectToGameParts(worldGame3D::IGameWorldGameParts* gwGameParts) {
	if (getIsActive()) {
		if (gpGridTurnBased == nullptr) {
			gpGridTurnBased = gwGameParts->getComponentAs<rpg3D::IGPTurnBasedPlay*>(true/*mustExist*/);
		}
		if (gpGrid2DContent == nullptr) {
			gpGrid2DContent = gwGameParts->getComponentAs<rpg3D::IGPGrid2DContent*>(false/*mustExist*/);
		}

		//GlobalAccessLogHandler::devLogHandler->post_debug("2 active turn state %i", gpGridTurnBased->getTurnState());//asd_tool_t;

		turnBasedInteractor = gpGridTurnBased->registerTurnBasedEntity(this);
	}

	// Call after because the supers call an extended function that is in this class.
	super::connectToGameParts(gwGameParts);
}

void LBGridTurnBasedMobBrain::disconnectFromGamePartsPre() {
	if (turnBasedInteractor != nullptr) {
		turnBasedInteractor->unregister();
		turnBasedInteractor = nullptr;
	}

	gpGridTurnBased = nullptr;
	gpGrid2DContent = nullptr;

	super::disconnectFromGamePartsPre();
}

int LBGridTurnBasedMobBrain::getTeam() {
	if (team == nullptr) {
		// This should never occur.
		throw LogicException(LOC);
	}

	return team->getTeamIndex();
}

void LBGridTurnBasedMobBrain::onCurrentActionChanged() {
	onActiveActionInfoChanged();
}

LBGridTurnBasedMobBrain::~LBGridTurnBasedMobBrain() {
	//void
}
