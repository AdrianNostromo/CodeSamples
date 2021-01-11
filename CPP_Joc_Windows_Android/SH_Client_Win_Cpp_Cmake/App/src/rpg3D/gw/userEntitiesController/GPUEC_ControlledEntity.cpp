#include <rpg3D/gw/entity/module/userControlled/IUserControlledModule.h>
#include <rpg3D/gw/entity/template/userControlled/UserControlledTemplate.h>
#include "GPUEC_ControlledEntity.h"
#include <worldGame3D/gw/IGameWorld.h>
#include <worldGame3D/gw/interaction/IGPInteraction.h>
#include <rpg3D/gw/interaction/deviceinput/IManagedDeviceInput.h>
#include <rpg3D/gw/entity/module/movingEntity/MovingEntityModule.h>
#include <rpg3D/gw/entity/template/ToolsHandlerTemplate.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/playerUnit/moveDPadLocalFrontBack/IToolMDPadLocalFrontBack.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/playerUnit/moveGlobalDestination_TMoveDirection/IToolMGlobalDestination_TMoveDirection.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/playerUnit/moveTurnGridTurnBased/IToolMTGridTurnBased.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/playerUnit/moveGlobalDirection_TMoveDirection/IToolMGlobalDirection_TMoveDirection.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/playerUnit/moveGlobalDirection/IToolMGlobalDirection.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/playerUnit/moveLocalStrafe/IToolMLocalStrafe.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/playerUnit/turnDPASSY_LMOSP/IToolTDPASSY_LMOSP.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/playerUnit/turnGlobalFocusDirectionYaw/IToolTGlobalFocusDirectionYaw.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/playerUnit/turnMouseOrSensorPitchYaw/IToolTMouseOrSensorPitchYaw.h>
#include <rpg3D/gw/entity/module/toolsHandler/IToolsHandlerModule.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/groups/IToolMovement.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/groups/IToolTurn.h>
#include <base/event/listener/EventListener.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>

using namespace rpg3D;

base::GWEvent* GPUEC_ControlledEntity::GWEvent_onEntityControlTypeChangePre = new base::GWEvent("GWEvent_onEntityControlTypeChangePre");
base::GWEvent* GPUEC_ControlledEntity::GWEvent_onEntityControlTypeChanged = new base::GWEvent("GWEvent_onEntityControlTypeChanged");

GPUEC_ControlledEntity::GPUEC_ControlledEntity(base::IGameWorld* gw)
    : super(gw)
{
    //void
}

bool GPUEC_ControlledEntity::getIsWorldInteractorEnabled() {
	return isWorldInteractorEnabledCounter >= 1 ? true : false;
}

void GPUEC_ControlledEntity::setIsWorldInteractorEnabledIncremental(bool isWorldInteractorEnabledIncremental) {
	this->isWorldInteractorEnabledCounter += isWorldInteractorEnabledIncremental == true ? 1 : -1;

	if (this->isWorldInteractorEnabledCounter > 1) {
		throw LogicException(LOC);
	}

	if ((isWorldInteractorEnabledIncremental && this->isWorldInteractorEnabledCounter == 1) || 
		(!isWorldInteractorEnabledIncremental && this->isWorldInteractorEnabledCounter == 0))
	{
		onIsWorldInteractorEnabledChanged();
	}
}

void GPUEC_ControlledEntity::onIsWorldInteractorEnabledChanged() {
	if (!getIsWorldInteractorEnabled()) {
		deactivateActiveEntityControlType();
	}else {
		activateActiveEntityControlType();
	}
}

void GPUEC_ControlledEntity::createMain() {
	super::createMain();

	if(controledEntityTarget != nullptr && controledEntityTarget->entity != nullptr) {
		activateActiveEntityControlType();
	}
}

