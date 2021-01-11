#include "LB_Mob_Chicken_TB_A_V1.h"
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
#include <rpg3D/gw/entity/module/movingEntity/IMovingEntityModule.h>
#include <base/math/Vector2.h>
#include <rpg3D/gw/entity/template/movingEntity/movementConfig/MovementConfigDirect.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <base/event/listener/IEventListener.h>
#include <worldGame3D/gw/entity/event/EntityEvent.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/melleArea/ToolMelleArea.h>
#include <rpg3D/gw/entity/module/ILivingEntityModule.h>

using namespace gridAdventure_a_v1;
using namespace gridAdventure_a_v1::GridBattleA;

std::string LB_Mob_Chicken_TB_A_V1::LOCAL_ACTION_INITIATOR_IDENTIFIER = "LB_Mob_Chicken_TB_A_V1";

LB_Mob_Chicken_TB_A_V1::LB_Mob_Chicken_TB_A_V1()
	: super()
{
	//void
}

void LB_Mob_Chicken_TB_A_V1::onEntityChanged() {
	super::onEntityChanged();

	if (e == nullptr) {
		//void
	} else {
		//void
	}
}

void LB_Mob_Chicken_TB_A_V1::onIsActiveChanged(bool isActive_new, worldGame3D::IGameWorldGameParts* gwGameParts) {
	super::onIsActiveChanged(isActive_new, gwGameParts);

	if (!isActive_new) {
		startedActionType = rpg3D::ITurnBasedInteractor::ActionType::None;

		currentActionAutoListenersList.clear();
	} else {
		if (getIsZoneGamePartsReady()) {
			startedActionType = rpg3D::ITurnBasedInteractor::ActionType::None;
		} else {
			startedActionType = rpg3D::ITurnBasedInteractor::ActionType::None;
		}
	}
}

void LB_Mob_Chicken_TB_A_V1::connectToGameParts(worldGame3D::IGameWorldGameParts* gwGameParts) {
	if (getIsActive()) {
		// Add a listener for gpGridTurnBased state change.
		// This is required here because using the baked_isActive locgic will disable it when waiting for the event should occur.
		startedActionType = rpg3D::ITurnBasedInteractor::ActionType::None;
	}

	// Call after because the supers call an extended function that is in this class.
	super::connectToGameParts(gwGameParts);
}

void LB_Mob_Chicken_TB_A_V1::disconnectFromGamePartsPre() {
	startedActionType = rpg3D::ITurnBasedInteractor::ActionType::None;

	currentActionAutoListenersList.clear();

	super::disconnectFromGamePartsPre();
}

void LB_Mob_Chicken_TB_A_V1::onActiveActionInfoChanged() {
	if (turnBasedInteractor == nullptr) {
		//void
	}

	// Maybe use these to show a indicator above the mob that is currently acting.
	else if (turnBasedInteractor->getCurrentActionType() == rpg3D::ITurnBasedInteractor::ActionType::None) {
		//void
	}

	else if (turnBasedInteractor->getCurrentActionType() == rpg3D::ITurnBasedInteractor::ActionType::AnyAction) {
		if (startedActionType == rpg3D::ITurnBasedInteractor::ActionType::None) {
			// Only when starting a _Pre action, increment this counter. This way, it will be the same until the tool activation finishes.
			toolsHandler->incrementTurnBasedActionStateCounter();

			//void
		} else {
			//void
		}
	}

	else if (turnBasedInteractor->getCurrentActionType() == rpg3D::ITurnBasedInteractor::ActionType::Movement) {
		if (startedActionType == rpg3D::ITurnBasedInteractor::ActionType::None) {
			// Only when starting a _Pre action, increment this counter. This way, it will be the same until the tool activation finishes.
			toolsHandler->incrementTurnBasedActionStateCounter();

			//void
		} else {
			//void
		}
	}

	else if (turnBasedInteractor->getCurrentActionType() == rpg3D::ITurnBasedInteractor::ActionType::Ability) {
		if (startedActionType == rpg3D::ITurnBasedInteractor::ActionType::None) {
			// Only when starting a _Pre action, increment this counter. This way, it will be the same until the tool activation finishes.
			toolsHandler->incrementTurnBasedActionStateCounter();

			//void
		} else {
			//void
		}
	}

	else {
		// Unahndled action type.
		throw LogicException(LOC);
	}
}

