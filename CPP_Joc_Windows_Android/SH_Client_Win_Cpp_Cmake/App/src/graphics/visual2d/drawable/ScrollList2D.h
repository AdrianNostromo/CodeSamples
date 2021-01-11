#pragma once

#include <base/gh.h>
#include "List2D.h"
#include <base/input/util/Touch.h>

namespace base {
class ScrollList2D : public List2D {
private: typedef List2D super;
public:
	class IListener : virtual public super::IListener {
	public:
		virtual void onScrollOccurred(ScrollList2D* list, float scrollDeltaX, float scrollDeltaY, float newPosX, float newPosY, float scrollDeltaTotal) = 0;
		virtual void onScrollStarted(ScrollList2D* list) = 0;
		virtual void onScrollEnded(ScrollList2D* list) = 0;
		virtual void onBgTouchDownOccurred(ScrollList2D* list) = 0;

		virtual ~IListener() = default;
	};
	class CbListener : public super::CbListener, virtual public IListener {
	public:
		std::function<void(
			ScrollList2D* list, float scrollDeltaX, float scrollDeltaY, float newPosX, float newPosY, float scrollDeltaTotal)> cb_onScrollOccurred;
		std::function<void(
			ScrollList2D* list)> cb_onScrollStarted;
		std::function<void(
			ScrollList2D* list)> cb_onScrollEnded;
		std::function<void(
			ScrollList2D* list)> cb_onBgTouchDownOccurred;
	public:
		CbListener(CbListener const&) = delete;
		CbListener(CbListener&&) = default;
		CbListener& operator=(CbListener const&) = delete;
		CbListener& operator=(CbListener&&) = default;

		explicit CbListener();

		void onScrollOccurred(ScrollList2D* list, float scrollDeltaX, float scrollDeltaY, float newPosX, float newPosY, float scrollDeltaTotal) override;
		void onScrollStarted(ScrollList2D* list) override;
		void onScrollEnded(ScrollList2D* list) override;
		void onBgTouchDownOccurred(ScrollList2D* list) override;

		~CbListener() override;
	};
private:
	class ScrollStatus {
	public:
		static const int Off;
		static const int Drag_Pre;
		static const int Drag_Manual;
		static const int Flying;
		static const int ScrollToPosition;
	};
	class ActiveScroll {
		pub bool useAnimation = false;
		pub bool dispatchEvent = false;

		pub bool isScrollX = false;
		pub float targetListPosX = 0.0f;

		pub bool isScrollY = false;
		pub float targetListPosY = 0.0f;

		pub void set(float* listPosX, float* listPosY, bool useAnimation, bool dispatchEvent) {
			this->useAnimation = useAnimation;
			this->dispatchEvent = dispatchEvent;

			if (listPosX != nullptr) {
				this->isScrollX = true;
				this->targetListPosX = *listPosX;
			} else {
				this->isScrollX = false;
			}
			if (listPosY != nullptr) {
				this->isScrollY = true;
				this->targetListPosY = *listPosY;
			} else {
				this->isScrollY = false;
			}
		}
	};

private:
	CbTouchListener touchListener{};

	IListener* listener = nullptr;

	// The drag distance required before scroll starts.
	float preScrollDeltaThreshold = 0.0f;
	// Drag distance at which the touch down list listEntry is no longer activated on touch up.
	// Recommended value is "~2.5 * preScrollDeltaThreshold";
	float touchDeselectionScrollDeltaThreshold = 0.0f;

	float animatedMinScrollSpeedS = 50.0f;
	float animatedMaxScrollSpeedS = 100.0f;
	float animatedMaxScrollSpeedAtDistance = 100.0f;
	std::string* animatedScrollEaseFunction = nullptr;

	ListEntry2D* touchDownListEntry = nullptr;

	int scrollStatus = ScrollStatus::Off;
	Vector2 currentDragLastTickPos{};
	Vector2 currentDragTotalDelta{};
	Vector2 currentDragTickDelta{};

