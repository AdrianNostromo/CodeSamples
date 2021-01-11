#include "InteractiveDrawable2D.h"
#include "IRoot2DInteractive.h"
#include <base/exceptions/LogicException.h>
#include <graphics/visual2d/util/WrappedTouchListener2D.h>
#include <base/math/util/BoolUtil.h>
#include <base/geometry/Circle.h>
#include <base/log/GlobalAccessLogHandler.h>

InteractiveDrawable2D::InteractiveDrawable2D()
  :super()
{
    //void
}

bool InteractiveDrawable2D::getIsInputInteractive() {
    return inputInterractiveButtonsBitMap != 0 ? true : false;
}

int InteractiveDrawable2D::getInputInterractiveButtonsBitMap() {
    return inputInterractiveButtonsBitMap;
}

IInteractiveDrawable2D* InteractiveDrawable2D::setIsInputInteractive(bool isInputInteractive) {
    int new_inputInterractiveButtonsBitMap = isInputInteractive ? base::Touch::ButtonCode::LeftOrTouch->bitGroup : 0;
    
    return setInputInterractiveButtonsBitMap(new_inputInterractiveButtonsBitMap);
}

IInteractiveDrawable2D* InteractiveDrawable2D::setInputInterractiveButtonsBitMap(int _inputInterractiveButtonsBitMap) {
    int old_inputInterractiveButtonsBitMap = this->inputInterractiveButtonsBitMap;

    if (this->inputInterractiveButtonsBitMap == _inputInterractiveButtonsBitMap) {
        return this;
    }

    this->inputInterractiveButtonsBitMap = _inputInterractiveButtonsBitMap;

    if ((old_inputInterractiveButtonsBitMap == 0 && this->inputInterractiveButtonsBitMap != 0)
        || (old_inputInterractiveButtonsBitMap != 0 && this->inputInterractiveButtonsBitMap == 0))
    {
        onIsInputInteractiveChanged();
    }

    return this;
}

int InteractiveDrawable2D::syncDrawSpecs(int combinedInvalidationBitMap, graphics::IRenderer& renderer, RenderData2D* preRenderData) {
    int r = super::syncDrawSpecs(combinedInvalidationBitMap, renderer, preRenderData);
    if (r != 0) {
        return r;
    }

    if (BoolUtil::isBitAtIndex(combinedInvalidationBitMap, InvalidationFlags::transform)) {
        if (std::shared_ptr<Rectangle> touchShape_rectangle = std::dynamic_pointer_cast<Rectangle>(touchShape)) {
			float w;
			if (touchWidthOverride != nullptr) {
				w = *touchWidthOverride;
			} else {
				w = getWidth();
			}
			float h;
			if (touchHeightOverride != nullptr) {
				h = *touchHeightOverride;
			} else {
				h = getHeight();
			}

            touchShape_rectangle->set(
                -w * getAnchorX(),
                -h * getAnchorY(),
                w,
                h
            );
        } else if (std::shared_ptr<Circle> touchShape_circle = std::dynamic_pointer_cast<Circle>(touchShape)) {
            float w = (touchWidthOverride != nullptr) ? *touchWidthOverride : getWidth();
            float h = (touchHeightOverride != nullptr) ? *touchHeightOverride : getHeight();
            // Divide by 4 to also convert from diameter to radius;
            float radius = (touchRadiusOverride != nullptr) ? *touchRadiusOverride : (w + h) / 4.0f;

            touchShape_circle->set(
                w / 2.0f - w * getAnchorX(),
                h / 2.0f - h * getAnchorY(),
                radius
            );
        } else {
            return -2;
        }

        onTouchBoxChanged();
    }

    return 0;
}

void InteractiveDrawable2D::onTouchBoxChanged() {
    isTouchBoxChanged = true;

    dispatchTouchBoxChangedIsPossible(false);
}

bool InteractiveDrawable2D::onCursorDown(
	IInteractiveDrawable2D* target,
	int cursorIndex, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos, Vector2& localPos)
{
	return false;
}

void InteractiveDrawable2D::onCursorUp(
	IInteractiveDrawable2D* target,
	int cursorIndex, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos, Vector2& localPos)
{
	//void
}

