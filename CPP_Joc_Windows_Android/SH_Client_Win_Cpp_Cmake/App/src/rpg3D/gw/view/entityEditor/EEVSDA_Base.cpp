#include "EEVSDA_Base.h"

using namespace rpg3D;

EEVSDA_Base::EEVSDA_Base(
	IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList,
	IAppUtils* appUtils)
	: super(
		app, viewItemConfigs, viewEaseDurationsSList,
		appUtils
	)
{
	//void
}

void EEVSDA_Base::hideSettingsDrawerA() {
	if (!isSettingsDrawerAEnabled) {
		return;
	}

	isSettingsDrawerAEnabled = false;

	updateShowStateIfChanged(true, nullptr);
}

void EEVSDA_Base::toggleSettingsDrawerA() {
	isSettingsDrawerAEnabled = !isSettingsDrawerAEnabled;

	updateShowStateIfChanged(true, nullptr);
}

std::string EEVSDA_Base::computeStateFlags(std::string baseState) {
	std::string state = super::computeStateFlags(baseState);

	if (isSettingsDrawerAEnabled) {
		state += "_isSettingsDrawerAEnabled:true_";
	}

	return state;
}

EEVSDA_Base::~EEVSDA_Base() {
	//void
}