	bool isScrollEnabled = true;

	// The bound values are inverted so that xMin is scroll 0% and xMax is 100%.
	float scrollLimit_x_start = 0.0f;
	float scrollLimit_x_end = 0.0f;

	float scrollLimit_y_start = 0.0f;
	float scrollLimit_y_end = 0.0f;

	int scrollTouchIndex = -1;
	base::Touch::ButtonCode* scrollButtonId = nullptr;

	priv ActiveScroll activeScroll{};

public:
    ScrollList2D(ScrollList2D const&) = delete;
    ScrollList2D(ScrollList2D &&) = default;
    ScrollList2D& operator=(ScrollList2D const&) = delete;
    ScrollList2D& operator=(ScrollList2D &&) = default;

    explicit ScrollList2D(IAppMetrics* appMetrics, IAppAssets* appAssets);

	void setListener(IListener* listener);

	bool isScrollActive();

	ListEntry2D* createNewEntry(
		ArrayList<MenuItemConfig*>* useItemConfigs, ArrayList<StateChangeDurations*>* easeDurationsSList, int listPositionIndex, Map1D<std::string, std::shared_ptr<base::IWrappedValue>>* extraDataMap,
		int entryUid = -1) override;
	ListEntry2D* createNewEntry(
		ArrayList<MenuItemConfig*>* itemInstanceConfigOverride, ArrayList<StateChangeDurations*>* easeDurationsSList, Map1D<std::string, std::shared_ptr<base::IWrappedValue>>* extraDataMap,
		int entryUid = -1) override;

	void removeEntry(ListEntry2D* listEntry) override;
	bool getIsScrollEnabled();
	void setIsScrollEnabled(bool isScrollEnabled);

	void removeAllEntries() override;

	float getScrollOffset();
	float getScrollOffsetX();
	float getScrollOffsetY();
	float getScrollOffsetDIP();

	float getScrollLimit_x_start();
	float getScrollLimit_x_end();

	float getScrollLimit_y_start();
	float getScrollLimit_y_end();

	void scrollEntryIntoView(ListEntry2D* entry, bool useAnimation);
	void scrollEntryIntoView(int entryIndex, bool useAnimation);

	void scrollToPosition(float* listPosX, float* listPosY, bool dispatchEvent, bool useAnimation);
	void setScrollPosition(float* listPosX, float* listPosY, bool dispatchEvent);

	bool isScrollAtBeginning();
	bool isScrollAtEnd();

	void visualLoop(float deltaS, int deltaMS) override;
	void tickActiveScrollToPosition(float deltaS);

	std::shared_ptr<base::IWrappedValue> getPropertyValue(std::string property) override;
	void setPropertyValue(std::string property, std::shared_ptr<base::IWrappedValue> value) override;

    ~ScrollList2D() override;
protected:
	void initializeListTouchAreaBackTouchListener() override;
	void initializeNewEntryTouchHandling(IInteractiveDrawable2D* touchArea) override;

	void syncVisuals(bool syncAbsoluteValues, bool syncAnimatingValues, std::string startStateID, std::string endStateID, float stateCompletionPercent) override;

	void handleContentAlignmentX() override;
	void handleContentAlignmentY() override;

private:
	bool v2d_onCursorDown(
		IInteractiveDrawable2D* target,
		int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos);
	void v2d_onCursorUp(
		IInteractiveDrawable2D* target,
		int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos);
	void v2d_onCursorUpOutside(
		IInteractiveDrawable2D* target,
		int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos);
	void v2d_onCursorDrag(
		IInteractiveDrawable2D* target,
        int cursorIndex, base::Touch::ButtonCode* buttonId,
        Vector2& screenPos, Vector2& localPos,
		Vector2& deltaPosW);

	int getScrollStatus();
	void setScrollStatus(int scrollStatus);

	void updateScrollingLimits();
	void startScroll(float worldX, float worldY);
	void endScroll(bool doFling);
	void scrollMove(float worldX, float worldY);

};
};
