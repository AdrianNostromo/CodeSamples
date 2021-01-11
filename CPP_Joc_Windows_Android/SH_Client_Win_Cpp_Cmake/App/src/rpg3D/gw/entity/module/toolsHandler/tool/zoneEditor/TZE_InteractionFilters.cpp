#include "TZE_InteractionFilters.h"
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
#include "util/ToolActivationInfo_Base.h"

using namespace rpg3D;

int TZE_InteractionFilters::ActionContinuousActivate::TYPE = super::NewTypeIndex();
int TZE_InteractionFilters::ActionContinuousUpdateDrag::TYPE = super::NewTypeIndex();
int TZE_InteractionFilters::ActionContinuousDeactivate::TYPE = super::NewTypeIndex();

const int TZE_InteractionFilters::CachedActionInfo::TYPE_ActivateContinuous = 1;
const int TZE_InteractionFilters::CachedActionInfo::TYPE_UpdateDrag = 2;
const int TZE_InteractionFilters::CachedActionInfo::TYPE_DeactivateContinuous = 3;

TZE_InteractionFilters::TZE_InteractionFilters(ToolConfigZoneEditor* config, std::shared_ptr<ExtraData> extraData,
	bool usesAutoToolModeActivationSound)
	: super(config, extraData,
		usesAutoToolModeActivationSound)
{
	//void
}

void TZE_InteractionFilters::addAction_ActionContinuousActivate(sp<ToolActivationInfo_Base> touchData) {
	actions->addAction(std::make_shared<ActionContinuousActivate>(
		entity->getLocalEntityId(), entity->getSyncedEntityId(),
		worldGame3D::EntityAction::Priorities::Normal,
		touchData
	));
}

void TZE_InteractionFilters::addAction_ActionContinuousUpdateDrag(sp<ToolActivationInfo_Base> touchData) {
	actions->addAction(std::make_shared<ActionContinuousUpdateDrag>(
		entity->getLocalEntityId(), entity->getSyncedEntityId(),
		worldGame3D::EntityAction::Priorities::Normal,
		touchData
	));
}

void TZE_InteractionFilters::addAction_ActionContinuousDeactivate(sp<ToolActivationInfo_Base> touchData) {
	actions->addAction(std::make_shared<ActionContinuousDeactivate>(
		entity->getLocalEntityId(), entity->getSyncedEntityId(),
		worldGame3D::EntityAction::Priorities::Normal,
		touchData
	));
}

void TZE_InteractionFilters::onAction(IEventListener& eventListener, worldGame3D::IAction& action) {
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

void TZE_InteractionFilters::onEntityAction_ActionContinuousActivate(ActionContinuousActivate* action) {
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
	cachedActionInfosList.append_emplace(CachedActionInfo::TYPE_ActivateContinuous, action->data);
}

void TZE_InteractionFilters::onEntityAction_ActionContinuousUpdateDrag(ActionContinuousUpdateDrag* action) {
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
	cachedActionInfosList.append_emplace(CachedActionInfo::TYPE_UpdateDrag, action->data);
}

void TZE_InteractionFilters::onEntityAction_ActionContinuousDeactivate(ActionContinuousDeactivate* action) {
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
	cachedActionInfosList.append_emplace(CachedActionInfo::TYPE_DeactivateContinuous, action->data);
}

void TZE_InteractionFilters::onGameLoop_simulationC(float deltaS, int deltaMS, float gameTimeS) {
	super::onGameLoop_simulationC(deltaS, deltaMS, gameTimeS);

	if (cachedActionInfosList.size() > 0) {
		while (cachedActionInfosList.size() > 0) {
			CachedActionInfo* actionInfo = cachedActionInfosList.getPointer(0);

			if (actionInfo->type == CachedActionInfo::TYPE_ActivateContinuous) {
				if (!getIsOperatingAny() && canTrigger()) {
					setIsOperating(true, actionInfo->data->cursorIndex);

					chargeCooldown();

					toolFilterStart(actionInfo->data);
				}
			} else if (actionInfo->type == CachedActionInfo::TYPE_UpdateDrag) {
				if (getIsOperating(actionInfo->data->cursorIndex)) {
					toolFilterUpdate(actionInfo->data);
				}
			} else if (actionInfo->type == CachedActionInfo::TYPE_DeactivateContinuous) {
				if (getIsOperating(actionInfo->data->cursorIndex)) {
					toolFilterEnd(actionInfo->data);
				}
			} else {
				throw LogicException(LOC);
			}

			cachedActionInfosList.remove(0);
		}
	}
}

void TZE_InteractionFilters::onFilterInteraction_innactiveForActivation(sp<ToolActivationInfo_Base> touchData) {
	addAction_ActionContinuousActivate(touchData);
}

void TZE_InteractionFilters::onFilterInteraction_activeContinuousForUpdate(sp<ToolActivationInfo_Base> touchData) {
	addAction_ActionContinuousUpdateDrag(touchData);
}

void TZE_InteractionFilters::onFilterInteraction_activeContinuousForDeactivation(sp<ToolActivationInfo_Base> touchData) {
	addAction_ActionContinuousDeactivate(touchData);
}

void TZE_InteractionFilters::onEntityChanged() {
	super::onEntityChanged();

	if (entity == nullptr) {
		actions = nullptr;
	} else {
		actions = entity->getComponentAs<worldGame3D::IActionsModule*>(true/*mustExist*/);
	}
}

void TZE_InteractionFilters::onSelectedAndGWCapabilitiesEnabledStateChanged(bool isSelectedAndGWCapabilitiesExist, worldGame3D::IGameWorldGameParts* gwGamePartsOptional) {
	super::onSelectedAndGWCapabilitiesEnabledStateChanged(isSelectedAndGWCapabilitiesExist, gwGamePartsOptional);

	if (!isSelectedAndGWCapabilitiesExist) {
		// Remove the event listeners.
		autoListenersList.clear();
	} else {
		actions->setActionProcessor(
			ActionContinuousActivate::TYPE, ActionContinuousUpdateDrag::TYPE, ActionContinuousDeactivate::TYPE,
			std::bind(&TZE_InteractionFilters::onAction, this, std::placeholders::_1, std::placeholders::_2),
			autoListenersList
		);
	}
}

TZE_InteractionFilters::~TZE_InteractionFilters() {
	//void
}
