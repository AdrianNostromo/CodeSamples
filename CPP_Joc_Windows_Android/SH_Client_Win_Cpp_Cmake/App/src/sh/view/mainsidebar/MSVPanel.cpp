#include "MSVPanel.h"
#include <base/visual2D/util/VCC_Group.h>
#include <base/visual2D/util/VCC_TouchArea.h>
#include <base/visual2D/util/VCC_Sprite.h>
#include <base/visual2D/util/VCC_SpriteButton.h>
#include <base/visual2D/util/VCC_BitmapLabel.h>
#include <base/menu/util/MenuItemConfig_ComposedVisual.h>
#include <base/menu/util/MenuItemConfig_Data.h>
#include <base/menu/util/MenuItemConfig_Visual.h>

using namespace sh;

MenuItemConfig* MSVPanel::new_viewItemConfig_panel() {
	return new MenuItemConfig_ComposedVisual("panel", (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("props", (new VisualPropertiesConfig())
			->put(new VisualPropertyValueConfig("width",
				new VisualValueConfig(
					6.0f, "_standardIncrement",
					VisualValueConfig::OPERATION_MIN,
					new VisualValueConfig(
						1.0f, "_appSize.width",
						VisualValueConfig::OPERATION_ADD,
						-1.0f, "_standardIncrement"
					)
				)
			))
			->put("height", 1.0f, "_appSize.height")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("layer", "parent.layer", VCC_Group::New(), (new VisualPropertiesConfig())
			// The "x", "alpha" and "visible" are manually update in code.
			->putAnimating("x", (new MenuVisualAnimationProperty())
				->putState("_in_", 0.0f, EaseConfig::DECELERATION)
				->putState("_", -1.0f, "props.width", EaseConfig::ACCELERATION)
			)
			->put("y", 0.0f)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("taBg", "layer", VCC_TouchArea::New(), (new VisualPropertiesConfig())
			->put("width", 1.0f, "props.width")
			->put("height", 1.0f, "props.height")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("bg", "layer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "bg/simple_white_FFFFFF"), (new VisualPropertiesConfig())
			->put("width", 1.0f, "props.width")
			->put("height", 1.0f, "props.height")
			->put("tintColor", Color("#FFFFFFFF"))
			->put("alpha", 0.8f)
		))
		
		/*->appendDirect_chain(new MenuItemConfig_Visual("shadow", "layer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "shadow/leftDrawer_right"), (new VisualPropertiesConfig())
			->put("width", 30.0f, "_dpm.width")
			->put("height", 1.0f, "props.height")
			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("x", 1.0f, "props.width")
			->put("y", 0.0f)
		))*/

		->appendDirect_chain(new_viewItemConfig_panel_header())
		->appendDirect_chain(new_viewItemConfig_panel_body())

		/*asdA29;
		menuItemConfigs_panel_accountSettingsDimmer,
		menuItemConfigs_panel_accountSettingsPanel*/
	);
}

MSVPanel::MSVPanel(
    IApp* app,
    ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList,
    worldGame3D::IAppGame* appGame, IAppUtils* appUtils)
    : super(app, viewItemConfigs, viewEaseDurationsSList, appGame, appUtils)
{
	touchListener.cb_onCursorDown = std::bind(&MSVPanel::v2d_onCursorDown, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
}

void MSVPanel::createContent() {
	super::createContent();

	(taBg = getItemOptionalAs<IInteractiveDrawable2D*>("panel.taBg"))->addTouchListener(&touchListener);
}

bool MSVPanel::v2d_onCursorDown(
	IInteractiveDrawable2D* target,
	int cursorIndex, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos, Vector2& localPos)
{
	if (!isActive()) {
		return false;
	}

	if (dynamic_cast<void*>(target) == dynamic_cast<void*>(taBg)) {
		//void
	}

	else {
		//"Unhandled touch target.";

		throw LogicException(LOC);
	}

	return false;
}

MSVPanel::~MSVPanel() {
    //void
}
