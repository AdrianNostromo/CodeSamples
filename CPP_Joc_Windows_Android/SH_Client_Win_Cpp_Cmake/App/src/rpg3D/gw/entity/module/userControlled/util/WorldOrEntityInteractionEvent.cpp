#include "WorldOrEntityInteractionEvent.h"
#include <worldGame3D/gw/entity/IWorldEntity.h>

using namespace rpg3D;

WorldOrEntityInteractionEvent::WorldOrEntityInteractionEvent(int type, int triggerEventIndex)
	: type(type), triggerEventIndex(triggerEventIndex)
{
	//void
}

std::shared_ptr<WorldOrEntityInteractionEvent> WorldOrEntityInteractionEvent::getNew_GroundDown(int triggerEventIndex, int cursorIndex, Vector3& worldPos) {
	std::shared_ptr<WorldOrEntityInteractionEvent> ret = std::make_shared<WorldOrEntityInteractionEvent>(IWorldOrEntityInteractionFilter::Type::GroundDown, triggerEventIndex);

	ret->cursorIndex = cursorIndex;
	ret->worldPos.set(worldPos);

	return ret;
}

std::shared_ptr<WorldOrEntityInteractionEvent> WorldOrEntityInteractionEvent::getNew_GroundUp(int triggerEventIndex, int cursorIndex, Vector3& worldPos) {
	std::shared_ptr<WorldOrEntityInteractionEvent> ret = std::make_shared<WorldOrEntityInteractionEvent>(IWorldOrEntityInteractionFilter::Type::GroundUp, triggerEventIndex);

	ret->cursorIndex = cursorIndex;
	ret->worldPos.set(worldPos);

    return ret;
}

std::shared_ptr<WorldOrEntityInteractionEvent> WorldOrEntityInteractionEvent::getNew_GroundMove(int triggerEventIndex, int cursorIndex, Vector3& worldPos) {
	std::shared_ptr<WorldOrEntityInteractionEvent> ret = std::make_shared<WorldOrEntityInteractionEvent>(IWorldOrEntityInteractionFilter::Type::GroundMove, triggerEventIndex);

	ret->cursorIndex = cursorIndex;
	ret->worldPos.set(worldPos);

    return ret;
}

std::shared_ptr<WorldOrEntityInteractionEvent> WorldOrEntityInteractionEvent::getNew_EntityDown(int triggerEventIndex, int cursorIndex, IWorldEntity* targetEntity) {
	std::shared_ptr<WorldOrEntityInteractionEvent> ret = std::make_shared<WorldOrEntityInteractionEvent>(IWorldOrEntityInteractionFilter::Type::EntityDown, triggerEventIndex);

	ret->cursorIndex = cursorIndex;
	ret->targetEntity = targetEntity;
	ret->targetWEntity = targetEntity->getWrapper();

    return ret;
}	

std::shared_ptr<WorldOrEntityInteractionEvent> WorldOrEntityInteractionEvent::getNew_EntityUp(int triggerEventIndex, int cursorIndex, IWorldEntity* targetEntity) {
	std::shared_ptr<WorldOrEntityInteractionEvent> ret = std::make_shared<WorldOrEntityInteractionEvent>(IWorldOrEntityInteractionFilter::Type::EntityUp, triggerEventIndex);

	ret->cursorIndex = cursorIndex;
	ret->targetEntity = targetEntity;
	ret->targetWEntity = targetEntity->getWrapper();

    return ret;
}

std::shared_ptr<WorldOrEntityInteractionEvent> WorldOrEntityInteractionEvent::getNew_GlobalFocusDirection2D(int triggerEventIndex, float globalRotZDeg, bool isSource_cursor) {
	std::shared_ptr<WorldOrEntityInteractionEvent> ret = std::make_shared<WorldOrEntityInteractionEvent>(IWorldOrEntityInteractionFilter::Type::GlobalFocusDirection2D, triggerEventIndex);

	if (isnan(globalRotZDeg)) {
		// This occured when the window was minimised.
		throw LogicException(LOC);
	}

	ret->globalRotZDeg = globalRotZDeg;
	ret->isSource_cursor = isSource_cursor;

    return ret;
}

