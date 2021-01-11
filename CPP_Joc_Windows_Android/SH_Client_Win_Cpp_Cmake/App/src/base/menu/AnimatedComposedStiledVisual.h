#pragma once

#include <base/menu/util/StateChangeDurations.h>
#include "ComposedStiledVisual.h"
#include "IAnimatedComposedStiledVisual.h"
#include <base/screen/IScreen.h>

class AnimatedComposedStiledVisual : public ComposedStiledVisual, virtual public IAnimatedComposedStiledVisual {
private: typedef ComposedStiledVisual super;
protected:
    std::string startStateID = "";
    std::string endStateID = "";
    float stateCompletionPercent = 1.0f;

    IAnimationListener* menuAnimationListener = nullptr;

private:
    ArrayList<StateChangeDurations*>* easeDurationsSList;

    float animationDurationS = 0.0f;

	base::IScreen::CbListener screenListener{"AnimatedComposedStiledVisual"};

	bool isDirty_viewportMetrics = false;
	// This is currently used only by lists.
	bool isDirty_property = false;

	std::string manualStateFlags = "";

public:
    AnimatedComposedStiledVisual(AnimatedComposedStiledVisual const&) = delete;
    AnimatedComposedStiledVisual(AnimatedComposedStiledVisual &&) = default;
    AnimatedComposedStiledVisual& operator=(AnimatedComposedStiledVisual const&) = delete;
    AnimatedComposedStiledVisual& operator=(AnimatedComposedStiledVisual &&) = default;

    explicit AnimatedComposedStiledVisual(IAppMetrics* appMetrics, IAppAssets* appAssets, ArrayList<MenuItemConfig*>* itemConfigs, ArrayList<StateChangeDurations*>* easeDurationsSList);

    bool isActive();
    bool isShowing();
    bool isHiding();

    std::string& getEndStateID();

	std::string getManualStateFlags() final;
	void setManualStateFlags(std::string& manualStateFlags, bool doAnimation) final;

    bool isAnimating();
    bool isActiveOrAnimating();

    void doFullVisualsSync() override;
    void putDataMapValue(std::string& key, std::shared_ptr<base::IWrappedValue> val, bool doAnimatingVisualsSync) override;

    bool isOnDisplayList();

    std::string getBaseStateFromVariation(std::string& stateID);

    void visualLoop(float deltaS, int deltaMS) override;
    void visualLoopB(float deltaS, int deltaMS) override;

    virtual std::string computeStateFlags(std::string baseState);

    void gotoShowState(std::string endStateID, bool doAnimation, IAnimationListener* menuAnimationListener, bool restartAnimation);

    ~AnimatedComposedStiledVisual() override;
protected:
	void createPost() override;

    float getStateCompletionPercent();

	void setIsDirty_property();

    virtual void onSelectedStateChanged();

    void setShowState(std::string startStateID, std::string& endStateID, bool doAnimation, IAnimationListener* menuAnimationListener, bool restartAnimation);
    bool isVisible();
    virtual void isOnDisplayListChanged();

    void createFramework() override;

    void createVisuals(Map1D<std::string, void*>* createConfigMap) override;

    virtual void animationFinishedEvent();

    void updateShowStateIfChanged(bool doAnimation, IAnimationListener* menuAnimationListener=nullptr);
    void visualItemCreated(IGenericDrawable* item, MenuItemConfig* itemConfig, IComposedVisual* itemsMap, IComposedVisual* itemsMapLayer) override;

	void disposeMain() override;
private:
	void onActiveScreenMetricsChange(
		int resolutionX, int resolutionY,
		float ppiX, float ppiY);

    StateChangeDurations* getFirstValidEaseDurationConfig(std::string& startStateID, std::string& endStateID);

    void updateStateData(bool syncAbsoluteValues, bool syncAnimatingValues);

};
