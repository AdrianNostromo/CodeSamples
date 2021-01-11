#pragma once

#include <base/gh.h>
#include <string>
#include <base/math/Vector3.h>
#include <base/math/Vector2.h>
#include "IWorldOrEntityInteractionFilter.h"
#include <memory>

class IWorldEntity;
namespace base {
	class WrappedWorldEntity;
};

namespace rpg3D {
class WorldOrEntityInteractionEvent {pub dCtor(WorldOrEntityInteractionEvent);
	pub int type;
	// This is used to allow some tools (generally auto activation tools) to block filter interactions using a mouse or key action index.
	// Mouse move events use a -1 for this and is not blockable.
	// >= 0;// Value usable for blocking.
	// < 0;// Value not used for blocking (eg: mouse_move events).
	pub int triggerEventIndex;

	pub IWorldOrEntityInteractionFilter* filter = nullptr;

	pub Vector3 worldPos{};
	pub Vector3 worldDir{};

	pub IWorldEntity* targetEntity;
	pub std::shared_ptr<base::WrappedWorldEntity> targetWEntity;

	pub Vector2 moveDirVector{};

	pub float globalRotZDeg;

	pub bool isSource_cursor;

	pub float strafeLeftRightMoveVector;
	pub float backFrontMoveVector;

	pub Vector2 sensitisedViewPanDelta;
	
	pub float angZDeg;
	pub float angElevationDeg;

	pub Vector2 screenCursorPos{};
	pub Vector2 screenCursorMoveDelta{};
	// This can be used to rotate something in a consistent way accross resolutions, densities, screen_sizes.
	pub Vector2 screenCursorMoveDelta_sensitised{};
	pub Vector2 lockedCursorSensitisedDelta{};
	pub int cursorIndex = -1;

	pub float continuousFocusView3DYaw_yawDegDelta;

	pub Vector3 cameraDirection{};

	pub int toolInstanceId;
	pub int toolExtraControlSourceId;

    pub explicit WorldOrEntityInteractionEvent(int type, int triggerEventIndex);

    pub static std::shared_ptr<WorldOrEntityInteractionEvent> getNew_GroundDown(int triggerEventIndex, int cursorIndex, Vector3& worldPos);
    pub static std::shared_ptr<WorldOrEntityInteractionEvent> getNew_GroundUp(int triggerEventIndex, int cursorIndex, Vector3& worldPos);
    pub static std::shared_ptr<WorldOrEntityInteractionEvent> getNew_GroundMove(int triggerEventIndex, int cursorIndex, Vector3& worldPos);
	pub static std::shared_ptr<WorldOrEntityInteractionEvent> getNew_EntityDown(int triggerEventIndex, int cursorIndex, IWorldEntity* targetEntity);
	pub static std::shared_ptr<WorldOrEntityInteractionEvent> getNew_EntityUp(int triggerEventIndex, int cursorIndex, IWorldEntity* targetEntity);

	pub static std::shared_ptr<WorldOrEntityInteractionEvent> getNew_GlobalFocusDirection2D(int triggerEventIndex, float globalRotZDeg, bool isSource_cursor);
	pub static std::shared_ptr<WorldOrEntityInteractionEvent> getNew_GlobalFocusDirection2DRelativeToUnit(int triggerEventIndex, float globalRotZDeg);

	pub static std::shared_ptr<WorldOrEntityInteractionEvent> getNew_ContinuousFocusView3D_Absolute(int triggerEventIndex, float angZDeg, float angElevationDeg);
	pub static std::shared_ptr<WorldOrEntityInteractionEvent> getNew_ContinuousFocusView3DElevation_Absolute(int triggerEventIndex, float angElevationDeg);
	pub static std::shared_ptr<WorldOrEntityInteractionEvent> getNew_ContinuousFocusView3D_Delta(int triggerEventIndex, Vector2& sensitisedViewPanDelta);

	pub static std::shared_ptr<WorldOrEntityInteractionEvent> getNew_LockedCursor_Down_Main(int triggerEventIndex, int cursorIndex, Vector2& screenCursorPos);
	pub static std::shared_ptr<WorldOrEntityInteractionEvent> getNew_LockedCursor_Down_Alternative(int triggerEventIndex, int cursorIndex, Vector2& screenCursorPos);
	
	pub static std::shared_ptr<WorldOrEntityInteractionEvent> getNew_LockedCursor_Move(int triggerEventIndex, int cursorIndex, Vector2& screenCursorPos, Vector2& lockedCursorSensitisedDelta);
	
	pub static std::shared_ptr<WorldOrEntityInteractionEvent> getNew_LockedCursor_Drag_Main(int triggerEventIndex, Vector2& screenCursorPos, Vector2& lockedCursorSensitisedDelta);
	pub static std::shared_ptr<WorldOrEntityInteractionEvent> getNew_LockedCursor_Drag_Alternative(int triggerEventIndex, Vector2& screenCursorPos, Vector2& lockedCursorSensitisedDelta);

	pub static std::shared_ptr<WorldOrEntityInteractionEvent> getNew_LockedCursor_Up_Main(int triggerEventIndex, int cursorIndex, Vector2& screenCursorPos);
	pub static std::shared_ptr<WorldOrEntityInteractionEvent> getNew_LockedCursor_Up_Alternative(int triggerEventIndex, int cursorIndex, Vector2& screenCursorPos);

