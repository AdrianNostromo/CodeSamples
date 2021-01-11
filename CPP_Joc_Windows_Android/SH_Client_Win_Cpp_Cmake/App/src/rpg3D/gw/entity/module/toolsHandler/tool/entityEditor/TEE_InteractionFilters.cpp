#include "TEE_InteractionFilters.h"
#include <worldGame3D/gw/entity/template/actions/ActionsTemplate.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/entity/module/actions/IActionsModule.h>
#include <rpg3D/gw/entity/template/userControlled/UserControlledTemplate.h>
#include <rpg3D/gw/entity/module/userControlled/IUserControlledModule.h>
#include <base/math/Math.h>
#include <worldGame3D/gw/entity/util/IEntityHandlerBase.h>
#include <worldGame3D/gw/entity/WorldEntity.h>
#include <rpg3D/gw/entity/module/TeamModule.h>
#include <base/visual3d/Visual3DUtil.h>
#include <rpg3D/gw/entity/module/projectile/ProjectileModule.h>
#include <base/log/GlobalAccessLogHandler.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <base/app/IAppMetrics.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/entityEditor/painter/config/ToolConfigEEPainter.h>
#include <worldGame3D/gw/IGameWorld.h>

using namespace rpg3D;

int TEE_InteractionFilters::ActionContinuousActivate::TYPE = super::NewTypeIndex();
int TEE_InteractionFilters::ActionContinuousUpdateDrag::TYPE = super::NewTypeIndex();
int TEE_InteractionFilters::ActionContinuousDeactivate::TYPE = super::NewTypeIndex();

const int TEE_InteractionFilters::CachedActionInfo::TYPE_ActivateContinuous = 1;
const int TEE_InteractionFilters::CachedActionInfo::TYPE_UpdateDrag = 2;
const int TEE_InteractionFilters::CachedActionInfo::TYPE_DeactivateContinuous = 3;

TEE_InteractionFilters::TEE_InteractionFilters(ToolConfigEntityEditor* config, std::shared_ptr<ExtraData> extraData,
	bool usesAutoToolModeActivationSound)
	: super(config, extraData,
		usesAutoToolModeActivationSound)
{
	activationFiltersList_innactiveForActivation->append_emplace((int)IWorldOrEntityInteractionFilter::Type::LockedCursor_Down_Main);
	activationFiltersList_innactiveForActivation->append_emplace((int)IWorldOrEntityInteractionFilter::Type::ScreenCursor_Down_Main);

	activationFiltersList_activeContinuousForUpdate->append_emplace((int)IWorldOrEntityInteractionFilter::Type::LockedCursor_Move);
	activationFiltersList_activeContinuousForUpdate->append_emplace((int)IWorldOrEntityInteractionFilter::Type::ScreenCursor_Drag_Main);

	activationFiltersList_activeContinuousForDeactivation->append_emplace((int)IWorldOrEntityInteractionFilter::Type::LockedCursor_Up_Main);
	activationFiltersList_activeContinuousForDeactivation->append_emplace((int)IWorldOrEntityInteractionFilter::Type::ScreenCursor_Up_Main);
}

void TEE_InteractionFilters::addAction_ActionContinuousActivate(int cursorIndex, Vector2& screenPos) {
	actions->addAction(std::make_shared<ActionContinuousActivate>(
		entity->getLocalEntityId(), entity->getSyncedEntityId(),
		worldGame3D::EntityAction::Priorities::Normal,
		cursorIndex,
		screenPos
		));
}

void TEE_InteractionFilters::addAction_ActionContinuousUpdateDrag(int cursorIndex, Vector2& screenCursorPos, Vector2& sensitisedScreenCursorMoveDelta) {
	actions->addAction(std::make_shared<ActionContinuousUpdateDrag>(
		entity->getLocalEntityId(), entity->getSyncedEntityId(),
		worldGame3D::EntityAction::Priorities::Normal,
		cursorIndex,
		screenCursorPos, sensitisedScreenCursorMoveDelta
		));
}

void TEE_InteractionFilters::addAction_ActionContinuousDeactivate(int cursorIndex) {
	actions->addAction(std::make_shared<ActionContinuousDeactivate>(
		entity->getLocalEntityId(), entity->getSyncedEntityId(),
		worldGame3D::EntityAction::Priorities::Normal,
		cursorIndex
		));
}

