#include "AppStyles.h"
#include <base/menu/util/MenuItemConfig_ComposedVisual.h>
#include <base/menu/util/MenuItemConfig_Visual.h>
#include <base/menu/util/VisualPropertiesConfig.h>
#include <base/menu/util/VisualPropertyValueConfig.h>
#include <base/visual2D/util/VCC_Sprite.h>
#include <base/menu/util/VisualValueConfig.h>
#include <graphics/visual/Color.h>
#include <base/MM.h>
#include <base/memory/IMemoryManager.h>

Color* AppStyles::THEME_COLOR = newt Color("#4285f4ff");
Color* AppStyles::BG_COLOR_WHITE = newt Color("#ffffffff");
Color* AppStyles::BG_COLOR_GREY = newt Color("#ecececff");
Color* AppStyles::BG_COLOR_DARK = newt Color("#323232ff");
Color* AppStyles::BG_COLOR_BLACK = newt Color("#000000ff");
Color* AppStyles::LIGHT_TEXT_COLOR = newt Color("#E4E4E4ff");
Color* AppStyles::DARK_TEXT_COLOR = newt Color("#000000ff");

Color* AppStyles::RED_COLOR = newt Color("#ea1d24ff");

Color* AppStyles::ICON_DARK_COLOR = newt Color("#000000ff");
const float AppStyles::ICON_DARK_ALPHA = 0.54f;// 0.87f;

const float AppStyles::uiIncrementSizeDPM = 56.0f;

const float AppStyles::ICON_BLACK_ALPHA = 0.87f;

Color* AppStyles::DIVIDER_DARK_COLOR = newt Color("#000000ff");
const float AppStyles::DIVIDER_DARK_ALPHA = 0.12f;
const float AppStyles::DIVIDER_THICKNESS_DPM = 2.0f;

const float AppStyles::DARK_PRIMARY_TEXT_ALPHA = 0.87f;
const float AppStyles::DARK_SECONDARY_TEXT_ALPHA = 0.54f;
const float AppStyles::DARK_DISABLED_TEXT_ALPHA = 0.38f;

const float AppStyles::LIGHT_PRIMARY_TEXT_ALPHA = 1.0f;
const float AppStyles::LIGHT_SECONDARY_TEXT_ALPHA = 0.7f;
const float AppStyles::LIGHT_DISABLED_TEXT_ALPHA = 0.5f;

const float AppStyles::SCREEN_DIMMER_ALPHA = 0.4f;// 0.19f;
const float AppStyles::BG_LIGHT_ALPHA = 0.4f;// This is made by mE.
const float AppStyles::BG_DARK_ALPHA = 0.19f;// This is made by mE.

const float AppStyles::ANIMATION_COMPLEX_DURATION_S = 0.375f;
const float AppStyles::ANIMATION_ENTER_SCREEN_DURATION_S = 0.225f;
const float AppStyles::ANIMATION_LEAVE_SCREEN_DURATION_S = 0.195f;
// This value is intuitive, not from material template.
const float AppStyles::ANIMATION_IN_STATE_CHANGE_DURATION = 0.2f;

const float AppStyles::ANIMATION_SWITCH_STATE_CHANGE = 0.100f;

const float AppStyles::SLIDER_MOVEMENT_SPEED_MULTIPLIER_S = 10.0f;

const float AppStyles::ANIMATION_DURATION_S_DEFAULT = 0.225f;

const float AppStyles::ANIMATION_SPLASH_SCREEN_OUT_DURATION_S = 1.2f;

const float AppStyles::SHADOW_DIALOG_BASE_SIZE = 35.0f;

