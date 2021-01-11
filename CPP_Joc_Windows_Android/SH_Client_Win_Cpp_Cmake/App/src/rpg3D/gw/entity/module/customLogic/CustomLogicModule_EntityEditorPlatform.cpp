#include "CustomLogicModule_EntityEditorPlatform.h"
#include <worldGame3D/gw/entity/module/visual3D/Visual3DModule.h>
#include <rpg3D/gw/entity/module/platform/PlatformModule.h>
#include <graphics/visual3d/drawable/IDrawableNode3D.h>
#include <rpg3D/gw/config/entity/platform/EntityTemplates_Platform_CreatureCreatorPad.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <rpg3D/gw/ui/IGamePartUserInterface.h>
#include <rpg3D/gw/entity/module/stackable/IStackableModule.h>
#include <rpg3D/gw/entity/template/stackable/StackableTemplate.h>
#include <rpg3D/gw/config/entity/EntityTags.h>
#include <rpg3D/gw/view/entityEditor/IEntityEditorView.h>
#include <worldGame3D/gw/entity/template/visual3D/Visual3DTemplate.h>
#include <worldGame3D/gw/content/IGPContent.h>
#include <worldGame3D/gw/entity/event/IPluggableEventsDispatcher.h>

using namespace rpg3D;

CustomLogicModule_EntityEditorPlatform::CustomLogicModule_EntityEditorPlatform(
    IWorldEntity* entity,
	base::CustomLogicTemplate* t)
    : super(entity, t)
{
	entity->addEventListener(
		rpg3D::PlatformModule::EVENT_onPlatformContactAdded->type, rpg3D::PlatformModule::EVENT_onPlatformContactRemoved->type,
		std::bind(&CustomLogicModule_EntityEditorPlatform::onEvent, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);

	e->getGWEvents()->addEventListener(
		IGamePartUserInterface::GWEvent_onEntityEditor_activated->type, IGamePartUserInterface::GWEvent_onEntityEditor_deactivated->type,
		std::bind(&CustomLogicModule_EntityEditorPlatform::onGWEvent, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
}

void CustomLogicModule_EntityEditorPlatform::createBMain() {
	super::createBMain();

	platform = entity->getComponentAs<IPlatformModule*>(true/*mustExist*/);

	worldGame3D::IVisual3DModule* visual3D = entity->getComponentAs<worldGame3D::IVisual3DModule*>(true/*mustExist*/);

	v_base = visual3D->getDrawableMustExistAs<IDrawableNode3D*>(EntityTemplates_Platform_CreatureCreatorPad::V_ID_Base);
	v_base_disabled = visual3D->getDrawableMustExistAs<IDrawableNode3D*>(EntityTemplates_Platform_CreatureCreatorPad::V_ID_Base_Disabled);
	v_icon = visual3D->getDrawableMustExistAs<IDrawableNode3D*>(EntityTemplates_Platform_CreatureCreatorPad::V_ID_Icon);
}

bool CustomLogicModule_EntityEditorPlatform::getIsEditableEntitiesOnPlatform() {
	return editableEntitiesOnPlatformCount >= 1 ? true : false;
}

void CustomLogicModule_EntityEditorPlatform::setIsEditableEntitiesOnPlatformIncremental(bool isEditableEntitiesOnPlatform) {
	if (editableEntitiesOnPlatformCount == 0 && isEditableEntitiesOnPlatform == false) {
		// Counter will go below 0 into an error state.
		throw LogicException(LOC);
	}

	bool prev_isEditableEntitiesOnPlatform = getIsEditableEntitiesOnPlatform();

	editableEntitiesOnPlatformCount += isEditableEntitiesOnPlatform ? 1 : -1;

	if (prev_isEditableEntitiesOnPlatform != getIsEditableEntitiesOnPlatform()) {
		// State changed.
		syncVisualsState();
	}
}

IWorldEntity* CustomLogicModule_EntityEditorPlatform::getPlayerEntityOnPlatform() {
	return playerEntityOnPlatform;
}

void CustomLogicModule_EntityEditorPlatform::setPlayerEntityOnPlatform(IWorldEntity* playerEntityOnPlatform) {
	if (this->playerEntityOnPlatform != nullptr) {
		playerEntityOnPlatofrmEventsListener.clear();

		this->playerEntityOnPlatform = nullptr;
	}

	if (playerEntityOnPlatform != nullptr) {
		this->playerEntityOnPlatform = playerEntityOnPlatform;

		// Add listener for pickup.
		this->playerEntityOnPlatform->addEventListener(
			rpg3D::IStackableModule::EVENT_carryingChanged->type,
			std::bind(&CustomLogicModule_EntityEditorPlatform::playerEntityOnPlatform_onEvent, this, std::placeholders::_1, std::placeholders::_2),
			playerEntityOnPlatofrmEventsListener
		);
	}
}

bool CustomLogicModule_EntityEditorPlatform::getIsInvalidPlayerOnPlatform() {
	return isInvalidPlayerOnPlatform;
}

void CustomLogicModule_EntityEditorPlatform::setIsInvalidPlayerOnPlatform(bool isInvalidPlayerOnPlatform) {
	if (this->isInvalidPlayerOnPlatform == isInvalidPlayerOnPlatform) {
		// This must not be re-set with the same value. The reason is to make sure the setter logic doesn't contain bugs.
		throw LogicException(LOC);
	}

	this->isInvalidPlayerOnPlatform = isInvalidPlayerOnPlatform;

	syncVisualsState();
}

bool CustomLogicModule_EntityEditorPlatform::getIsEditorActive() {
	return isEditorActiveCounter >= 1 ? true : false;
}

void CustomLogicModule_EntityEditorPlatform::setIsEditorActiveIncremental(bool isEditorActive) {
	if (isEditorActiveCounter == 0 && isEditorActive == false) {
		// Counter will go below 0 into an error state.
		throw LogicException(LOC);
	}

	bool prev_isEditorActive = getIsEditorActive();

	isEditorActiveCounter += isEditorActive ? 1 : -1;

	if (prev_isEditorActive != getIsEditorActive()) {
		// State changed.
		syncVisualsState();
	}
}

void CustomLogicModule_EntityEditorPlatform::syncVisualsState() {
	if (getIsEditorActive() || (!getIsEditableEntitiesOnPlatform() && !getIsInvalidPlayerOnPlatform())) {
		// No_editable_creatures_on_pad or no_bad_player_on_pad or editor_active.

		v_base->setIsVisible(true);
		v_base_disabled->setIsVisible(false);
	} else {
		// Editable_creatures_on_pad and editor_innactive.

		v_base->setIsVisible(false);
		v_base_disabled->setIsVisible(true); 
	}

	if (!getIsEditorActive()) {
		v_icon->setIsVisible(true);
	} else {
		v_icon->setIsVisible(false);
	}
}

void CustomLogicModule_EntityEditorPlatform::connectToGameParts(worldGame3D::IGameWorldGameParts* gwGameParts) {
	super::connectToGameParts(gwGameParts);

	// Pool the current entity_editor active state.
	bool b = entity->getGW()->getComponentAs<IGamePartUserInterface*>(true/*mustExist*/)->getIsEntityEditorActive();
	isEditorActiveCounter = b ? 1 : 0;

	syncVisualsState();
}

void CustomLogicModule_EntityEditorPlatform::disconnectFromGamePartsPre() {
	//void

	super::disconnectFromGamePartsPre();
}

void CustomLogicModule_EntityEditorPlatform::onEvent(IEventListener& eventListener, base::EntityEvent& event) {
	IWorldEntity* contactedEntity = event.getEntityDataOther();
	if (contactedEntity == nullptr) {
		throw LogicException(LOC);
	}

	if (event.type == rpg3D::PlatformModule::EVENT_onPlatformContactAdded->type) {
		if (entity->getGW()->getGPContent()->getSelectedPlayerEntity(true/*mustExist*/) == contactedEntity) {
			setPlayerEntityOnPlatform(contactedEntity);

			if (getIsEditableEntitiesOnPlatform()) {
				// The pad is occupied and not activatable.
				// This will register the player as a bad pad activation attempt. The player unit will have to exit and re-enter the pad to activate it.
				setIsInvalidPlayerOnPlatform(true);
				return;
			}

			int editorType = -1;
			IWorldEntity* entityToEdit = nullptr;
			IWorldEntity* entityToEditB = nullptr;
			
			// Check if the player is hauling only_1 editable creature.
			rpg3D::IStackableModule* contactedEntity_stackable = contactedEntity->getComponentAs< rpg3D::IStackableModule*>(true/*mustExist*/);
			rpg3D::IStackableModule* contactedEntityStackTop_stackable = contactedEntity_stackable->getTopStackable();

			if (contactedEntity_stackable == contactedEntityStackTop_stackable) {
				// Not hauling anything, creature creation required.
				editorType = IEntityEditorView::EditorType::EDIT_OTHER;
			} else if (contactedEntityStackTop_stackable != nullptr && contactedEntityStackTop_stackable == contactedEntity_stackable->getCarrying()) {
				// Carrying only 1 stackable.
				// Check if the stackable is a editable creature.
				IWorldEntity* contactedEntityStackTop = contactedEntityStackTop_stackable->getEntity();

				if (contactedEntityStackTop->getTemplate()->hasTag(EntityTags::EDITABLE_CREATURE)) {
					entityToEdit = contactedEntityStackTop;
					editorType = IEntityEditorView::EditorType::EDIT_OTHER;
				}else if (contactedEntityStackTop->getTemplate()->hasTag(EntityTags::EDITABLE_PLAYR_UNIT_DUMMY)) {
					entityToEdit = contactedEntityStackTop;
					editorType = IEntityEditorView::EditorType::PAINT_OTHER;
					entityToEditB = entity->getGW()->getGPContent()->getSelectedPlayerEntity(true/*mustExist*/);
				} else {
					// Hauled entity is not an editable creature. Don't edit.

					setIsInvalidPlayerOnPlatform(true);
					return;
				}
			} else {
				// Hauling more than 1 creature. Don't edit.

				setIsInvalidPlayerOnPlatform(true);
				return;
			}

			if (editorType != -1) {
				entity->getGW()->getComponentAs<IGamePartUserInterface*>(true/*mustExist*/)->startEntityEditor(
					editorType, true/*isEditorStructureEnabled*/,
					entity->getPos(), entityToEdit, entityToEditB
				);
			}
		} else {
			// Any entity on the pad will block edit logic.
			setIsEditableEntitiesOnPlatformIncremental(true);
		}
	} else if (event.type == rpg3D::PlatformModule::EVENT_onPlatformContactRemoved->type) {
		if (entity->getGW()->getGPContent()->getSelectedPlayerEntity(true/*mustExist*/) == contactedEntity) {
			setPlayerEntityOnPlatform(nullptr);

			if (getIsInvalidPlayerOnPlatform()) {
				setIsInvalidPlayerOnPlatform(false);
			}
		} else {
			// Any entity on the pad will block edit logic.
			setIsEditableEntitiesOnPlatformIncremental(false);
		}
	} else {
		throw LogicException(LOC);
	}
}

void CustomLogicModule_EntityEditorPlatform::playerEntityOnPlatform_onEvent(IEventListener& eventListener, base::EntityEvent& event) {
	if (event.type == rpg3D::IStackableModule::EVENT_carryingChanged->type) {
		if (!getIsInvalidPlayerOnPlatform()) {
			setIsInvalidPlayerOnPlatform(true);
		}
	} else {
		throw LogicException(LOC);
	}
}

void CustomLogicModule_EntityEditorPlatform::onGWEvent(IEventListener& eventListener, base::GWEvent& event) {
	if (event.type == IGamePartUserInterface::GWEvent_onEntityEditor_activated->type) {
		setIsEditorActiveIncremental(true);
	} else if (event.type == IGamePartUserInterface::GWEvent_onEntityEditor_deactivated->type) {
		setIsEditorActiveIncremental(false);
	} else {
		throw LogicException(LOC);
	}
}

CustomLogicModule_EntityEditorPlatform::~CustomLogicModule_EntityEditorPlatform() {
    //void
}
