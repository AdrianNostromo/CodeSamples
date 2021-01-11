#pragma once

#include <base/gh.h>
#include "SH_Base.h"
#include <worldGame3D/scenariohandler/util/ScenarioConfig.h>
#include <string>
#include <memory>
#include <worldGame3D/gw/IGameWorld.h>
#include <worldGame3D/scenariotemplate/ScenarioTemplate.h>
#include <base/rendering/IRenderingHandler.h>
#include <worldGame3D/gw/handler/IHandler_GameWorld.h>

class IEventListener;
namespace base {
    class LoopEvent;
    class AppEvent;
};
namespace worldGame3D {
    class IGamePartContentZones;
    class ZoneStartExtraData;
};

namespace worldGame3D {
class SH_ActiveScenario : public SH_Base, virtual public IHandler_GameWorld {priv typedef SH_Base super;pub dCtor(SH_ActiveScenario);
    priv IRenderingHandler* renderingHandler;

    priv base::IGameWorld* gameWorld = nullptr;
    priv worldGame3D::IGamePartContentZones* gpContentZones = nullptr;
    priv ScenarioTemplate* scenarioTemplate = nullptr;

    priv bool isDirty_scenarioTarget = false;
    priv std::string targetScenario_id = "";
    priv ScenarioConfig* targetScenario_config = nullptr;
    priv bool forceScenarioRestartIfSame = false;
    priv std::string optionalZoneName = "";
    priv bool forceZoneRestartIfSame = false;
    priv std::shared_ptr<worldGame3D::ZoneStartExtraData> zoneStartExtraData = nullptr;

    priv ArrayList<std::shared_ptr<IEventListener>> localAutoListenersList{};

    pub explicit SH_ActiveScenario(
	    IRenderingHandler* renderingHandler,
        IAppAssets* appAssets, IAppMetrics* appMetrics, IAppUtils* appUtils,
        IContainer2D* layer2DGameWorld,
		IApp* app);

	priv void onAppEvent(IEventListener& eventListener, base::AppEvent& event);

	pub void create() override;
    /// <summary>
    /// 
    /// </summary>
    /// <param name="scenarioId"></param>
    /// <param name="scenarioConfig"></param>
    /// <param name="forceScenarioRestartIfSame"></param>
    /// <param name="optionalZoneName">
    /// Set this to "same" to use the same zone.
    /// // Set to "default" or "" to use the default zone.
    /// </param>
    /// <param name="forceZoneRestartIfSame"></param>
    /// <returns></returns>
    pub int setActiveScenario(
		std::string scenarioId, ScenarioConfig* scenarioConfig, bool forceScenarioRestartIfSame,
		std::string optionalZoneName, bool forceZoneRestartIfSame, std::shared_ptr<worldGame3D::ZoneStartExtraData> zoneStartExtraData = nullptr);

    pub IAppAssets* getAppAssets() override {
        return appAssets;
    }

    pub IAppMetrics* getAppMetrics() override {
        return appMetrics;
    }

    pub IAppUtils* getAppUtils() override {
        return appUtils;
    }
    
    pub IApp* getApp() override {
        return app;
    }

    pub virtual IRenderingHandler* getRenderingHandler() override {
        return renderingHandler;
    }
    pub void loopFinished() override;

    prot virtual void onLoopEvent_appLoop_general(IEventListener& eventListener, base::LoopEvent& event);

    pub ScenarioTemplate* getActiveScenarioTemplate();
    pub base::IGameWorld* getActiveScenatioGameWorld();

    prot void onGWEvent(std::string id, std::shared_ptr<base::GWEventDataBase> data) override;

    priv void disposeScenario();

    prot virtual void onActiveScenarioDeactivatePre(base::IGameWorld* gameWorld);
    prot virtual void onActiveScenarioActivated(base::IGameWorld* gameWorld);

    priv void gameWorld_onGamePartsCreated(base::IGameWorld* gameWorld);

	prot void disposeMainPre() override;
	pub ~SH_ActiveScenario() override;
};
};
