#pragma once

#include <base/gh.h>
#include <graphics/visual2d/drawable/SizedDrawable2D.h>
#include <graphics/visual2d/drawable/IInteractiveDrawable2D.h>
#include <string>
#include <base/map/Map1D.h>
#include <base/geometry/Shape2D.h>
#include <base/geometry/Rectangle.h>

class InteractiveDrawable2D : public SizedDrawable2D, virtual public IInteractiveDrawable2D {priv typedef SizedDrawable2D super;pub dCtor(InteractiveDrawable2D);
	// These overrides should only be used on a visual2D that already has valid size values.
	// The values are only meant to expand,override and existing area, not to replace the size system.
	priv std::shared_ptr<float> touchWidthOverride = nullptr;
	priv std::shared_ptr<float> touchHeightOverride = nullptr;
	priv std::shared_ptr<float> touchRadiusOverride = nullptr;

	// Bounds used for touch detection using local touch pos.
	priv std::shared_ptr<Shape2D> touchShape = std::static_pointer_cast<Shape2D>(std::make_shared<Rectangle>(
		0.0f, 0.0f,
		0.0f, 0.0f
	));

	//asdAAAA;// use sharred_ptr;
	priv ArrayList<WrappedTouchListener2D*> wrappedTouchListenersList{};

	// This controls if the touch shape is checked.
	priv int inputInterractiveButtonsBitMap = 0;
	priv bool isTouchFallthroughEnabled = false;
    // This controls if touch handling occurs (eg touch_down_hooking) and touch_events dispatching.
	priv bool isInputFrozen = false;

    //isCursorButtonPressedList[cursorIndex][buttonId.index]
    // Used as bool.
    // nullptr: not pressed
    // value: buttonId
	priv ArrayList<ArrayList<base::Touch::ButtonCode*>> isCursorButtonPressedList{};
	priv int pressedCursorButtonsCount = 0;

    //isCursorOverList[cursorIndex]
    // Used as bool.
    // 0: false
    // 1: true
	priv ArrayList<int> isCursorOverList{};

    // This is update on each cursor_move event.
    // This is used to detect cursor_out. If it not updated on a cursor_move, it means a cursor_out should occur.
	priv int cursorMoveEventIndex = 0;

	// This controls if touch events are dispatched. This visual is still added to the hooking targets and touches like touch_up_outside are still detected.
	// This is also set by buttons on enabled state changes.
	priv bool isEventsEnabled = true;

	priv Map1D<std::string, std::shared_ptr<base::IWrappedValue>>* userDataMap = nullptr;

    // When this is made true, it percalates up the display list and causes the stage to dispatch a
    // move event for all mouse_cursors. This will make sure mouse_move, mouse_over and mouse_out are triggered
    // when a visual moves, even if the mouse cursor doesn't move.
    // This value is reset on touch_shape tests.
	priv bool isTouchBoxChanged = false;

	priv void* extraData_voidP = nullptr;
	priv int extraData_int = -1;
	priv int extraDataB_int = -1;

    pub explicit InteractiveDrawable2D();

	pub bool getIsInputInteractive() final;
	pub int getInputInterractiveButtonsBitMap() final;
	// Return self for function call chaining.
	pub IInteractiveDrawable2D* setIsInputInteractive(bool isInputInteractive) override;
	pub IInteractiveDrawable2D* setInputInterractiveButtonsBitMap(int inputInterractiveButtonsBitMap) override;

	pub bool getIsTouchFallthroughEnabled() override;
	pub void setIsTouchFallthroughEnabled(bool isTouchFallthroughEnabled) override;

	pub bool getIsInputFrozen() override;

	pub ArrayList<WrappedTouchListener2D*>* getWrappedTouchListenersList() override;

	pub bool getIsEventsEnabled() override;
	pub void setIsEventsEnabled(bool isEventsEnabled) override;

	pub void addTouchListener(ITouchListener* touchListener) override;
	pub void removeTouchListener(ITouchListener* touchListener) override;

	pub void onIsVisibleChanged() override;

	pub void onRootChanged_pre() override;