MenuItemConfig* AppStyles::new_ITEM_CONFIG_shadow_CARD() {
	VisualValueConfig* t1 = newt VisualValueConfig(
		0.0f,
		VisualValueConfig::OPERATION_MAX,
		new VisualValueConfig(
			1.0f, "_local.parent.props.width",
			VisualValueConfig::OPERATION_SUBTRACT,
			new VisualValueConfig(20.0f, "_dpm.width")
		)
	);

	MenuItemConfig* ret = newt MenuItemConfig_ComposedVisual("shadow", (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Visual("_TL", "_local.parent.layer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "shadow/card_composed/_TL"), (new VisualPropertiesConfig())
			->put("width", 20.0f, "_dpm.width")
			->put("height", 20.0f, "_dpm.width")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				-0.5f, "_local.parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				1.0f, "_local.parent.shadowOffset.x"
			)
			->put("y",
				0.5f, "_local.parent.props.height",
				VisualValueConfig::OPERATION_ADD,
				1.0f, "_local.parent.shadowOffset.y"
			)
		))
		->appendDirect_chain(new MenuItemConfig_Visual("_TR", "_local.parent.layer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "shadow/card_composed/_TR"), (new VisualPropertiesConfig())
			->put("width", 20.0f, "_dpm.width")
			->put("height", 20.0f, "_dpm.width")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				0.5f, "_local.parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				1.0f, "_local.parent.shadowOffset.x"
			)
			->put("y",
				0.5f, "_local.parent.props.height",
				VisualValueConfig::OPERATION_ADD,
				1.0f, "_local.parent.shadowOffset.y"
			)
		))
		->appendDirect_chain(new MenuItemConfig_Visual("_BL", "_local.parent.layer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "shadow/card_composed/_BL"), (new VisualPropertiesConfig())
			->put("width", 20.0f, "_dpm.width")
			->put("height", 20.0f, "_dpm.width")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				-0.5f, "_local.parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				1.0f, "_local.parent.shadowOffset.x"
			)
			->put("y",
				-0.5f, "_local.parent.props.height",
				VisualValueConfig::OPERATION_ADD,
				1.0f, "_local.parent.shadowOffset.y"
			)
		))
		->appendDirect_chain(new MenuItemConfig_Visual("_BR", "_local.parent.layer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "shadow/card_composed/_BR"), (new VisualPropertiesConfig())
			->put("width", 20.0f, "_dpm.width")
			->put("height", 20.0f, "_dpm.width")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				0.5f, "_local.parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				1.0f, "_local.parent.shadowOffset.x"
			)
			->put("y",
				-0.5f, "_local.parent.props.height",
				VisualValueConfig::OPERATION_ADD,
				1.0f, "_local.parent.shadowOffset.y"
			)
		))
		->appendDirect_chain(new MenuItemConfig_Visual("_TC", "_local.parent.layer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "shadow/card_composed/_TC"), (new VisualPropertiesConfig())
			->put(new VisualPropertyValueConfig("width",
				new VisualValueConfig(
					0.0f,
					VisualValueConfig::OPERATION_MAX,
					new VisualValueConfig(
						1.0f, "_local.parent.props.width",
						VisualValueConfig::OPERATION_SUBTRACT,
						new VisualValueConfig(20.0f, "_dpm.width")
					)
				)
			))
			->put("height", 20.0f, "_dpm.width")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				0.0f, "_local.parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				1.0f, "_local.parent.shadowOffset.x"
			)
			->put("y",
				0.5f, "_local.parent.props.height",
				VisualValueConfig::OPERATION_ADD,
				1.0f, "_local.parent.shadowOffset.y"
			)
		))
		->appendDirect_chain(new MenuItemConfig_Visual("_BC", "_local.parent.layer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "shadow/card_composed/_BC"), (new VisualPropertiesConfig())
			->put(new VisualPropertyValueConfig("width",
				new VisualValueConfig(
					0.0f,
					VisualValueConfig::OPERATION_MAX,
					new VisualValueConfig(
						1.0f, "_local.parent.props.width",
						VisualValueConfig::OPERATION_SUBTRACT,
						new VisualValueConfig(20.0f, "_dpm.width")
					)
				)
			))
			->put("height", 20.0f, "_dpm.width")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				0.0f, "_local.parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				1.0f, "_local.parent.shadowOffset.x"
			)
			->put("y",
				-0.5f, "_local.parent.props.height",
				VisualValueConfig::OPERATION_ADD,
				1.0f, "_local.parent.shadowOffset.y"
			)
		))
		->appendDirect_chain(new MenuItemConfig_Visual("_CL", "_local.parent.layer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "shadow/card_composed/_CL"), (new VisualPropertiesConfig())
			->put("width", 20.0f, "_dpm.width")
			->put(new VisualPropertyValueConfig("height",
				new VisualValueConfig(
					0.0f,
					VisualValueConfig::OPERATION_MAX,
					new VisualValueConfig(
						1.0f, "_local.parent.props.height",
						VisualValueConfig::OPERATION_SUBTRACT,
						new VisualValueConfig(20.0f, "_dpm.width")
					)
				)
			))
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				-0.5f, "_local.parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				1.0f, "_local.parent.shadowOffset.x"
			)
			->put("y",
				0.0f, "_local.parent.props.height",
				VisualValueConfig::OPERATION_ADD,
				1.0f, "_local.parent.shadowOffset.y"
			)
		))
		->appendDirect_chain(new MenuItemConfig_Visual("_CR", "_local.parent.layer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "shadow/card_composed/_CR"), (new VisualPropertiesConfig())
			->put("width", 20.0f, "_dpm.width")
			->put(new VisualPropertyValueConfig("height",
				new VisualValueConfig(
					0.0f,
					VisualValueConfig::OPERATION_MAX,
					new VisualValueConfig(
						1.0f, "_local.parent.props.height",
						VisualValueConfig::OPERATION_SUBTRACT,
						new VisualValueConfig(20.0f, "_dpm.width")
					)
				)
			))
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				0.5f, "_local.parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				1.0f, "_local.parent.shadowOffset.x"
			)
			->put("y",
				0.0f, "_local.parent.props.height",
				VisualValueConfig::OPERATION_ADD,
				1.0f, "_local.parent.shadowOffset.y"
			)
		))
		->appendDirect_chain(new MenuItemConfig_Visual("_CC", "_local.parent.layer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "shadow/card_composed/_CC"), (new VisualPropertiesConfig())
			->put(new VisualPropertyValueConfig("width",
				new VisualValueConfig(
					0.0f, 
					VisualValueConfig::OPERATION_MAX,
					new VisualValueConfig(
						1.0f, "_local.parent.props.width",
						VisualValueConfig::OPERATION_SUBTRACT,
						new VisualValueConfig(20.0f, "_dpm.width")
					)
				)
			))
			->put(new VisualPropertyValueConfig("height",
				new VisualValueConfig(
					0.0f,
					VisualValueConfig::OPERATION_MAX,
					new VisualValueConfig(
						1.0f, "_local.parent.props.height",
						VisualValueConfig::OPERATION_SUBTRACT,
						new VisualValueConfig(20.0f, "_dpm.width")
					)
				)
			))
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				0.0f, "_local.parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				1.0f, "_local.parent.shadowOffset.x"
			)
			->put("y",
				0.0f, "_local.parent.props.height",
				VisualValueConfig::OPERATION_ADD,
				1.0f, "_local.parent.shadowOffset.y"
			)
		))
	);

	return ret;
}

