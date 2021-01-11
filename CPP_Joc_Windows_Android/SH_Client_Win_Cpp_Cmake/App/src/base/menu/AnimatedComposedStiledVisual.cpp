#include "AnimatedComposedStiledVisual.h"
#include <base/exceptions/LogicException.h>
#include <base/app/config/AppConfig.h>
#include <base/math/Math.h>
#include <base/menu/util/IMenuItemConfig_DynamicProperties.h>
#include <base/menu/util/MenuItemConfig_SmartItem.h>
#include <base/util/StringUtil.h>
#include <base/screen/ScreenUtil.h>
#include <base/app/IAppMetrics.h>

AnimatedComposedStiledVisual::AnimatedComposedStiledVisual(IAppMetrics* appMetrics, IAppAssets* appAssets, ArrayList<MenuItemConfig*>* itemConfigs, ArrayList<StateChangeDurations*>* easeDurationsSList)
    : super(appMetrics, appAssets, itemConfigs),
      easeDurationsSList(easeDurationsSList)
{
    screenListener.cb_onActiveScreenMetricsChange = std::bind(&AnimatedComposedStiledVisual::onActiveScreenMetricsChange, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
}

void AnimatedComposedStiledVisual::createPost() {
    super::createPost();

    base::ScreenUtil::screen->addListener(&screenListener, 1);
}

void AnimatedComposedStiledVisual::onActiveScreenMetricsChange(
	int resolutionX, int resolutionY,
	float ppiX, float ppiY)
{
	isDirty_viewportMetrics = true;
}

bool AnimatedComposedStiledVisual::isActive() {
    if(StringUtil::contains(endStateID, "_in_")) {
        return true;
    }

    return false;
}

bool AnimatedComposedStiledVisual::isShowing() {
    if(StringUtil::contains(startStateID, "_out_") && StringUtil::contains(endStateID, "_in_")) {
        return true;
    }

    return false;
}

bool AnimatedComposedStiledVisual::isHiding() {
    if(StringUtil::contains(startStateID, "_in_") && StringUtil::contains(endStateID, "_out_")) {
        return true;
    }

    return false;
}

std::string& AnimatedComposedStiledVisual::getEndStateID() {
    return endStateID;
}


std::string AnimatedComposedStiledVisual::getManualStateFlags() {
	return manualStateFlags;
}

void AnimatedComposedStiledVisual::setManualStateFlags(std::string& manualStateFlags, bool doAnimation) {
	this->manualStateFlags = manualStateFlags;

	updateShowStateIfChanged(doAnimation, nullptr);
}


float AnimatedComposedStiledVisual::getStateCompletionPercent() {
    return stateCompletionPercent;
}

void AnimatedComposedStiledVisual::setIsDirty_property() {
	isDirty_property = true;
}

bool AnimatedComposedStiledVisual::isAnimating() {
    if(stateCompletionPercent < 1) {
        return true;
    }

    return false;
}

bool AnimatedComposedStiledVisual::isActiveOrAnimating() {
    return isActive() || isAnimating();
}

void AnimatedComposedStiledVisual::onSelectedStateChanged() {
    //void
}

void AnimatedComposedStiledVisual::gotoShowState(std::string endStateID, bool doAnimation, IAnimationListener* menuAnimationListener, bool restartAnimation) {
    setShowState(this->endStateID, endStateID, doAnimation, menuAnimationListener, restartAnimation);
}

StateChangeDurations* AnimatedComposedStiledVisual::getFirstValidEaseDurationConfig(std::string& startStateID, std::string& endStateID) {
    for(int i=0;i<easeDurationsSList->count();i++) {
        StateChangeDurations* valueEntry = easeDurationsSList->getDirect(i);

        bool isStartStateCheckPassed = checkIsStatePassed(startStateID, valueEntry->startComposedStateID);
        bool isEndStateCheckPassed = checkIsStatePassed(endStateID, valueEntry->endComposedStateID);

        if(isStartStateCheckPassed && isEndStateCheckPassed) {
            return valueEntry;
        }
    }

    throw LogicException(LOC);
}

void AnimatedComposedStiledVisual::setShowState(std::string startStateID, std::string& endStateID, bool doAnimation, IAnimationListener* menuAnimationListener, bool restartAnimation) {
    this->menuAnimationListener = menuAnimationListener;

    if(!restartAnimation && (this->startStateID == startStateID && this->endStateID == endStateID)) {
        return ;
    }

    bool oldActiveState = isActive();

    this->startStateID = startStateID;
    this->endStateID = endStateID;

    StateChangeDurations* easeDurationConfig = getFirstValidEaseDurationConfig(startStateID, endStateID);
    if(easeDurationConfig == nullptr) {
        throw LogicException(LOC);
    }
    this->animationDurationS = easeDurationConfig->durationS;

    if(doAnimation && easeDurationConfig->durationS > 0.0f) {
        stateCompletionPercent = 0.0f;
    }else {
        stateCompletionPercent = 1.0f;
    }

    if(oldActiveState != isActive()) {
        onSelectedStateChanged();
    }

    // Also update absolute values because some absolute values may use other absolute values that are manually modified when the menu state change is requested.
    // Example of problem: Manually setting a visual2D (maybe even a dynamic visual2D) property (eg: width) and using that property for another visual2D.
    updateStateData(true, true);

    if(stateCompletionPercent >= 1) {
        if(menuAnimationListener != nullptr) {
            menuAnimationListener->animationFinished(this);

            menuAnimationListener = nullptr;

            animationFinishedEvent();
        }
    }
}

bool AnimatedComposedStiledVisual::isVisible() {
    return visualsHolder->getIsVisible();
}

void AnimatedComposedStiledVisual::isOnDisplayListChanged() {
    //void
}

void AnimatedComposedStiledVisual::doFullVisualsSync() {
    // Don't call the super function because this does the same thing but also includes animation data.
    //super::doFullVisualsSync();

    updateStateData(true, true);
}

void AnimatedComposedStiledVisual::putDataMapValue(std::string& key, std::shared_ptr<base::IWrappedValue> val, bool doAnimatingVisualsSync) {
    super::putDataMapValue(key, val, doAnimatingVisualsSync);

    if(doAnimatingVisualsSync) {
        updateStateData(false, true);
    }
}

void AnimatedComposedStiledVisual::createFramework() {
    super::createFramework();

    // These values need to be initialised early on.
    startStateID = computeStateFlags("_out_toIn_");
    endStateID = computeStateFlags("_out_toIn_");
}

void AnimatedComposedStiledVisual::createVisuals(Map1D<std::string, void*>* createConfigMap) {
    super::createVisuals(createConfigMap);

    visualsHolder = getItemOptionalAs<IContainer2D*>("_root.visualsHolder");

    visualsHolder->setIsVisible(false);
}

void AnimatedComposedStiledVisual::updateStateData(bool syncAbsoluteValues, bool syncAnimatingValues) {
    if (isActive() && !visualsHolder->getIsVisible()) {
        visualsHolder->setIsVisible(true);

        isOnDisplayListChanged();
    } else if (!isActive() && stateCompletionPercent >= 1 && visualsHolder->getIsVisible()) {
        visualsHolder->setIsVisible(false);

        isOnDisplayListChanged();
    }

    if (!visualsHolder->getIsVisible()) {
        return;
    }

    syncVisuals(syncAbsoluteValues, syncAnimatingValues, startStateID, endStateID, stateCompletionPercent);
}

bool AnimatedComposedStiledVisual::isOnDisplayList() {
    if(!visualsHolder->getIsVisible()) {
        return false;
    }

    return true;
}

std::string AnimatedComposedStiledVisual::getBaseStateFromVariation(std::string& stateID) {
    if (StringUtil::contains(stateID, "_in_")) {
        return "_in_";
    } else if (StringUtil::contains(stateID, "_out_")) {
        return "_out_";
    }else {
        throw LogicException(LOC);
    }
}

void AnimatedComposedStiledVisual::visualLoop(float deltaS, int deltaMS) {
    super::visualLoop(deltaS, deltaMS);
	
    if(stateCompletionPercent < 1) {
        if(isDirty_viewportMetrics || isDirty_property) {
			isDirty_viewportMetrics = false;
			isDirty_property = false;

            updateStateData(true, false);
        }

        stateCompletionPercent = Math::min(1.0f, stateCompletionPercent + deltaS * (1.0f / animationDurationS));

        updateStateData(false, true);

        if(stateCompletionPercent >= 1) {
            if(menuAnimationListener != nullptr) {
                menuAnimationListener->animationFinished(this);

                if (getIsDisposed()) {
                    // The above call dispatched an event and the processor instantly disposed this view.
                    /// View disposal not allowed while code still runs in it, need to use some delayed disposal logic.,
                    throw LogicException(LOC);
                }

                menuAnimationListener = nullptr;
            }

            animationFinishedEvent();
        }
    }else {
        if(isDirty_viewportMetrics || isDirty_property) {
			isDirty_viewportMetrics = false;
			isDirty_property = false;

            updateStateData(true, true);
        }
    }
}

void AnimatedComposedStiledVisual::visualLoopB(float deltaS, int deltaMS) {
	super::visualLoopB(deltaS, deltaMS);

	// This is used so smart_children won't have any update delay (eg. on window resize).
	if (isDirty_viewportMetrics || isDirty_property) {
		isDirty_viewportMetrics = false;
		isDirty_property = false;

		updateStateData(true, true);
	}
}

void AnimatedComposedStiledVisual::animationFinishedEvent() {
    if (isActive()) {
        std::string inState = computeStateFlags("_in_");
        if(getEndStateID() != inState) {
            gotoShowState(inState, true, nullptr, false);
        }
    }
}

// baseState: "_in_" / "_out_" / "_out_toIn_";
std::string AnimatedComposedStiledVisual::computeStateFlags(std::string baseState) {
    std::string state = baseState + manualStateFlags;

    if(appMetrics->getScreen()->getWindowDiagonalInches() < AppConfig::MEDIUM_DEVICE_DIAGONAL_INCHES_MIN) {
        state = state + "SmallDevice_";
        state = state + "Mobile_";
    }else {
        state = state + "Tablet_";
        if(appMetrics->getScreen()->getWindowDiagonalInches() < AppConfig::LARGE_DEVICE_DIAGONAL_INCHES_MIN) {
            state = state + "MediumDevice_";
        }else {
            state = state + "LargeDevice_";
        }
    }

    if(appMetrics->getScreen()->getWindowSizeInches()->x >= appMetrics->getScreen()->getWindowSizeInches()->y) {
        state = state + "Landscape_";
    }else {
        state = state + "Portrait_";
    }

    /*if(!app.profileData.getDataWithID_boolean(ProfileDataIDs.IS_ADS_DISABLED)) {
        state = state + "adsOn_";
    }else {
        state = state + "adsOff_";
    }*/


    return state;
}

void AnimatedComposedStiledVisual::updateShowStateIfChanged(bool doAnimation, IAnimationListener* menuAnimationListener) {
    if(!isActive() || isAnimating()) {
        return ;
    }

    std::string stateID = computeStateFlags("_in_");
    if(!doAnimation || endStateID != stateID) {
        gotoShowState(stateID, doAnimation, menuAnimationListener, false);
    }
}

void AnimatedComposedStiledVisual::visualItemCreated(IGenericDrawable* item, MenuItemConfig* itemConfig, IComposedVisual* itemsMap, IComposedVisual* itemsMapLayer) {
    // Local code include code from super method.
    // super::visualItemCreated(item, itemConfig, itemsMap, itemsMapLayer);

    if(dynamic_cast<IDynamicPropertiesObject2D*>(item) && dynamic_cast<IMenuItemConfig_DynamicProperties*>(itemConfig)) {
        syncVisualItem(
            dynamic_cast<IDynamicPropertiesObject2D*>(item), (dynamic_cast<IMenuItemConfig_DynamicProperties*>(itemConfig))->getItemProperties(),
            true, true,
            startStateID, endStateID,
            0.0f,
            itemsMap, itemsMapLayer,
            getDataMap(),
            appMetrics
        );
    }

    if(dynamic_cast<ComposedStiledVisual*>(item) && dynamic_cast<IDynamicPropertiesObject2D*>(item)) {
        (dynamic_cast<ComposedStiledVisual*>(item))->createVisuals(createConfigMap);
    }

    if(dynamic_cast<ComposedStiledVisual*>(item)) {
        (dynamic_cast<ComposedStiledVisual*>(item))->doFullVisualsSync();
    }

    if(dynamic_cast<AnimatedComposedStiledVisual*>(item) && dynamic_cast<MenuItemConfig_SmartItem*>(itemConfig)) {
        MenuItemConfig_SmartItem* cItemConfig = dynamic_cast<MenuItemConfig_SmartItem*>(itemConfig);

        std::string newState = (dynamic_cast<AnimatedComposedStiledVisual*>(item))->computeStateFlags(cItemConfig->initialBaseStateId);

        (dynamic_cast<AnimatedComposedStiledVisual*>(item))->gotoShowState(newState, false, nullptr, false);
    }
}

void AnimatedComposedStiledVisual::disposeMain() {
    if(screenListener.getIsListening()) {
        base::ScreenUtil::screen->removeListener(&screenListener, 1);
    }

    super::disposeMain();
}

AnimatedComposedStiledVisual::~AnimatedComposedStiledVisual() {
    //void
}
