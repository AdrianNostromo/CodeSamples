#pragma once

#include <base/gh.h>
#include "IZoneEditorModule.h"
#include <worldGame3D/gw/entity/module/EntityModule.h>
#include <base/input/util/Touch.h>

namespace rpg3D {
	class ZoneEditorTemplate;
	class CbListenerZoneEditorView;
	class IHandlerZoneEditorModule;
	class IPhysics3DModule;
};

namespace rpg3D {
namespace zoneEditorModule {
class ZEM_Base : public EntityModule, virtual public IZoneEditorModule {priv typedef EntityModule super;pub dCtor(ZEM_Base);
	pub ZoneEditorTemplate* t;

	priv CbListenerZoneEditorView* listener_zoneEditorView = nullptr;

	prot IHandlerZoneEditorModule* handler = nullptr;

	priv rpg3D::IPhysics3DModule* physics3DModule = nullptr;

	prot IZoneEditorView_Edit* zoneEditorView = nullptr;
	priv std::shared_ptr<graphics::ICinematicController> cinematicController = nullptr;

	priv ArrayList<std::shared_ptr<IEventListener>> localAutoListenersList{};

	priv float postPlayerFallFreezeTimerS = 0.0f;
	priv Vector3 postPlayerFallFreezePos{};

    pub explicit ZEM_Base(
		IWorldEntity* entity,
		ZoneEditorTemplate* t);

	pub IWorldEntity* getEntity() final;

	priv void zoneEditorView_onBtn(IZoneEditorView_Edit* target, int uiButtonId);

	pub void startEdit(
		IHandlerZoneEditorModule* handler,
		IZoneEditorView_Edit* zoneEditorView,
		std::shared_ptr<graphics::ICinematicController> cinematicController) final;
	prot virtual void onStartEditPost();
	pub void endEditIfAny() final;
	prot virtual void onEndEditPre();
	prot virtual void onEndEditPost();

	priv void onGWEvent_gameLoop_simulation(IEventListener& eventListener, base::GWEvent& event);

	pub std::shared_ptr<graphics::ICinematicController> getCinematicController() final;

    pub ~ZEM_Base() override;
};
};
};