MenuItemConfig* AppStyles::new_ITEM_CONFIG_shadow_DIALOG() {
	MenuItemConfig* ret = newt MenuItemConfig_ComposedVisual("shadow", (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Visual("_TL", "_local.parent.layer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "shadow/dialog_composed/_TL"), (new VisualPropertiesConfig())
			->put("width", SHADOW_DIALOG_BASE_SIZE * 2.0f, "_dpm.width")
			->put("height", SHADOW_DIALOG_BASE_SIZE * 2.0f, "_dpm.width")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				-0.5f, "_local.parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				1.0f, "_local.parent.shadowOffset.x"
			)
			->put("y",
				0.5f, "_local.parent.props.height",
				VisualValueConfig::OPERATION_ADD,
				1.0f, "_local.parent.shadowOffset.y"
			)
		))
		->appendDirect_chain(new MenuItemConfig_Visual("_TR", "_local.parent.layer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "shadow/dialog_composed/_TR"), (new VisualPropertiesConfig())
			->put("width", SHADOW_DIALOG_BASE_SIZE * 2.0f, "_dpm.width")
			->put("height", SHADOW_DIALOG_BASE_SIZE * 2.0f, "_dpm.width")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				0.5f, "_local.parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				1.0f, "_local.parent.shadowOffset.x"
			)
			->put("y",
				0.5f, "_local.parent.props.height",
				VisualValueConfig::OPERATION_ADD,
				1.0f, "_local.parent.shadowOffset.y"
			)
		))
		->appendDirect_chain(new MenuItemConfig_Visual("_BL", "_local.parent.layer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "shadow/dialog_composed/_BL"), (new VisualPropertiesConfig())
			->put("width", SHADOW_DIALOG_BASE_SIZE * 2.0f, "_dpm.width")
			->put("height", SHADOW_DIALOG_BASE_SIZE * 2.0f, "_dpm.width")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				-0.5f, "_local.parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				1.0f, "_local.parent.shadowOffset.x"
			)
			->put("y",
				-0.5f, "_local.parent.props.height",
				VisualValueConfig::OPERATION_ADD,
				1.0f, "_local.parent.shadowOffset.y"
			)
		))
		->appendDirect_chain(new MenuItemConfig_Visual("_BR", "_local.parent.layer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "shadow/dialog_composed/_BR"), (new VisualPropertiesConfig())
			->put("width", SHADOW_DIALOG_BASE_SIZE * 2.0f, "_dpm.width")
			->put("height", SHADOW_DIALOG_BASE_SIZE * 2.0f, "_dpm.width")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				0.5f, "_local.parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				1.0f, "_local.parent.shadowOffset.x"
			)
			->put("y",
				-0.5f, "_local.parent.props.height",
				VisualValueConfig::OPERATION_ADD,
				1.0f, "_local.parent.shadowOffset.y"
			)
		))
		->appendDirect_chain(new MenuItemConfig_Visual("_TC", "_local.parent.layer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "shadow/dialog_composed/_TC"), (new VisualPropertiesConfig())
			->put(new VisualPropertyValueConfig("width",
				new VisualValueConfig(
					0.0f,
					VisualValueConfig::OPERATION_MAX,
					new VisualValueConfig(
						1.0f, "_local.parent.props.width",
						VisualValueConfig::OPERATION_SUBTRACT,
						new VisualValueConfig(SHADOW_DIALOG_BASE_SIZE * 2.0f, "_dpm.width")
					)
				)
			))
			->put("height", SHADOW_DIALOG_BASE_SIZE * 2.0f, "_dpm.width")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				0.0f, "_local.parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				1.0f, "_local.parent.shadowOffset.x"
			)
			->put("y",
				0.5f, "_local.parent.props.height",
				VisualValueConfig::OPERATION_ADD,
				1.0f, "_local.parent.shadowOffset.y"
			)
		))
		->appendDirect_chain(new MenuItemConfig_Visual("_BC", "_local.parent.layer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "shadow/dialog_composed/_BC"), (new VisualPropertiesConfig())
			->put(new VisualPropertyValueConfig("width",
				new VisualValueConfig(
					0.0f,
					VisualValueConfig::OPERATION_MAX,
					new VisualValueConfig(
						1.0f, "_local.parent.props.width",
						VisualValueConfig::OPERATION_SUBTRACT,
						new VisualValueConfig(SHADOW_DIALOG_BASE_SIZE * 2.0f, "_dpm.width")
					)
				)
			))
			->put("height", SHADOW_DIALOG_BASE_SIZE * 2.0f, "_dpm.width")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				0.0f, "_local.parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				1.0f, "_local.parent.shadowOffset.x"
			)
			->put("y",
				-0.5f, "_local.parent.props.height",
				VisualValueConfig::OPERATION_ADD,
				1.0f, "_local.parent.shadowOffset.y"
			)
		))
		->appendDirect_chain(new MenuItemConfig_Visual("_CL", "_local.parent.layer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "shadow/dialog_composed/_CL"), (new VisualPropertiesConfig())
			->put("width", SHADOW_DIALOG_BASE_SIZE * 2.0f, "_dpm.width")
			->put(new VisualPropertyValueConfig("height",
				new VisualValueConfig(
					0.0f,
					VisualValueConfig::OPERATION_MAX,
					new VisualValueConfig(
						1.0f, "_local.parent.props.height",
						VisualValueConfig::OPERATION_SUBTRACT,
						new VisualValueConfig(SHADOW_DIALOG_BASE_SIZE * 2.0f, "_dpm.width")
					)
				)
			))
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				-0.5f, "_local.parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				1.0f, "_local.parent.shadowOffset.x"
			)
			->put("y",
				0.0f, "_local.parent.props.height",
				VisualValueConfig::OPERATION_ADD,
				1.0f, "_local.parent.shadowOffset.y"
			)
		))
		->appendDirect_chain(new MenuItemConfig_Visual("_CR", "_local.parent.layer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "shadow/dialog_composed/_CR"), (new VisualPropertiesConfig())
			->put("width", SHADOW_DIALOG_BASE_SIZE * 2.0f, "_dpm.width")
			->put(new VisualPropertyValueConfig("height",
				new VisualValueConfig(
					0.0f,
					VisualValueConfig::OPERATION_MAX,
					new VisualValueConfig(
						1.0f, "_local.parent.props.height",
						VisualValueConfig::OPERATION_SUBTRACT,
						new VisualValueConfig(SHADOW_DIALOG_BASE_SIZE * 2.0f, "_dpm.width")
					)
				)
			))
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				0.5f, "_local.parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				1.0f, "_local.parent.shadowOffset.x"
			)
			->put("y",
				0.0f, "_local.parent.props.height",
				VisualValueConfig::OPERATION_ADD,
				1.0f, "_local.parent.shadowOffset.y"
			)
		))
		->appendDirect_chain(new MenuItemConfig_Visual("_CC", "_local.parent.layer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "shadow/dialog_composed/_CC"), (new VisualPropertiesConfig())
			->put(new VisualPropertyValueConfig("width",
				new VisualValueConfig(
					0.0f,
					VisualValueConfig::OPERATION_MAX,
					new VisualValueConfig(
						1.0f, "_local.parent.props.width",
						VisualValueConfig::OPERATION_SUBTRACT,
						new VisualValueConfig(SHADOW_DIALOG_BASE_SIZE * 2.0f, "_dpm.width")
					)
				)
			))
			->put(new VisualPropertyValueConfig("height",
				new VisualValueConfig(
					0.0f,
					VisualValueConfig::OPERATION_MAX,
					new VisualValueConfig(
						1.0f, "_local.parent.props.height",
						VisualValueConfig::OPERATION_SUBTRACT,
						new VisualValueConfig(SHADOW_DIALOG_BASE_SIZE * 2.0f, "_dpm.width")
					)
				)
			))
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				0.0f, "_local.parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				1.0f, "_local.parent.shadowOffset.x"
			)
			->put("y",
				0.0f, "_local.parent.props.height",
				VisualValueConfig::OPERATION_ADD,
				1.0f, "_local.parent.shadowOffset.y"
			)
		))
	);

	return ret;
}