void TEE_InteractionFilters::onAction(IEventListener& eventListener, worldGame3D::IAction& action) {
	if (action.getType() == ActionContinuousActivate::TYPE) {
		onEntityAction_ActionContinuousActivate((ActionContinuousActivate*)action.getRawSubclassPointer(ActionContinuousActivate::TYPE));
	} else if (action.getType() == ActionContinuousUpdateDrag::TYPE) {
		onEntityAction_ActionContinuousUpdateDrag((ActionContinuousUpdateDrag*)action.getRawSubclassPointer(ActionContinuousUpdateDrag::TYPE));
	} else if (action.getType() == ActionContinuousDeactivate::TYPE) {
		onEntityAction_ActionContinuousDeactivate((ActionContinuousDeactivate*)action.getRawSubclassPointer(ActionContinuousDeactivate::TYPE));
	} else {
		throw LogicException(LOC);
	}
}

void TEE_InteractionFilters::onEntityAction_ActionContinuousActivate(ActionContinuousActivate* action) {
	if (action->getIsReplaced() || action->getIsFinished() || !action->getIsPreRequirementsMet()) {
		// This is the last time this action is called.
		// Do action finish,cleanup logic (eg. remove active status effects from the list).

		//void
	}
	if (action->getIsReplaced() || action->getIsOverridden() || !action->getIsPreRequirementsMet()) {
		// Action replaced. This is the last time this action is called.
		// Or action is overridden. Should not do any active logic now excet for the above one in certain cases (eg. to remove active status effects from the list).

		return;
	}

	// Need to use a extra list and process in gameLoop_simulationC to make sure the turn is processed before this.
	cachedActionInfosList.append_emplace(CachedActionInfo::TYPE_ActivateContinuous, action->cursorIndex, action->screenPos);
}

void TEE_InteractionFilters::onEntityAction_ActionContinuousUpdateDrag(ActionContinuousUpdateDrag* action) {
	if (action->getIsReplaced() || action->getIsFinished() || !action->getIsPreRequirementsMet()) {
		// This is the last time this action is called.
		// Do action finish,cleanup logic (eg. remove active status effects from the list).

		//void
	}
	if (action->getIsReplaced() || action->getIsOverridden() || !action->getIsPreRequirementsMet()) {
		// Action replaced. This is the last time this action is called.
		// Or action is overridden. Should not do any active logic now excet for the above one in certain cases (eg. to remove active status effects from the list).

		return;
	}

	// Need to use a extra list and process in gameLoop_simulationC to make sure the turn is processed before this.
	cachedActionInfosList.append_emplace(CachedActionInfo::TYPE_UpdateDrag, action->cursorIndex, action->screenPos, action->sensitisedScreenCursorMoveDelta);
}

void TEE_InteractionFilters::onEntityAction_ActionContinuousDeactivate(ActionContinuousDeactivate* action) {
	if (action->getIsReplaced() || action->getIsFinished() || !action->getIsPreRequirementsMet()) {
		// This is the last time this action is called.
		// Do action finish,cleanup logic (eg. remove active status effects from the list).

		//void
	}
	if (action->getIsReplaced() || action->getIsOverridden() || !action->getIsPreRequirementsMet()) {
		// Action replaced. This is the last time this action is called.
		// Or action is overridden. Should not do any active logic now excet for the above one in certain cases (eg. to remove active status effects from the list).

		return;
	}

	// Need to use a extra list and process in gameLoop_simulationC to make sure the turn is processed before this.
	cachedActionInfosList.append_emplace(CachedActionInfo::TYPE_DeactivateContinuous, action->cursorIndex);
}

