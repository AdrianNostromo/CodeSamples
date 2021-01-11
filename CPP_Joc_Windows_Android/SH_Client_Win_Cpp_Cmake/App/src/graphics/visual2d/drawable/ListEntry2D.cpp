#include "ListEntry2D.h"

using namespace base;

ListEntry2D::ListEntry2D(
	IAppMetrics* appMetrics, IAppAssets* appAssets, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, List2D* list,
	int entryUid)
	: super(appMetrics, appAssets, viewItemConfigs, viewEaseDurationsSList),
	list(list),
	entryUid(entryUid)
{
	//void
}

Map1D<std::string, std::shared_ptr<base::IWrappedValue>>* ListEntry2D::getUserDataMap() {
	if (userDataMap == nullptr) {
		userDataMap = new Map1D<std::string, std::shared_ptr<base::IWrappedValue>>();
	}

	return userDataMap;
}

bool ListEntry2D::getIsActive() {
	return isActive;
}

void ListEntry2D::setIsActive(bool isActive, bool doAnimation) {
	if (this->isActive == isActive) {
		return;
	}
	this->isActive = isActive;
    
	updateShowStateIfChanged(doAnimation, nullptr);
}

void ListEntry2D::show(bool doAnimation) {
	std::string targetStateId = computeStateFlags("_in_");
	setShowState(computeStateFlags("_out_toIn_"), targetStateId, doAnimation, nullptr, false);
}

void ListEntry2D::setLocalStateFlags(std::shared_ptr<std::string> localStateFlags) {
	this->localStateFlags = localStateFlags;

	updateShowStateIfChanged(true, nullptr);
}

std::string ListEntry2D::computeStateFlags(std::string baseState) {
	std::string state = super::computeStateFlags(baseState);

	if (localStateFlags != nullptr) {
		state += *localStateFlags;
	}

	if (isActive) {
		state += "active_";
	} else {
		state += "inactive_";
	}

	return state;
}

ListEntry2D::~ListEntry2D() {
	if (userDataMap != nullptr) {
		delete userDataMap;

		userDataMap = nullptr;
	}
}
