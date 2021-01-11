#pragma once

#include <base/gh.h>
#include <worldGame3D/scenariotemplate/ScenarioTemplate.h>
#include <worldGame3D/scenariohandler/util/ScenarioConfig.h>
#include <graphics/visual2d/drawable/IContainer2D.h>
#include <base/event/dispatcher/EventDispatcher.h>
#include <worldGame3D/gw/event/GWEvent.h>
#include <worldGame3D/gw/flags/IFlags.h>
#include <worldGame3D/gw/capabilities/ICapabilities.h>

class IApp;

class IGameWorldBase {
	pub virtual IApp* getApp() = 0;
	pub virtual IAppAssets* getAppAssets() = 0;

	pub virtual bool getIsEnabled() = 0;
	pub virtual void setIsEnabledIncremental(bool isEnabled) = 0;
    
	pub virtual void onAppBecomingInnactive() = 0;
	pub virtual void prepareForDisposal() = 0;

	pub virtual void startLevelPlay() = 0;
	pub virtual void setIsPlayEnabled(bool isPlayEnabled) = 0;

	pub virtual ScenarioTemplate* getScenarioTemplate() = 0;
	pub virtual ScenarioConfig* getScenarioConfig() = 0;

	pub virtual IContainer2D* getGameWorldContainer2D() = 0;

	pub virtual worldGame3D::gw::IFlags& getFlags() = 0;
	pub virtual worldGame3D::gw::ICapabilities& getCapabilities() = 0;

	pub virtual ~IGameWorldBase() = default;
};