	// This checks all available cursors.
	pub bool getIsButtonPressed(base::Touch::ButtonCode* buttonId) override;

	pub bool getIsCursorButtonPressed(int cursorIndex, base::Touch::ButtonCode* buttonId) override;
	pub void setIsCursorButtonPressed(int cursorIndex, base::Touch::ButtonCode* buttonId, bool isPressed) override;

	pub bool getIsCursorOver(int cursorIndex) override;
	pub void setIsCursorOver(int cursorIndex, bool isOver) override;
	pub int getCursorMoveEventIndex() override;
	pub void setCursorMoveEventIndex(int cursorMoveEventIndex) override;

	pub Map1D<std::string, std::shared_ptr<base::IWrappedValue>>* getUserDataMap() override;
	pub void putUserData(std::string key, std::shared_ptr<base::IWrappedValue> value) override;
	pub std::shared_ptr<base::IWrappedValue> getUserData(std::string key) override;

	pub std::shared_ptr<Shape2D> getTouchShape() override;
	pub void setTouchShape(std::shared_ptr<Shape2D> touchShape) override;

	pub std::shared_ptr<float> getTouchWidthOverride() override { return touchWidthOverride; }
	pub void setTouchWidthOverride(std::shared_ptr<float> touchWidthOverride) override;

	pub std::shared_ptr<float> getTouchHeightOverride() override { return touchHeightOverride; }
	pub void setTouchHeightOverride(std::shared_ptr<float> touchHeightOverride) override;

	pub std::shared_ptr<float> getTouchRadiusOverride() override { return touchRadiusOverride; }
	pub void setTouchRadiusOverride(std::shared_ptr<float> touchRadiusOverride) override;

	pub void clearIsTouchBoxChanged() override;

	pub std::shared_ptr<base::IWrappedValue> getPropertyValue(std::string property) override;
	pub void setPropertyValue(std::string property, std::shared_ptr<base::IWrappedValue> value) override;

	// This can also be manually called to force a mouse_move event to be dispatched. Is used in some cases.
	pub void onTouchBoxChanged() override;

	// NOTE. These events are received even if isEventsEnabled is false. Use the events system if isEventsEnabled should be used.
	pub bool onCursorDown(
		IInteractiveDrawable2D* target,
		int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos) override;

	pub void onCursorUp(
		IInteractiveDrawable2D* target,
		int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos) override;

	pub void onCursorUpOutside(
		IInteractiveDrawable2D* target,
		int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos) override;

	pub void onCursorTap(
		IInteractiveDrawable2D* target,
		int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos) override;

	pub void onCursorDrag(
		IInteractiveDrawable2D* target,
		int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos,
		Vector2& deltaPosW) override;

	pub void onCursorMove(
		IInteractiveDrawable2D* target,
		int cursorIndex,
		Vector2& screenPos, Vector2& localPos,
		Vector2& deltaPosW) override;

	pub void onCursorOver(
		IInteractiveDrawable2D* target,
		int cursorIndex,
		Vector2& screenPos, Vector2& localPos) override;
	pub void onCursorOut(
		IInteractiveDrawable2D* target,
		int cursorIndex,
		Vector2& screenPos, Vector2& localPos) override;

	pub void* getExtraData_voidP() final;
	pub void setExtraData_voidP(void* extraData_voidP) final;

	pub int getExtraData_int() final;
	pub void setExtraData_int(int extraData_int) final;

	pub int getExtraDataB_int() final;
	pub void setExtraDataB_int(int extraDataB_int) final;

	prot int syncDrawSpecs(int combinedInvalidationBitMap, graphics::IRenderer& renderer, RenderData2D* preRenderData) override;

	prot virtual void onIsInputInteractiveChanged();

	prot virtual void onIsEventsEnabledChanged();

	prot void onParentChangePre() override;
	prot void onParentChanged() override;

	prot bool isAnyCursorPressed();

	priv WrappedTouchListener2D* getWrappedTouchListener(ITouchListener* touchListener);
	priv void clearActiveTouchDowns();

	priv void dispatchTouchBoxChangedIsPossible(bool skipVisibleCheck);

	pub ~InteractiveDrawable2D() override;
};
