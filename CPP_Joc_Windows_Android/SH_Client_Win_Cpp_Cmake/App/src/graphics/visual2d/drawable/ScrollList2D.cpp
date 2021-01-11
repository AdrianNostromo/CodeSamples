#include "ScrollList2D.h"
#include <base/log/GlobalAccessLogHandler.h>
#include <base/math/Math.h>
#include <base/app/IAppMetrics.h>
#include <base/ease/Ease.h>

using namespace base;

const int ScrollList2D::ScrollStatus::Off = 0;
const int ScrollList2D::ScrollStatus::Drag_Pre = 1;
const int ScrollList2D::ScrollStatus::Drag_Manual = 2;
const int ScrollList2D::ScrollStatus::Flying = 3;
const int ScrollList2D::ScrollStatus::ScrollToPosition = 4;

ScrollList2D::CbListener::CbListener()
	: super::CbListener()
{
	//void
}

void ScrollList2D::CbListener::onScrollOccurred(ScrollList2D* list, float scrollDeltaX, float scrollDeltaY, float newPosX, float newPosY, float scrollDeltaTotal) {
	if (cb_onScrollOccurred) {
		cb_onScrollOccurred(list, scrollDeltaX, scrollDeltaY, newPosX, newPosY, scrollDeltaTotal);
	}
}

void ScrollList2D::CbListener::onScrollStarted(ScrollList2D* list) {
	if (cb_onScrollStarted) {
		cb_onScrollStarted(list);
	}
}

void ScrollList2D::CbListener::onScrollEnded(ScrollList2D* list) {
	if (cb_onScrollEnded) {
		cb_onScrollEnded(list);
	}
}

void ScrollList2D::CbListener::onBgTouchDownOccurred(ScrollList2D* list) {
	if (cb_onBgTouchDownOccurred) {
		cb_onBgTouchDownOccurred(list);
	}
}

ScrollList2D::CbListener::~CbListener() {
	//void
}

