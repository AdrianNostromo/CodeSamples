#include "A_PreRequirements.h"

using namespace worldGame3D;

A_PreRequirements::A_PreRequirements(
	int type, std::string name,
	bool isSyncedToServer,
	int persistenceDurationMS,
	int groupsBitMap,
	int preRequirementsGroupsBitMask)
	: super(
		type, name,
		isSyncedToServer,
		persistenceDurationMS,
		groupsBitMap),
	preRequirementsGroupsBitMask(preRequirementsGroupsBitMask)
{
	//void
}

int A_PreRequirements::getPreRequirementsGroupsBitMask() {
	return preRequirementsGroupsBitMask;
}

bool A_PreRequirements::getIsPreRequirementsMet() {
	if (preRequirementsGroupsBitMask == 0 || preRequirementsActionsList.count() > 0) {
		return true;
	}

	return false;
}

void A_PreRequirements::connectPreRequirementAction(std::shared_ptr<IAction> preRequirementAction) {
	if (preRequirementsActionsList.contains(preRequirementAction)) {
		throw LogicException(LOC);
	}

	preRequirementsActionsList.appendDirect(preRequirementAction);

	if (actionsListEntry == nullptr) {
		throw LogicException(LOC);
	}
	preRequirementAction->_connectPreRequirerAction(actionsListEntry->data);
}

void A_PreRequirements::disconnectPreRequirementAction(std::shared_ptr<IAction> preRequirementAction) {
	bool b = preRequirementsActionsList.removeDirect(preRequirementAction);
	if (!b) {
		throw LogicException(LOC);
	}

	if (actionsListEntry == nullptr) {
		throw LogicException(LOC);
	}
	preRequirementAction->_disconnectPreRequirerAction(actionsListEntry->data);
}

void A_PreRequirements::disconnectAllPreRequirementActions() {
	if (actionsListEntry == nullptr) {
		throw LogicException(LOC);
	}

	while (preRequirementsActionsList.count() > 0) {
		std::shared_ptr<IAction> preRequirementAction = preRequirementsActionsList.removeAndGetDirect(0);

		preRequirementAction->_disconnectPreRequirerAction(actionsListEntry->data);
	}
}

void A_PreRequirements::disconnectAllPreRequirerActions() {
	if (actionsListEntry == nullptr) {
		throw LogicException(LOC);
	}

	while (preRequirersActionsList.count() > 0) {
		std::shared_ptr<IAction> preRequirerAction = preRequirersActionsList.getDirect(0);

		preRequirerAction->disconnectPreRequirementAction(actionsListEntry->data);
	}
}

void A_PreRequirements::_connectPreRequirerAction(std::shared_ptr<IAction> preRequirerAction) {
	if (preRequirersActionsList.contains(preRequirerAction)) {
		throw LogicException(LOC);
	}

	preRequirersActionsList.appendDirect(preRequirerAction);
}

void A_PreRequirements::_disconnectPreRequirerAction(std::shared_ptr<IAction> preRequirerAction) {
	bool b = preRequirersActionsList.removeDirect(preRequirerAction);
	if (b == false) {
		throw LogicException(LOC);
	}
}

void A_PreRequirements::onRemoveFromActionsList_pre(int actionsStatus) {
	super::onRemoveFromActionsList_pre(actionsStatus);

	disconnectAllPreRequirementActions();
	disconnectAllPreRequirerActions();
}

A_PreRequirements::~A_PreRequirements() {
	//void
}