void LB_Mob_Chicken_TB_A_V1::logicTick(float deltaS) {
	super::logicTick(deltaS);

	if (turnBasedInteractor == nullptr) {
		return;
	}

	if (turnBasedInteractor->getCurrentActionType() == rpg3D::ITurnBasedInteractor::ActionType::None || turnBasedInteractor->getIsCurrentActionFinshed()) {
		return;
	}

	if (turnBasedInteractor->getCurrentActionType() == rpg3D::ITurnBasedInteractor::ActionType::AnyAction) {
		if (startedActionType == rpg3D::ITurnBasedInteractor::ActionType::None) {
			isActionStartEventReceived = false;

			bool isToolUsed = false;
			sp<rpg3D::ToolMelleArea> cToolBeak = nullptr;

			{
				ListDL<sp<rpg3D::Tool>>& selectedToolsList = toolsHandler->getSelectedToolsList();
				if (selectedToolsList.count() > 1) {
					// Too many tools. Current logic cna handle only 1 selected tool.
					throw LogicException(LOC);
				} else if (selectedToolsList.count() == 1) {
					cToolBeak = selectedToolsList.getFirst()->data.dCast<rpg3D::ToolMelleArea>(true/*mustExtend*/);
					if (cToolBeak == nullptr) {
						// Wrong tool it seems.
						throw LogicException(LOC);
					}
				}
			}

			static Array1D<Vector2> neighbourGCellOffsets{ 4, new Vector2[4] {
							    Vector2{0,  1}, 
				Vector2{-1,  0},				Vector2{1,  0},
							    Vector2{0, -1}, 
			}};
			IWorldEntity* aggroEnemyUnit = nullptr;
			{
				for (int i = 0; i < neighbourGCellOffsets.count(); i++) {
					Vector2 gPos{ static_cast<float>(grid2DContent->getGPosRef().x), static_cast<float>(grid2DContent->getGPosRef().y) };
					gPos.add(neighbourGCellOffsets.getReference(i));

					sp<ListDL<rpg3D::IGrid2DContentModule*>> neighbourGCellEntities = gpGrid2DContent->getAllEntitiesAtGPos(gPos.x, gPos.y, false/*errorOnOutOfBounds*/);
					if (neighbourGCellEntities == nullptr) {
						continue;
					}

					for (ListDL<rpg3D::IGrid2DContentModule*>::Entry* listEntry = neighbourGCellEntities->getFirst(); listEntry != nullptr; listEntry = listEntry->next) {
						rpg3D::IGrid2DContentModule* neighbourEntity_grid2DContent = listEntry->data;
						if (neighbourEntity_grid2DContent == nullptr) {
							continue;
						}

						IWorldEntity* neighbourEntity = neighbourEntity_grid2DContent->getEntity();

						bool hasLivingModule = neighbourEntity->hasModule(rpg3D::ILivingEntityModule::TYPE);
						if (!hasLivingModule) {
							continue;
						}

						rpg3D::ITeamModule* neighbourEntity_team = neighbourEntity->getComponentAs<rpg3D::ITeamModule*>(false/*mustExist*/);
						if (neighbourEntity_team == nullptr || team->getTeamIndex() == neighbourEntity_team->getTeamIndex()) {
							continue;
						}

						aggroEnemyUnit = neighbourEntity;

						break;
					}

					if (aggroEnemyUnit != nullptr) {
						break;
					}
				}
			}

			if (cToolBeak != nullptr && aggroEnemyUnit) {
				//asd_1; if (asd_enemy_in_range) {
				startedActionType = rpg3D::ITurnBasedInteractor::ActionType::Ability;
				startedActionDurationS = 0.0f;
				onActiveActionInfoChanged();

				isToolUsed = true;

				entity->addEventListener(
					rpg3D::ITool::EVENT_tool_triggerPointOfNoReturn_start->type, rpg3D::ITool::EVENT_tool_extendedActivationFinished->type,
					std::bind(&LB_Mob_Chicken_TB_A_V1::onEvent, this, std::placeholders::_1, std::placeholders::_2),
					currentActionAutoListenersList
				);

				Vector3* entityPos = entity->getPos();
				Vector3 dir{
					aggroEnemyUnit->getX() - entityPos->x,
					aggroEnemyUnit->getY() - entityPos->y,
					0.0f
				};
				dir.nor();
				cToolBeak->addAction_ActionOperate_Activate(
					0/*cursorIndex*/, nullptr/*worldPos*/, std::make_shared<Vector3>(dir)/*worldDir*/, nullptr/*worldPosXY*/,
					true/*isEnabledPreTurning*/
				);
			}
			
			if (!isToolUsed) {
				// Do the patroll if possible.
				Vector2Int targetGridPos{
					grid2DContent->getGPosRef().x + nextPatrollMoveDirectionX,
					grid2DContent->getGPosRef().y
				};

				if (gpGrid2DContent->getIsGCellOccupied(targetGridPos.x, targetGridPos.y)) {
					// Target gCell is occupied, don't move there. Skip the movement for now.
					turnBasedInteractor->currentActionFinished(startedActionType);
				} else {
					if (!isToolUsed) {
						startedActionType = rpg3D::ITurnBasedInteractor::ActionType::Movement;
						startedActionDurationS = 0.0f;
						onActiveActionInfoChanged();

						Vector2 targetWorldPos{};
						gpGrid2DContent->gPosCenterToWorldPos(targetGridPos, targetWorldPos);

						nextPatrollMoveDirectionX *= -1;

						entity->addEventListener(
							IMovingEntityModule::EVENT_move_Start->type, IMovingEntityModule::EVENT_move_destinationReached->type,
							std::bind(&LB_Mob_Chicken_TB_A_V1::onEvent, this, std::placeholders::_1, std::placeholders::_2),
							currentActionAutoListenersList
						);

						movingEntity->addAction_ActionToTargetMovementContinuous(
							IMovingEntityModule::ControlType::LocalEntityDirection_BackForwardOnly,
							targetWorldPos, nullptr/*targetEntity*/,
							0.0f/*maxRange*/, true/*isStopAtDestination*/,
							true/*isAutoTurn*/, true/*isInstantTurn*/,
							LOCAL_ACTION_INITIATOR_IDENTIFIER/*initiatorIdentifier*/,
							rpg3D::MovementConfigDirect::TYPE/*movementTypeOverride*/
						);
					}
				}
			}
		} else {
			// Action started, waiting for finish event.
			
			// Add a 5 seconds cooldown for error detection.
			startedActionDurationS += deltaS;
			if (startedActionDurationS >= 8.0f) {
				// All action should finish faster than 8 seconds. An error probably occured or a new type of action that lasted longer.
				throw LogicException(LOC);
			}
		}
	}
}

