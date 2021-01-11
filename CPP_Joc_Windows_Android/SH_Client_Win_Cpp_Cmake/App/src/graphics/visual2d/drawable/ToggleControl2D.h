#pragma once

#include <base/gh.h>
#include <base/menu/AnimatedComposedStiledVisual.h>
#include "IToggleControl2D.h"
#include <base/interaction/CbTouchListener.h>
#include <base/audio/sound/DynamicSound.h>

namespace base {
class ToggleControl2D : public AnimatedComposedStiledVisual, virtual public IToggleControl2D {
private: typedef AnimatedComposedStiledVisual super;
	priv base::audio::DynamicSound* toggleOffDSound = nullptr;
	priv float toggleOffSound_volume = 1.0f;

	priv base::audio::DynamicSound* toggleOnDSound = nullptr;
	priv float toggleOnSound_volume = 1.0f;

	priv IListener* eventListener = nullptr;

	priv IInteractiveDrawable2D* touchAreaBack = nullptr;

	priv bool isOn = false;

	priv bool isEnabled = true;

	priv CbTouchListener touchListener{};

public:
   pub dCtor(ToggleControl2D);
    pub explicit ToggleControl2D(IAppMetrics* appMetrics, IAppAssets* appAssets, ArrayList<MenuItemConfig*>* itemConfigs, ArrayList<StateChangeDurations*>* easeDurationsSList);

	priv bool v2d_onCursorDown(
		IInteractiveDrawable2D* target,
		int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos);

	pub void setEventListener(IListener* eventListener) override;

	prot void createContent() override;
	prot void createVisuals(Map1D<std::string, void*>* createConfigMap) override;

	pub bool getIsOn() override;
	pub void setIsOn(bool isOn, bool doUpdate, bool doAnimation, bool playSound, bool dispatchEvent) override;
	pub void setIsEnabled(bool isEnabled, bool doUpdate, bool doAnimation) override;

	pub void setPosition(float x, float y);
	pub void setToggleOffSound(std::string* configId, float volume);
	pub void setToggleOnSound(std::string* configId, float volume);

	pub std::shared_ptr<base::IWrappedValue> getPropertyValue(std::string property) override;
	pub void setPropertyValue(std::string property, std::shared_ptr<base::IWrappedValue> value) override;

	pub std::string computeStateFlags(std::string baseState) override;

    pub ~ToggleControl2D() override;
};
};