void InteractiveDrawable2D::onCursorUpOutside(
	IInteractiveDrawable2D* target,
	int cursorIndex, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos, Vector2& localPos)
{
	//void
}

void InteractiveDrawable2D::onCursorTap(
	IInteractiveDrawable2D* target,
	int cursorIndex, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos, Vector2& localPos)
{
	//void
}

void InteractiveDrawable2D::onCursorDrag(
	IInteractiveDrawable2D* target,
	int cursorIndex, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos, Vector2& localPos,
	Vector2& deltaPosW)
{
	//void
}

void InteractiveDrawable2D::onCursorMove(
	IInteractiveDrawable2D* target,
	int cursorIndex,
	Vector2& screenPos, Vector2& localPos,
	Vector2& deltaPosW)
{
	//void
}

void InteractiveDrawable2D::onCursorOver(
	IInteractiveDrawable2D* target,
	int cursorIndex,
	Vector2& screenPos, Vector2& localPos)
{
	//void
}

void InteractiveDrawable2D::onCursorOut(
	IInteractiveDrawable2D* target,
	int cursorIndex,
	Vector2& screenPos, Vector2& localPos)
{
	//void
}

void InteractiveDrawable2D::clearIsTouchBoxChanged() {
    isTouchBoxChanged = false;
}

void InteractiveDrawable2D::dispatchTouchBoxChangedIsPossible(bool skipVisibleCheck) {
	if(isTouchBoxChanged && getIsInputInteractive() && (skipVisibleCheck || getIsVisible()) && getParent() != nullptr) {
		getParent()->onTouchBoxChanged();
	}
}

void InteractiveDrawable2D::onIsInputInteractiveChanged() {
    if(!getIsInputInteractive() && getRoot() != nullptr) {
        clearActiveTouchDowns();
    }

    if(!getIsInputInteractive()) {
        if(getParent() != nullptr) {
            getParent()->onChildIsInputInteractiveChanged(this, false);
        } else if (getInParent_drawablesWithInputInterractionListEntryRef() != nullptr) {
            throw LogicException(LOC);
        }
    }else {
        if(getParent() != nullptr) {
            getParent()->onChildIsInputInteractiveChanged(this, true);
        } else if (getInParent_drawablesWithInputInterractionListEntryRef() != nullptr) {
            throw LogicException(LOC);
        }
    }

	dispatchTouchBoxChangedIsPossible(false);
}

bool InteractiveDrawable2D::getIsTouchFallthroughEnabled() {
    return isTouchFallthroughEnabled;
}

void InteractiveDrawable2D::setIsTouchFallthroughEnabled(bool isTouchFallthroughEnabled) {
    this->isTouchFallthroughEnabled = isTouchFallthroughEnabled;
}

bool InteractiveDrawable2D::getIsInputFrozen() {
    return isInputFrozen;
}

ArrayList<WrappedTouchListener2D*>* InteractiveDrawable2D::getWrappedTouchListenersList() {
    return &wrappedTouchListenersList;
}

bool InteractiveDrawable2D::getIsEventsEnabled() {
    return isEventsEnabled;
}

void InteractiveDrawable2D::setIsEventsEnabled(bool isEventsEnabled) {
    if(this->isEventsEnabled == isEventsEnabled) {
        return ;
    }

    this->isEventsEnabled = isEventsEnabled;

    onIsEventsEnabledChanged();
}

void InteractiveDrawable2D::onIsEventsEnabledChanged() {
    //void
}

void InteractiveDrawable2D::onParentChangePre() {
	super::onParentChangePre();

	if(getParent() != nullptr && getIsInputInteractive()) {
        getParent()->onChildIsInputInteractiveChanged(this, false);
    } else if (getInParent_drawablesWithInputInterractionListEntryRef() != nullptr) {
        throw LogicException(LOC);
    }

    dispatchTouchBoxChangedIsPossible(false);
}

void InteractiveDrawable2D::onParentChanged() {
	super::onParentChanged();

    if(getParent() != nullptr && getIsInputInteractive()) {
	    getParent()->onChildIsInputInteractiveChanged(this, true);
    } else if (getInParent_drawablesWithInputInterractionListEntryRef() != nullptr) {
        throw LogicException(LOC);
    }

    dispatchTouchBoxChangedIsPossible(false);
}