void LB_Mob_Chicken_TB_A_V1::onEvent(IEventListener& eventListener, base::EntityEvent& event) {
	if (event.type == MovingEntityModule::EVENT_move_Start->type) {
		if (event.initiatorIdentifier != LOCAL_ACTION_INITIATOR_IDENTIFIER) {
			// This is not the movement started from here.
			throw LogicException(LOC);
		}

		if (startedActionType != rpg3D::ITurnBasedInteractor::ActionType::Movement) {
			throw LogicException(LOC);
		}

		if (isActionStartEventReceived) {
			// Start event already received.
			throw LogicException(LOC);
		}
		isActionStartEventReceived = true;
	}else if (event.type == MovingEntityModule::EVENT_move_destinationReached->type) {
		if (event.initiatorIdentifier != LOCAL_ACTION_INITIATOR_IDENTIFIER) {
			// This is not the movement started from here.
			throw LogicException(LOC);
		}
		if (!isActionStartEventReceived) {
			// Start event not received.
			throw LogicException(LOC);
		}

		currentActionAutoListenersList.clear();

		if (startedActionType != rpg3D::ITurnBasedInteractor::ActionType::Movement) {
			throw LogicException(LOC);
		}

		turnBasedInteractor->currentActionFinished(startedActionType);

		onActiveActionInfoChanged();
	}

	else if (event.type == rpg3D::ITool::EVENT_tool_triggerPointOfNoReturn_start->type) {
		//if (event.initiatorIdentifier != LOCAL_ACTION_INITIATOR_IDENTIFIER) {
		//	// This is not the movement started from here.
		//	throw LogicException(LOC);
		//}

		if (startedActionType != rpg3D::ITurnBasedInteractor::ActionType::Ability) {
			throw LogicException(LOC);
		}

		if (isActionStartEventReceived) {
			// Start event already received.
			throw LogicException(LOC);
		}
		isActionStartEventReceived = true;
	} else if (event.type == rpg3D::ITool::EVENT_tool_extendedActivationFinished->type) {
		//if (event.initiatorIdentifier != LOCAL_ACTION_INITIATOR_IDENTIFIER) {
		//	// This is not the movement started from here.
		//	throw LogicException(LOC);
		//}
		if (!isActionStartEventReceived) {
			// Start event not received.
			throw LogicException(LOC);
		}

		currentActionAutoListenersList.clear();

		if (startedActionType != rpg3D::ITurnBasedInteractor::ActionType::Ability) {
			throw LogicException(LOC);
		}

		turnBasedInteractor->currentActionFinished(startedActionType);

		onActiveActionInfoChanged();
	}

	else {
		throw LogicException(LOC);
	}
}

void LB_Mob_Chicken_TB_A_V1::onCurrentActionChanged() {
	// Call before super call because it is used in the super functions.
	startedActionType = rpg3D::ITurnBasedInteractor::ActionType::None;

	super::onCurrentActionChanged();
}

LB_Mob_Chicken_TB_A_V1::~LB_Mob_Chicken_TB_A_V1() {
	//void
}
