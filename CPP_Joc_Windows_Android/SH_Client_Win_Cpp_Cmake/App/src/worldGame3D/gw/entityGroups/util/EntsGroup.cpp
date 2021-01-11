#include "EntsGroup.h"
#include <worldGame3D/gw/entityGroups/event/EntGroupsEvent.h>

using namespace worldGame3D;

EntsGroup::EntsGroup(int uid)
	: uid(uid)
{
	//void
}

int EntsGroup::getUid() {
	return uid;
}

base::TypedEventDispatcher1D<EntGroupsEvent>& EntsGroup::getEvents() {
	return events;
}

int EntsGroup::getEntitiesCount() {
	return entitiesList.count();
}

bool EntsGroup::hasEntity(IWorldEntity* entity) {
	ManagedEntityData* managedEntityData = static_cast<ManagedEntityData*>(entity->getInEntitiesGroup_entryRef());
	if (managedEntityData != nullptr) {
		return true;
	}

	return false;
}

void EntsGroup::addEntity(IWorldEntity* entity) {
	if (entity->getInEntitiesGroup_entryRef() != nullptr) {
		throw LogicException(LOC);
	}

	ListDL<ManagedEntityData>::Entry* lEntry = entitiesList.appendEmplace(entity);
	ManagedEntityData& managedEntityData = lEntry->data;
	managedEntityData.listEntry = lEntry;

	entity->getInEntitiesGroup_entryRef() = &managedEntityData;

	entity->addEventListener(
		IWorldEntity::EVENT_onRemoved_pre->type,
		std::bind(&EntsGroup::entity_onRemoved_Pre, this, std::placeholders::_1, std::placeholders::_2),
		managedEntityData.localAutoListenersList
	);

	events.dispatchEvent(*EGV_EntityAdded->setData(this, entity));
}

void EntsGroup::entity_onRemoved_Pre(IEventListener& eventListener, base::EntityEvent& event) {
	removeEntity(event.getEntityData(), false);
}

void EntsGroup::removeEntity(IWorldEntity* entity, bool disposeTheEntity) {
	ManagedEntityData* managedEntityData = static_cast<ManagedEntityData*>(entity->getInEntitiesGroup_entryRef());
	if (managedEntityData == nullptr) {
		// The entity is not part of the managed player entities.
		throw LogicException(LOC);
	}

	events.dispatchEvent(*EGV_EntityRemovePre->setData(this, entity));

	managedEntityData->localAutoListenersList.clear();

	entity->getInEntitiesGroup_entryRef() = nullptr;
	managedEntityData->listEntry->remove();
	// Delete occurs in the above remove;
	//managedEntityData->listEntry = nullptr;
	//delete managedEntityData;

	events.dispatchEvent(*EGV_EntityRemoved->setData(this, entity));

	if (disposeTheEntity) {
		entity->markEntityForRemoval(IWorldEntity::ENT_REM_REASON_PURPOSE_FINISHED, false);
	}
}

void EntsGroup::disposeAllEntities() {
	while (entitiesList.count() > 0) {
		ListDL<ManagedEntityData>::Entry* listEntry = entitiesList.getFirst();
		removeEntity(listEntry->data.entity, true/*disposeTheEntity*/);
	}
}

EntsGroup::~EntsGroup() {
	//void
}