int GPUEC_ControlledEntity::dispatchInteraction_NotFiltered(
	std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent> entityInteraction,
	base::EventDispatcher<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>>* dispatcher,
	int validationType)
{
	if(entityInteraction->type != validationType) {
		throw LogicException(LOC);
	}

	int ct = 0;

	ListDL<EventListener<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>>*>* cbList = dispatcher->peekCbList();
	cbList->nextEnumeratingEntry = cbList->getFirst();
	cbList->isEnumeratingInProgress = true;

	typename ListDL<EventListener<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>>*>::Entry* listEntry;
	while((listEntry = cbList->nextEnumeratingEntry) != nullptr) {
		cbList->nextEnumeratingEntry = cbList->nextEnumeratingEntry->next;

		IWorldOrEntityInteractionFilter* interactionFilter = static_cast<IWorldOrEntityInteractionFilter*>(listEntry->extraData);
		if (interactionFilter == nullptr) {
			throw LogicException(LOC);
		}

		entityInteraction->filter = interactionFilter;

		EventListener<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>>* eventListener = listEntry->data;
		eventListener->cb(*eventListener, entityInteraction);
		ct++;
	}

	cbList->isEnumeratingInProgress = false;

	return ct;
}

int GPUEC_ControlledEntity::dispatchInteraction_Filtered_Entity(
	IWorldEntity* selfEntity,
	std::shared_ptr<ArrayList<base::IGamePartMain::DT_Entity_Distance>> filterableEntitiesList,
	int interactionType, int triggerEventIndex, int cursorIndex,
	base::EventDispatcher<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>>* dispatcher)
{
	int ct = 0;

	ListDL<EventListener<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>>*>* cbList = dispatcher->peekCbList();
	cbList->nextEnumeratingEntry = cbList->getFirst();
	cbList->isEnumeratingInProgress = true;

	typename ListDL<EventListener<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>>*>::Entry* listEntry;
	while((listEntry = cbList->nextEnumeratingEntry) != nullptr) {
		cbList->nextEnumeratingEntry = cbList->nextEnumeratingEntry->next;

		IWorldOrEntityInteractionFilter* interactionFilter = static_cast<IWorldOrEntityInteractionFilter*>(listEntry->extraData);
		if (interactionFilter == nullptr) {
			throw LogicException(LOC);
		}

		for (int i = 0; i< filterableEntitiesList->count(); i++) {
			base::IGamePartMain::DT_Entity_Distance *intersectionEntry = filterableEntitiesList->getPointer(i);

			if (interactionFilter->testEntity(selfEntity, intersectionEntry->entity)) {
				// Dispatch for the closest intersected entity that passes the filter test.

				std::shared_ptr<WorldOrEntityInteractionEvent> entityInteraction;
				if (interactionType == IWorldOrEntityInteractionFilter::Type::EntityDown) {
					entityInteraction = rpg3D::WorldOrEntityInteractionEvent::getNew_EntityDown(
						triggerEventIndex, cursorIndex,
						intersectionEntry->entity
					);
				}else if (interactionType == IWorldOrEntityInteractionFilter::Type::EntityUp) {
					entityInteraction = rpg3D::WorldOrEntityInteractionEvent::getNew_EntityUp(
						triggerEventIndex, cursorIndex,
						intersectionEntry->entity
					);
				}else {
					throw LogicException(LOC);
				}

				entityInteraction->filter = interactionFilter;

				EventListener<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>>* eventListener = listEntry->data;
				eventListener->cb(*eventListener, entityInteraction);
				ct++;

				break;
			}
		}
	}

	cbList->isEnumeratingInProgress = false;

	return ct;
}

void GPUEC_ControlledEntity::dispatchInteraction_Filtered_WorldPos(
	IWorldEntity* selfEntity,
	std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent> entityInteraction,
	base::EventDispatcher<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>>* dispatcher,
	int validationType)
{
	if (entityInteraction->type != validationType) {
		throw LogicException(LOC);
	}

	ListDL<EventListener<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>>*>* cbList = dispatcher->peekCbList();
	cbList->nextEnumeratingEntry = cbList->getFirst();
	cbList->isEnumeratingInProgress = true;

	typename ListDL<EventListener<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>>*>::Entry* listEntry;
	while ((listEntry = cbList->nextEnumeratingEntry) != nullptr) {
		cbList->nextEnumeratingEntry = cbList->nextEnumeratingEntry->next;

		IWorldOrEntityInteractionFilter* interactionFilter = static_cast<IWorldOrEntityInteractionFilter*>(listEntry->extraData);
		if (interactionFilter == nullptr) {
			throw LogicException(LOC);
		}

		if (interactionFilter->testWorldPos(selfEntity, entityInteraction->worldPos)) {
			entityInteraction->filter = interactionFilter;

			EventListener<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>>* eventListener = listEntry->data;
			eventListener->cb(*eventListener, entityInteraction);
		}
	}

	cbList->isEnumeratingInProgress = false;
}

