#pragma once

#include <base/gh.h>
#include <base/component/ComponentsHandler.h>
#include "IGameWorld.h"
#include <worldGame3D/scenariohandler/util/ScenarioConfig.h>
#include <worldGame3D/gw/handler/IHandler_GameWorld.h>
#include <worldGame3D/scenariotemplate/ScenarioTemplate.h>
#include <graphics/visual2d/drawable/IDrawableNode2D.h>
#include <memory>

class IEventListener;
namespace base {
	class LoopEvent;
};
namespace worldGame3D {
	namespace gw {
		class Flags;
		class Capabilities;
	};
};

namespace base {
class GWBase : public ComponentsHandler, virtual public IGameWorld {priv typedef ComponentsHandler super;pub dCtor(GWBase);
    prot IHandler_GameWorld* handler;
	prot ScenarioTemplate* scenarioTemplate;
	prot ScenarioConfig* scenarioConfig;

	prot IApp* app;
	prot IAppMetrics* appMetrics;

	// Game play may be enabled even if the level hasn't started yet.
	priv bool isLevelStarted = false;
	priv bool isLevelEnded = false;

	// false : game is paused;
	// true : game play is enabled;
	priv bool isPlayEnabled = false;

    // Controls loops and 2d, 3d active states.
    // 1: Enabled.
    // <1: Disabled.
    // <1: Logic error.
	priv int isEnabledCounter = 0;
	priv float gameTimeS = 0.0f;

	priv int activeRenderableId = -1;

	// Use shared_ptr to avoid large map block memory usage on change.
	priv base::TypedEventDispatcher1D<base::GWEvent> eventDispatcher{};

	priv std::function<void(IGameWorld* gameWorld)> cb_onGamePartsCreated = nullptr;

	priv ArrayList<std::shared_ptr<IEventListener>> localAutoListenersList{};

	priv worldGame3D::gw::Flags* flags = nullptr;
	priv worldGame3D::gw::Capabilities* capabilities = nullptr;

	pub explicit GWBase(IHandler_GameWorld* handler, ScenarioTemplate* scenarioTemplate, ScenarioConfig* scenarioConfig);

	pub void startLevelPlay() override;
	pub void setIsPlayEnabled(bool isPlayEnabled) override;
	
	pub void setCb_onGamePartsCreated(std::function<void(IGameWorld* gameWorld)> cb_onGamePartsCreated) final;

	pub IApp* getApp() override;
	pub IAppAssets* getAppAssets() override;

	pub ScenarioTemplate* getScenarioTemplate() override {
        return scenarioTemplate;
    }
	pub ScenarioConfig* getScenarioConfig() override {
        return scenarioConfig;
    }

	pub bool getIsLevelStarted() final { return isLevelStarted; }
	pub bool getIsLevelEnded() final { return isLevelEnded; }
	pub bool getIsPlayEnabled() final { return isPlayEnabled; }

	pub bool getIsEnabled() override;
	pub void setIsEnabledIncremental(bool isEnabled) override;
    
	pub void onAppBecomingInnactive() override;
	pub void prepareForDisposal() override;

	pub IHandler_GameWorld* getHandler() final;

	prot virtual void onLoopEvent_appLoop_gameWorld(IEventListener& eventListener, base::LoopEvent& event);
	
	prot virtual void isPlayEnabled_changedEvent();

	prot virtual void onIsEnabledChanged();

	prot void create() final;

	prot virtual void createFramework();
	prot virtual void createGParts();
	prot virtual void createGPartsPost();
	prot virtual int createLayers();
	prot virtual int createComponents();
	prot virtual int createComponentsPost();
	prot virtual int createContent();
	prot void createPost() override;
	prot virtual int createServerConnection();

	prot base::TypedEventDispatcher1D<base::GWEvent>* getEventDispatcher() override;
	prot std::shared_ptr<IEventListener> addEventListener(
		int type,
		std::function<void(IEventListener& eventListener, base::GWEvent& event)> cb) override;
	prot void addEventListener(
		int type,
		std::function<void(IEventListener& eventListener, base::GWEvent& event)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList) override;
	prot void addEventListener(
		int type1, int type2,
		std::function<void(IEventListener& eventListener, base::GWEvent& event)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList) override;
	prot void addEventListener(
		int type1, int type2, int type3,
		std::function<void(IEventListener& eventListener, base::GWEvent& event)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList) override;
	prot int dispatchEvent(base::GWEvent& event) override;
	prot int dispatchEvents(ArrayList<base::GWEvent*>* eventsList) override;

	pub worldGame3D::gw::IFlags& getFlags() final;
	pub worldGame3D::gw::ICapabilities& getCapabilities() final;

	prot void disposeMainPre() override;
	prot void disposeMain() override;
	pub ~GWBase() override;
};
};
