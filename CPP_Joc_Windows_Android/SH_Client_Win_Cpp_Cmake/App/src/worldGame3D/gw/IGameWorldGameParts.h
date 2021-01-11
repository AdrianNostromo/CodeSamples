#pragma once

#include <base/gh.h>
#include "gamepart/IGamePart.h"
#include "IGameWorldBase.h"
#include <base/component/IComponentsHandler.h>

namespace base {
	class IGamePartMain;
	class IGPContent;
	class IGPInteraction;
	class IGPSim;
	class IGPOnline;
	class IGPManager;
};

namespace worldGame3D {
class IGameWorldGameParts : virtual public IGameWorldBase, virtual public IComponentsHandler {
	pub virtual base::IGamePartMain* getGPMain() = 0;
	pub virtual base::IGamePartMain* getMain() = 0;
	pub virtual base::IGPContent* getContent() = 0;
	pub virtual base::IGPContent* getGPContent() = 0;
	pub virtual base::IGPInteraction* getGPInteraction() = 0;
	pub virtual base::IGPInteraction* getInteraction() = 0;
	pub virtual base::IGPSim* getSim() = 0;
	pub virtual base::IGPSim* getGPSim() = 0;
	pub virtual base::IGPOnline* getOnline() = 0;
	pub virtual base::IGPOnline* getGPOnline() = 0;
	pub virtual base::IGPManager* getManager() = 0;

	pub template <typename T>
	T getMainAs();
	pub template <typename T>
	T getContentAs();
	pub template <typename T>
	T getInteractionAs();
	pub template <typename T>
	T getSimAs();
	pub template <typename T>
	T getOnlineAs();
	pub template <typename T>
	T getManagerAs();

    pub ~IGameWorldGameParts() override = default;
};

template <typename T>
T IGameWorldGameParts::getMainAs() {
	T cObj = dynamic_cast<T>(getMain());
	if (!cObj) {
		throw LogicException(LOC);
	}

	return cObj;
}

template <typename T>
T IGameWorldGameParts::getContentAs() {
	T cObj = dynamic_cast<T>(getContent());
	if (!cObj) {
		throw LogicException(LOC);
	}

	return cObj;
}

template <typename T>
T IGameWorldGameParts::getInteractionAs() {
	T cObj = dynamic_cast<T>(getInteraction());
	if (!cObj) {
		throw LogicException(LOC);
	}

	return cObj;
}

template <typename T>
T IGameWorldGameParts::getSimAs() {
	T cObj = dynamic_cast<T>(getSim());
	if (!cObj) {
		throw LogicException(LOC);
	}

	return cObj;
}

template <typename T>
T IGameWorldGameParts::getOnlineAs() {
	T cObj = dynamic_cast<T>(getOnline());
	if (!cObj) {
		throw LogicException(LOC);
	}

	return cObj;
}

template <typename T>
T IGameWorldGameParts::getManagerAs() {
	T cObj = dynamic_cast<T>(getManager());
	if (!cObj) {
		throw LogicException(LOC);
	}

	return cObj;
}

};