int GPUEC_ControlledEntity::getEntityControlType() {
    return entityControlType;
}

void GPUEC_ControlledEntity::setEntityControlType(int entityControlType) {
    if(this->entityControlType == entityControlType) {
        return ;
    }

	onEntityControlTypeChangePre();
	gw->dispatchEvent(*GWEvent_onEntityControlTypeChangePre);

    this->entityControlType = entityControlType;

	onEntityControlTypeChanged();
	gw->dispatchEvent(*GWEvent_onEntityControlTypeChanged);
}

base::TypedEventDispatcher1D<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>>& GPUEC_ControlledEntity::getGlobalInteractionsDispatcher() {
	return globalInteractionsDispatcher;
}

void GPUEC_ControlledEntity::setControlledEntity(IWorldEntity* controlledEntity) {
	if (isControlledEntityChangeLocked) {
		// This occurs if this function is called from a input filter (the current code doesn't allow this).
		// asd_x; Maybe allow this to occur in the future (or not?) when the logic is changed but it is fine for now.
		throw LogicException(LOC);
	}

	onControlledEntityChange_pre();
	
	if (this->controledEntityTarget != nullptr) {
		this->controledEntityTarget->listEntry->remove();

		controledEntityTarget = nullptr;
	}

	if (controlledEntity != nullptr) {
		ListDL<ControledTarget>::Entry* listEntry = controledTargetsList.appendEmplace(controlledEntity);
		ControledTarget& controledTarget = listEntry->data;
		this->controledEntityTarget = &controledTarget;
		controledTarget.listEntry = listEntry;

		controledTarget.userControlledModule = controledEntityTarget->entity->getComponentAs<IUserControlledModule*>(true/*mustExist*/);
		controledTarget.movingEntityModule = controledEntityTarget->entity->getComponentAs<IMovingEntityModule*>(true/*mustExist*/);
		controledTarget.toolsHandler = controledEntityTarget->entity->getComponentAs<rpg3D::IToolsHandlerModule*>(true/*mustExist*/);

		activateActiveEntityControlType();
	}

    onControlledEntityChanged();
}

void GPUEC_ControlledEntity::onGameLoop_localInput(float deltaS, int deltaMS, float gameTimeS) {
	//void
}

void GPUEC_ControlledEntity::onControlledEntityChange_pre() {
	//void
}

void GPUEC_ControlledEntity::onControlledEntityChanged() {
	//void
}

void GPUEC_ControlledEntity::onEntityControlTypeChangePre() {
	deactivateActiveEntityControlType();
}

void GPUEC_ControlledEntity::onEntityControlTypeChanged() {
	activateActiveEntityControlType();
}

