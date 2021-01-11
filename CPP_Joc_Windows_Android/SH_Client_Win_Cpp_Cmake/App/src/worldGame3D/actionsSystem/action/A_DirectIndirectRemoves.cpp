#include "A_DirectIndirectRemoves.h"

using namespace worldGame3D;

A_DirectIndirectRemoves::A_DirectIndirectRemoves(
	int type, std::string name,
	bool isSyncedToServer,
	int persistenceDurationMS,
	int groupsBitMap,
	int preRequirementsGroupsBitMask,
	int replaceGroupsBitMask)
	: super(
		type, name,
		isSyncedToServer,
		persistenceDurationMS,
		groupsBitMap,
		preRequirementsGroupsBitMask,
		replaceGroupsBitMask)
{
	//void
}

ArrayList<std::shared_ptr<IAction>>* A_DirectIndirectRemoves::getDirectIndirectRemovedActionsList() {
	return &directIndirectRemovedActionsList;
}

void A_DirectIndirectRemoves::pushDirectIndirectRemovedAction(std::shared_ptr<IAction> action, int actionsStatus) {
	if (action->getIsReplaced()) {
		throw LogicException(LOC);
	}

	action->setIsReplaced(true);
	
	action->removeFromActionsList(actionsStatus);

	directIndirectRemovedActionsList.appendReference(action);
}

A_DirectIndirectRemoves::~A_DirectIndirectRemoves() {
	//void
}
