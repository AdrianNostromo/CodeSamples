#include "T_GWCapabilitiesRequirements.h"
#include <base/exceptions/LogicException.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <worldGame3D/gw/capabilities/ICapabilities.h>
#include <worldGame3D/gw/capabilities/event/CapabilityEvent.h>

using namespace rpg3D;

T_GWCapabilitiesRequirements::T_GWCapabilitiesRequirements(ToolConfigBase* config)
	: super(config),
	gwCapabilityRequirementBitIndex(worldGame3D::gw::ICapabilities::NOT_USED_INDEX)
{
	//void
}

void T_GWCapabilitiesRequirements::setCb_isSelectedAndGWCapabilitiesExist_changed(std::function<void(ITool* target, bool isSelectedAndGWCapabilitiesExist)> cb_isSelectedAndGWCapabilitiesExist_changed) {
	this->cb_isSelectedAndGWCapabilitiesExist_changed = cb_isSelectedAndGWCapabilitiesExist_changed;
}

void T_GWCapabilitiesRequirements::initGWCapabilityRequirementBitIndex(unsigned char gwCapabilityRequirementBitIndex) {
	if (getIsCreated() || this->gwCapabilityRequirementBitIndex != worldGame3D::gw::ICapabilities::NOT_USED_INDEX) {
		// This functon should be called from a constructor only and only once.
		throw LogicException(LOC);
	}

	this->gwCapabilityRequirementBitIndex = gwCapabilityRequirementBitIndex;
}

void T_GWCapabilitiesRequirements::_onSelectedStateChanged(bool isToolSelected, worldGame3D::IGameWorldGameParts* gwGamePartsOptional) {
	super::_onSelectedStateChanged(isToolSelected, gwGamePartsOptional);

	handleActivationChange(isToolSelected, gwGamePartsOptional);
}

void T_GWCapabilitiesRequirements::connectToGameParts(worldGame3D::IGameWorldGameParts* gwGameParts) {
	super::connectToGameParts(gwGameParts);

	handleActivationChange(getIsToolSelected(), gwGameParts/*gwGamePartsOptional*/);
}

void T_GWCapabilitiesRequirements::disconnectFromGamePartsPre() {
	// Note. Must use nullptr for gwGamePartsOptional at this location.
	handleActivationChange(getIsToolSelected(), nullptr/*gwGamePartsOptional*/);

	super::disconnectFromGamePartsPre();
}

void T_GWCapabilitiesRequirements::handleActivationChange(bool isToolSelected, worldGame3D::IGameWorldGameParts* gwGamePartsOptional) {
	if (gwCapabilityRequirementBitIndex == worldGame3D::gw::ICapabilities::NOT_USED_INDEX) {
		// No capabilities required, pass through.
		isGWCapabilitiesExist = true;
	} else {
		// Need to handle capabilities.
		if (!isToolSelected || gwGamePartsOptional == nullptr) {
			// Remove capabilities change event listeners if exist.
			localAutoListenersList.clear();

			isGWCapabilitiesExist = false;
		} else {
			// Tool selected and gParts exist.
			if (localAutoListenersList.count() != 0) {
				// This should never occur, a bug exists.
				throw LogicException(LOC);
			}

			// Add capabilities change event listeners;
			gwGamePartsOptional->getCapabilities().getEvents().addEventListener(
				worldGame3D::gw::ICapabilities::EV_CapabilityChanged->type,
				std::bind(&T_GWCapabilitiesRequirements::onGWCapabilityChanged, this, std::placeholders::_1, std::placeholders::_2),
				localAutoListenersList
			);

			isGWCapabilitiesExist = gwGamePartsOptional->getCapabilities().hasWithBitIndex(gwCapabilityRequirementBitIndex);
		}
	}

	bool isSelectedAndGWCapabilitiesExist = isToolSelected && isGWCapabilitiesExist;
	
	if (lastDispatched_isSelectedAndGWCapabilitiesExist != isSelectedAndGWCapabilitiesExist) {
		lastDispatched_isSelectedAndGWCapabilitiesExist = isSelectedAndGWCapabilitiesExist;
		
		onSelectedAndGWCapabilitiesEnabledStateChanged(isSelectedAndGWCapabilitiesExist, gwGamePartsOptional);
	}
}

void T_GWCapabilitiesRequirements::onGWCapabilityChanged(IEventListener& eventListener, worldGame3D::gw::CapabilityEvent& event) {
	if (event.capabilityBitIndex == worldGame3D::gw::ICapabilities::EV_CapabilityChanged->type) {
		if (gwGamePartsOptional == nullptr) {
			//Bug.
			throw LogicException(LOC);
		}

		handleActivationChange(getIsToolSelected(), gwGamePartsOptional);
	} else {
		throw LogicException(LOC);
	}
}

void T_GWCapabilitiesRequirements::onSelectedAndGWCapabilitiesEnabledStateChanged(bool isSelectedAndGWCapabilitiesExist, worldGame3D::IGameWorldGameParts* gwGamePartsOptional) {
	if (cb_isSelectedAndGWCapabilitiesExist_changed == nullptr) {
		// This should always be set at this location.
		throw LogicException(LOC);
	}

	cb_isSelectedAndGWCapabilitiesExist_changed(this, isSelectedAndGWCapabilitiesExist);
}

T_GWCapabilitiesRequirements::~T_GWCapabilitiesRequirements() {
	//void
}
