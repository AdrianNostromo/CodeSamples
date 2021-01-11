#pragma once

#include <base/gh.h>
#include "GPUEC_DirectMovement2D.h"
#include <base/MM.h>

namespace rpg3D {
class GPUEC_LockedCursorMoved : public GPUEC_DirectMovement2D {priv typedef GPUEC_DirectMovement2D super; pub dCtor(GPUEC_LockedCursorMoved);
	priv class CursorHandlingData {pub dCtor(CursorHandlingData);
		pub int cursorIndex;

		// This is used on move,drag updates when there is no_new_data (eg. force_update, camera_update, unit_pos_change).
		pub std::shared_ptr<Vector2> cacheScreenPos = nullptr;

		pub std::shared_ptr<Vector3> lastDispatched_move_unitPos = nullptr;
		pub int lastDispatched_move_cameraUpdateIndex = -1;

		pub std::shared_ptr<Vector3> lastDispatched_drag_Main_unitPos = nullptr;
		pub int lastDispatched_drag_Main_cameraUpdateIndex = -1;

		pub std::shared_ptr<Vector3> lastDispatched_drag_Alternative_unitPos = nullptr;
		pub int lastDispatched_drag_Alternative_cameraUpdateIndex = -1;

		pub bool isData_move = false;
		pub Vector2 data_move_screenPos{};
		pub Vector2 data_move_lockedCursorSensitisedDelta{};

		pub bool isData_drag_Main = false;
		pub Vector2 data_drag_Main_screenPos{};
		pub Vector2 data_drag_Main_lockedCursorSensitisedDelta{};

		pub bool isData_drag_Alternative = false;
		pub Vector2 data_drag_Alternative_screenPos{};
		pub Vector2 data_drag_Alternative_lockedCursorSensitisedDelta{};

		pub explicit CursorHandlingData(int cursorIndex)
		: cursorIndex(cursorIndex)
		{}

		pub void onData_move(Vector2& screenPos, Vector2& lockedCursorSensitisedDelta) {
			isData_move = true;
			data_move_screenPos.set(screenPos);
			data_move_lockedCursorSensitisedDelta.set(lockedCursorSensitisedDelta);
		};

		pub void onData_drag_Main(Vector2& screenPos, Vector2& lockedCursorSensitisedDelta) {
			isData_drag_Main = true;
			data_drag_Main_screenPos.set(screenPos);
			data_drag_Main_lockedCursorSensitisedDelta.set(lockedCursorSensitisedDelta);
		};

		pub void onData_drag_Alternative(Vector2& screenPos, Vector2& lockedCursorSensitisedDelta) {
			isData_drag_Alternative = true;
			data_drag_Alternative_screenPos.set(screenPos);
			data_drag_Alternative_lockedCursorSensitisedDelta.set(lockedCursorSensitisedDelta);
		};

		pub virtual ~CursorHandlingData() {}
	};

	priv ArrayList<std::shared_ptr<CursorHandlingData>> cursorHandlingDataList{};

	// This are highest priority first sorted.
	priv ListDL<IWorldInteractionFilter*> customFiltersLists_LockedCursor_Main_Down{LOC};
	priv ListDL<IWorldInteractionFilter*> customFiltersLists_LockedCursor_Alt_Down{LOC};

	priv ListDL<IWorldInteractionFilter*> customFiltersLists_LockedCursor_Main_Up{LOC};
	priv ListDL<IWorldInteractionFilter*> customFiltersLists_LockedCursor_Alt_Up{LOC};

	priv ListDL<IWorldInteractionFilter*> customFiltersLists_LockedCursor_Main_Drag{LOC};
	priv ListDL<IWorldInteractionFilter*> customFiltersLists_LockedCursor_Alt_Drag{LOC};

    pub explicit GPUEC_LockedCursorMoved(base::IGameWorld* gw);

	priv CursorHandlingData* getCursorHandlingData(int cursorIndex);

	prot bool processCacheInput(std::shared_ptr<base::SensorEvent>& input) override;

	pub void onGameLoop_localInput(float deltaS, int deltaMS, float gameTimeS) override;
	priv void onGameLoop_move(CursorHandlingData* cursorHD);
	priv void onGameLoop_drag_Main(CursorHandlingData* cursorHD);
	priv void onGameLoop_drag_Alternative(CursorHandlingData* cursorHD);

	priv void dispatchWorldInteractionFilters(CursorHandlingData* cursorHD, base::Touch::ButtonCode* buttonId, TouchEventType* touchType, Vector2& screenPos);

	priv void processAction_lockedCursorDown_Main(CursorHandlingData* cursorHD, base::Touch::ButtonCode* buttonId, TouchEventType* touchType, Vector2& screenPos);
	priv void processAction_lockedCursorDown_Main_B(CursorHandlingData* cursorHD, Vector2& screenPos);

	priv void processAction_lockedCursorDown_Alternative(CursorHandlingData* cursorHD, Vector2& screenPos);

	priv void tick_lockedCursorMove(
		bool isDirty_newData, bool isDirty_forceUpdate, bool isDirty_cameraUpdate, bool isDirty_unitPos,
		CursorHandlingData* cursorHD, int cursorIndex, Vector2& screenPos, Vector2& lockedCursorSensitisedDelta);
	
	priv void tick_lockedCursorDrag_Main(
		bool isDirty_newData, bool isDirty_forceUpdate, bool isDirty_cameraUpdate, bool isDirty_unitPos,
		CursorHandlingData* cursorHD, int cursorIndex, Vector2& screenPos, Vector2& lockedCursorSensitisedDelta);
	priv void tick_lockedCursorDrag_Main_B(
		bool isDirty_newData, bool isDirty_forceUpdate, bool isDirty_cameraUpdate, bool isDirty_unitPos,
		CursorHandlingData* cursorHD, int cursorIndex, Vector2& screenPos, Vector2& lockedCursorSensitisedDelta);
	priv void tick_lockedCursorDrag_Alternative(
		bool isDirty_newData, bool isDirty_forceUpdate, bool isDirty_cameraUpdate, bool isDirty_unitPos,
		CursorHandlingData* cursorHD, int cursorIndex, Vector2& screenPos, Vector2& lockedCursorSensitisedDelta);
	
	priv void processAction_lockedCursorUp_Main(int cursorIndex, Vector2& screenPos);
	priv void processAction_lockedCursorUp_Alternative(int cursorIndex, Vector2& screenPos);

	prot void onHookCustomFilter(rpg3D::IWorldInteractionFilter* filter) override;

	pub ~GPUEC_LockedCursorMoved() override;

};
};
