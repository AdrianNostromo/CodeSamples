#include "A_Base.h"

using namespace worldGame3D;

const bool A_Base::IS_DEBUG_ACTIONS = true;

A_Base::A_Base(
	int type, std::string name,
	bool isSyncedToServer,
	int persistenceDurationMS,
	int groupsBitMap)
	: super(),
	type(type), name(name),
	isSyncedToServer(isSyncedToServer),
	persistenceDurationMS(persistenceDurationMS),
	groupsBitMap(groupsBitMap)
{
	//void
}

int A_Base::getType() {
	return type;
}

std::string A_Base::getName() {
	return name;
}

bool A_Base::getIsSyncedToServer() {
	return isSyncedToServer;
}

bool A_Base::getCanHaveNoListeners() {
	return canHaveNoListeners;
}

int A_Base::getPersistenceDurationMS() {
	return persistenceDurationMS;
}

bool A_Base::getIsFinished() {
	if (isActionLogicFinished || (persistenceDurationMS >= 0 && totalDeltaMS >= persistenceDurationMS)) {
		return true;
	}

	return false;
}

int A_Base::getGroupsBitMap() {
	return groupsBitMap;
}

ListDL<std::shared_ptr<worldGame3D::IAction>>::Entry* A_Base::getActionsListEntry() {
	return actionsListEntry;
}

void A_Base::setActionsListEntry(ListDL<std::shared_ptr<worldGame3D::IAction>>::Entry* actionsListEntry) {
	this->actionsListEntry = actionsListEntry;
}

float A_Base::getTotalDeltaS() {
	return totalDeltaS;
}

int A_Base::getTotalDeltaMS() {
	return totalDeltaMS;
}

void A_Base::setTotalDeltaTimes(int totalDeltaMS) {
	this->totalDeltaMS = totalDeltaMS;
	this->totalDeltaS = this->totalDeltaMS / 1000.0f;
}

void A_Base::incrementTicksCounter() {
	ticksCounter++;
}

void A_Base::setData(int deltaMS) {
	this->deltaMS = deltaMS;
	this->deltaS = this->deltaMS / 1000.0f;
}

void A_Base::clearData() {
	deltaMS = 0;
	deltaS = 0.0f;
}

void A_Base::persistentActionFinished() {
	// Don't check if the action is persistent. Some configured actions may or may not be persistent,animated and call this function when they finished even if they finish in 1 frame, with a deltaMS of 0.
	if (isActionLogicFinished) {
		throw LogicException(LOC);
	}

	isActionLogicFinished = true;
}

void* A_Base::getRawSubclassPointer(int validationType) {
	if (type != validationType || rawSubclassPointer == nullptr) {
		throw LogicException(LOC);
	}

	return rawSubclassPointer;
}

void A_Base::removeFromActionsList(int actionsStatus) {
	if (actionsListEntry == nullptr) {
		throw LogicException(LOC);
	}

	onRemoveFromActionsList_pre(actionsStatus);

	actionsListEntry->remove();
	actionsListEntry = nullptr;
}

void A_Base::onRemoveFromActionsList_pre(int actionsStatus) {
	//void
}

A_Base::~A_Base() {
	//void
}
