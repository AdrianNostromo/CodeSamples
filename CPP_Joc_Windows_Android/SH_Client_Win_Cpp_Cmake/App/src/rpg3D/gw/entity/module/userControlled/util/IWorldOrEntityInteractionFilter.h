#pragma once

#include <base/gh.h>
#include <base/list/ArrayList.h>
#include <base/math/Vector3.h>
#include <base/math/Vector2.h>
#include <rpg3D/gw/entity/filter/IEntityFilter.h>

namespace rpg3D {
class IWorldOrEntityInteractionFilter : virtual public IEntityFilter {
public:
	class Type {
		priv static int NewType();
	public:
		// NOTE : Currently this uses ground at pos_z=0.0f;
		// Dispatches on cursor_button_action.
		static const int GroundDown;
		// Dispatches on cursor_button_action.
		static const int GroundUp;
		// Dispatches on cursor_move_or_drag, force_update, camera_update.
		static const int GroundMove;

		// Dispatches on cursor_button_action.
		static const int EntityDown;
		static const int EntityUp;

		// Dispatches on dPad_change, camera_dir_change, control_type_change.
		static const int GlobalMoveDirection2D;

		// This occurs only on mouse_move, NOT on unit_moves.
		// Dispatches on cursor_move_or_drag, force_update, camera_update.
		static const int GlobalFocusDirection2D;

		// This dispatches when the mouse_moves or the unit_moves.
		// Dispatches on cursor_move_or_drag, unit_pos_change, force_update, camera_update.
		static const int GlobalFocusDirection2DRelativeToUnit;
		// Eg. Locked_cursor_move
		static const int ContinuousFocusView3D_Delta;
		// Eg. Accelerometer+Compas / Gyroscope
		static const int ContinuousFocusView3D_Absolute;
		// Eg. Accelerometer only.
		static const int ContinuousFocusView3DElevation_Absolute;

		// Dispatches on dPad_change, control_type_change.
		static const int MovementDPad;
		// Dispatches on dPad_change, control_type_change.
		static const int MovementDPadBackFront;
		// Dispatches on dPad_change, control_type_change.
		static const int MovementDPadLeftRight;

		// Left_mouse_button or touch.
		static const int LockedCursor_Down_Main;
		// Right_mouse_button
		static const int LockedCursor_Down_Alternative;
		
		// This is used for the entity editor, entity rotations.
		static const int LockedCursor_Move;

		static const int LockedCursor_Drag_Main;
		static const int LockedCursor_Drag_Alternative;

		// Left_mouse_button or touch.
		static const int LockedCursor_Up_Main;
		// Right_mouse_button
		static const int LockedCursor_Up_Alternative;

		// Left_mouse_button or touch.
		// Dispatches on cursor_button_action.
		static const int ScreenCursor_Down_Main;
		// Right_mouse_button
		// Dispatches on cursor_button_action.
		static const int ScreenCursor_Down_Alternative;
		
		// Dispatches when the cursor moves.
		// Doesn't dispatch on player movements.
		// Dispatches on cursor_move, force_update.
		static const int ScreenCursor_Move;
		// Dispatches on cursor_drag, force_update.
		static const int ScreenCursor_Drag_Main;
		// Dispatches on cursor_drag, force_update.
		static const int ScreenCursor_Drag_Alternative;

		// Dispatches on cursor_button_action.
		static const int ScreenCursor_Up_Main;
		// Dispatches on cursor_button_action.
		static const int ScreenCursor_Up_Alternative;

		// This is currently a custom logic used to do a yaw turn on the entity from a device roll. Used if there is only accelerometer input.
		static const int ContinuousFocusView3DYaw_Delta;

		// These 2 groups are connected and only one is dispatched. The ground plane is checked first and if it fails, the ray direction is dispatched in the second.
		// Dispatches on cursor_button_action
		static const int UnitFeetGroundPlanePos_1stPersonCamera_Down_MainBtn;
		static const int UnitFeetGroundPlanePos_3rdPersonCamera_Down_MainBtn;
		// Note. This is dispatched only if the above is not dispatcher, there was no ray intersection with a ground_plane located at the player feet.
		// Note. This doesn't actualy test against a dome, the bottom part may never be returned (eg. may always be intersected with the ground, depends on the camera direction).
		// Dispatches on cursor_button_action.
		static const int UnitFeetHalfSphereDomeDir_1stPersonCamera_Down_MainBtn;
		static const int UnitFeetHalfSphereDomeDir_3rdPersonCamera_Down_MainBtn;
		
		// Dispatched on cursor_drag and camera_change.
		// These 2 groups are connected and only one is dispatched. The ground plane is checked first and if it fails, the ray direction is dispatched in the second.
		// Dispatches on cursor_drag, camera_update, unit_pos_change.
		static const int UnitFeetGroundPlanePos_1stPersonCamera_Drag_MainBtn;
		static const int UnitFeetGroundPlanePos_3rdPersonCamera_Drag_MainBtn;
		// Dispatched on cursor_drag and camera_change.
		// Note. This is dispatched only if the above is not dispatcher, there was no ray intersection with a ground_plane located at the player feet.
		// Note. This doesn't actualy test against a dome, the bottom part may never be returned (eg. may always be intersected with the ground, depends on the camera direction).
		// Dispatches on cursor_move_or_drag, 
		static const int UnitFeetHalfSphereDomeDir_1stPersonCamera_Drag_MainBtn;
		static const int UnitFeetHalfSphereDomeDir_3rdPersonCamera_Drag_MainBtn;
		
		// Dispatched on: camera_dir_change, control_type_change.
		static const int CameraDirection;

		static const int ToolExtraActivation_ListControl_Down_MainBtn;
		static const int ToolExtraActivation_SecondaryControl_Down_MainBtn;

	};

public:
	virtual int getType() = 0;

	~IWorldOrEntityInteractionFilter() override = default;
};
};
