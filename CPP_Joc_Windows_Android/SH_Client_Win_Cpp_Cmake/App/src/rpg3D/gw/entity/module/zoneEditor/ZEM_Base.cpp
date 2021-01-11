#include "ZEM_Base.h"
#include <rpg3D/gw/entity/template/zoneEditor/ZoneEditorTemplate.h>
#include <rpg3D/gw/entity/module/editableVoxelised/IEditableVoxelisedM_Edit.h>
#include <rpg3D/gw/view/zoneEditor/IZoneEditorView_Edit.h>
#include <rpg3D/gw/view/zoneEditor/util/CbListenerZoneEditorView.h>
#include <rpg3D/gw/entity/module/zoneEditor/util/IHandlerZoneEditorModule.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <worldGame3D/gw/entity/event/IPluggableEventsDispatcher.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <rpg3D/gw/entity/module/physics3D/IPhysics3DModule.h>
#include <cassert>
#include <rpg3D/gw/manager/IGPManager.h>

using namespace rpg3D;
using namespace rpg3D::zoneEditorModule;

ZEM_Base::ZEM_Base(
	IWorldEntity* entity,
	ZoneEditorTemplate* t)
	: super(t->moduleName, entity),
	t(t)
{
	listener_zoneEditorView = new CbListenerZoneEditorView();
	listener_zoneEditorView->cb_onBtn = std::bind(&ZEM_Base::zoneEditorView_onBtn, this, std::placeholders::_1, std::placeholders::_2);
}

IWorldEntity* ZEM_Base::getEntity() {
	return e;
}

void ZEM_Base::zoneEditorView_onBtn(IZoneEditorView_Edit* target, int uiButtonId) {
	if (uiButtonId == IListenerZoneEditorView::ButtonIds::CLOSE) {
		if (handler != nullptr) {
			handler->zoneEditor_requestStopEditor();
		}
	}

	else {
		throw LogicException(LOC);
	}
}

void ZEM_Base::startEdit(
	IHandlerZoneEditorModule* handler,
	IZoneEditorView_Edit* zoneEditorView,
	std::shared_ptr<graphics::ICinematicController> cinematicController)
{
	if (this->handler != nullptr) {
		// Edit mode already active.
		throw LogicException(LOC);
	}
	
	if (zoneEditorView == nullptr || handler == nullptr) {
		// Parameters error.
		throw LogicException(LOC);
	}

	this->handler = handler;
	physics3DModule = entity->getComponentAs<rpg3D::IPhysics3DModule*>(true/*mustExist*/);
	this->zoneEditorView = zoneEditorView;
	this->cinematicController = cinematicController;
	postPlayerFallFreezeTimerS = 0.0f;
	entity->getGW()->getManagerAs<rpg3D::IGPManager*>()->setIsPlatformsActivationEnabled(false);

	e->getGWEvents()->addEventListener(
		base::IGameWorld::GWEvent_gameLoop_simulation->type,
		std::bind(&ZEM_Base::onGWEvent_gameLoop_simulation, this, std::placeholders::_1, std::placeholders::_2),
		localAutoListenersList
	);

	this->zoneEditorView->setHandlerAndListener(this, listener_zoneEditorView);

	onStartEditPost();
}

void ZEM_Base::onStartEditPost() {
	//void;
}

void ZEM_Base::endEditIfAny() {
	onEndEditPre();

	localAutoListenersList.clear();

	handler = nullptr;
	physics3DModule = nullptr;
	zoneEditorView = nullptr;
	postPlayerFallFreezeTimerS = 0.0f;
	entity->getGW()->getManagerAs<rpg3D::IGPManager*>()->setIsPlatformsActivationEnabled(true);

	onEndEditPost();
}

void ZEM_Base::onEndEditPre() {
	//void;
}

void ZEM_Base::onEndEditPost() {
	//void;
}

void ZEM_Base::onGWEvent_gameLoop_simulation(IEventListener& eventListener, base::GWEvent& event) {
	if (entity->getIsMarkedForRemoval() || handler == nullptr) {
		return;
	}

	if (e->getZ() < -1.5f) {
		postPlayerFallFreezePos.set(
			e->getX(),
			e->getY(),
			1.0f
		);

		postPlayerFallFreezeTimerS = 2.0f;
	}

	if(postPlayerFallFreezeTimerS > 0.0f) {
		postPlayerFallFreezeTimerS -= event.deltaS;

		physics3DModule->clearAllForces();
		e->setPos(postPlayerFallFreezePos);
	}
}

std::shared_ptr<graphics::ICinematicController> ZEM_Base::getCinematicController() {
	return cinematicController;
}

ZEM_Base::~ZEM_Base() {
	if (handler != nullptr) {
		assert(false);
	}
	if (listener_zoneEditorView != nullptr) {
		delete listener_zoneEditorView;
		listener_zoneEditorView = nullptr;
	}
}
