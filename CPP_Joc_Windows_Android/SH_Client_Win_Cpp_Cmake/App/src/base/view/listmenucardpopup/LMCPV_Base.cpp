#include "LMCPV_Base.h"
#include <base/util/StringUtil.h>
#include <base/log/GlobalAccessLogHandler.h>

using namespace base;

LMCPV_Base::LMCPV_Base(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, IHandlerAutoDisposal* handlerAutoDisposal)
	: super(app, viewItemConfigs, viewEaseDurationsSList, handlerAutoDisposal)
{
	//void
}

void LMCPV_Base::show(bool doAnimation, IAnimationListener* menuAnimationListener, std::shared_ptr<Config> viewConfig, IListener* listener) {
	if (listener == nullptr) {
		throw LogicException(LOC);
	}
	if (StringUtil::contains(endStateID, "_in_")) {
		// View is already active.

		throw LogicException(LOC);
	}

	this->viewConfig = viewConfig;
	this->listener = listener;

	std::string stateID = computeStateFlags("_in_");
	if (!doAnimation || endStateID != stateID) {
		setShowState(computeStateFlags("_out_toIn_"), stateID, doAnimation, menuAnimationListener, false);
	}
}

void LMCPV_Base::onSelectedStateChanged() {
	super::onSelectedStateChanged();

	if (!isActive()) {
		//void
	} else {
		//void
	}
}

std::string LMCPV_Base::computeStateFlags(std::string baseState) {
	std::string state = super::computeStateFlags(baseState);

	if (viewConfig != nullptr && viewConfig->isFullScreenDimmer) {
		state += "isFullScreenDimmer_";
	}
	
	return state;
}

void LMCPV_Base::disposeMain() {
	//void

	super::disposeMain();
}

LMCPV_Base::~LMCPV_Base() {
	//void
}
