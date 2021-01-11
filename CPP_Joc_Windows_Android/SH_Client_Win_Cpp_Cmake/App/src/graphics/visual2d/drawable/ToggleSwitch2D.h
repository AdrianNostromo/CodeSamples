#pragma once

#include <base/gh.h>
#include "ToggleControl2D.h"

namespace base {
class ToggleSwitch2D : public ToggleControl2D {
private: typedef ToggleControl2D super;
	priv static ArrayList<MenuItemConfig*>* itemConfigs;
	priv static ArrayList<StateChangeDurations*>* animationDurations;

	// These are used in the config and are accesed as "_root.anchorX", "_root.anchorY";
	priv float anchorX = 0.0f;
	priv float anchorY = 0.0f;
	priv bool isDirty_anchor = false;

	pub dCtor(ToggleSwitch2D);
    pub explicit ToggleSwitch2D(IAppMetrics* appMetrics, IAppAssets* appAssets);

	prot void visualLoop(float deltaS, int deltaMS) override;

	pub float getAnchorX();
	pub float getAnchorY();
	pub void setAnchor(float anchorX, float anchorY);

	pub std::shared_ptr<base::IWrappedValue> getPropertyValue(std::string property) override;
	pub void setPropertyValue(std::string property, std::shared_ptr<base::IWrappedValue> value) override;

    pub ~ToggleSwitch2D() override;
};
};
