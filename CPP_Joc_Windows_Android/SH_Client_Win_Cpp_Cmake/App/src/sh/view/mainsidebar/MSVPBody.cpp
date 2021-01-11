#include "MSVPBody.h"
#include <base/visual2D/util/VCC_Group.h>
#include <base/visual2D/util/VCC_TouchArea.h>
#include <base/visual2D/util/VCC_Sprite.h>
#include <base/visual2D/util/VCC_SpriteButton.h>
#include <base/visual2D/util/VCC_BitmapLabel.h>
#include <base/menu/util/MenuItemConfig_ComposedVisual.h>
#include <base/menu/util/MenuItemConfig_Data.h>
#include <base/menu/util/MenuItemConfig_Visual.h>
#include <base/util/AppStyles.h>
#include <base/app/config/AppConfig.h>
#include <base/menu/util/MenuItemConfig_SmartItem.h>
#include <graphics/visual2d/drawable/ToggleSwitch2D.h>
#include <rpg3D/config/sounds/SoundConfigsList.h>
#include <graphics/visual2d/drawable/BitmapLabel.h>
#include <graphics/visual2d/drawable/SpriteButton2D.h>
#include <sh/scenarios/startarea_a_v1/gw/config/ScenarioFactory.h>
#include <worldGame3D/scenariohandler/ScenariosHandler.h>
#include <worldGame3D/app/IAppGame.h>
#include <worldGame3D/scenariotemplate/ScenarioTemplate.h>

using namespace sh;

const float MSVPBody::SPACE_M = 16.0f;
const float MSVPBody::SPACE_S = 8.0f;
const float MSVPBody::TEXT_OFFSET_Y = 20.0f;
const float MSVPBody::TITLE_OFFSET_Y = 16.0f;
const float MSVPBody::TITLE_H = 58.0f;
const float MSVPBody::ENTRY_H = 48.0f;
const float MSVPBody::BTN_W = 24.0f;