std::shared_ptr<WorldOrEntityInteractionEvent> WorldOrEntityInteractionEvent::getNew_GlobalFocusDirection2DRelativeToUnit(int triggerEventIndex, float globalRotZDeg) {
	std::shared_ptr<WorldOrEntityInteractionEvent> ret = std::make_shared<WorldOrEntityInteractionEvent>(IWorldOrEntityInteractionFilter::Type::GlobalFocusDirection2DRelativeToUnit, triggerEventIndex);

	if (isnan(globalRotZDeg)) {
		// This occured when the window was minimised.
		throw LogicException(LOC);
	}

	ret->globalRotZDeg = globalRotZDeg;

    return ret;
}

std::shared_ptr<WorldOrEntityInteractionEvent> WorldOrEntityInteractionEvent::getNew_ContinuousFocusView3D_Absolute(int triggerEventIndex, float angZDeg, float angElevationDeg) {
	std::shared_ptr<WorldOrEntityInteractionEvent> ret = std::make_shared<WorldOrEntityInteractionEvent>(IWorldOrEntityInteractionFilter::Type::ContinuousFocusView3D_Absolute, triggerEventIndex);

	ret->angZDeg = angZDeg;
	ret->angElevationDeg = angElevationDeg;

    return ret;
}

std::shared_ptr<WorldOrEntityInteractionEvent> WorldOrEntityInteractionEvent::getNew_ContinuousFocusView3DElevation_Absolute(int triggerEventIndex, float angElevationDeg) {
	std::shared_ptr<WorldOrEntityInteractionEvent> ret = std::make_shared<WorldOrEntityInteractionEvent>(IWorldOrEntityInteractionFilter::Type::ContinuousFocusView3DElevation_Absolute, triggerEventIndex);

	ret->angElevationDeg = angElevationDeg;

    return ret;
}

std::shared_ptr<WorldOrEntityInteractionEvent> WorldOrEntityInteractionEvent::getNew_ContinuousFocusView3D_Delta(int triggerEventIndex, Vector2& sensitisedViewPanDelta) {
	std::shared_ptr<WorldOrEntityInteractionEvent> ret = std::make_shared<WorldOrEntityInteractionEvent>(IWorldOrEntityInteractionFilter::Type::ContinuousFocusView3D_Delta, triggerEventIndex);

	ret->sensitisedViewPanDelta.set(sensitisedViewPanDelta);

    return ret;
}

std::shared_ptr<WorldOrEntityInteractionEvent> WorldOrEntityInteractionEvent::getNew_LockedCursor_Down_Main(int triggerEventIndex, int cursorIndex, Vector2& screenCursorPos) {
	std::shared_ptr<WorldOrEntityInteractionEvent> ret = std::make_shared<WorldOrEntityInteractionEvent>(IWorldOrEntityInteractionFilter::Type::LockedCursor_Down_Main, triggerEventIndex);

	ret->cursorIndex = cursorIndex;
	ret->screenCursorPos.set(screenCursorPos);

    return ret;
}

std::shared_ptr<WorldOrEntityInteractionEvent> WorldOrEntityInteractionEvent::getNew_LockedCursor_Down_Alternative(int triggerEventIndex, int cursorIndex, Vector2& screenCursorPos) {
	std::shared_ptr<WorldOrEntityInteractionEvent> ret = std::make_shared<WorldOrEntityInteractionEvent>(IWorldOrEntityInteractionFilter::Type::LockedCursor_Down_Alternative, triggerEventIndex);

	ret->cursorIndex = cursorIndex;
	ret->screenCursorPos.set(screenCursorPos);

    return ret;
}

std::shared_ptr<WorldOrEntityInteractionEvent> WorldOrEntityInteractionEvent::getNew_LockedCursor_Move(int triggerEventIndex, int cursorIndex, Vector2& screenCursorPos, Vector2& lockedCursorSensitisedDelta) {
	std::shared_ptr<WorldOrEntityInteractionEvent> ret = std::make_shared<WorldOrEntityInteractionEvent>(IWorldOrEntityInteractionFilter::Type::LockedCursor_Move, triggerEventIndex);

	ret->cursorIndex = cursorIndex;
	ret->screenCursorPos.set(screenCursorPos);

	ret->lockedCursorSensitisedDelta.set(lockedCursorSensitisedDelta);

    return ret;
}

