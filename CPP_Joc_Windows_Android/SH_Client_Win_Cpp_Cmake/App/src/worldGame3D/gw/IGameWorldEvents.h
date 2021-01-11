#pragma once

#include <base/gh.h>
#include "IGameWorldGeneral.h"
#include <base/event/dispatcher/TypedEventDispatcher1D.h>
#include <memory>

class IEventListener;
namespace base {
	class GWEvent;
};

namespace worldGame3D {
class IGameWorldEvents : public IGameWorldGeneral {
	pub static base::GWEvent* GWEvent_createComponentsPost;
	pub static base::GWEvent* GWEvent_createContent;

	pub static base::GWEvent* GWEvent_isPlayEnabled_changed;

	pub static base::GWEvent* GWEvent_dispose_views;
	pub static base::GWEvent* GWEvent_saveData;
	// This is the persistent entities.
	pub static base::GWEvent* GWEvent_disposePre_contentA;
	pub static base::GWEvent* GWEvent_zoneDeactivate_pre;

	pub static base::GWEvent* GWEvent_gameLoop_netInput;
	pub static base::GWEvent* GWEvent_gameLoop_sync;

	pub static base::GWEvent* GWEvent_gameLoop_localInput;
	pub static base::GWEvent* GWEvent_gameLoop_localInputPost;

	pub static base::GWEvent* GWEvent_gameLoop_worldLogic;
	
	pub static base::GWEvent* GWEvent_gameLoop_contentCreation;
	
	pub static base::GWEvent* GWEvent_gameLoop_aiLogic;
	
	pub static base::GWEvent* GWEvent_gameLoop_actionsProcessingSimulation;
	pub static base::GWEvent* GWEvent_gameLoop_prePhysicsSimulation;
	pub static base::GWEvent* GWEvent_gameLoop_physicsSimulation;
	pub static base::GWEvent* GWEvent_gameLoop_simulation;
	pub static base::GWEvent* GWEvent_gameLoop_simulationB;
	pub static base::GWEvent* GWEvent_gameLoop_simulationC;

	pub static base::GWEvent* GWEvent_gameLoop_entityInteractions;
	pub static base::GWEvent* GWEvent_gameLoop_entitiesRemoval;
	
	pub static base::GWEvent* GWEvent_gameLoop_visualsUpdate;
	pub static base::GWEvent* GWEvent_gameLoop_frustumCulling3DUpdate;
	pub static base::GWEvent* GWEvent_gameLoop_visualsCacheUpdate;
	
	pub static base::GWEvent* GWEvent_gameLoop_camera;
	
	pub static base::GWEvent* GWEvent_gameLoop_actionsSending;
	
	pub static base::GWEvent* GWEvent_gameLoop_ui;
	
	pub static base::GWEvent* GWEvent_gameLoop_post;

	pub virtual base::TypedEventDispatcher1D<base::GWEvent>* getEventDispatcher() = 0;
	pub virtual std::shared_ptr<IEventListener> addEventListener(
		int type,
		std::function<void(IEventListener& eventListener, base::GWEvent& event)> cb) = 0;
	pub virtual void addEventListener(
		int type,
		std::function<void(IEventListener& eventListener, base::GWEvent& event)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList) = 0;
	pub virtual void addEventListener(
		int type1, int type2,
		std::function<void(IEventListener& eventListener, base::GWEvent& event)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList) = 0;
	pub virtual void addEventListener(
		int type1, int type2, int type3,
		std::function<void(IEventListener& eventListener, base::GWEvent& event)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList) = 0;
	pub virtual int dispatchEvent(base::GWEvent& event) = 0;
	pub virtual int dispatchEvents(ArrayList<base::GWEvent*>* eventsList) = 0;

    pub ~IGameWorldEvents() override = default;
};
};
