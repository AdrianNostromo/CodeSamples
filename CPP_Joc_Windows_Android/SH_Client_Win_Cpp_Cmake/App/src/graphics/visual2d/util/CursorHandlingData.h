#pragma once

#include <base/math/Vector3.h>
#include <base/list/ArrayList.h>
#include <base/input/util/Touch.h>

class IInteractiveDrawable2D;

namespace base {
class CursorHandlingData {
public:
	class ButtonHD {
	public:
		CursorHandlingData* cursorHD;

		base::Touch::ButtonCode* buttonId;

		// Touch interaction variables.
		ArrayList<IInteractiveDrawable2D*> cursorDownTargetsList{};
		// This object will receive move events even if they are not over it.
		ArrayList<IInteractiveDrawable2D*> cursorDownHookingTargetsList{};

		bool isPressed = false;

		Vector2 downPosRawScreen{};
		Vector2 downPosW{};

		Vector2 upPosRawScreen{};
		Vector2 upPosW{};

		bool syncChecker_down = false;
		bool syncChecker_downCanceled = false;
		bool syncChecker_up = false;

	public:
		ButtonHD(ButtonHD const&) = delete;
		ButtonHD(ButtonHD &&) = default;
		ButtonHD& operator=(ButtonHD const&) = delete;
		ButtonHD& operator=(ButtonHD &&) = default;
		explicit ButtonHD(CursorHandlingData* cursorHD, base::Touch::ButtonCode* buttonId);

		void addCursorDownTarget(IInteractiveDrawable2D* entry);
		void removeCursorDownTarget(IInteractiveDrawable2D* entry);
		void addCursorDownHookingTarget(IInteractiveDrawable2D* entry);

		virtual ~ButtonHD();
	};
public:
    int cursorIndex;
	base::Touch::ToolType* toolType = base::Touch::ToolType::None;

	ArrayList<std::shared_ptr<ButtonHD>> buttonsHandlingDataList{};

	ArrayList<IInteractiveDrawable2D*> cursorOverTargetsList{};

	// This received from native as is (Top_Left origin).
	// This is also saved because some manually dispatched events are required and they use the unprojected position.
	Vector2 posRawScreen{};

	// This is camera projected to bottom_left origin.
	Vector2 posW{};
	Vector2 deltaPosW{};

	bool syncChecker_dragged = false;
	bool syncChecker_moveCanceled = false;

	// This is used to detect if a mouse didn't move on a frame. That is used to dispatch move events to visuals as the visual moves but the mouse cursor doesn't.
	int moveEventsLoopTickIndex = 0;

public:
    CursorHandlingData(CursorHandlingData const&) = delete;
    CursorHandlingData(CursorHandlingData &&) = default;
    CursorHandlingData& operator=(CursorHandlingData const&) = delete;
    CursorHandlingData& operator=(CursorHandlingData &&) = default;

    explicit CursorHandlingData(int cursorIndex);

	ButtonHD* getButtonHandlingData(base::Touch::ButtonCode* buttonId);

	void addCursorOverTarget(IInteractiveDrawable2D* entry);
	void removeCursorOverTarget(IInteractiveDrawable2D* entry);
	void removeAllCursorOverTargets();

    virtual ~CursorHandlingData();
};
};

#include <graphics/visual2d/drawable/IInteractiveDrawable2D.h>