WrappedTouchListener2D* InteractiveDrawable2D::getWrappedTouchListener(ITouchListener* touchListener) {
    for(int i = 0; i< wrappedTouchListenersList.size(); i++) {
        WrappedTouchListener2D* entry = *wrappedTouchListenersList.getPointer(i);
        if(entry->listener == touchListener) {
            return entry;
        }
    }

    return nullptr;
}

void InteractiveDrawable2D::addTouchListener(ITouchListener* touchListener) {
     if(touchListener == nullptr) {
        throw LogicException(LOC);
    }

    if(!getIsInputInteractive()) {
        // "Attempted to set a touch listener on a drawable without input interactive enabled.";
        throw LogicException(LOC);
    }

    if(getWrappedTouchListener(touchListener) != nullptr) {
        throw LogicException(LOC);
    }

    this->wrappedTouchListenersList.appendDirect(new WrappedTouchListener2D(touchListener));
}

void InteractiveDrawable2D::removeTouchListener(ITouchListener* touchListener) {
    if(touchListener == nullptr) {
        throw LogicException(LOC);
    }

    WrappedTouchListener2D* wrappedTouchListener = getWrappedTouchListener(touchListener);
    int i = wrappedTouchListenersList.indexOf(wrappedTouchListener);
    if(i < 0) {
        throw LogicException(LOC);
    }
    this->wrappedTouchListenersList.remove(i);
}

void InteractiveDrawable2D::onIsVisibleChanged() {
    super::onIsVisibleChanged();

    if(!getIsVisible() && getRoot() != nullptr) {
        clearActiveTouchDowns();
    }

	dispatchTouchBoxChangedIsPossible(true);
}

void InteractiveDrawable2D::onRootChanged_pre() {
    if(getRoot() != nullptr) {
        clearActiveTouchDowns();
    }

    super::onRootChanged_pre();
}

bool InteractiveDrawable2D::getIsButtonPressed(base::Touch::ButtonCode* buttonId) {
	for (int cursorIndex = 0; cursorIndex < isCursorButtonPressedList.size(); cursorIndex++) {
		ArrayList<base::Touch::ButtonCode*>* isPressedButtonList = isCursorButtonPressedList.getPointer(cursorIndex);

        if (buttonId->index < isPressedButtonList->size()) {
            base::Touch::ButtonCode* isPressedBtnId = isPressedButtonList->getDirect(buttonId->index);
            bool isPressed = (isPressedBtnId == nullptr) ? false : true;

            if (isPressed) {
                return true;
            }
        }
	}

	return false;
}

bool InteractiveDrawable2D::getIsCursorButtonPressed(int cursorIndex, base::Touch::ButtonCode* buttonId) {
    while(isCursorButtonPressedList.size() <= cursorIndex) {
        isCursorButtonPressedList.append_emplace();
    }

    ArrayList<base::Touch::ButtonCode*>* isPressedButtonList = isCursorButtonPressedList.getPointer(cursorIndex);
    while(isPressedButtonList->size() <= buttonId->index) {
        isPressedButtonList->appendDirect(0);
    }

    base::Touch::ButtonCode* isPressedBtnId = isPressedButtonList->getDirect(buttonId->index);
    bool isPressed = (isPressedBtnId == nullptr) ? false : true;

    return isPressed;
}

bool InteractiveDrawable2D::isAnyCursorPressed() {
    if(pressedCursorButtonsCount > 0) {
        return true;
    }

    return false;
}

void InteractiveDrawable2D::setIsCursorButtonPressed(int cursorIndex, base::Touch::ButtonCode* buttonId, bool isPressed) {
    bool currentIsPressedState = getIsCursorButtonPressed(cursorIndex, buttonId);

    // GlobalAccessLogHandler::devLogHandler->post_debug("Press status: %i, %i, %s.", cursorIndex, buttonId, isPressed ? "true" : "false");

    if(currentIsPressedState == isPressed) {
        throw LogicException(LOC);
    }

    if(!isPressed) {
        pressedCursorButtonsCount--;
        if(pressedCursorButtonsCount < 0) {
            throw LogicException(LOC);
        }
    }else {
        pressedCursorButtonsCount++;
    }

    // Sub array already exists here because of the call to getIsCursorButtonPressed(...) above.
    base::Touch::ButtonCode* isPressedBtnId = (isPressed == false) ? nullptr : buttonId;
    isCursorButtonPressedList.getPointer(cursorIndex)->setDirect(buttonId->index, isPressedBtnId);
}

