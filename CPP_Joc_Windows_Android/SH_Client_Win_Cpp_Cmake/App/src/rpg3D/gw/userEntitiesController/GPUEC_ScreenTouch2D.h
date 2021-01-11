#pragma once

#include <base/gh.h>
#include <rpg3D/gw/entity/module/userControlled/util/WorldOrEntityInteractionEvent.h>
#include <worldGame3D/gw/main/IGamePartMain.h>
#include "GPUEC_Camera.h"
#include <base/MM.h>

namespace base {
	class IScreen;
};

namespace rpg3D {
class GPUEC_ScreenTouch2D : public GPUEC_Camera {priv typedef GPUEC_Camera super; pub dCtor(GPUEC_ScreenTouch2D);
	priv class CursorHandlingData {pub dCtor(CursorHandlingData);
		priv static const float AUTO_REMOVE_TIMER_S;
		pub int cursorIndex;

		// This is used to remove cursor that are iddle soe they are not used to spam filters.
		pub float autoRemoveTimerS = 0.0f;

		// This is used on move,drag updates when there is no_new_data (eg. force_update, camera_update, unit_pos_change).
		pub std::shared_ptr<Vector2> cacheScreenPos = nullptr;

		pub std::shared_ptr<Vector2> lastDispatched_move_screenPos = nullptr;
		pub std::shared_ptr<Vector3> lastDispatched_move_unitPos = nullptr;
		pub int lastDispatched_move_cameraUpdateIndex = -1;

		// This is also set on touth_down.
		pub std::shared_ptr<Vector2> lastDispatched_drag_Main_screenPos = nullptr;
		pub std::shared_ptr<Vector3> lastDispatched_drag_Main_unitPos = nullptr;
		pub int lastDispatched_drag_Main_cameraUpdateIndex = -1;

		pub std::shared_ptr<Vector2> lastDispatched_drag_Alternative_screenPos = nullptr;
		pub std::shared_ptr<Vector3> lastDispatched_drag_Alternative_unitPos = nullptr;
		pub int lastDispatched_drag_Alternative_cameraUpdateIndex = -1;

		pub bool isData_move = false;
		pub Vector2 data_move_screenPos{};

		pub bool isData_drag_Main = false;
		pub Vector2 data_drag_Main_screenPos{};

		pub bool isData_drag_Alternative = false;
		pub Vector2 data_drag_Alternative_screenPos{};

		pub explicit CursorHandlingData(int cursorIndex)
		: cursorIndex(cursorIndex)
		{}

		pub void onData_move(Vector2& screenPos) {
			isData_move = true;
			data_move_screenPos.set(screenPos);

			autoRemoveTimerS = AUTO_REMOVE_TIMER_S;
		};

		pub void onData_drag_Main(Vector2& screenPos) {
			isData_drag_Main = true;
			data_drag_Main_screenPos.set(screenPos);

			autoRemoveTimerS = AUTO_REMOVE_TIMER_S;
		};

		pub void onData_drag_Alternative(Vector2& screenPos) {
			isData_drag_Alternative = true;
			data_drag_Alternative_screenPos.set(screenPos);

			autoRemoveTimerS = AUTO_REMOVE_TIMER_S;
		};

		pub virtual ~CursorHandlingData() {
			//void
		}
	};

	// This contains nullptr entries when they expire or cursor indexes are skipped by input.
	priv ArrayList<std::shared_ptr<CursorHandlingData>> cursorHandlingDataList{};
	
	// This are highest priority first sorted.
	priv ListDL<IWorldInteractionFilter*> customFiltersLists_ScreenCursor_Main_Down{LOC};
	priv ListDL<IWorldInteractionFilter*> customFiltersLists_ScreenCursor_Alt_Down{LOC};

	priv ListDL<IWorldInteractionFilter*> customFiltersLists_ScreenCursor_Main_Up{LOC};
	priv ListDL<IWorldInteractionFilter*> customFiltersLists_ScreenCursor_Alt_Up{LOC};
	
	priv ListDL<IWorldInteractionFilter*> customFiltersLists_ScreenCursor_Main_Drag{LOC};
	priv ListDL<IWorldInteractionFilter*> customFiltersLists_ScreenCursor_Alt_Drag{LOC};

	priv base::IScreen* screen = nullptr;

    pub explicit GPUEC_ScreenTouch2D(base::IGameWorld* gw);
	prot void createMain() override;

	priv CursorHandlingData* getOrCreateCursorHandlingData(int cursorIndex);

	prot bool processCacheInput(std::shared_ptr<base::SensorEvent>& input) override;

	pub void onGameLoop_localInput(float deltaS, int deltaMS, float gameTimeS) override;
	priv void onGameLoop_move(CursorHandlingData* cursorHD);
	priv void onGameLoop_drag_Main(CursorHandlingData* cursorHD);
	priv void onGameLoop_drag_Alternative(CursorHandlingData* cursorHD);

	priv void dispatchWorldInteractionFilters(CursorHandlingData* cursorHD, base::Touch::ButtonCode* buttonId, TouchEventType* touchType, Vector2& screenPos);

	priv void processAction_cursorDown_Main(CursorHandlingData* cursorHD, base::Touch::ButtonCode* buttonId, TouchEventType* touchType, Vector2& screenPos);
	priv void processAction_cursorDown_Main_B(CursorHandlingData* cursorHD, Vector2& screenPos);
	priv void processAction_cursorDown_Alternative(CursorHandlingData* cursorHD, Vector2& screenPos);
	
	priv void tick_cursorMove(
		bool isDirty_newData, bool isDirty_forceUpdate, bool isDirty_cameraUpdate, bool isDirty_unitPos,
		CursorHandlingData* cursorHD, int cursorIndex, Vector2& screenPos, Vector2& mousePosDelta, Vector2& mousePosDelta_sensitised);
	priv void tick_cursorDrag_Main(
		bool isDirty_newData, bool isDirty_forceUpdate, bool isDirty_cameraUpdate, bool isDirty_unitPos,
		CursorHandlingData* cursorHD, int cursorIndex, Vector2& screenPos, Vector2& mousePosDelta, Vector2& mousePosDelta_sensitised);
	priv void tick_cursorDrag_Main_B(
		bool isDirty_newData, bool isDirty_forceUpdate, bool isDirty_cameraUpdate, bool isDirty_unitPos,
		CursorHandlingData* cursorHD, int cursorIndex, Vector2& screenPos, Vector2& mousePosDelta);
	priv void tick_cursorDrag_Alternative(
		bool isDirty_newData, bool isDirty_forceUpdate, bool isDirty_cameraUpdate, bool isDirty_unitPos,
		CursorHandlingData* cursorHD, int cursorIndex, Vector2& screenPos, Vector2& mousePosDelta, Vector2& mousePosDelta_sensitised);

	priv void processAction_cursorUp_Main(int cursorIndex, Vector2& screenPos);
	priv void processAction_cursorUp_Alternative(int cursorIndex, Vector2& screenPos);

	prot void onHookCustomFilter(rpg3D::IWorldInteractionFilter* filter) override;

	prot void onControlledEntityChanged() override;

	pub void clearInput() override;

	pub ~GPUEC_ScreenTouch2D() override;
};
};
