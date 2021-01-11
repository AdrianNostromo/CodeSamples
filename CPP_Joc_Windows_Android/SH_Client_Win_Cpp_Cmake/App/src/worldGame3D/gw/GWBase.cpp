#include "GWBase.h"
#include <base/math/Math.h>
#include <base/exceptions/LogicException.h>
#include <base/visual3d/Visual3DUtil.h>
#include <base/visual2D/VisualUtil2D.h>
#include <worldGame3D/gw/main/IGamePartMain.h>
#include <base/app/IApp.h>
#include <graphics/environment/environment3D/Environment3D.h>
#include <graphics/visual3d/drawable/Stage3D.h>
#include <TestLog.h>
#include <base/app/IApp.h>
#include <base/appLoop/ILoopDistribution.h>
#include <base/appLoop/event/LoopEvent.h>
#include <base/component/IComponent_GameWorldComponent.h>
#include "flags/Flags.h"
#include "capabilities/Capabilities.h"

using namespace base;

GWBase::GWBase(IHandler_GameWorld* _handler, ScenarioTemplate* _scenarioTemplate, ScenarioConfig* _scenarioConfig)
	: super(IComponent_GameWorldComponent::COMPONENT_CATEGORY),
    handler(_handler), scenarioTemplate(_scenarioTemplate), scenarioConfig(_scenarioConfig)
{
    this->app = handler->getApp();
    this->appMetrics = handler->getAppMetrics();

    flags = new worldGame3D::gw::Flags();
    capabilities = new worldGame3D::gw::Capabilities();
}

void GWBase::startLevelPlay() {
	isLevelStarted = true;
}

void GWBase::setIsPlayEnabled(bool isPlayEnabled) {
	if (this->isPlayEnabled == isPlayEnabled) {
		return;
	}

	this->isPlayEnabled = isPlayEnabled;
	isPlayEnabled_changedEvent();

    dispatchEvent(*GWEvent_isPlayEnabled_changed);
}

void GWBase::setCb_onGamePartsCreated(std::function<void(IGameWorld* gameWorld)> cb_onGamePartsCreated) {
    this->cb_onGamePartsCreated = cb_onGamePartsCreated;
}

IApp* GWBase::getApp() {
    return app;
}

IAppAssets* GWBase::getAppAssets() {
    return app;
}

void GWBase::isPlayEnabled_changedEvent() {
	//void
}

bool GWBase::getIsEnabled() {
    if(isEnabledCounter < 1) {
        return false;
    }

    return true;
}

void GWBase::setIsEnabledIncremental(bool isEnabled) {
    if(!isEnabled) {
        isEnabledCounter -= 1;
    }else {
        if(isEnabledCounter >= 1) {
            // Value will go out of range.

            throw LogicException(LOC);
        }

        isEnabledCounter += 1;
    }

    if(isEnabledCounter == 0 || isEnabledCounter == 1) {
        onIsEnabledChanged();
    }
}

void GWBase::onAppBecomingInnactive() {
	dispatchEvent(*GWEvent_saveData);
}

void GWBase::prepareForDisposal() {
	dispatchEvent(*GWEvent_saveData);
	dispatchEvent(*GWEvent_disposePre_contentA);
}

void GWBase::onIsEnabledChanged() {
    if(!getIsEnabled()) {
        if(activeRenderableId >= 0) {
            handler->getRenderingHandler()->removeRenderableMustExist(activeRenderableId);

			activeRenderableId = -1;
        }
    }else {
        if(activeRenderableId < 0) {
			activeRenderableId = handler->getRenderingHandler()->addRenderable(
                getGPMain()->getStage3D(), getGPMain()->getEnvironment3D(), getGPMain()->getRenderer3D(), getGPMain()->getRenderer3D(),
                IRenderingHandler::RenderOrderGroup::ROG_3D, IRenderingHandler::RendererProfilerType::RPT_3D
            );
        }
    }
}