void GPUEC_ControlledEntity::activateActiveEntityControlType() {
	if(controledEntityTarget != nullptr && controledEntityTarget->entity != nullptr && getIsWorldInteractorEnabled()) {
		if(getEntityControlType() == EntityControlType::None) {
			controledEntityTarget->toolsHandler->selectSingleToolInGroup(rpg3D::IToolMovement::GroupID, -1/*toolId*/, -1/*toolSlotIndex*/);
			controledEntityTarget->toolsHandler->selectSingleToolInGroup(rpg3D::IToolTurn::GroupID, -1/*toolId*/, -1/*toolSlotIndex*/);
		}else if(getEntityControlType() == EntityControlType::IndirectMoveToTarget_AutoRotToMoveDirection) {
			controledEntityTarget->toolsHandler->selectSingleToolInGroup(rpg3D::IToolMovement::GroupID, rpg3D::playerUnit::IToolMGlobalDestination_TMoveDirection::ID, -1/*toolSlotIndex*/);
			controledEntityTarget->toolsHandler->selectSingleToolInGroup(rpg3D::IToolTurn::GroupID, -1/*toolId*/, -1/*toolSlotIndex*/);
		}else if(getEntityControlType() == EntityControlType::GridTurnBased) {
			controledEntityTarget->toolsHandler->selectSingleToolInGroup(rpg3D::IToolMovement::GroupID, rpg3D::playerUnit::IToolMTGridTurnBased::ID, -1/*toolSlotIndex*/);
			controledEntityTarget->toolsHandler->selectSingleToolInGroup(rpg3D::IToolTurn::GroupID, -1/*toolId*/, -1/*toolSlotIndex*/);
		}else if(getEntityControlType() == EntityControlType::DPadCameraAxesDirectionMove_AutoRotToMoveDirection) {
			controledEntityTarget->toolsHandler->selectSingleToolInGroup(rpg3D::IToolMovement::GroupID, rpg3D::playerUnit::IToolMGlobalDirection_TMoveDirection::ID, -1/*toolSlotIndex*/);
			controledEntityTarget->toolsHandler->selectSingleToolInGroup(rpg3D::IToolTurn::GroupID, -1/*toolId*/, -1/*toolSlotIndex*/);
		}else if(getEntityControlType() == EntityControlType::DPadUnitAxesStrafe_CursorRotationBothAxes) {
			IManagedDeviceInput* input = gw->getInteractionAs<base::IGPInteraction*>()->getInputAs<IManagedDeviceInput*>();
			if(input->getIsCursorBlockedModeSupported()) {
				input->setIsCursorBlockedMode(true);
				localCache_isCursorBlockedMode = true;

				controledEntityTarget->toolsHandler->selectSingleToolInGroup(rpg3D::IToolMovement::GroupID, rpg3D::playerUnit::IToolMLocalStrafe::ID, -1/*toolSlotIndex*/);
				controledEntityTarget->toolsHandler->selectSingleToolInGroup(rpg3D::IToolTurn::GroupID, rpg3D::playerUnit::IToolTMouseOrSensorPitchYaw::ID, -1/*toolSlotIndex*/);
			}else if(input->getIsSensorSupported(base::Touch::SensorType::RotationsPitchRoll_Oriented)) {
				input->setIsSensorEnabled(base::Touch::SensorType::RotationsPitchRoll_Oriented, true);
				
				controledEntityTarget->toolsHandler->selectSingleToolInGroup(rpg3D::IToolMovement::GroupID, rpg3D::playerUnit::IToolMDPadLocalFrontBack::ID, -1/*toolSlotIndex*/);
				controledEntityTarget->toolsHandler->selectSingleToolInGroup(rpg3D::IToolTurn::GroupID, rpg3D::playerUnit::IToolTDPASSY_LMOSP::ID, -1/*toolSlotIndex*/);
			}
		}else if(getEntityControlType() == EntityControlType::DPadCameraAxesDirectionMove_LookAtCursorOrAutoRotToMoveDirection) {
			//asdA29;// On android this will not turn the entity.
			controledEntityTarget->toolsHandler->selectSingleToolInGroup(rpg3D::IToolMovement::GroupID, rpg3D::playerUnit::IToolMGlobalDirection::ID, -1/*toolSlotIndex*/);
			controledEntityTarget->toolsHandler->selectSingleToolInGroup(rpg3D::IToolTurn::GroupID, rpg3D::playerUnit::IToolTGlobalFocusDirectionYaw::ID, -1/*toolSlotIndex*/);
		}else {
			throw LogicException(LOC);
		}
	}
}

void GPUEC_ControlledEntity::deactivateActiveEntityControlType() {
	if (controledEntityTarget != nullptr && controledEntityTarget->entity != nullptr) {
		controledEntityTarget->toolsHandler->selectSingleToolInGroup(rpg3D::IToolMovement::GroupID, -1/*toolId*/, -1/*toolSlotIndex*/);
		controledEntityTarget->toolsHandler->selectSingleToolInGroup(rpg3D::IToolTurn::GroupID, -1/*toolId*/, -1/*toolSlotIndex*/);

		if(getEntityControlType() == EntityControlType::DPadUnitAxesStrafe_CursorRotationBothAxes) {
			IManagedDeviceInput* input = gw->getInteractionAs<base::IGPInteraction*>()->getInputAs<IManagedDeviceInput*>();
			if(input->getIsCursorBlockedModeSupported()) {
				input->setIsCursorBlockedMode(false);
				localCache_isCursorBlockedMode = false;
			}else if(input->getIsSensorSupported(base::Touch::SensorType::RotationsPitchRoll_Oriented)) {
				input->setIsSensorEnabled(base::Touch::SensorType::RotationsPitchRoll_Oriented, false);
			}
		}
	}
}

void GPUEC_ControlledEntity::clearInput() {
    //void
}

void GPUEC_ControlledEntity::disposeMain() {
    //void

	super::disposeMain();
}

GPUEC_ControlledEntity::~GPUEC_ControlledEntity() {
    //void
}