bool InteractiveDrawable2D::getIsCursorOver(int cursorIndex) {
	while(isCursorOverList.size() <= cursorIndex) {
		isCursorOverList.append_emplace();
	}

	int isOverI = isCursorOverList.getDirect(cursorIndex);
	bool isOver = (isOverI == 0) ? false : true;

	return isOver;
}

void InteractiveDrawable2D::setIsCursorOver(int cursorIndex, bool isOver) {
	bool currentIsOver = getIsCursorOver(cursorIndex);

	if(currentIsOver == isOver) {
		throw LogicException(LOC);
	}

	// Sub array already exists here because of the call to getIsCursorOver(...) above.
	int isOverI = (isOver == false) ? 0 : 1;
	isCursorOverList.setDirect(cursorIndex, isOverI);
}

int InteractiveDrawable2D::getCursorMoveEventIndex() {
    return cursorMoveEventIndex;
}

void InteractiveDrawable2D::setCursorMoveEventIndex(int cursorMoveEventIndex) {
    this->cursorMoveEventIndex = cursorMoveEventIndex;
}

Map1D<std::string, std::shared_ptr<base::IWrappedValue>>* InteractiveDrawable2D::getUserDataMap() {
    if(userDataMap == nullptr) {
        userDataMap = new Map1D<std::string, std::shared_ptr<base::IWrappedValue>>();
    }

    return userDataMap;
}

void InteractiveDrawable2D::putUserData(std::string key, std::shared_ptr<base::IWrappedValue> value) {
    getUserDataMap()->putDirect(key, value);
}

std::shared_ptr<base::IWrappedValue> InteractiveDrawable2D::getUserData(std::string key) {
    return getUserDataMap()->getDirect(key);
}

std::shared_ptr<Shape2D> InteractiveDrawable2D::getTouchShape() {
    return touchShape;
}

void InteractiveDrawable2D::setTouchShape(std::shared_ptr<Shape2D> touchShape) {
    this->touchShape = touchShape;

    if (getParent()) {
        invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0,
            InvalidationFlags::transform
        ));
    }

    onTouchBoxChanged();
}

void InteractiveDrawable2D::setTouchWidthOverride(std::shared_ptr<float> touchWidthOverride) {
    if((std::dynamic_pointer_cast<Rectangle>(touchShape)) == nullptr) {
        // "Unable to set touch area width override on a non-rectangle shape.";

        throw LogicException(LOC);
    }

    if((this->touchWidthOverride == nullptr && touchWidthOverride == nullptr)
		|| (this->touchWidthOverride != nullptr && touchWidthOverride != nullptr && *this->touchWidthOverride == *touchWidthOverride))
	{
        return ;
    }

    this->touchWidthOverride = touchWidthOverride;

    if(getParent()) {
        invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0,
            InvalidationFlags::transform
        ));
    }
}

void InteractiveDrawable2D::setTouchHeightOverride(std::shared_ptr<float> touchHeightOverride) {
    if((std::dynamic_pointer_cast<Rectangle>(touchShape)) == nullptr) {
        // "Unable to set touch area height override on a non-rectangle shape.";

        throw LogicException(LOC);
    }

	if ((this->touchHeightOverride == nullptr && touchHeightOverride == nullptr)
		|| (this->touchHeightOverride != nullptr && touchHeightOverride != nullptr && *this->touchHeightOverride == *touchHeightOverride))
	{
        return ;
    }

    this->touchHeightOverride = touchHeightOverride;

    if(getParent()) {
        invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0,
            InvalidationFlags::transform
        ));
    }
}

