#include "ContentGroup.h"
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/entity/util/EntityRemoveReason.h>

using namespace worldGame3D;

ContentGroup::ContentGroup() {
	//void
}

IListEntry* ContentGroup::appendGroupedEntity(IWorldEntity* entity) {
	return entitiesList.appendDirect(entity);
}

void ContentGroup::setAllEntitiesEnabledState(bool isEnabled) {
	ListDL<IWorldEntity*>::Entry* listEntry = entitiesList.getFirst();
	while (listEntry != nullptr && !listEntry->data->getIsMarkedForRemoval()) {
		//asd_1; listEntry->data->setIsEnabled(isEnabled);

		listEntry = listEntry->next;
	}
}

ContentGroup::~ContentGroup() {
	//void
}