void GWBase::create() {
    super::create();

    createFramework();
    createGParts();
    
    if (cb_onGamePartsCreated) {
        cb_onGamePartsCreated(this);
    }

    createGPartsPost();
    int i;
    if((i = createLayers()) != 0) {
        throw LogicException(LOC);
    }
    if((i = createComponents()) != 0) {
        throw LogicException(LOC);
    }
    if((i = createComponentsPost()) != 0) {
        throw LogicException(LOC);
    }
    if((i = createContent()) != 0) {
        throw LogicException(LOC);
    }

    handler->getApp()->getLoopDistribution()->addEventListener(
        base::ILoopDistribution::AppLoopEvent_gameWorld->type,
        std::bind(&GWBase::onLoopEvent_appLoop_gameWorld, this, std::placeholders::_1, std::placeholders::_2),
        localAutoListenersList
    );
}

void GWBase::createFramework() {
    //void
}

void GWBase::createGParts() {
    //void
}

void GWBase::createGPartsPost() {
    //void
}

int GWBase::createLayers() {
    //void

    return 0;
}

int GWBase::createComponents() {
    //void

    return 0;
}

int GWBase::createComponentsPost() {
    dispatchEvent(*GWEvent_createComponentsPost);

    return 0;
}

int GWBase::createContent() {
    //void

    return 0;
}

void GWBase::createPost() {
    super::createPost();

	dispatchEvent(*GWEvent_createContent);

    createServerConnection();
}

int GWBase::createServerConnection() {
    //void

    return 0;
}

base::TypedEventDispatcher1D<base::GWEvent>* GWBase::getEventDispatcher() {
    return &eventDispatcher;
}

std::shared_ptr<IEventListener> GWBase::addEventListener(
    int type,
    std::function<void(IEventListener& eventListener, base::GWEvent& event)> cb)
{
    return eventDispatcher.getDispatcher(type, true)->addListener(cb);
}

void GWBase::addEventListener(
    int type,
    std::function<void(IEventListener& eventListener, base::GWEvent& event)> cb,
    ArrayList<std::shared_ptr<IEventListener>>& autoListenersList)
{
    eventDispatcher.getDispatcher(type, true)->addListener(cb, &autoListenersList);
}

void GWBase::addEventListener(
    int type1, int type2,
    std::function<void(IEventListener& eventListener, base::GWEvent& event)> cb,
    ArrayList<std::shared_ptr<IEventListener>>& autoListenersList)
{
    addEventListener(type1, cb, autoListenersList);
    addEventListener(type2, cb, autoListenersList);
}

void GWBase::addEventListener(
    int type1, int type2, int type3,
    std::function<void(IEventListener& eventListener, base::GWEvent& event)> cb,
    ArrayList<std::shared_ptr<IEventListener>>& autoListenersList)
{
    addEventListener(type1, cb, autoListenersList);
    addEventListener(type2, cb, autoListenersList);
    addEventListener(type3, cb, autoListenersList);
}

int GWBase::dispatchEvent(base::GWEvent& event) {
    int ct = 0;

    base::EventDispatcher<base::GWEvent>* dis = eventDispatcher.getDispatcher(event.type, false);
    if(dis != nullptr) {
        ct += dis->dispatch(event);
    }

    event.clearData();

    return ct;
}

int GWBase::dispatchEvents(ArrayList<base::GWEvent*>* eventsList) {
    int ct = 0;
    if(eventsList != nullptr) {
        for(int i=0;i<eventsList->count();i++) {
            base::GWEvent& event = *eventsList->getDirect(i);

            ct += dispatchEvent(event);
        }
    }

    return ct;
}

IHandler_GameWorld* GWBase::getHandler() {
    return handler;
}

//asdA36;// replace all loops to the events system and remove the local functions.