void TEE_InteractionFilters::onGameLoop_simulationC(float deltaS, int deltaMS, float gameTimeS) {
	super::onGameLoop_simulationC(deltaS, deltaMS, gameTimeS);

	if (cachedActionInfosList.size() > 0) {
		while (cachedActionInfosList.size() > 0) {
			CachedActionInfo* actionInfo = cachedActionInfosList.getPointer(0);

			if (actionInfo->type == CachedActionInfo::TYPE_ActivateContinuous) {
				if (!getIsOperatingAny() && canTrigger()) {
					setIsOperating(true, actionInfo->cursorIndex);

					chargeCooldown();

					toolFilterStart(actionInfo->screenPos);
				}
			} else if (actionInfo->type == CachedActionInfo::TYPE_UpdateDrag) {
				if (getIsOperating(actionInfo->cursorIndex)) {
					toolFilterUpdate(actionInfo->screenPos, actionInfo->sensitisedScreenCursorMoveDelta);
				}
			} else if (actionInfo->type == CachedActionInfo::TYPE_DeactivateContinuous) {
				if (getIsOperating(actionInfo->cursorIndex)) {
					toolFilterEnd(actionInfo->cursorIndex);
				}
			} else {
				throw LogicException(LOC);
			}

			cachedActionInfosList.remove(0);
		}
	}
}

bool TEE_InteractionFilters::onFilterInteraction(int filterSource, std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>& interaction) {
	bool isProcessed = super::onFilterInteraction(filterSource, interaction);

	// Also check locally to detect error where multiple layers use the same filters (The filters lists are not set up for that and it should never occur).
	bool localIsProcessed = true;
	if (filterSource == FilterSource::innactiveForActivation) {
		if (interaction->type == rpg3D::WorldOrEntityInteractionFilter::Type::LockedCursor_Down_Main) {
			addAction_ActionContinuousActivate(interaction->cursorIndex, interaction->screenCursorPos);
		} else if (interaction->type == rpg3D::WorldOrEntityInteractionFilter::Type::ScreenCursor_Down_Main) {
			addAction_ActionContinuousActivate(interaction->cursorIndex, interaction->screenCursorPos);
		} else {
			localIsProcessed = false;
		}
	} else if (filterSource == FilterSource::activeContinuousForUpdate) {
		if (interaction->type == rpg3D::WorldOrEntityInteractionFilter::Type::LockedCursor_Move) {
			addAction_ActionContinuousUpdateDrag(interaction->cursorIndex, interaction->screenCursorPos, interaction->lockedCursorSensitisedDelta);
		} else if (interaction->type == rpg3D::WorldOrEntityInteractionFilter::Type::ScreenCursor_Drag_Main) {
			addAction_ActionContinuousUpdateDrag(interaction->cursorIndex, interaction->screenCursorPos, interaction->screenCursorMoveDelta_sensitised);
		} else {
			localIsProcessed = false;
		}
	} else if (filterSource == FilterSource::activeContinuousForDeactivation) {
		if (interaction->type == rpg3D::WorldOrEntityInteractionFilter::Type::LockedCursor_Up_Main) {
			addAction_ActionContinuousDeactivate(interaction->cursorIndex);
		} else if (interaction->type == rpg3D::WorldOrEntityInteractionFilter::Type::ScreenCursor_Up_Main) {
			addAction_ActionContinuousDeactivate(interaction->cursorIndex);
		} else {
			localIsProcessed = false;
		}
	} else {
		localIsProcessed = false;
	}

	if (isProcessed && localIsProcessed) {
		// Filter used in multiple layers. That should never occur. Filters code is not made for that.
		throw LogicException(LOC);
	}

	return isProcessed || localIsProcessed;
}

void TEE_InteractionFilters::onEntityChanged() {
	super::onEntityChanged();

	if (entity == nullptr) {
		actions = nullptr;
	} else {
		actions = entity->getComponentAs<worldGame3D::IActionsModule*>(true/*mustExist*/);
	}
}

void TEE_InteractionFilters::onSelectedAndGWCapabilitiesEnabledStateChanged(bool isSelectedAndGWCapabilitiesExist, worldGame3D::IGameWorldGameParts* gwGamePartsOptional) {
	super::onSelectedAndGWCapabilitiesEnabledStateChanged(isSelectedAndGWCapabilitiesExist, gwGamePartsOptional);

	if (!isSelectedAndGWCapabilitiesExist) {
		// Remove the event listeners.
		autoListenersList.clear();
	} else {
		actions->setActionProcessor(
			ActionContinuousActivate::TYPE, ActionContinuousUpdateDrag::TYPE, ActionContinuousDeactivate::TYPE,
			std::bind(&TEE_InteractionFilters::onAction, this, std::placeholders::_1, std::placeholders::_2),
			autoListenersList
		);
	}
}

TEE_InteractionFilters::~TEE_InteractionFilters() {
	//void
}