void InteractiveDrawable2D::setTouchRadiusOverride(std::shared_ptr<float> touchRadiusOverride) {
    if((std::dynamic_pointer_cast<Circle>(touchShape)) == nullptr) {
        // "Unable to set touch area radius override on a non-circle shape.";

        throw LogicException(LOC);
    }

	if ((this->touchRadiusOverride == nullptr && touchRadiusOverride == nullptr)
		|| (this->touchRadiusOverride != nullptr && touchRadiusOverride != nullptr && *this->touchRadiusOverride == *touchRadiusOverride))
	{
        return ;
    }

    this->touchRadiusOverride = touchRadiusOverride;

    if(getParent()) {
        invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0,
            InvalidationFlags::transform
        ));
    }
}

std::shared_ptr<base::IWrappedValue> InteractiveDrawable2D::getPropertyValue(std::string property) {
    if(property == "isTouchFallthroughEnabled") {
        return base::IWrappedValue::new_bool(new bool(getIsTouchFallthroughEnabled()));
    }

    else if(property == "touchWidthOverride") {
        return base::IWrappedValue::new_float_sharedPtr(getTouchWidthOverride());
    }else if(property == "touchHeightOverride") {
        return base::IWrappedValue::new_float_sharedPtr(getTouchHeightOverride());
    }

    else if(property == "touchRadiusOverride") {
        base::IWrappedValue::new_float_sharedPtr(getTouchRadiusOverride());
    }

    return super::getPropertyValue(property);
}

void InteractiveDrawable2D::setPropertyValue(std::string property, std::shared_ptr<base::IWrappedValue> value) {
    if(property == "isTouchFallthroughEnabled") {
        setIsTouchFallthroughEnabled(value->getDirectAs_bool());
    }

    else if(property == "touchWidthOverride") {
        setTouchWidthOverride(value->getDirectAs_float_sharedPtr());
    }else if(property == "touchHeightOverride") {
        setTouchHeightOverride(value->getDirectAs_float_sharedPtr());
    }

    else if(property == "touchRadiusOverride") {
        setTouchRadiusOverride(value->getDirectAs_float_sharedPtr());
    }

    else {
        super::setPropertyValue(property, value);
    }
}

void InteractiveDrawable2D::clearActiveTouchDowns() {
    if(getRoot() == nullptr) {
        // This should never occur.

        throw LogicException(LOC);
    }

    for(int cursorIndex=isCursorButtonPressedList.size()-1;cursorIndex>=0;cursorIndex--) {
        ArrayList<base::Touch::ButtonCode*>* isPressedButtonList = isCursorButtonPressedList.getPointer(cursorIndex);

        for(int buttonIndex=isPressedButtonList->size()-1; buttonIndex >=0; buttonIndex--) {
            base::Touch::ButtonCode* isPressedBtnId = isPressedButtonList->getDirect(buttonIndex);
            bool isPressed = (isPressedBtnId == nullptr) ? false : true;

            if(isPressed) {
                (dynamic_cast<IRoot2DInteractive*>(getRoot()))->clearExistingTouchDownTarget(this, cursorIndex, isPressedBtnId);
            }
        }
    }

	for(int cursorIndex=isCursorOverList.size()-1;cursorIndex>=0;cursorIndex--) {
		int isOverI = isCursorOverList.getDirect(cursorIndex);
        bool isOver = (isOverI == 0) ? false : true;

        if(isOver) {
            (dynamic_cast<IRoot2DInteractive*>(getRoot()))->clearExistingCursorOverTarget(this, cursorIndex);
        }
	}
}

void* InteractiveDrawable2D::getExtraData_voidP() {
    return extraData_voidP;
}

void InteractiveDrawable2D::setExtraData_voidP(void* extraData_voidP) {
    this->extraData_voidP = extraData_voidP;
}

int InteractiveDrawable2D::getExtraData_int() {
    return extraData_int;
}

void InteractiveDrawable2D::setExtraData_int(int extraData_int) {
    this->extraData_int = extraData_int;
}

int InteractiveDrawable2D::getExtraDataB_int() {
    return extraDataB_int;
}

void InteractiveDrawable2D::setExtraDataB_int(int extraDataB_int) {
    this->extraDataB_int = extraDataB_int;
}

InteractiveDrawable2D::~InteractiveDrawable2D() {
	if (userDataMap != nullptr) {
		delete userDataMap;

		userDataMap = nullptr;
	}
}
