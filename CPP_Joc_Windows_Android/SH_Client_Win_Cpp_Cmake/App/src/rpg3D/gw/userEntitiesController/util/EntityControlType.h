#pragma once

namespace rpg3D {
class EntityControlType {
public:
    static const int None;

    // RTS controls system.
    // Tap on ground or object and movement and rotation are automatically handled until action is finished.
	//> Mouse MOBA controls.
    static const int IndirectMoveToTarget_AutoRotToMoveDirection;

    // Used with third person fixed direction camera.
    // Control movement with keyboard or on_screen joystick. Movement directions are in the camera space.
    // Movement is camera front,back,left,right.
    // Rotation is in the direction of movement.
	//> BarbarQ controls.
    static const int DPadCameraAxesDirectionMove_AutoRotToMoveDirection;

    // Used with first person follower camera.
    // FPS controls system. The unit is directly controlled using device inputs.
    // Control movement with keyboard or on_screen joystick. Movement directions use the unit direction axes.
    // Movement is back, front, strafe left,right.
    // Rotation is controlled by mouse or device tilt.
	//> FPS Controls.
    static const int DPadUnitAxesStrafe_CursorRotationBothAxes;

	//> Action-adventure 3rd person controls.
	// Direct strafe movement in camera axes.
	// Rot to look at mouse and if not mouse input for ~3-5 seconds auto-rotate to move direction until mouse input occurs.
    static const int DPadCameraAxesDirectionMove_LookAtCursorOrAutoRotToMoveDirection;
    
	//> Rpg turn-based 3rd person controls.
    // Sets movement to IToolMTGridTurnBased and clears the turn tool.
    // This works with the optional game_part GPTurnBasedPlay.
    static const int GridTurnBased;

};
};
