#include "IWorldOrEntityInteractionFilter.h"

using namespace rpg3D;

int IWorldOrEntityInteractionFilter::Type::NewType() {
	static int COUNTER = 0;

	COUNTER++;

	return COUNTER;
}

// NOTE : Currently this uses ground at pos_z=0.0f;
const int IWorldOrEntityInteractionFilter::Type::GroundDown = NewType();
const int IWorldOrEntityInteractionFilter::Type::GroundUp = NewType();
const int IWorldOrEntityInteractionFilter::Type::GroundMove = NewType();

const int IWorldOrEntityInteractionFilter::Type::EntityDown = NewType();
const int IWorldOrEntityInteractionFilter::Type::EntityUp = NewType();

const int IWorldOrEntityInteractionFilter::Type::GlobalMoveDirection2D = NewType();

const int IWorldOrEntityInteractionFilter::Type::GlobalFocusDirection2D = NewType();

const int IWorldOrEntityInteractionFilter::Type::GlobalFocusDirection2DRelativeToUnit = NewType();
const int IWorldOrEntityInteractionFilter::Type::ContinuousFocusView3D_Delta = NewType();
const int IWorldOrEntityInteractionFilter::Type::ContinuousFocusView3D_Absolute = NewType();
const int IWorldOrEntityInteractionFilter::Type::ContinuousFocusView3DElevation_Absolute = NewType();

const int IWorldOrEntityInteractionFilter::Type::MovementDPad = NewType();
const int IWorldOrEntityInteractionFilter::Type::MovementDPadBackFront = NewType();
const int IWorldOrEntityInteractionFilter::Type::MovementDPadLeftRight = NewType();

const int IWorldOrEntityInteractionFilter::Type::LockedCursor_Down_Main = NewType();
const int IWorldOrEntityInteractionFilter::Type::LockedCursor_Down_Alternative = NewType();

const int IWorldOrEntityInteractionFilter::Type::LockedCursor_Move = NewType();

const int IWorldOrEntityInteractionFilter::Type::LockedCursor_Drag_Main = NewType();
const int IWorldOrEntityInteractionFilter::Type::LockedCursor_Drag_Alternative = NewType();

const int IWorldOrEntityInteractionFilter::Type::LockedCursor_Up_Main = NewType();
const int IWorldOrEntityInteractionFilter::Type::LockedCursor_Up_Alternative = NewType();

const int IWorldOrEntityInteractionFilter::Type::ScreenCursor_Down_Main = NewType();
const int IWorldOrEntityInteractionFilter::Type::ScreenCursor_Down_Alternative = NewType();

const int IWorldOrEntityInteractionFilter::Type::ScreenCursor_Move = NewType();
const int IWorldOrEntityInteractionFilter::Type::ScreenCursor_Drag_Main = NewType();
const int IWorldOrEntityInteractionFilter::Type::ScreenCursor_Drag_Alternative = NewType();

const int IWorldOrEntityInteractionFilter::Type::ScreenCursor_Up_Main = NewType();
const int IWorldOrEntityInteractionFilter::Type::ScreenCursor_Up_Alternative = NewType();

const int IWorldOrEntityInteractionFilter::Type::ContinuousFocusView3DYaw_Delta = NewType();

const int IWorldOrEntityInteractionFilter::Type::UnitFeetGroundPlanePos_1stPersonCamera_Down_MainBtn = NewType();
const int IWorldOrEntityInteractionFilter::Type::UnitFeetGroundPlanePos_3rdPersonCamera_Down_MainBtn = NewType();
const int IWorldOrEntityInteractionFilter::Type::UnitFeetHalfSphereDomeDir_1stPersonCamera_Down_MainBtn = NewType();
const int IWorldOrEntityInteractionFilter::Type::UnitFeetHalfSphereDomeDir_3rdPersonCamera_Down_MainBtn = NewType();

const int IWorldOrEntityInteractionFilter::Type::UnitFeetGroundPlanePos_1stPersonCamera_Drag_MainBtn = NewType();
const int IWorldOrEntityInteractionFilter::Type::UnitFeetGroundPlanePos_3rdPersonCamera_Drag_MainBtn = NewType();
const int IWorldOrEntityInteractionFilter::Type::UnitFeetHalfSphereDomeDir_1stPersonCamera_Drag_MainBtn = NewType();
const int IWorldOrEntityInteractionFilter::Type::UnitFeetHalfSphereDomeDir_3rdPersonCamera_Drag_MainBtn = NewType();

const int IWorldOrEntityInteractionFilter::Type::CameraDirection = NewType();

const int IWorldOrEntityInteractionFilter::Type::ToolExtraActivation_ListControl_Down_MainBtn = NewType();
const int IWorldOrEntityInteractionFilter::Type::ToolExtraActivation_SecondaryControl_Down_MainBtn = NewType();