ScrollList2D::ScrollList2D(IAppMetrics* appMetrics, IAppAssets* appAssets)
	: super(appMetrics, appAssets)
{
	touchListener.cb_onCursorDown = std::bind(&ScrollList2D::v2d_onCursorDown, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
	touchListener.cb_onCursorUp = std::bind(&ScrollList2D::v2d_onCursorUp, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
	touchListener.cb_onCursorUpOutside = std::bind(&ScrollList2D::v2d_onCursorUpOutside, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
	touchListener.cb_onCursorDrag = std::bind(&ScrollList2D::v2d_onCursorDrag, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6);
}

void ScrollList2D::setListener(IListener* listener) {
	super::setListener(listener);

	this->listener = listener;
}

bool ScrollList2D::v2d_onCursorDown(
	IInteractiveDrawable2D* target,
	int cursorIndex, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos, Vector2& localPos)
{
	// Touch events are ignored while the menu is animating.
	if (!getIsScrollEnabled()) {
		return false;
	}

	if (target == touchAreaBack) {
		scrollTouchIndex = cursorIndex;
		scrollButtonId = buttonId;

		startScroll(screenPos.x, screenPos.y);

		if (listener != nullptr) {
			listener->onBgTouchDownOccurred(this);
		}

		// Return true so we receive move events and/or touchUpOutside events.
		return true;
	} else if (target->getUserDataMap()->getDirectB("listEntry", nullptr) != nullptr) {
		touchDownListEntry = target->getUserDataMap()->getDirectB("listEntry")->getDirectAs_Custom<ListEntry2D*>(-1);
	} else {
		GlobalAccessLogHandler::devLogHandler->post_debug("Error", "Touch event received for a unknown target.");
	}

	return false;
}

void ScrollList2D::v2d_onCursorUp(
	IInteractiveDrawable2D* target,
	int cursorIndex, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos, Vector2& localPos)
{
	if (!getIsScrollEnabled()) {
		return;
	}

	if (target == touchAreaBack) {
		if (scrollTouchIndex == cursorIndex && scrollButtonId == buttonId) {
			endScroll(true);
		}
	} else if (target->getUserDataMap()->getDirectB("listEntry", nullptr) != nullptr) {
		if (touchDownListEntry != nullptr) {
			if (touchDownListEntry == target->getUserDataMap()->getDirectB("listEntry")->getDirectAs_Custom<ListEntry2D*>(-1)) {
				if (listener != nullptr) {
					ListEntry2D* entry = target->getUserDataMap()->getDirectB("listEntry")->getDirectAs_Custom<ListEntry2D*>(-1);
					listener->onEntryTouched(entry->list, entry);
				}

				entryActivateDSound->play(entryActivateSound_volume);
			}

			touchDownListEntry = nullptr;
		}
	}
}

void ScrollList2D::v2d_onCursorUpOutside(
	IInteractiveDrawable2D* target,
	int cursorIndex, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos, Vector2& localPos)
{
	if (!getIsScrollEnabled()) {
		return;
	}

	if (target == touchAreaBack) {
		if (scrollTouchIndex == cursorIndex && scrollButtonId == buttonId) {
			endScroll(true);
		}
	} else if (target->getUserDataMap()->getDirectB("listEntry", nullptr) != nullptr) {
		touchDownListEntry = nullptr;
	}
}

void ScrollList2D::v2d_onCursorDrag(
	IInteractiveDrawable2D* target,
	int cursorIndex, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos, Vector2& localPos,
	Vector2& deltaPosW)
{
	if (!getIsScrollEnabled()) {
		return;
	}

	if (target == touchAreaBack) {
		if (scrollTouchIndex == cursorIndex && scrollButtonId == buttonId) {
			scrollMove(screenPos.x, screenPos.y);
		}
	}
}

void ScrollList2D::initializeListTouchAreaBackTouchListener() {
	// super::initializeListTouchAreaBackTouchListener();

	touchAreaBack->addTouchListener(&touchListener);
}

void ScrollList2D::initializeNewEntryTouchHandling(IInteractiveDrawable2D* touchArea) {
	// super::initializeNewEntryTouchHandling(touchArea);

	// Enable item touch fall through.
	touchArea->setIsTouchFallthroughEnabled(true);
	touchArea->addTouchListener(&touchListener);
}

int ScrollList2D::getScrollStatus() {
	return scrollStatus;
}

bool ScrollList2D::isScrollActive() {
	if (getScrollStatus() == ScrollStatus::Off) {
		return false;
	}

	return true;
}

void ScrollList2D::setScrollStatus(int scrollStatus) {
	if (this->scrollStatus == scrollStatus) {
		return;
	}

	int old_scrollStatus = this->scrollStatus;
	this->scrollStatus = scrollStatus;

	if (old_scrollStatus == ScrollStatus::Off && getScrollStatus() != ScrollStatus::Off) {
		if (listener != nullptr) {
			listener->onScrollStarted(this);
		}
	} else if(old_scrollStatus != ScrollStatus::Off && getScrollStatus() == ScrollStatus::Off) {
		if (listener != nullptr) {
			listener->onScrollEnded(this);
		}
	}
}

void ScrollList2D::syncVisuals(bool syncAbsoluteValues, bool syncAnimatingValues, std::string startStateID, std::string endStateID, float stateCompletionPercent) {
	super::syncVisuals(syncAbsoluteValues, syncAnimatingValues, startStateID, endStateID, stateCompletionPercent);

	updateScrollingLimits();

	if (rowsCount != 0) {
		//Horizontal list.
		float posX = layerEntries->getX();
		setScrollPosition(&posX, nullptr, false);
	} else if (columnsCount != 0) {
		// Vertical list.
		float posY = layerEntries->getY();
		setScrollPosition(nullptr, &posY, false);
	} else {
		throw LogicException(LOC);
	}
}

void ScrollList2D::handleContentAlignmentX() {
	if (rowsCount == 0 || contentBounds.getWidth() <= listWidth) {
		// Not_horizontal_list or content_size_x not_bigger than list_size_x.
		super::handleContentAlignmentX();
	}
}

void ScrollList2D::handleContentAlignmentY() {
	if (columnsCount == 0 || contentBounds.getHeight() <= listHeight) {
		// Not_vertical_list or content_size_y not_bigger than list_size_y.
		super::handleContentAlignmentY();
	}
}

ListEntry2D* ScrollList2D::createNewEntry(
	ArrayList<MenuItemConfig*>* useItemConfigs, ArrayList<StateChangeDurations*>* easeDurationsSList, int listPositionIndex, Map1D<std::string, std::shared_ptr<base::IWrappedValue>>* extraDataMap,
	int entryUid)
{
	ListEntry2D* ret = super::createNewEntry(
		useItemConfigs, easeDurationsSList, listPositionIndex, extraDataMap,
		entryUid
	);

	updateScrollingLimits();

	return ret;
}

ListEntry2D* ScrollList2D::createNewEntry(
	ArrayList<MenuItemConfig*>* itemInstanceConfigOverride, ArrayList<StateChangeDurations*>* easeDurationsSList, Map1D<std::string, std::shared_ptr<base::IWrappedValue>>* extraDataMap,
	int entryUid)
{
	return createNewEntry(
		itemInstanceConfigOverride, easeDurationsSList, -1, extraDataMap,
		entryUid
	);
}

void ScrollList2D::removeEntry(ListEntry2D* entry) {
	super::removeEntry(entry);

	updateScrollingLimits();
}

bool ScrollList2D::getIsScrollEnabled() {
	return isScrollEnabled;
}

void ScrollList2D::setIsScrollEnabled(bool isScrollEnabled) {
	this->isScrollEnabled = isScrollEnabled;

	endScroll(false);

	if (!this->isScrollEnabled) {
		touchDownListEntry = nullptr;
	}
}

void ScrollList2D::removeAllEntries() {
	super::removeAllEntries();

	touchDownListEntry = nullptr;
}

float ScrollList2D::getScrollOffset() {
	return Math::dist(0.0f, 0.0f, layerEntries->getX(), layerEntries->getY());
}

float ScrollList2D::getScrollOffsetX() {
	return layerEntries->getX();
}

float ScrollList2D::getScrollOffsetY() {
	return layerEntries->getY();
}

float ScrollList2D::getScrollOffsetDIP() {
	return Math::dist(0.0f, 0.0f, layerEntries->getX(), layerEntries->getY()) / appMetrics->getScreen()->getScreenDensityMultiplier()->x;
}

float ScrollList2D::getScrollLimit_x_start() {
	return scrollLimit_x_start;
}

float ScrollList2D::getScrollLimit_x_end() {
	return scrollLimit_x_end;
}

float ScrollList2D::getScrollLimit_y_start() {
	return scrollLimit_y_start;
}
float ScrollList2D::getScrollLimit_y_end() {
	return scrollLimit_y_end;
}

void ScrollList2D::scrollEntryIntoView(ListEntry2D* entry, bool useAnimation) {
	IDynamicPropertiesObject2D* size = entry->getItemMustExistAs< IDynamicPropertiesObject2D* >("size");
	
	IDrawableNode2D* entryMainLayer = entry->getItemMustExistAs< IDrawableNode2D* >("layer");

	if (rowsCount != 0) {
		//Horizontal list.
		float entryWidth = size->getPropertyValue("width")->getDirectAs_float();

		float posInVisibleArea_left = entryMainLayer->getX() + layerEntries->getX() - listWidth * anchorX;
		float posInVisibleArea_right = posInVisibleArea_left + entryWidth;

		if (posInVisibleArea_left < 0.0f) {
			float f = layerEntries->getX() - posInVisibleArea_left;
			scrollToPosition(&f, nullptr, true, useAnimation);
		} else if (posInVisibleArea_right > listWidth) {
			float f = layerEntries->getX() - (posInVisibleArea_right - listWidth);
			scrollToPosition(&f, nullptr, true, useAnimation);
		}
	} else if (columnsCount != 0) {
		float entryHeight = size->getPropertyValue("height")->getDirectAs_float();

		float posInVisibleArea_bottom = entryMainLayer->getY() + layerEntries->getY() - listHeight * anchorY;
		float posInVisibleArea_top = posInVisibleArea_bottom + entryHeight;

		if (posInVisibleArea_bottom < 0.0f) {
			float f = layerEntries->getY() - posInVisibleArea_bottom;
			scrollToPosition(nullptr, &f, true, useAnimation);
		} else if (posInVisibleArea_top > listHeight) {
			float f = layerEntries->getY() - (posInVisibleArea_top - listHeight);
			scrollToPosition(nullptr, &f, true, useAnimation);
		}
	} else {
		throw LogicException(LOC);
	}
}

void ScrollList2D::scrollEntryIntoView(int entryIndex, bool useAnimation) {
	ListEntry2D* entry = getEtryWithIndex(entryIndex);
	if (entry == nullptr) {
		throw LogicException(LOC);
	}

	scrollEntryIntoView(entry, useAnimation);
}

void ScrollList2D::scrollToPosition(float* listPosX, float* listPosY, bool dispatchEvent, bool useAnimation) {
	activeScroll.set(listPosX, listPosY, useAnimation, dispatchEvent);
	setScrollStatus(ScrollStatus::ScrollToPosition);

	if (!useAnimation) {
		tickActiveScrollToPosition(0.0f);
	}
}

void ScrollList2D::setScrollPosition(float* listPosX, float* listPosY, bool dispatchEvent) {
	float targetPosX;
	float targetPosY;
	if (listPosX != nullptr && contentBounds.getWidth() > listWidth) {
		// Max, min is inverted because the bounds values are inverted.
		targetPosX = Math::max(scrollLimit_x_end, Math::min(scrollLimit_x_start, *listPosX));
	} else {
		targetPosX = layerEntries->getX();
	}
	if (listPosY != nullptr && contentBounds.getHeight() > listHeight) {
		// Max, min is inverted because the bounds values are inverted.
		targetPosY = Math::max(scrollLimit_y_end, Math::min(scrollLimit_y_start, *listPosY));
	} else {
		targetPosY = layerEntries->getY();
	}

	layerEntries->setPosition(targetPosX, targetPosY);
}

bool ScrollList2D::isScrollAtBeginning() {
	// Check only the scrolled axis. The other axis may be aligned and not be valid in this logic.
	if (rowsCount != 0) {
		//Horizontal list.
		if (contentBounds.getWidth() <= listWidth || Math::isEqual(layerEntries->getX(), scrollLimit_x_start)) {
			return true;
		}
	} else if (columnsCount != 0) {
		// Vertical list.
		if (contentBounds.getHeight() <= listHeight || Math::isEqual(layerEntries->getY(), scrollLimit_y_start)) {
			return true;
		}
	} else {
		throw LogicException(LOC);
	}

	return false;
}

bool ScrollList2D::isScrollAtEnd() {
	// Check only the scrolled axis. The other axis may be aligned and not be valid in this logic.
	if (rowsCount != 0) {
		//Horizontal list.
		if (contentBounds.getWidth() <= listWidth || Math::isEqual(layerEntries->getX(), scrollLimit_x_end)) {
			return true;
		}
	} else if (columnsCount != 0) {
		// Vertical list.
		if (contentBounds.getHeight() <= listHeight || Math::isEqual(layerEntries->getY(), scrollLimit_y_end)) {
			return true;
		}
	} else {
		throw LogicException(LOC);
	}

	return false;
}

void ScrollList2D::visualLoop(float deltaS, int deltaMS) {
	super::visualLoop(deltaS, deltaMS);

	if (!getIsScrollEnabled()) {
		return;
	}
	
	if (getScrollStatus() != ScrollStatus::Off) {
		if (getScrollStatus() == ScrollStatus::Drag_Pre
			|| getScrollStatus() == ScrollStatus::Drag_Manual)
		{
			if (Math::abs(currentDragTotalDelta.len()) >= touchDeselectionScrollDeltaThreshold) {
				touchDownListEntry = nullptr;
			}
		}

		if (getScrollStatus() == ScrollStatus::Drag_Pre) {
			if (Math::abs(currentDragTotalDelta.len()) >= preScrollDeltaThreshold) {
				setScrollStatus(ScrollStatus::Drag_Manual);
			}
		} else if (getScrollStatus() == ScrollStatus::Drag_Manual) {
			if (currentDragTickDelta.x != 0 || currentDragTickDelta.y != 0) {
				if (rowsCount != 0) {
					//Horizontal list.
					float newListPosX = layerEntries->getX() + currentDragTickDelta.x;
					setScrollPosition(&newListPosX, nullptr, true);
				} else if (columnsCount != 0) {
					// Vertical list.
					float newListPosY = layerEntries->getY() + currentDragTickDelta.y;
					setScrollPosition(nullptr, &newListPosY, true);
				} else {
					throw LogicException(LOC);
				}
			}
		} else if (getScrollStatus() == ScrollStatus::Flying) {
			//asd_002;
		}

		currentDragTickDelta.set(0.0f, 0.0f);
	}

	if (getScrollStatus() == ScrollStatus::ScrollToPosition) {
		tickActiveScrollToPosition(deltaS);
	}
}

void ScrollList2D::tickActiveScrollToPosition(float deltaS) {
	if (getScrollStatus() == ScrollStatus::ScrollToPosition) {
		float targetPosX;
		float targetPosY;
		if (activeScroll.isScrollX && contentBounds.getWidth() > listWidth) {
			// Max, min is inverted because the bounds values are inverted.
			targetPosX = Math::max(scrollLimit_x_end, Math::min(scrollLimit_x_start, activeScroll.targetListPosX));
		} else {
			targetPosX = layerEntries->getX();
		}
		if (activeScroll.isScrollY && contentBounds.getHeight() > listHeight) {
			// Max, min is inverted because the bounds values are inverted.
			targetPosY = Math::max(scrollLimit_y_end, Math::min(scrollLimit_y_start, activeScroll.targetListPosY));
		} else {
			targetPosY = layerEntries->getY();
		}

		float newPosX;
		float newPosY;
		bool isEndReached = false;
		if (!activeScroll.useAnimation) {
			newPosX = targetPosX;
			newPosY = targetPosY;

			isEndReached = true;
		}else{
			// Animate.
			Vector2 delta{
				targetPosX - layerEntries->getX(),
				targetPosY - layerEntries->getY()
			};

			float speedPercent = Math::min(1.0f, delta.len() / animatedMaxScrollSpeedAtDistance);
			float easedSpeedPercent;
			if (animatedScrollEaseFunction != nullptr) {
				easedSpeedPercent = Ease::computeEaseValue(*animatedScrollEaseFunction, speedPercent);
			} else {
				easedSpeedPercent = speedPercent;
			}

			float speedS = animatedMinScrollSpeedS + (animatedMaxScrollSpeedS - animatedMinScrollSpeedS) * easedSpeedPercent;
			float speedTick = speedS * deltaS;

			if (speedTick >= delta.len()) {
				isEndReached = true;

				newPosX = targetPosX;
				newPosY = targetPosY;
			} else {
				delta.nor();

				newPosX = layerEntries->getX() + delta.x * speedTick;
				newPosY = layerEntries->getY() + delta.y * speedTick;
			}
		}

		layerEntries->setPosition(newPosX, newPosY);

		float scrollDeltaX = newPosX - layerEntries->getX();
		float scrollDeltaY = newPosY - layerEntries->getY();

		float scrollDeltaTotal = Math::dist(0.0f, 0.0f, scrollDeltaX, scrollDeltaY);

		if (activeScroll.dispatchEvent && listener != nullptr) {
			if (scrollDeltaTotal != 0) {
				listener->onScrollOccurred(this, scrollDeltaX, scrollDeltaY, newPosX, newPosY, scrollDeltaTotal);
			}
		}

		if (isEndReached) {
			setScrollStatus(ScrollStatus::Off);
		}
	}
}

//asd_002;// implement scrolling inertia fling;
//asd_002;// when the list is scrolled to the end, add a loading more spinner preloader.
//asd_002;// list scroll bar as indicator only.
//asd_002;// the list has some elasticity when over-dragging is attempted.
//asd_002;// implement a refresh when the list is dragged down on the first listEntry.

void ScrollList2D::updateScrollingLimits() {
	Bounds2D* contentBounds = getContentBounds();

	scrollLimit_x_start = -contentBounds->min.x;
	scrollLimit_x_end = (listWidth - contentBounds->getWidth()) - contentBounds->min.x;

	scrollLimit_y_start = -contentBounds->min.y;
	scrollLimit_y_end = (listHeight - contentBounds->getHeight()) - contentBounds->min.y;
}

void ScrollList2D::startScroll(float worldX, float worldY) {
	if (getScrollStatus() != ScrollStatus::Off) {
		endScroll(false);
	}

	float processedWorldX = 0.0f;
	float processedWorldY = 0.0f;
	if (rowsCount != 0) {
		//Horizontal list.
		processedWorldX = worldX;
	} else if (columnsCount != 0) {
		// Vertical list.
		processedWorldY = worldY;
	} else {
		throw LogicException(LOC);
	}

	setScrollStatus(ScrollStatus::Drag_Pre);
	currentDragTotalDelta.set(0.0f, 0.0f);
	currentDragTickDelta.set(0.0f, 0.0f);
	currentDragLastTickPos.set(processedWorldX, processedWorldY);
}

void ScrollList2D::endScroll(bool doFling) {
	if (getScrollStatus() == ScrollStatus::Off || getScrollStatus() == ScrollStatus::ScrollToPosition) {
		return;
	}

	if (!doFling) {
		setScrollStatus(ScrollStatus::Off);
	} else {
		setScrollStatus(ScrollStatus::Off);//asd_tst;
		/*asd_tst;
		if(asd_enoughVelocityForFling) {
			setScrollStatus(DRAG_PHASE_FLING);
		}*/
	}
}

void ScrollList2D::scrollMove(float worldX, float worldY) {
	if (getScrollStatus() == ScrollStatus::Off || getScrollStatus() == ScrollStatus::ScrollToPosition) {
		return;
	}

	float processedWorldX = 0.0f;
	float processedWorldY = 0.0f;
	if (rowsCount != 0) {
		//Horizontal list.
		processedWorldX = worldX;
	} else if (columnsCount != 0) {
		// Vertical list.
		processedWorldY = worldY;
	} else {
		throw LogicException(LOC);
	}

	float deltaX = processedWorldX - currentDragLastTickPos.x;
	float deltaY = processedWorldY - currentDragLastTickPos.y;

	currentDragLastTickPos.set(processedWorldX, processedWorldY);

	currentDragTotalDelta.set(currentDragTotalDelta.x + deltaX, currentDragTotalDelta.y + deltaY);
	currentDragTickDelta.set(currentDragTickDelta.x + deltaX, currentDragTickDelta.y + deltaY);
}

std::shared_ptr<base::IWrappedValue> ScrollList2D::getPropertyValue(std::string property) {
	if (property == "preScrollDeltaThreshold") {
		return base::IWrappedValue::new_float(preScrollDeltaThreshold);
	} else if (property == "touchDeselectionScrollDeltaThreshold") {
		return base::IWrappedValue::new_float(touchDeselectionScrollDeltaThreshold);
	}
	
	else if (property == "animatedMinScrollSpeedS") {
		return base::IWrappedValue::new_float(animatedMinScrollSpeedS);
	} else if (property == "animatedMaxScrollSpeedS") {
		return base::IWrappedValue::new_float(animatedMaxScrollSpeedS);
	} else if (property == "animatedMaxScrollSpeedAtDistance") {
		return base::IWrappedValue::new_float(animatedMaxScrollSpeedAtDistance);
	} else if (property == "animatedScrollEaseFunction") {
		return base::IWrappedValue::new_String_ptr(animatedScrollEaseFunction);
	}

	return super::getPropertyValue(property);
}

void ScrollList2D::setPropertyValue(std::string property, std::shared_ptr<base::IWrappedValue> value) {
	bool isChange = false;

	if (property == "preScrollDeltaThreshold") {
		isChange = CheckedSet(preScrollDeltaThreshold, value->getDirectAs_float());
	} else if (property == "touchDeselectionScrollDeltaThreshold") {
		isChange = CheckedSet(touchDeselectionScrollDeltaThreshold, value->getDirectAs_float());
	}
	
	else if (property == "animatedMinScrollSpeedS") {
		isChange = CheckedSet(animatedMinScrollSpeedS, value->getDirectAs_float());
	}else if (property == "animatedMaxScrollSpeedS") {
		isChange = CheckedSet(animatedMaxScrollSpeedS, value->getDirectAs_float());
	}else if (property == "animatedMaxScrollSpeedAtDistance") {
		isChange = CheckedSet(animatedMaxScrollSpeedAtDistance, value->getDirectAs_float());
	}else if (property == "animatedScrollEaseFunction") {
		isChange = CheckedSet(&animatedScrollEaseFunction, value->getDirectAs_String_ptr());
	}
	
	else {
		super::setPropertyValue(property, value);
	}

	if (isChange) {
		setIsDirty_property();
	}
}

ScrollList2D::~ScrollList2D() {
	//void
}
