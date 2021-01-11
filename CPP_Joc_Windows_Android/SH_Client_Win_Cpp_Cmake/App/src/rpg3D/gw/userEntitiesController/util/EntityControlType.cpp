#include "EntityControlType.h"

using namespace rpg3D;

const int EntityControlType::None = 0;

const int EntityControlType::IndirectMoveToTarget_AutoRotToMoveDirection = 1;

const int EntityControlType::DPadCameraAxesDirectionMove_AutoRotToMoveDirection = 2;

const int EntityControlType::DPadUnitAxesStrafe_CursorRotationBothAxes = 3;

const int EntityControlType::DPadCameraAxesDirectionMove_LookAtCursorOrAutoRotToMoveDirection = 4;

const int EntityControlType::GridTurnBased = 5;
