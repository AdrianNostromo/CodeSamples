#pragma once

#include <base/gh.h>
#include <graphics/visual2d/drawable/BasicStage2D.h>
#include <graphics/visual2d/drawable/IRoot2DInteractive.h>
#include <graphics/visual2d/util/CursorHandlingData.h>
#include "IInteractiveDrawable2D.h"

class IInputHandler;
class IEventListener;
namespace base {
	class SensorEvent;
}

class TouchStage2D : public BasicStage2D, virtual public IRoot2DInteractive {priv typedef BasicStage2D super; pub dCtor(TouchStage2D);
	priv IInputHandler* inputHandler;

    prot ArrayList<std::shared_ptr<base::CursorHandlingData>> cursorHandlingDatasList{};

    prot int cursorEventIndexCounter = 0;

	priv int moveEventsLoopTickIndexCounter = 0;

	priv prot ArrayList<std::shared_ptr<IEventListener>> autoListenersList{};

    pub explicit TouchStage2D(graphics::IEnvironment2D* environment, IInputHandler* inputHandler);

	priv bool onCursorDown(
		int cursorIndex, base::Touch::ToolType* toolType, base::Touch::ButtonCode* buttonId,
		float screenX, float screenY);
    priv bool onCursorDownCanceled(
		int cursorIndex, base::Touch::ToolType* toolType, base::Touch::ButtonCode* buttonId,
		float screenX, float screenY);

    priv bool onCursorUp(
		int cursorIndex, base::Touch::ToolType* toolType, base::Touch::ButtonCode* buttonId,
		float screenX, float screenY);

    priv bool onCursorMoved(
		int cursorIndex, base::Touch::ToolType* toolType,
		float screenX, float screenY);
    priv bool onCursorMoveCanceled(
		int cursorIndex,
		float screenX, float screenY);

	priv void onCursorCacheLoopTickDispatchFinished();

    pub void clearExistingTouchDownTarget(IInteractiveDrawable2D* entry, int cursorIndex, base::Touch::ButtonCode* buttonId) override;
	pub void clearExistingCursorOverTarget(IInteractiveDrawable2D* entry, int cursorIndex) override;

    prot base::CursorHandlingData* getCursorHandlingData(int cursorIndex);
    prot base::CursorHandlingData::ButtonHD* getCursorButtonHandlingData(int cursorIndex, base::Touch::ButtonCode* buttonId);

	priv bool handlePressedCursorButtonsMove(base::CursorHandlingData* cursorHD);

    priv void onSensorEvent(IEventListener& eventListener, std::shared_ptr<base::SensorEvent>& sensorEvent);

    pub ~TouchStage2D() override;
};