std::shared_ptr<WorldOrEntityInteractionEvent> WorldOrEntityInteractionEvent::getNew_LockedCursor_Drag_Main(int triggerEventIndex, Vector2& screenCursorPos, Vector2& lockedCursorSensitisedDelta) {
	std::shared_ptr<WorldOrEntityInteractionEvent> ret = std::make_shared<WorldOrEntityInteractionEvent>(IWorldOrEntityInteractionFilter::Type::LockedCursor_Drag_Main, triggerEventIndex);

	ret->screenCursorPos.set(screenCursorPos);

	ret->lockedCursorSensitisedDelta.set(lockedCursorSensitisedDelta);

    return ret;
}

std::shared_ptr<WorldOrEntityInteractionEvent> WorldOrEntityInteractionEvent::getNew_LockedCursor_Drag_Alternative(int triggerEventIndex, Vector2& screenCursorPos, Vector2& lockedCursorSensitisedDelta) {
	std::shared_ptr<WorldOrEntityInteractionEvent> ret = std::make_shared<WorldOrEntityInteractionEvent>(IWorldOrEntityInteractionFilter::Type::LockedCursor_Drag_Alternative, triggerEventIndex);

	ret->screenCursorPos.set(screenCursorPos);

	ret->lockedCursorSensitisedDelta.set(lockedCursorSensitisedDelta);

    return ret;
}

std::shared_ptr<WorldOrEntityInteractionEvent> WorldOrEntityInteractionEvent::getNew_LockedCursor_Up_Main(int triggerEventIndex, int cursorIndex, Vector2& screenCursorPos) {
	std::shared_ptr<WorldOrEntityInteractionEvent> ret = std::make_shared<WorldOrEntityInteractionEvent>(IWorldOrEntityInteractionFilter::Type::LockedCursor_Up_Main, triggerEventIndex);

	ret->cursorIndex = cursorIndex;
	ret->screenCursorPos.set(screenCursorPos);

    return ret;
}

std::shared_ptr<WorldOrEntityInteractionEvent> WorldOrEntityInteractionEvent::getNew_LockedCursor_Up_Alternative(int triggerEventIndex, int cursorIndex, Vector2& screenCursorPos) {
	std::shared_ptr<WorldOrEntityInteractionEvent> ret = std::make_shared<WorldOrEntityInteractionEvent>(IWorldOrEntityInteractionFilter::Type::LockedCursor_Up_Alternative, triggerEventIndex);

	ret->cursorIndex = cursorIndex;
	ret->screenCursorPos.set(screenCursorPos);

    return ret;
}

std::shared_ptr<WorldOrEntityInteractionEvent> WorldOrEntityInteractionEvent::getNew_GlobalMoveDirection2D(int triggerEventIndex, Vector2& moveDirVector) {
	std::shared_ptr<WorldOrEntityInteractionEvent> ret = std::make_shared<WorldOrEntityInteractionEvent>(IWorldOrEntityInteractionFilter::Type::GlobalMoveDirection2D, triggerEventIndex);

	ret->moveDirVector.set(moveDirVector);

    return ret;
}

std::shared_ptr<WorldOrEntityInteractionEvent> WorldOrEntityInteractionEvent::getNew_MovementDPad(int triggerEventIndex, float strafeLeftRightMoveVector, float backFrontMoveVector) {
	std::shared_ptr<WorldOrEntityInteractionEvent> ret = std::make_shared<WorldOrEntityInteractionEvent>(IWorldOrEntityInteractionFilter::Type::MovementDPad, triggerEventIndex);

	ret->strafeLeftRightMoveVector = strafeLeftRightMoveVector;
	ret->backFrontMoveVector = backFrontMoveVector;

    return ret;
}

std::shared_ptr<WorldOrEntityInteractionEvent> WorldOrEntityInteractionEvent::getNew_MovementDPadBackFront(int triggerEventIndex, float backFrontMoveVector) {
	std::shared_ptr<WorldOrEntityInteractionEvent> ret = std::make_shared<WorldOrEntityInteractionEvent>(IWorldOrEntityInteractionFilter::Type::MovementDPadBackFront, triggerEventIndex);

	ret->backFrontMoveVector = backFrontMoveVector;

    return ret;
}

std::shared_ptr<WorldOrEntityInteractionEvent> WorldOrEntityInteractionEvent::getNew_MovementDPadLeftRight(int triggerEventIndex, float strafeLeftRightMoveVector) {
	std::shared_ptr<WorldOrEntityInteractionEvent> ret = std::make_shared<WorldOrEntityInteractionEvent>(IWorldOrEntityInteractionFilter::Type::MovementDPadLeftRight, triggerEventIndex);

	ret->strafeLeftRightMoveVector = strafeLeftRightMoveVector;

    return ret;
}

