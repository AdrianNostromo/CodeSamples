#pragma once

#include <base/gh.h>
#include <graphics/visual2d/drawable/ISizedDrawable2D.h>
#include <string>
#include <base/map/Map1D.h>
#include <base/list/ArrayList.h>
#include <base/interaction/ITouchListener.h>
#include <base/input/util/Touch.h>

class WrappedTouchListener2D;

class IInteractiveDrawable2D : virtual public ISizedDrawable2D, virtual public ITouchListener {
	pub virtual bool getIsInputInteractive() = 0;
	pub virtual int getInputInterractiveButtonsBitMap() = 0;
	// Return self for function call chaining.
	// Note. This function only sets the LMB button monitor. To monitor others, use setInputInterractiveButtonsBitMap.
	pub virtual IInteractiveDrawable2D* setIsInputInteractive(bool isInputInteractive) = 0;
	pub virtual IInteractiveDrawable2D* setInputInterractiveButtonsBitMap(int inputInterractiveButtonsBitMap) = 0;

	pub virtual bool getIsTouchFallthroughEnabled() = 0;
	pub virtual void setIsTouchFallthroughEnabled(bool isTouchFallthroughEnabled) = 0;
	pub virtual bool getIsInputFrozen() = 0;
    
	pub virtual ArrayList<WrappedTouchListener2D*>* getWrappedTouchListenersList() = 0;
	pub virtual void addTouchListener(ITouchListener* touchListener) = 0;
	pub virtual void removeTouchListener(ITouchListener* touchListener) = 0;

	pub virtual bool getIsButtonPressed(base::Touch::ButtonCode* buttonId) = 0;

	pub virtual bool getIsCursorButtonPressed(int cursorIndex, base::Touch::ButtonCode* buttonId) = 0;
	pub virtual void setIsCursorButtonPressed(int cursorIndex, base::Touch::ButtonCode* buttonId, bool isPressed) = 0;

	pub virtual bool getIsCursorOver(int cursorIndex) = 0;
	pub virtual void setIsCursorOver(int cursorIndex, bool isOver) = 0;
	pub virtual int getCursorMoveEventIndex() = 0;
	pub virtual void setCursorMoveEventIndex(int cursorMoveEventIndex) = 0;

	pub virtual Map1D<std::string, std::shared_ptr<base::IWrappedValue>>* getUserDataMap() = 0;
	pub virtual void putUserData(std::string key, std::shared_ptr<base::IWrappedValue> value) = 0;
	pub virtual std::shared_ptr<base::IWrappedValue> getUserData(std::string key) = 0;
    
	pub virtual bool getIsEventsEnabled() = 0;
	pub virtual void setIsEventsEnabled(bool isEventsEnabled) = 0;

	pub virtual std::shared_ptr<Shape2D> getTouchShape() = 0;
	pub virtual void setTouchShape(std::shared_ptr<Shape2D> touchShape) = 0;

	pub virtual std::shared_ptr<float> getTouchWidthOverride() = 0;
	pub virtual void setTouchWidthOverride(std::shared_ptr<float> touchWidthOverride) = 0;
	pub virtual std::shared_ptr<float> getTouchHeightOverride() = 0;
	pub virtual void setTouchHeightOverride(std::shared_ptr<float> touchHeightOverride) = 0;

	pub virtual std::shared_ptr<float> getTouchRadiusOverride() = 0;
	pub virtual void setTouchRadiusOverride(std::shared_ptr<float> touchRadiusOverride) = 0;

	pub virtual void onTouchBoxChanged() = 0;
	pub virtual void clearIsTouchBoxChanged() = 0;

	pub virtual void* getExtraData_voidP() = 0;
	pub virtual void setExtraData_voidP(void* extraData_voidP) = 0;

	pub virtual int getExtraData_int() = 0;
	pub virtual void setExtraData_int(int extraData_int) = 0;

	pub virtual int getExtraDataB_int() = 0;
	pub virtual void setExtraDataB_int(int extraDataB_int) = 0;

};
