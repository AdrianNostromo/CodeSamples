#include "GCEG_EntsGroups.h"
#include <base/exceptions/LogicException.h>
#include "util/EntsGroup.h"
#include <worldGame3D/gw/entity/IWorldEntity.h>

using namespace worldGame3D;

GCEG_EntsGroups::GCEG_EntsGroups(base::IGameWorld* gw)
	: super(gw)
{
	//void
}

IEntsGroup* GCEG_EntsGroups::createEntitiesGroup() {
	int uid = getNextGroupIndexUid();
	sp<EntsGroup> entsGroup = new EntsGroup(uid);

	entGroupsList.insertDirectRangeFill(uid, entsGroup, nullptr);

	return entsGroup.get();
}

IEntsGroup* GCEG_EntsGroups::getEntitiesGroup(int groupId) {
	sp<EntsGroup>* entsGroup = entGroupsList.getPointer(groupId, nullptr);
	if (entsGroup == nullptr) {
		return nullptr;
	}

	return entsGroup->get();
}

void GCEG_EntsGroups::removeEntitiesGroup(IEntsGroup* _entsGroup, bool disposeGroupEntities) {
	if (_entsGroup == nullptr) {
		throw LogicException(LOC);
	}

	sp<EntsGroup>& entsGroup = entGroupsList.getReference(_entsGroup->getUid());
	if (entsGroup == nullptr || entsGroup->uid != _entsGroup->getUid()) {
		throw LogicException(LOC);
	}

	if (entsGroup->entitiesList.count() > 0) {
		if (!disposeGroupEntities) {
			throw LogicException(LOC);
		}

		entsGroup->disposeAllEntities();
	}
}

int GCEG_EntsGroups::getNextGroupIndexUid() {
	int ct = entGroupsList.count();
	for (int i = 0; i < ct; i++) {
		if (entGroupsList.getReference(i) == nullptr) {
			return i;
		}
	}

	return ct;
}

GCEG_EntsGroups::~GCEG_EntsGroups() {
	//void
}