std::shared_ptr<WorldOrEntityInteractionEvent> WorldOrEntityInteractionEvent::getNew_ScreenCursor_Down_Main(int triggerEventIndex, int cursorIndex, Vector2& screenCursorPos) {
	std::shared_ptr<WorldOrEntityInteractionEvent> ret = std::make_shared<WorldOrEntityInteractionEvent>(IWorldOrEntityInteractionFilter::Type::ScreenCursor_Down_Main, triggerEventIndex);

	ret->cursorIndex = cursorIndex;
	ret->screenCursorPos.set(screenCursorPos);

    return ret;
}

std::shared_ptr<WorldOrEntityInteractionEvent> WorldOrEntityInteractionEvent::getNew_ScreenCursor_Down_Alternative(int triggerEventIndex, int cursorIndex, Vector2& screenCursorPos) {
	std::shared_ptr<WorldOrEntityInteractionEvent> ret = std::make_shared<WorldOrEntityInteractionEvent>(IWorldOrEntityInteractionFilter::Type::ScreenCursor_Down_Alternative, triggerEventIndex);

	ret->cursorIndex = cursorIndex;
	ret->screenCursorPos.set(screenCursorPos);

    return ret;
}

std::shared_ptr<WorldOrEntityInteractionEvent> WorldOrEntityInteractionEvent::getNew_ScreenCursor_Move(
	int triggerEventIndex, int cursorIndex,
	Vector2& screenCursorPos, Vector2& screenCursorMoveDelta, Vector2& screenCursorMoveDelta_sensitised)
{
	std::shared_ptr<WorldOrEntityInteractionEvent> ret = std::make_shared<WorldOrEntityInteractionEvent>(IWorldOrEntityInteractionFilter::Type::ScreenCursor_Move, triggerEventIndex);

	ret->cursorIndex = cursorIndex;
	ret->screenCursorPos.set(screenCursorPos);
	ret->screenCursorMoveDelta.set(screenCursorMoveDelta);
	ret->screenCursorMoveDelta_sensitised.set(screenCursorMoveDelta_sensitised);

    return ret;
}

std::shared_ptr<WorldOrEntityInteractionEvent> WorldOrEntityInteractionEvent::getNew_ScreenCursor_Drag_Main(
	int triggerEventIndex, int cursorIndex,
	Vector2& screenCursorPos, Vector2& screenCursorMoveDelta, Vector2& screenCursorMoveDelta_sensitised)
{
	std::shared_ptr<WorldOrEntityInteractionEvent> ret = std::make_shared<WorldOrEntityInteractionEvent>(IWorldOrEntityInteractionFilter::Type::ScreenCursor_Drag_Main, triggerEventIndex);

	ret->cursorIndex = cursorIndex;
	ret->screenCursorPos.set(screenCursorPos);
	ret->screenCursorMoveDelta.set(screenCursorMoveDelta);
	ret->screenCursorMoveDelta_sensitised.set(screenCursorMoveDelta_sensitised);

    return ret;
}

std::shared_ptr<WorldOrEntityInteractionEvent> WorldOrEntityInteractionEvent::getNew_ScreenCursor_Drag_Alternative(
	int triggerEventIndex, int cursorIndex,
	Vector2& screenCursorPos, Vector2& screenCursorMoveDelta, Vector2& screenCursorMoveDelta_sensitised)
{
	std::shared_ptr<WorldOrEntityInteractionEvent> ret = std::make_shared<WorldOrEntityInteractionEvent>(IWorldOrEntityInteractionFilter::Type::ScreenCursor_Drag_Alternative, triggerEventIndex);

	ret->cursorIndex = cursorIndex;
	ret->screenCursorPos.set(screenCursorPos);
	ret->screenCursorMoveDelta.set(screenCursorMoveDelta);
	ret->screenCursorMoveDelta_sensitised.set(screenCursorMoveDelta_sensitised);

    return ret;
}

std::shared_ptr<WorldOrEntityInteractionEvent> WorldOrEntityInteractionEvent::getNew_ScreenCursor_Up_Main(int triggerEventIndex, int cursorIndex, Vector2& screenCursorPos) {
	std::shared_ptr<WorldOrEntityInteractionEvent> ret = std::make_shared<WorldOrEntityInteractionEvent>(IWorldOrEntityInteractionFilter::Type::ScreenCursor_Up_Main, triggerEventIndex);

	ret->cursorIndex = cursorIndex;
	ret->screenCursorPos.set(screenCursorPos);

    return ret;
}

