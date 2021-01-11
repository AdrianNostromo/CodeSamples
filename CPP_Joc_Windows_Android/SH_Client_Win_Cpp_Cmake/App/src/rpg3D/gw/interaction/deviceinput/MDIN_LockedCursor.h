#pragma once

#include <base/gh.h>
#include "MDIN_Move_UIDirectionalPad.h"

namespace rpg3D {
class MDIN_LockedCursor : public MDIN_Move_UIDirectionalPad {priv typedef MDIN_Move_UIDirectionalPad super; pub dCtor(MDIN_LockedCursor);
	pub static float mouseSensitivityMultiplier;

	priv ArrayList<std::shared_ptr<IEventListener>> localAutoListenersList{};

	pub explicit MDIN_LockedCursor(base::IGameWorld* gw);
	prot void createMain() override;
	prot void createBMain() override;

	pub bool getIsCursorBlockedModeSupported() override;
	pub void setIsCursorBlockedMode(bool isCursorBlockedMode) override;

	priv void onSensorEvent(IEventListener& eventListener, std::shared_ptr<base::SensorEvent>& sensorEvent);
	priv void onLockedCursorDown(
		int cursorIndex, base::Touch::ToolType* toolType, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& lockedCursorSpace);
	priv void onLockedCursorMoved(
		int cursorIndex, base::Touch::ToolType* toolType,
		Vector2& screenPos, Vector2& delta);
	priv void onLockedCursorDrag(
		int cursorIndex, base::Touch::ToolType* toolType, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& delta);
	priv void onLockedCursorUp(
		int cursorIndex, base::Touch::ToolType* toolType, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& lockedCursorSpace);
	priv void onLockedCursorDownCanceled(
		int cursorIndex, base::Touch::ToolType* toolType, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& lockedCursorSpace);

	prot void disposePre() override;
	pub ~MDIN_LockedCursor() override;
};
};
