#pragma once

#include <base/gh.h>
#include <worldGame3D/scenariohandler/util/ScenarioConfig.h>
#include "IGameWorldBubbledEntityEvents.h"
#include "IGameWorldBase.h"
#include <functional>
#include <base/component/IComponent_ScenariosHandlerComponent.h>

class IGamePart;

namespace base {
class IGameWorld : virtual public worldGame3D::IGameWorldBubbledEntityEvents, public virtual IGameWorldBase, public virtual IComponent_ScenariosHandlerComponent {
	pub static std::string GWE_RequestShowMainSidebarView;
	pub static std::string GWE_RequestRunScript;

	// Views are not put in the GW component handler because only game_parts should be in the gw.
	/// Different types of components (eg. views) should be in a dedicated components handler.
	pub virtual IComponentsHandler* getViewComponentsHandler() = 0;

	pub virtual void addGamePartUninitialised(IGamePart* gamePart) = 0;
	// This version requires templating because needs to fetch a component object and dispose that;
	pub template <typename T>
	void disposeGamePart();
	pub virtual void disposeGamePart(IGamePart* gamePart) = 0;
	
	pub virtual ScenarioConfig* getScenarioConfig() = 0;

	pub virtual bool getIsLevelStarted() = 0;
	pub virtual bool getIsLevelEnded() = 0;
	pub virtual bool getIsPlayEnabled() = 0;

	pub virtual void setCb_onGamePartsCreated(std::function<void(IGameWorld* gameWorld)> cb_onGamePartsCreated) = 0;
	
    virtual ~IGameWorld() = default;
};

template <typename T>
void IGameWorld::disposeGamePart() {
	T cGp = getComponentAs<T>(true/*mustExist*/);

	disposeGamePart(cGp);
}

};