void GWBase::onLoopEvent_appLoop_gameWorld(IEventListener& eventListener, base::LoopEvent& event) {
    // The app loop is converted to a game loop. Game pause mechanisms is implemented here.
    if(isPlayEnabled && getIsEnabled()) {
        gameTimeS += event.deltaS;

		dispatchEvent(*GWEvent_gameLoop_netInput->setLoopData(event.deltaS, event.deltaMS, gameTimeS));
		dispatchEvent(*GWEvent_gameLoop_sync->setLoopData(event.deltaS, event.deltaMS, gameTimeS));

		dispatchEvent(*GWEvent_gameLoop_localInput->setLoopData(event.deltaS, event.deltaMS, gameTimeS));
		dispatchEvent(*GWEvent_gameLoop_localInputPost->setLoopData(event.deltaS, event.deltaMS, gameTimeS));

		dispatchEvent(*GWEvent_gameLoop_worldLogic->setLoopData(event.deltaS, event.deltaMS, gameTimeS));

		dispatchEvent(*GWEvent_gameLoop_contentCreation->setLoopData(event.deltaS, event.deltaMS, gameTimeS));
		
		dispatchEvent(*GWEvent_gameLoop_aiLogic->setLoopData(event.deltaS, event.deltaMS, gameTimeS));

		dispatchEvent(*GWEvent_gameLoop_actionsProcessingSimulation->setLoopData(event.deltaS, event.deltaMS, gameTimeS));
		dispatchEvent(*GWEvent_gameLoop_prePhysicsSimulation->setLoopData(event.deltaS, event.deltaMS, gameTimeS));
		dispatchEvent(*GWEvent_gameLoop_physicsSimulation->setLoopData(event.deltaS, event.deltaMS, gameTimeS));
		dispatchEvent(*GWEvent_gameLoop_simulation->setLoopData(event.deltaS, event.deltaMS, gameTimeS));
		dispatchEvent(*GWEvent_gameLoop_simulationB->setLoopData(event.deltaS, event.deltaMS, gameTimeS));
		dispatchEvent(*GWEvent_gameLoop_simulationC->setLoopData(event.deltaS, event.deltaMS, gameTimeS));
        
		dispatchEvent(*GWEvent_gameLoop_entityInteractions->setLoopData(event.deltaS, event.deltaMS, gameTimeS));
		dispatchEvent(*GWEvent_gameLoop_entitiesRemoval->setLoopData(event.deltaS, event.deltaMS, gameTimeS));

		dispatchEvent(*GWEvent_gameLoop_visualsUpdate->setLoopData(event.deltaS, event.deltaMS, gameTimeS));
		dispatchEvent(*GWEvent_gameLoop_frustumCulling3DUpdate->setLoopData(event.deltaS, event.deltaMS, gameTimeS));
		dispatchEvent(*GWEvent_gameLoop_visualsCacheUpdate->setLoopData(event.deltaS, event.deltaMS, gameTimeS));

		dispatchEvent(*GWEvent_gameLoop_actionsSending->setLoopData(event.deltaS, event.deltaMS, gameTimeS));

		dispatchEvent(*GWEvent_gameLoop_ui->setLoopData(event.deltaS, event.deltaMS, gameTimeS));

		dispatchEvent(*GWEvent_gameLoop_post->setLoopData(event.deltaS, event.deltaMS, gameTimeS));

		// Do the camera after pos because post may do some player unit hard-repositioning (eg: Rogue Zones zone change).
		dispatchEvent(*GWEvent_gameLoop_camera->setLoopData(event.deltaS, event.deltaMS, gameTimeS));
    }

    handler->loopFinished();
}

worldGame3D::gw::IFlags& GWBase::getFlags() {
    return *flags;
}

worldGame3D::gw::ICapabilities& GWBase::getCapabilities() {
    return *capabilities;
}

void GWBase::disposeMainPre() {
    //void

    super::disposeMainPre();
}

void GWBase::disposeMain() {
    if (flags != nullptr) {
        delete flags;
        flags = nullptr;
    }
    if (capabilities != nullptr) {
        delete capabilities;
        capabilities = nullptr;
    }

    /*asdB
    if(gameWorldContainer2D != null) {
        if(gameWorldContainer2D.getParent() != null) {
            gameWorldContainer2D.getParent().removeChild(gameWorldContainer2D);
        }

        gameWorldContainer2D = null;
    }

    for (Map.Entry<String, IDrawableNode2D> entry : visualsMap.entrySet()) {
        if(entry != null && entry.getValue() != null) {
            if(entry.getValue().getParent() != null) {
                entry.getValue().getParent().removeChild(entry.getValue());
            }
        }
    }*/

    super::disposeMain();
}

GWBase::~GWBase() {
	//void
}
