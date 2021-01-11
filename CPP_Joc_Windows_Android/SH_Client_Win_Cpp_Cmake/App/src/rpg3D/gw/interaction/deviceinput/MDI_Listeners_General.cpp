#include "MDI_Listeners_General.h"
#include <base/exceptions/LogicException.h>
#include <memory>

using namespace rpg3D;

MDI_Listeners_General::MDI_Listeners_General(base::IGameWorld* gw)
    : super(gw)
{
    //void
}

void MDI_Listeners_General::setCb_onEntityControlInputs(std::function<void(
	std::shared_ptr<base::SensorEvent>& sensorEvent)> cb_onEntityControlInputs)
{
	if (this->cb_onEntityControlInputs != nullptr && cb_onEntityControlInputs != nullptr) {
		throw LogicException(LOC);
	}

	this->cb_onEntityControlInputs = cb_onEntityControlInputs;
}

void MDI_Listeners_General::dispatch_onEntityControlInputs(
	std::shared_ptr<base::SensorEvent>& sensorEvent)
{
	// Dispatch a up instead of a down_canceled in this situation.
	if (cb_onEntityControlInputs) {
		cb_onEntityControlInputs(
			sensorEvent
		);
	}
}

MDI_Listeners_General::~MDI_Listeners_General() {
    //void
}