	pub static std::shared_ptr<WorldOrEntityInteractionEvent> getNew_GlobalMoveDirection2D(int triggerEventIndex, Vector2& moveDirVector);

	pub static std::shared_ptr<WorldOrEntityInteractionEvent> getNew_MovementDPad(int triggerEventIndex, float strafeLeftRightMoveVector, float backFrontMoveVector);
	pub static std::shared_ptr<WorldOrEntityInteractionEvent> getNew_MovementDPadBackFront(int triggerEventIndex, float backFrontMoveVector);
	pub static std::shared_ptr<WorldOrEntityInteractionEvent> getNew_MovementDPadLeftRight(int triggerEventIndex, float strafeLeftRightMoveVector);

	pub static std::shared_ptr<WorldOrEntityInteractionEvent> getNew_ScreenCursor_Down_Main(int triggerEventIndex, int cursorIndex, Vector2& screenCursorPos);
	pub static std::shared_ptr<WorldOrEntityInteractionEvent> getNew_ScreenCursor_Down_Alternative(int triggerEventIndex, int cursorIndex, Vector2& screenCursorPos);

	pub static std::shared_ptr<WorldOrEntityInteractionEvent> getNew_ScreenCursor_Move(
		int triggerEventIndex, int cursorIndex, 
		Vector2& screenCursorPos, Vector2& screenCursorMoveDelta, Vector2& screenCursorMoveDelta_sensitised);

	pub static std::shared_ptr<WorldOrEntityInteractionEvent> getNew_ScreenCursor_Drag_Main(
		int triggerEventIndex, int cursorIndex, 
		Vector2& screenCursorPos, Vector2& screenCursorMoveDelta, Vector2& screenCursorMoveDelta_sensitised);
	pub static std::shared_ptr<WorldOrEntityInteractionEvent> getNew_ScreenCursor_Drag_Alternative(
		int triggerEventIndex, int cursorIndex, 
		Vector2& screenCursorPos, Vector2& screenCursorMoveDelta, Vector2& screenCursorMoveDelta_sensitised);

	pub static std::shared_ptr<WorldOrEntityInteractionEvent> getNew_ScreenCursor_Up_Main(int triggerEventIndex, int cursorIndex, Vector2& screenCursorPos);
	pub static std::shared_ptr<WorldOrEntityInteractionEvent> getNew_ScreenCursor_Up_Alternative(int triggerEventIndex, int cursorIndex, Vector2& screenCursorPos);

	pub static std::shared_ptr<WorldOrEntityInteractionEvent> getNew_ContinuousFocusView3DYaw_Delta(int triggerEventIndex, float yawDegDelta);

	pub static std::shared_ptr<WorldOrEntityInteractionEvent> getNew_UnitFeetGroundPlanePos_1stPersonCamera_Down_MainBtn(int triggerEventIndex, int cursorIndex, Vector3& worldPos);
	pub static std::shared_ptr<WorldOrEntityInteractionEvent> getNew_UnitFeetGroundPlanePos_3rdPersonCamera_Down_MainBtn(int triggerEventIndex, int cursorIndex, Vector3& worldPos);
	pub static std::shared_ptr<WorldOrEntityInteractionEvent> getNew_UnitFeetHalfSphereDomeDir_1stPersonCamera_Down_MainBtn(int triggerEventIndex, int cursorIndex, Vector3& worldDir);
	pub static std::shared_ptr<WorldOrEntityInteractionEvent> getNew_UnitFeetHalfSphereDomeDir_3rdPersonCamera_Down_MainBtn(int triggerEventIndex, int cursorIndex, Vector3& worldDir);

	pub static std::shared_ptr<WorldOrEntityInteractionEvent> getNew_UnitFeetGroundPlanePos_1stPersonCamera_Drag_MainBtn(int triggerEventIndex, int cursorIndex, Vector3& worldPos);
	pub static std::shared_ptr<WorldOrEntityInteractionEvent> getNew_UnitFeetGroundPlanePos_3rdPersonCamera_Drag_MainBtn(int triggerEventIndex, int cursorIndex, Vector3& worldPos);
	pub static std::shared_ptr<WorldOrEntityInteractionEvent> getNew_UnitFeetHalfSphereDomeDir_1stPersonCamera_Drag_MainBtn(int triggerEventIndex, int cursorIndex, Vector3& worldDir);
	pub static std::shared_ptr<WorldOrEntityInteractionEvent> getNew_UnitFeetHalfSphereDomeDir_3rdPersonCamera_Drag_MainBtn(int triggerEventIndex, int cursorIndex, Vector3& worldDir);
	
	pub static std::shared_ptr<WorldOrEntityInteractionEvent> getNew_CameraDirection(int triggerEventIndex, Vector3& cameraDirection);

	pub static std::shared_ptr<WorldOrEntityInteractionEvent> getNew_ToolExtraActivation_ListControl_Down_MainBtn(int triggerEventIndex, int toolInstanceId, int toolExtraControlSourceId);
	pub static std::shared_ptr<WorldOrEntityInteractionEvent> getNew_ToolExtraActivation_SecondaryControl_Down_MainBtn(int triggerEventIndex, int toolInstanceId, int toolExtraControlSourceId);

	pub virtual ~WorldOrEntityInteractionEvent();
};
};
