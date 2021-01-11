#include "EEVSettingsDrawerA.h"
#include <base/menu/util/MenuItemConfig_ComposedVisual.h>
#include <base/menu/util/MenuItemConfig_Data.h>
#include <base/menu/util/MenuItemConfig_Visual.h>
#include <base/visual2D/util/VCC_Group.h>
#include <base/visual2D/util/VCC_TouchArea.h>
#include <base/visual2D/util/VCC_Sprite.h>
#include <base/visual2D/util/VCC_SpriteButton.h>
#include <base/visual2D/VisualUtil2D.h>
#include <base/log/GlobalAccessLogHandler.h>
#include <base/config/sounds/SoundConfigsList.h>
#include <base/util/AppStyles.h>

using namespace rpg3D;

MenuItemConfig* EEVSettingsDrawerA::new_viewItemConfig_settingsDrawerA() {
	return new MenuItemConfig_ComposedVisual("settingsDrawerA", (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("props", (new VisualPropertiesConfig())
			->put("width", 1.0f, "_appSize.width")
			->put("height", 1.0f, "_appSize.height")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("layer", "parent.layer", VCC_Group::New(), (new VisualPropertiesConfig())
			->put("x", 0.0f)
			->put("y", 0.0f)
			
			->putAnimating("_visibleFloat", (new MenuVisualAnimationProperty())
				->putState("_isSettingsDrawerAEnabled:true_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("taScreen", "layer", VCC_TouchArea::New(), (new VisualPropertiesConfig())
			->put("width", 1.0f, "props.width")
			->put("height", 1.0f, "props.height")
		))

		->appendDirect_chain(new_viewItemConfig_settingsDrawerA_body())

		->appendDirect_chain(new MenuItemConfig_Visual("screenDimmer", "layer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "bg/simple_white_FFFFFF"), (new VisualPropertiesConfig())
			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("x", 0.0f)
			->put("y", 0.0f)
			// Set "width" after "x" because it uses "x" in logic.
			->put("width",
				1.0f, "body.layer.x",
				VisualValueConfig::OPERATION_ADD,
				-1.0f, "_self.x"
			)
			->put("height", 1.0f, "props.height")
			->putAnimating("alpha", (new MenuVisualAnimationProperty())
				->putState("_isSettingsDrawerAEnabled:true_", AppStyles::SCREEN_DIMMER_ALPHA, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
			->put("tintColor", Color("#000000FF"))
		))
	);
}

EEVSettingsDrawerA::EEVSettingsDrawerA(
	IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList,
	IAppUtils* appUtils)
	: super(
		app, viewItemConfigs, viewEaseDurationsSList,
		appUtils
	)
{
	touchListener.cb_onCursorDown = std::bind(&EEVSettingsDrawerA::v2d_onCursorDown, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
}

bool EEVSettingsDrawerA::v2d_onCursorDown(
	IInteractiveDrawable2D* target,
	int cursorIndex, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos, Vector2& localPos)
{
	// Touch events are ignored while the menu is animating.
	if (!isActive() || isAnimating()) {
		return false;
	}

	if (dynamic_cast<void*>(target) == dynamic_cast<void*>(taScreen)) {
		hideSettingsDrawerA();
	}

	else {
		throw LogicException(LOC);
	}

	return true;
}

void EEVSettingsDrawerA::createVisuals(Map1D<std::string, void*>* createConfigMap) {
	super::createVisuals(createConfigMap);

	(taScreen = getItemMustExistAs<IInteractiveDrawable2D*>("settingsDrawerA.taScreen"))->addTouchListener(&touchListener);
}

EEVSettingsDrawerA::~EEVSettingsDrawerA() {
	//void
}