std::shared_ptr<WorldOrEntityInteractionEvent> WorldOrEntityInteractionEvent::getNew_ScreenCursor_Up_Alternative(int triggerEventIndex, int cursorIndex, Vector2& screenCursorPos) {
	std::shared_ptr<WorldOrEntityInteractionEvent> ret = std::make_shared<WorldOrEntityInteractionEvent>(IWorldOrEntityInteractionFilter::Type::ScreenCursor_Up_Alternative, triggerEventIndex);

	ret->cursorIndex = cursorIndex;
	ret->screenCursorPos.set(screenCursorPos);

    return ret;
}

std::shared_ptr<WorldOrEntityInteractionEvent> WorldOrEntityInteractionEvent::getNew_ContinuousFocusView3DYaw_Delta(int triggerEventIndex, float yawDegDelta) {
	std::shared_ptr<WorldOrEntityInteractionEvent> ret = std::make_shared<WorldOrEntityInteractionEvent>(IWorldOrEntityInteractionFilter::Type::ContinuousFocusView3DYaw_Delta, triggerEventIndex);

	ret->continuousFocusView3DYaw_yawDegDelta = yawDegDelta;

    return ret;
}

std::shared_ptr<WorldOrEntityInteractionEvent> WorldOrEntityInteractionEvent::getNew_UnitFeetGroundPlanePos_1stPersonCamera_Down_MainBtn(int triggerEventIndex, int cursorIndex, Vector3& worldPos) {
	std::shared_ptr<WorldOrEntityInteractionEvent> ret = std::make_shared<WorldOrEntityInteractionEvent>(IWorldOrEntityInteractionFilter::Type::UnitFeetGroundPlanePos_1stPersonCamera_Down_MainBtn, triggerEventIndex);

	ret->cursorIndex = cursorIndex;
	ret->worldPos.set(worldPos);

    return ret;
}

std::shared_ptr<WorldOrEntityInteractionEvent> WorldOrEntityInteractionEvent::getNew_UnitFeetGroundPlanePos_3rdPersonCamera_Down_MainBtn(int triggerEventIndex, int cursorIndex, Vector3& worldPos) {
	std::shared_ptr<WorldOrEntityInteractionEvent> ret = std::make_shared<WorldOrEntityInteractionEvent>(IWorldOrEntityInteractionFilter::Type::UnitFeetGroundPlanePos_3rdPersonCamera_Down_MainBtn, triggerEventIndex);

	ret->cursorIndex = cursorIndex;
	ret->worldPos.set(worldPos);

    return ret;
}

std::shared_ptr<WorldOrEntityInteractionEvent> WorldOrEntityInteractionEvent::getNew_UnitFeetHalfSphereDomeDir_1stPersonCamera_Down_MainBtn(int triggerEventIndex, int cursorIndex, Vector3& worldDir) {
	std::shared_ptr<WorldOrEntityInteractionEvent> ret = std::make_shared<WorldOrEntityInteractionEvent>(IWorldOrEntityInteractionFilter::Type::UnitFeetHalfSphereDomeDir_1stPersonCamera_Down_MainBtn, triggerEventIndex);

	ret->cursorIndex = cursorIndex;
	ret->worldDir.set(worldDir);

    return ret;
}

std::shared_ptr<WorldOrEntityInteractionEvent> WorldOrEntityInteractionEvent::getNew_UnitFeetHalfSphereDomeDir_3rdPersonCamera_Down_MainBtn(int triggerEventIndex, int cursorIndex, Vector3& worldDir) {
	std::shared_ptr<WorldOrEntityInteractionEvent> ret = std::make_shared<WorldOrEntityInteractionEvent>(IWorldOrEntityInteractionFilter::Type::UnitFeetHalfSphereDomeDir_3rdPersonCamera_Down_MainBtn, triggerEventIndex);

	ret->cursorIndex = cursorIndex;
	ret->worldDir.set(worldDir);

    return ret;
}