MenuItemConfig* MSVPBody::new_viewItemConfig_panel_body() {
	return new MenuItemConfig_ComposedVisual("body", (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("props", (new VisualPropertiesConfig())
			->put("width", 1.0f, "parent.props.width")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("layer", "parent.layer", VCC_Group::New(), (new VisualPropertiesConfig())
			->put("x", 0.0f)
			->put("y",
				1.0f, "parent.props.height",
				VisualValueConfig::OPERATION_ADD,
				-1.f, "parent.header.props.height"
			)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("lblSettings", "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Medium-AaNSS-20-_dpFont_"), "Settings"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 20, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("x", SPACE_M, "_dpm.width")
			->put("y", -TITLE_H + TITLE_OFFSET_Y, "_dpm.height")
			->put("alpha", AppStyles::DARK_PRIMARY_TEXT_ALPHA)
			->put("tintColor", Color("#000000FF"))
		))

		->appendDirect_chain(new MenuItemConfig_Visual("lblHome", "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Medium-AaNSS-14-_dpFont_"), "Home"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("x", SPACE_M, "_dpm.width")
			->put("y", -TITLE_H - ENTRY_H * 1 + TEXT_OFFSET_Y, "_dpm.height")
			->put("alpha", AppStyles::DARK_SECONDARY_TEXT_ALPHA)
			->put("tintColor", Color("#000000FF"))
		))
		->appendDirect_chain(new MenuItemConfig_Visual("btnHome", "layer", VCC_SpriteButton::New(
			"visual2D/taGeneral.atlas", "buttons/btnHome_",
			&base::SoundConfigsList::UserInterface_Btn_Down->managedPath->path, 0.5f, nullptr, 0.5f, nullptr, 0.5f
		), (new VisualPropertiesConfig())
			->put("width", 24.0f, "_dpm.width")
			->put("height", 24.0f, "_dpm.height")
			->put("touchWidthOverride", std::make_shared<float>(32.0f), "_dpm.width")
			->put("touchHeightOverride", std::make_shared<float>(32.0f), "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				1.0f, "props.width",
				VisualValueConfig::OPERATION_ADD,
				-SPACE_M - BTN_W * 0.5f, "_dpm.width"
			)
			->put("y", -TITLE_H - ENTRY_H * 1 + ENTRY_H * 0.5f, "_dpm.height")
			->put("tintColor", AppStyles::ICON_DARK_COLOR)
			->put("alpha", AppStyles::ICON_DARK_ALPHA)
		))
		->appendDirect_chain(new MenuItemConfig_Visual("lblExitZone", "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Medium-AaNSS-14-_dpFont_"), "Exit zone"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("x", SPACE_M, "_dpm.width")
			->put("y", -TITLE_H - ENTRY_H * 2 + TEXT_OFFSET_Y, "_dpm.height")
			->put("alpha", AppStyles::DARK_SECONDARY_TEXT_ALPHA)
			->put("tintColor", Color("#000000FF"))
		))
		->appendDirect_chain(new MenuItemConfig_Visual("btnExitZone", "layer", VCC_SpriteButton::New(
			"visual2D/taGeneral.atlas", "buttons/btnExitZone_",
			&base::SoundConfigsList::UserInterface_Btn_Down->managedPath->path, 0.5f, nullptr, 0.5f, nullptr, 0.5f
		), (new VisualPropertiesConfig())
			->put("width", 24.0f, "_dpm.width")
			->put("height", 24.0f, "_dpm.height")
			->put("touchWidthOverride", std::make_shared<float>(32.0f), "_dpm.width")
			->put("touchHeightOverride", std::make_shared<float>(32.0f), "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				1.0f, "props.width",
				VisualValueConfig::OPERATION_ADD,
				-SPACE_M - BTN_W * 0.5f, "_dpm.width"
			)
			->put("y", -TITLE_H - ENTRY_H * 2 + ENTRY_H * 0.5f, "_dpm.height")
			->put("tintColor", AppStyles::ICON_DARK_COLOR)
			->put("alpha", AppStyles::ICON_DARK_ALPHA)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("dlblCameraZoom", "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Medium-AaNSS-14-_dpFont_"), "Camera zoom is #.##"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("x", SPACE_M, "_dpm.width")
			->put("y", -TITLE_H - ENTRY_H * 3 + TEXT_OFFSET_Y, "_dpm.height")
			->put("alpha", AppStyles::DARK_SECONDARY_TEXT_ALPHA)
			->put("tintColor", Color("#000000FF"))
		))
		->appendDirect_chain(new MenuItemConfig_Visual("btnCameraZoomOut", "layer", VCC_SpriteButton::New(
			"visual2D/taGeneral.atlas", "buttons/btnZoomOut_",
			&base::SoundConfigsList::UserInterface_Btn_Down->managedPath->path, 0.5f, nullptr, 0.5f, nullptr, 0.5f
		), (new VisualPropertiesConfig())
			->put("width", 24.0f, "_dpm.width")
			->put("height", 24.0f, "_dpm.height")
			->put("touchWidthOverride", std::make_shared<float>(32.0f), "_dpm.width")
			->put("touchHeightOverride", std::make_shared<float>(32.0f), "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				1.0f, "props.width",
				VisualValueConfig::OPERATION_ADD,
				-SPACE_M - BTN_W - SPACE_S - BTN_W * 0.5f, "_dpm.width"
			)
			->put("y", -TITLE_H - ENTRY_H * 3 + ENTRY_H * 0.5f, "_dpm.height")
			->put("tintColor", AppStyles::ICON_DARK_COLOR)
			->put("alpha", AppStyles::ICON_DARK_ALPHA)
		))
		->appendDirect_chain(new MenuItemConfig_Visual("btnCameraZoomIn", "layer", VCC_SpriteButton::New(
			"visual2D/taGeneral.atlas", "buttons/btnZoomIn_",
			&base::SoundConfigsList::UserInterface_Btn_Down->managedPath->path, 0.5f, nullptr, 0.5f, nullptr, 0.5f
		), (new VisualPropertiesConfig())
			->put("width", 24.0f, "_dpm.width")
			->put("height", 24.0f, "_dpm.height")
			->put("touchWidthOverride", std::make_shared<float>(32.0f), "_dpm.width")
			->put("touchHeightOverride", std::make_shared<float>(32.0f), "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				1.0f, "props.width",
				VisualValueConfig::OPERATION_ADD,
				-SPACE_M - BTN_W * 0.5f, "_dpm.width"
			)
			->put("y", -TITLE_H - ENTRY_H * 3 + ENTRY_H * 0.5f, "_dpm.height")
			->put("tintColor", AppStyles::ICON_DARK_COLOR)
			->put("alpha", AppStyles::ICON_DARK_ALPHA)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("dlblCameraDirection", "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Medium-AaNSS-14-_dpFont_"), "Camera direction is ##"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("x", SPACE_M, "_dpm.width")
			->put("y", -TITLE_H - ENTRY_H * 4 + TEXT_OFFSET_Y, "_dpm.height")
			->put("alpha", AppStyles::DARK_SECONDARY_TEXT_ALPHA)
			->put("tintColor", Color("#000000FF"))
		))
		->appendDirect_chain(new MenuItemConfig_Visual("btnCameraDirectionTurnLeft", "layer", VCC_SpriteButton::New(
			"visual2D/taGeneral.atlas", "buttons/btnRotateLeft_",
			&base::SoundConfigsList::UserInterface_Btn_Down->managedPath->path, 0.5f, nullptr, 0.5f, nullptr, 0.5f
		), (new VisualPropertiesConfig())
			->put("width", 24.0f, "_dpm.width")
			->put("height", 24.0f, "_dpm.height")
			->put("touchWidthOverride", std::make_shared<float>(32.0f), "_dpm.width")
			->put("touchHeightOverride", std::make_shared<float>(32.0f), "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				1.0f, "props.width",
				VisualValueConfig::OPERATION_ADD,
				-SPACE_M - BTN_W - SPACE_S - BTN_W * 0.5f, "_dpm.width"
			)
			->put("y", -TITLE_H - ENTRY_H * 4 + ENTRY_H * 0.5f, "_dpm.height")
			->put("tintColor", AppStyles::ICON_DARK_COLOR)
			->put("alpha", AppStyles::ICON_DARK_ALPHA)
		))
		->appendDirect_chain(new MenuItemConfig_Visual("btnCameraDirectionTurnRight", "layer", VCC_SpriteButton::New(
			"visual2D/taGeneral.atlas", "buttons/btnRotateRight_",
			&base::SoundConfigsList::UserInterface_Btn_Down->managedPath->path, 0.5f, nullptr, 0.5f, nullptr, 0.5f
		), (new VisualPropertiesConfig())
			->put("width", 24.0f, "_dpm.width")
			->put("height", 24.0f, "_dpm.height")
			->put("touchWidthOverride", std::make_shared<float>(32.0f), "_dpm.width")
			->put("touchHeightOverride", std::make_shared<float>(32.0f), "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				1.0f, "props.width",
				VisualValueConfig::OPERATION_ADD,
				-SPACE_M - BTN_W * 0.5f, "_dpm.width"
			)
			->put("y", -TITLE_H - ENTRY_H * 4 + ENTRY_H * 0.5f, "_dpm.height")
			->put("tintColor", AppStyles::ICON_DARK_COLOR)
			->put("alpha", AppStyles::ICON_DARK_ALPHA)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("lblOnScreenDPad", "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Medium-AaNSS-14-_dpFont_"), "On-screen D-pad"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("x", SPACE_M, "_dpm.width")
			->put("y", -TITLE_H - ENTRY_H * 5 + TEXT_OFFSET_Y, "_dpm.height")
			->put("alpha", AppStyles::DARK_SECONDARY_TEXT_ALPHA)
			->put("tintColor", Color("#000000FF"))
		))
		->appendDirect_chain(new MenuItemConfig_SmartItem("tbtnOnScreenDPad", "_local.layer", "_in_", [](IAppMetrics* appMetrics, IAppAssets* appAssets) -> ComposedStiledVisual * { return new base::ToggleSwitch2D(appMetrics, appAssets); }, (new VisualPropertiesConfig())
			->put("x",
				1.0f, "props.width",
				VisualValueConfig::OPERATION_SUBTRACT,
				16.0f, "_dpm.width"
			)
			->put("y", -TITLE_H - ENTRY_H * 5 + ENTRY_H * 0.5f, "_dpm.height")

			->put("anchorX", 1.0f)
			->put("anchorY", 0.5f)

			->put("toggleOffSoundConfig_id", &base::SoundConfigsList::UserInterface_Btn_Down->managedPath->path)
			->put("toggleOffSound_volume", 0.5f)
			->put("toggleOnSoundConfig_id", &base::SoundConfigsList::UserInterface_Btn_Down->managedPath->path)
			->put("toggleOnSound_volume", 0.5f)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("lblAppSound", "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Medium-AaNSS-14-_dpFont_"), "Game sound"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("x", SPACE_M, "_dpm.width")
			->put("y", -TITLE_H - ENTRY_H * 6 + TEXT_OFFSET_Y, "_dpm.height")
			->put("alpha", AppStyles::DARK_SECONDARY_TEXT_ALPHA)
			->put("tintColor", Color("#000000FF"))
		))
		->appendDirect_chain(new MenuItemConfig_SmartItem("tbtnAppSound", "_local.layer", "_in_", [](IAppMetrics* appMetrics, IAppAssets* appAssets) -> ComposedStiledVisual* { return new base::ToggleSwitch2D(appMetrics, appAssets); }, (new VisualPropertiesConfig())
			->put("x",
				1.0f, "props.width",
				VisualValueConfig::OPERATION_SUBTRACT,
				16.0f, "_dpm.width"
			)
			->put("y", -TITLE_H - ENTRY_H * 6 + ENTRY_H * 0.5f, "_dpm.height")

			->put("anchorX", 1.0f)
			->put("anchorY", 0.5f)

			->put("toggleOffSoundConfig_id", &base::SoundConfigsList::UserInterface_Btn_Down->managedPath->path)
			->put("toggleOffSound_volume", 0.5f)
			->put("toggleOnSoundConfig_id", &base::SoundConfigsList::UserInterface_Btn_Down->managedPath->path)
			->put("toggleOnSound_volume", 0.5f)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("hDividerSettingsBottom", "layer", VCC_Sprite::New("visual2D/taGeneral.atlas", "general/divider_horizontal_black"), (new VisualPropertiesConfig())
			->put("width", 1.0f, "_local.props.width")
			->put("height", AppStyles::DIVIDER_THICKNESS_DPM, "_dpm.width")
			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("x", 0.0f, "_dpm.width")
			->put("y", -TITLE_H - ENTRY_H * 6 - SPACE_S, "_dpm.height")
			->put("alpha", AppStyles::DIVIDER_DARK_ALPHA)
		))
		
		->appendDirect_chain(new MenuItemConfig_Visual("lblAppVersion", "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Medium-AaNSS-14-_dpFont_"), "App version"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("x", SPACE_M, "_dpm.width")
			->put("y", (-SPACE_M - SPACE_S - ENTRY_H * 7 - SPACE_S) + (-ENTRY_H * 1 + TEXT_OFFSET_Y), "_dpm.height")
			->put("alpha", AppStyles::DARK_SECONDARY_TEXT_ALPHA)
			->put("tintColor", Color("#000000FF"))
		))
		->appendDirect_chain(new MenuItemConfig_Visual("dlblAppVersion", "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Medium-AaNSS-14-_dpFont_"), "#.##"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 1.0f)
			->put("anchorY", 0.0f)
			->put("x",
				1.0f, "props.width",
				VisualValueConfig::OPERATION_ADD,
				-SPACE_M, "_dpm.width"
			)
			->put("y", (-SPACE_M - SPACE_S - ENTRY_H * 7 - SPACE_S) + (-ENTRY_H * 1 + TEXT_OFFSET_Y), "_dpm.height")
			->put("alpha", AppStyles::DARK_SECONDARY_TEXT_ALPHA)
			->put("tintColor", Color("#000000FF"))
		))

		->appendDirect_chain(new MenuItemConfig_Visual("hDividerAppVersionBottom", "layer", VCC_Sprite::New("visual2D/taGeneral.atlas", "general/divider_horizontal_black"), (new VisualPropertiesConfig())
			->put("width", 1.0f, "_local.props.width")
			->put("height", AppStyles::DIVIDER_THICKNESS_DPM, "_dpm.width")
			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("x", 0.0f, "_dpm.width")
			->put("y", (-SPACE_M - SPACE_S - ENTRY_H * 7 - SPACE_S) + (-SPACE_S - ENTRY_H * 1 - SPACE_S), "_dpm.height")
			->put("alpha", AppStyles::DIVIDER_DARK_ALPHA)
		))
	);
}

