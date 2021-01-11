#include "A_Override.h"

using namespace worldGame3D;

A_Override::A_Override(
	int type, std::string name,
	bool isSyncedToServer,
	int persistenceDurationMS,
	int priority, 
	int groupsBitMap,
	int preRequirementsGroupsBitMask,
	int replaceGroupsBitMask,
	int overridesGroupsBitMask)
	: super(
		type, name,
		isSyncedToServer,
		persistenceDurationMS,
		priority, 
		groupsBitMap,
		preRequirementsGroupsBitMask,
		replaceGroupsBitMask),
	overridesGroupsBitMask(overridesGroupsBitMask)
{
	//void
}

int A_Override::getOverridesGroupsBitMask() {
	return overridesGroupsBitMask;
}

bool A_Override::getIsOverridden() {
	return parentOverridersActionsList.count() > 0 ? true : false;
}

void A_Override::addChildOverriddenAction(IAction* childOverriddenAction) {
	if (childOverriddenActionsList.contains(childOverriddenAction)) {
		throw LogicException(LOC);
	}

	childOverriddenActionsList.appendDirect(childOverriddenAction);

	childOverriddenAction->addParentOverriderAction(this);
}

void A_Override::removeChildOverriddenAction(IAction* childOverriddenAction) {
	childOverriddenAction->removeParentOverriderAction(this);
	
	bool b = childOverriddenActionsList.removeDirect(childOverriddenAction);
	if (b == false) {
		throw LogicException(LOC);
	}
}

void A_Override::removeAllChildOverriddenActions() {
	while (childOverriddenActionsList.count() > 0) {
		IAction* childOverriddenAction = childOverriddenActionsList.removeAndGetDirect(0);

		childOverriddenAction->removeParentOverriderAction(this);
	}
}

void A_Override::addParentOverriderAction(IAction* parentOverriderAction) {
	if (parentOverridersActionsList.contains(parentOverriderAction)) {
		throw LogicException(LOC);
	}

	parentOverridersActionsList.appendDirect(parentOverriderAction);
}

void A_Override::removeParentOverriderAction(IAction* parentOverriderAction) {
	bool b = parentOverridersActionsList.removeDirect(parentOverriderAction);
	if (b == false) {
		throw LogicException(LOC);
	}
}

void A_Override::removeAllParentOverriderActions() {
	while (parentOverridersActionsList.count() > 0) {
		IAction* parentOverriderAction = parentOverridersActionsList.getDirect(0);
		
		parentOverriderAction->removeChildOverriddenAction(this);
	}
}

void A_Override::onRemoveFromActionsList_pre(int actionsStatus) {
	super::onRemoveFromActionsList_pre(actionsStatus);

	removeAllChildOverriddenActions();
	removeAllParentOverriderActions();
}

A_Override::~A_Override() {
	//void
}