std::shared_ptr<WorldOrEntityInteractionEvent> WorldOrEntityInteractionEvent::getNew_UnitFeetGroundPlanePos_1stPersonCamera_Drag_MainBtn(int triggerEventIndex, int cursorIndex, Vector3& worldPos) {
	std::shared_ptr<WorldOrEntityInteractionEvent> ret = std::make_shared<WorldOrEntityInteractionEvent>(IWorldOrEntityInteractionFilter::Type::UnitFeetGroundPlanePos_1stPersonCamera_Drag_MainBtn, triggerEventIndex);

	ret->cursorIndex = cursorIndex;
	ret->worldPos.set(worldPos);

    return ret;
}

std::shared_ptr<WorldOrEntityInteractionEvent> WorldOrEntityInteractionEvent::getNew_UnitFeetGroundPlanePos_3rdPersonCamera_Drag_MainBtn(int triggerEventIndex, int cursorIndex, Vector3& worldPos) {
	std::shared_ptr<WorldOrEntityInteractionEvent> ret = std::make_shared<WorldOrEntityInteractionEvent>(IWorldOrEntityInteractionFilter::Type::UnitFeetGroundPlanePos_3rdPersonCamera_Drag_MainBtn, triggerEventIndex);

	ret->cursorIndex = cursorIndex;
	ret->worldPos.set(worldPos);

    return ret;
}

std::shared_ptr<WorldOrEntityInteractionEvent> WorldOrEntityInteractionEvent::getNew_UnitFeetHalfSphereDomeDir_1stPersonCamera_Drag_MainBtn(int triggerEventIndex, int cursorIndex, Vector3& worldDir) {
	std::shared_ptr<WorldOrEntityInteractionEvent> ret = std::make_shared<WorldOrEntityInteractionEvent>(IWorldOrEntityInteractionFilter::Type::UnitFeetHalfSphereDomeDir_1stPersonCamera_Drag_MainBtn, triggerEventIndex);

	ret->cursorIndex = cursorIndex;
	ret->worldDir.set(worldDir);

    return ret;
}

std::shared_ptr<WorldOrEntityInteractionEvent> WorldOrEntityInteractionEvent::getNew_UnitFeetHalfSphereDomeDir_3rdPersonCamera_Drag_MainBtn(int triggerEventIndex, int cursorIndex, Vector3& worldDir) {
	std::shared_ptr<WorldOrEntityInteractionEvent> ret = std::make_shared<WorldOrEntityInteractionEvent>(IWorldOrEntityInteractionFilter::Type::UnitFeetHalfSphereDomeDir_3rdPersonCamera_Drag_MainBtn, triggerEventIndex);

	ret->cursorIndex = cursorIndex;
	ret->worldDir.set(worldDir);

    return ret;
}

std::shared_ptr<WorldOrEntityInteractionEvent> WorldOrEntityInteractionEvent::getNew_CameraDirection(int triggerEventIndex, Vector3& cameraDirection) {
	std::shared_ptr<WorldOrEntityInteractionEvent> ret = std::make_shared<WorldOrEntityInteractionEvent>(IWorldOrEntityInteractionFilter::Type::CameraDirection, triggerEventIndex);

	ret->cameraDirection.set(cameraDirection);

    return ret;
}

std::shared_ptr<WorldOrEntityInteractionEvent> WorldOrEntityInteractionEvent::getNew_ToolExtraActivation_ListControl_Down_MainBtn(int triggerEventIndex, int toolInstanceId, int toolExtraControlSourceId) {
	std::shared_ptr<WorldOrEntityInteractionEvent> ret = std::make_shared<WorldOrEntityInteractionEvent>(IWorldOrEntityInteractionFilter::Type::ToolExtraActivation_ListControl_Down_MainBtn, triggerEventIndex);

	ret->toolInstanceId = toolInstanceId;
	ret->toolExtraControlSourceId = toolExtraControlSourceId;

	return ret;
}

std::shared_ptr<WorldOrEntityInteractionEvent> WorldOrEntityInteractionEvent::getNew_ToolExtraActivation_SecondaryControl_Down_MainBtn(int triggerEventIndex, int toolInstanceId, int toolExtraControlSourceId) {
	std::shared_ptr<WorldOrEntityInteractionEvent> ret = std::make_shared<WorldOrEntityInteractionEvent>(IWorldOrEntityInteractionFilter::Type::ToolExtraActivation_SecondaryControl_Down_MainBtn, triggerEventIndex);

	ret->toolInstanceId = toolInstanceId;
	ret->toolExtraControlSourceId = toolExtraControlSourceId;

	return ret;
}

WorldOrEntityInteractionEvent::~WorldOrEntityInteractionEvent() {
	//void
}