//asdA29;
//private SoundControllerEventListener soundManagerEventListener = new SoundControllerEventListener() {
//    @Override
//    public void unmuteStatusChanged() {
//        super.unmuteStatusChanged();
//
//        if(isActiveOrAnimating()) {
//            tbtnAppSound.setIsOn(
//                appAssets.getSoundManager().getIsUnmuted(), true, true, false, false
//            );
//        }
//    }
//};

MSVPBody::MSVPBody(
    IApp* app,
    ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList,
    worldGame3D::IAppGame* appGame, IAppUtils* appUtils)
    : super(app, viewItemConfigs, viewEaseDurationsSList, appGame, appUtils)
{
	touchListener.cb_onCursorDown = std::bind(&MSVPBody::v2d_onCursorDown, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
	
	toggleSwitchListener.cb_onSelectedStateChanged = std::bind(&MSVPBody::v2d_onSelectedStateChanged, this, std::placeholders::_1);
}

bool MSVPBody::v2d_onCursorDown(
	IInteractiveDrawable2D* target,
	int cursorIndex, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos, Vector2& localPos)
{
	// Touch events are ignored while the menu is animating.
	if (!isActive() || isAnimating()) {
		return false;
	}

	if (dynamic_cast<void*>(target) == dynamic_cast<void*>(btnHome)) {
		int r = appGame->getScenariosHandler()->setActiveScenario(
			startarea_a_v1::ScenarioFactory::scenarioTemplate->id, new ScenarioConfig(nullptr/*serverConnectConfig*/), true/*forceScenarioRestartIfSame*/,
			""/*optionalZoneName*/, true/*forceZoneRestartIfSame*/
		);
		if (r != 0) {
			throw LogicException(LOC);
		}

		hide(true, nullptr);
	}else if (dynamic_cast<void*>(target) == dynamic_cast<void*>(btnExitZone)) {
		ScenarioTemplate* st = appGame->getScenariosHandler()->getActiveScenarioTemplate();
		if (st != nullptr) {
			int r = appGame->getScenariosHandler()->setActiveScenario(
				st->id, new ScenarioConfig(nullptr/*serverConnectConfig*/), false/*forceScenarioRestartIfSame*/,
				""/*optionalZoneName*/, false/*forceZoneRestartIfSame*/
			);
			if (r != 0) {
				throw LogicException(LOC);
			}
		}

		hide(true, nullptr);
	}
	
	else if (dynamic_cast<void*>(target) == dynamic_cast<void*>(btnCameraZoomOut)) {
		//asdA29;
	}else if (dynamic_cast<void*>(target) == dynamic_cast<void*>(btnCameraZoomIn)) {
		//asdA29;
	}
	
	else if (dynamic_cast<void*>(target) == dynamic_cast<void*>(btnCameraDirectionTurnLeft)) {
		//asdA29;
	}else if (dynamic_cast<void*>(target) == dynamic_cast<void*>(btnCameraDirectionTurnRight)) {
		//asdA29;
	}

	else {
		throw LogicException(LOC);
	}

	return true;
}

void MSVPBody::v2d_onSelectedStateChanged(base::IToggleControl2D* target) {
	if (dynamic_cast<void*>(target) == dynamic_cast<void*>(tbtnAppSound)) {
		if (!isActive()) {
			return;
		}

		bool isOn = tbtnAppSound->getIsOn();
		//asdA29; appAssets.getSoundManager().setIsUnmuted(isOn);
	} else if (dynamic_cast<void*>(target) == dynamic_cast<void*>(tbtnOnScreenDPad)) {
		if (!isActive()) {
			return;
		}

		bool isOn = tbtnOnScreenDPad->getIsOn();
		//asdA29;
	} else {
		throw LogicException(LOC);
	}
}

void MSVPBody::createVisuals(Map1D<std::string, void*>* createConfigMap) {
	super::createVisuals(createConfigMap);

	(tbtnAppSound = getItemMustExistAs<base::ToggleSwitch2D*>("panel.body.tbtnAppSound"))->setEventListener(&toggleSwitchListener);
	(tbtnOnScreenDPad = getItemMustExistAs<base::ToggleSwitch2D*>("panel.body.tbtnOnScreenDPad"))->setEventListener(&toggleSwitchListener);
	
	dlblCameraZoom = getItemMustExistAs<base::BitmapLabel*>("panel.body.dlblCameraZoom");
	
	(btnHome = getItemMustExistAs<base::SpriteButton2D*>("panel.body.btnHome"))->addTouchListener(&touchListener);
	(btnExitZone = getItemMustExistAs<base::SpriteButton2D*>("panel.body.btnExitZone"))->addTouchListener(&touchListener);
	
	(btnCameraZoomOut = getItemMustExistAs<base::SpriteButton2D*>("panel.body.btnCameraZoomOut"))->addTouchListener(&touchListener);
	(btnCameraZoomIn = getItemMustExistAs<base::SpriteButton2D*>("panel.body.btnCameraZoomIn"))->addTouchListener(&touchListener);

	dlblCameraDirection = getItemMustExistAs<base::BitmapLabel*>("panel.body.dlblCameraDirection");
	(btnCameraDirectionTurnLeft = getItemMustExistAs<base::SpriteButton2D*>("panel.body.btnCameraDirectionTurnLeft"))->addTouchListener(&touchListener);
	(btnCameraDirectionTurnRight = getItemMustExistAs<base::SpriteButton2D*>("panel.body.btnCameraDirectionTurnRight"))->addTouchListener(&touchListener);

	dlblAppVersion = getItemMustExistAs<base::BitmapLabel*>("panel.body.dlblAppVersion");
}

void MSVPBody::onSelectedStateChanged() {
	super::onSelectedStateChanged();

	if (!isActive()) {
		//void
	} else {
		/*asdA29
        tbtnAppSound.setIsOn(
            appAssets.getSoundManager().getIsUnmuted(),
            true, false, false, false
        );
		tbtnOnScreenDPad,
		*/

		dlblAppVersion->setTextDirect(AppConfig::APP_VERSION);
	}
}

void MSVPBody::isOnDisplayListChanged() {
	super::isOnDisplayListChanged();

	if (!isVisible()) {
		//asdA29; appAssets.getSoundManager().removeEventsListener(soundManagerEventListener);
	} else {
		//asdA29; appAssets.getSoundManager().addEventsListener(soundManagerEventListener);
	}
}

MSVPBody::~MSVPBody() {
    //void
}
