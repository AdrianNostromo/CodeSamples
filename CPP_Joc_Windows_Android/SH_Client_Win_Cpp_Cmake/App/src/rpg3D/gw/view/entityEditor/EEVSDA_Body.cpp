#include "EEVSDA_Body.h"
#include <base/menu/util/MenuItemConfig_ComposedVisual.h>
#include <base/menu/util/MenuItemConfig_Data.h>
#include <base/menu/util/MenuItemConfig_Visual.h>
#include <base/visual2D/util/VCC_Group.h>
#include <base/visual2D/util/VCC_TouchArea.h>
#include <base/visual2D/util/VCC_Sprite.h>
#include <base/visual2D/util/VCC_SpriteButton.h>
#include <base/visual2D/util/VCC_BitmapLabel.h>
#include <rpg3D/config/sounds/SoundConfigsList.h>
#include <base/util/AppStyles.h>
#include <base/log/GlobalAccessLogHandler.h>
#include <graphics/visual2d/drawable/BitmapLabel.h>
#include <base/util/StringUtil.h>
#include <base/menu/util/MenuItemConfig_SmartItem.h>
#include <graphics/visual2d/drawable/ToggleSwitch2D.h>
#include <rpg3D/gw/entity/module/stackable/IStackableModule.h>
#include <graphics/visual2d/drawable/SpriteButton2D.h>
#include <rpg3D/gw/entity/module/editableVoxelised/IEditableVoxelisedM_Edit.h>
#include <base/math/Math.h>
#include <graphics/visual3d/drawable/VoxelGridDrawable3D.h>

using namespace rpg3D;

const float EEVSDA_Body::ValBounds::gridOffset_x_min = -40.0f;
const float EEVSDA_Body::ValBounds::gridOffset_x_max = 40.0f;
const float EEVSDA_Body::ValBounds::gridOffset_y_min = -40.0f;
const float EEVSDA_Body::ValBounds::gridOffset_y_max = 40.0f;
const float EEVSDA_Body::ValBounds::gridOffset_z_min = -40.0f;
const float EEVSDA_Body::ValBounds::gridOffset_z_max = 40.0f;

const float EEVSDA_Body::ValBounds::voxelSize_min = 0.01f;
const float EEVSDA_Body::ValBounds::voxelSize_max = 1.0f;

const float EEVSDA_Body::ValBounds::haulOffsetZM_min = 0.0f;
const float EEVSDA_Body::ValBounds::haulOffsetZM_max = 4.0f;

MenuItemConfig* EEVSDA_Body::new_viewItemConfig_settingsDrawerA_body() {
	return new MenuItemConfig_ComposedVisual("body", (new ArrayList<MenuItemConfig*>())
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

			->put("x_in",
				1.0f, "parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				-1.0f, "props.width"
			)
			->put("x_out", 1.0f, "parent.props.width")

			->put("activeAreaWidth",
				1.0f, "_self.width",
				VisualValueConfig::OPERATION_ADD,
				-(16.0f * 2.0f), "_dpm.width"
			)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("layer", "parent.layer", VCC_Group::New(), (new VisualPropertiesConfig())
			->putAnimating("x", (new MenuVisualAnimationProperty())
				->putState("_in_._isSettingsDrawerAEnabled:true_", 1.0f, "props.x_in", EaseConfig::DECELERATION)
				->putState("_", 1.0f, "props.x_out", EaseConfig::ACCELERATION)
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

		/*->appendDirect_chain(new MenuItemConfig_Visual("shadow", "layer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "shadow/rightDrawer_left"), (new VisualPropertiesConfig())
			->put("width", 30.0f, "_dpm.width")
			->put("height", 1.0f, "props.height")
			->put("anchorX", 1.0f)
			->put("anchorY", 0.0f)
			->put("x", 0.0f)
			->put("y", 0.0f)
		))*/

		->appendDirect_chain(new_viewItemConfig_settingsDrawerA_body_gridSize())
		->appendDirect_chain(new_viewItemConfig_settingsDrawerA_body_origin())
		->appendDirect_chain(new_viewItemConfig_settingsDrawerA_body_gridOffset())
		->appendDirect_chain(new_viewItemConfig_settingsDrawerA_body_voxelSize())
		->appendDirect_chain(new_viewItemConfig_settingsDrawerA_body_carryOffsetZ())
		->appendDirect_chain(new_viewItemConfig_settingsDrawerA_body_carryOriginIndicator())
	);
}

MenuItemConfig* EEVSDA_Body::new_viewItemConfig_settingsDrawerA_body_gridSize() {
	return new MenuItemConfig_ComposedVisual("gridSize", (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("props", (new VisualPropertiesConfig())
			->put("height", 61.0f, "_dpm.height")
			->put("y",
				1.0f, "parent.props.height",
				VisualValueConfig::OPERATION_ADD,
				-1.0f, "_self.height"
			)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("layer", "_local.parent.layer", VCC_Group::New(), (new VisualPropertiesConfig())
			->put("x", 0.0f, "_dpm.width")
			->put("y", 1.0f, "props.y")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("lblTitle", "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Regular-AaNSS-16-_dpFont_"), "Grid size"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Regular)
			->put("fontSize", 16, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("x", 16.0f, "_dpm.width")
			->put("y", 32.0f, "_dpm.height")
			->put("alpha", 0.7f)
			->put("tintColor", Color("#000000FF"))
		))
		->appendDirect_chain(new MenuItemConfig_Visual("bgGSizeX", "layer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "bg/simple_white_FFFFFF"), (new VisualPropertiesConfig())
			->put("anchorX", 0.5f)
			->put("anchorY", 0.0f)
			->put("x",
				16.0f, "_dpm.width",
				VisualValueConfig::OPERATION_ADD,
				1.0f / 6.0f * 1.0f, "parent.props.activeAreaWidth"
			)
			->put("y", 8.0f, "_dpm.height")
			->put("width", 1.0f / 3.0f, "parent.props.activeAreaWidth")
			->put("height", 18.0f, "_dpm.height")
			->put("tintColor", Color("#FF0000FF"))
			->put("alpha", 0.22f)
		))
		->appendDirect_chain(new MenuItemConfig_Visual("bgGSizeY", "layer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "bg/simple_white_FFFFFF"), (new VisualPropertiesConfig())
			->put("anchorX", 0.5f)
			->put("anchorY", 0.0f)
			->put("x",
				16.0f, "_dpm.width",
				VisualValueConfig::OPERATION_ADD,
				1.0f / 6.0f * 3.0f, "parent.props.activeAreaWidth"
			)
			->put("y", 8.0f, "_dpm.height")
			->put("width", 1.0f / 3.0f, "parent.props.activeAreaWidth")
			->put("height", 18.0f, "_dpm.height")
			->put("tintColor", Color("#00FF00FF"))
			->put("alpha", 0.22f)
		))
		->appendDirect_chain(new MenuItemConfig_Visual("bgGSizeZ", "layer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "bg/simple_white_FFFFFF"), (new VisualPropertiesConfig())
			->put("anchorX", 0.5f)
			->put("anchorY", 0.0f)
			->put("x",
				16.0f, "_dpm.width",
				VisualValueConfig::OPERATION_ADD,
				1.0f / 6.0f * 5.0f, "parent.props.activeAreaWidth"
			)
			->put("y", 8.0f, "_dpm.height")
			->put("width", 1.0f / 3.0f, "parent.props.activeAreaWidth")
			->put("height", 18.0f, "_dpm.height")
			->put("tintColor", Color("#0000FFFF"))
			->put("alpha", 0.22f)
		))
		->appendDirect_chain(new MenuItemConfig_Visual("dlblGSizeX", "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Regular-AaNSS-14-_dpFont_"), "x##"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Regular)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.5f)
			->put("anchorY", 0.0f)
			->put("x",
				16.0f, "_dpm.width",
				VisualValueConfig::OPERATION_ADD,
				1.0f / 6.0f * 1.0f, "parent.props.activeAreaWidth"
			)
			->put("y", 13.0f, "_dpm.height")
			->put("alpha", 0.4f)
			->put("tintColor", Color("#000000FF"))
		))
		->appendDirect_chain(new MenuItemConfig_Visual("dlblGSizeY", "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Regular-AaNSS-14-_dpFont_"), "y##"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Regular)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.5f)
			->put("anchorY", 0.0f)
			->put("x",
				16.0f, "_dpm.width",
				VisualValueConfig::OPERATION_ADD,
				1.0f / 6.0f * 3.0f, "parent.props.activeAreaWidth"
			)
			->put("y", 13.0f, "_dpm.height")
			->put("alpha", 0.4f)
			->put("tintColor", Color("#000000FF"))
		))
		->appendDirect_chain(new MenuItemConfig_Visual("dlblGSizeZ", "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Regular-AaNSS-14-_dpFont_"), "z##"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Regular)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.5f)
			->put("anchorY", 0.0f)
			->put("x",
				16.0f, "_dpm.width",
				VisualValueConfig::OPERATION_ADD,
				1.0f / 6.0f * 5.0f, "parent.props.activeAreaWidth"
			)
			->put("y", 13.0f, "_dpm.height")
			->put("alpha", 0.4f)
			->put("tintColor", Color("#000000FF"))
		))

		->appendDirect_chain(new MenuItemConfig_Visual("hDivider", "layer", VCC_Sprite::New("visual2D/taGeneral.atlas", "general/divider_horizontal_black"), (new VisualPropertiesConfig())
			->put("width",
				1.0f, "parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				-16.0f * 2.0f, "_dpm.width"
			)
			->put("height", AppStyles::DIVIDER_THICKNESS_DPM, "_dpm.width")
			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("x", 16.0f, "_dpm.width")
			->put("y", 0.0f, "_dpm.height")
			->put("alpha", AppStyles::DIVIDER_DARK_ALPHA)
		))
	);
}

MenuItemConfig* EEVSDA_Body::new_viewItemConfig_settingsDrawerA_body_origin() {
	return new MenuItemConfig_ComposedVisual("origin", (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("props", (new VisualPropertiesConfig())
			->put("height", 54.0f, "_dpm.height")
			->put("y",
				1.0f, "parent.gridSize.props.y",
				VisualValueConfig::OPERATION_ADD,
				-1.0f, "_self.height"
			)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("layer", "_local.parent.layer", VCC_Group::New(), (new VisualPropertiesConfig())
			->put("x", 0.0f, "_dpm.width")
			->put("y", 1.0f, "props.y")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("lblTitle", "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Regular-AaNSS-16-_dpFont_"), "Origin"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Regular)
			->put("fontSize", 16, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("x", 16.0f, "_dpm.width")
			->put("y", 32.0f, "_dpm.height")
			->put("alpha", 0.7f)
			->put("tintColor", Color("#000000FF"))
		))

		->appendDirect_chain(new MenuItemConfig_Visual("lblTitle2", "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Regular-AaNSS-14-_dpFont_"), "Show unit origin axes"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Regular)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("x", 16.0f, "_dpm.width")
			->put("y", 13.0f, "_dpm.height")
			->put("alpha", 0.4f)
			->put("tintColor", Color("#000000FF"))
		))

		->appendDirect_chain(new MenuItemConfig_SmartItem("tbtnShowOrigin", "_local.layer", "_in_", [](IAppMetrics* appMetrics, IAppAssets* appAssets) -> ComposedStiledVisual * { return new base::ToggleSwitch2D(appMetrics, appAssets); }, (new VisualPropertiesConfig())
			->put("x",
				1.0f, "parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				-16.0f, "_dpm.width"
			)
			->put("y", 27.0f, "_dpm.height")

			->put("anchorX", 1.0f)
			->put("anchorY", 0.5f)

			->put("toggleOffSoundConfig_id", &SoundConfigsList::UserInterface_Btn_Down->managedPath->path)
			->put("toggleOffSound_volume", 0.5f)
			->put("toggleOnSoundConfig_id", &SoundConfigsList::UserInterface_Btn_Down->managedPath->path)
			->put("toggleOnSound_volume", 0.5f)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("hDivider", "layer", VCC_Sprite::New("visual2D/taGeneral.atlas", "general/divider_horizontal_black"), (new VisualPropertiesConfig())
			->put("width",
				1.0f, "parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				-16.0f * 2.0f, "_dpm.width"
			)
			->put("height", AppStyles::DIVIDER_THICKNESS_DPM, "_dpm.width")
			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("x", 16.0f, "_dpm.width")
			->put("y", 0.0f, "_dpm.height")
			->put("alpha", AppStyles::DIVIDER_DARK_ALPHA)
		))
	);
}

MenuItemConfig* EEVSDA_Body::new_viewItemConfig_settingsDrawerA_body_gridOffset() {
	return new MenuItemConfig_ComposedVisual("gridOffset", (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("props", (new VisualPropertiesConfig())
			->putAnimating("height", (new MenuVisualAnimationProperty())
				->putState("_EditorType:edit_", 79.0f, "_dpm.height", EaseConfig::LINEAR)
				// This height is without the buttons to change the grid offset. The buttons padded height is 28.0f;
				->putState("_", 79.0f - 24.0f, "_dpm.height", EaseConfig::LINEAR)
			)
			->put("y",
				1.0f, "parent.origin.props.y",
				VisualValueConfig::OPERATION_ADD,
				-1.0f, "_self.height"
			)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("layer", "_local.parent.layer", VCC_Group::New(), (new VisualPropertiesConfig())
			->put("x", 0.0f, "_dpm.width")
			->put("y", 1.0f, "props.y")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("lblTitle", "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Regular-AaNSS-16-_dpFont_"), "Grid offset"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Regular)
			->put("fontSize", 16, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("x", 16.0f, "_dpm.width")
			->put("y",
				1.0f, "props.height",
				VisualValueConfig::OPERATION_ADD,
				-23.0f, "_dpm.height"
			)
			->put("alpha", 0.7f)
			->put("tintColor", Color("#000000FF"))
		))
		->appendDirect_chain(new MenuItemConfig_Visual("bgGOffsetX", "layer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "bg/simple_white_FFFFFF"), (new VisualPropertiesConfig())
			->put("anchorX", 0.5f)
			->put("anchorY", 0.0f)
			->put("x",
				16.0f, "_dpm.width",
				VisualValueConfig::OPERATION_ADD,
				1.0f / 6.0f * 1.0f, "parent.props.activeAreaWidth"
			)
			->put("y", 9.0f, "_dpm.height")
			->put("width", 1.0f / 3.0f, "parent.props.activeAreaWidth")
			->putAnimating("height", (new MenuVisualAnimationProperty())
				->putState("_EditorType:edit_", 42.0f, "_dpm.height", EaseConfig::LINEAR)
				->putState("_", 18.0f, "_dpm.height", EaseConfig::LINEAR)
			)
			->put("tintColor", Color("#FF0000FF"))
			->put("alpha", 0.22f)
		))
		->appendDirect_chain(new MenuItemConfig_Visual("bgGOffsetY", "layer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "bg/simple_white_FFFFFF"), (new VisualPropertiesConfig())
			->put("anchorX", 0.5f)
			->put("anchorY", 0.0f)
			->put("x",
				16.0f, "_dpm.width",
				VisualValueConfig::OPERATION_ADD,
				1.0f / 6.0f * 3.0f, "parent.props.activeAreaWidth"
			)
			->put("y", 9.0f, "_dpm.height")
			->put("width", 1.0f / 3.0f, "parent.props.activeAreaWidth")
			->putAnimating("height", (new MenuVisualAnimationProperty())
				->putState("_EditorType:edit_", 42.0f, "_dpm.height", EaseConfig::LINEAR)
				->putState("_", 18.0f, "_dpm.height", EaseConfig::LINEAR)
			)
			->put("tintColor", Color("#00FF00FF"))
			->put("alpha", 0.22f)
		))
		->appendDirect_chain(new MenuItemConfig_Visual("bgGOffsetZ", "layer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "bg/simple_white_FFFFFF"), (new VisualPropertiesConfig())
			->put("anchorX", 0.5f)
			->put("anchorY", 0.0f)
			->put("x",
				16.0f, "_dpm.width",
				VisualValueConfig::OPERATION_ADD,
				1.0f / 6.0f * 5.0f, "parent.props.activeAreaWidth"
			)
			->put("y", 9.0f, "_dpm.height")
			->put("width", 1.0f / 3.0f, "parent.props.activeAreaWidth")
			->putAnimating("height", (new MenuVisualAnimationProperty())
				->putState("_EditorType:edit_", 42.0f, "_dpm.height", EaseConfig::LINEAR)
				->putState("_", 18.0f, "_dpm.height", EaseConfig::LINEAR)
			)
			->put("tintColor", Color("#0000FFFF"))
			->put("alpha", 0.22f)
		))
		->appendDirect_chain(new MenuItemConfig_Visual("dlblGridOffsetX", "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Regular-AaNSS-14-_dpFont_"), "x###.#"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Regular)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.5f)
			->put("anchorY", 0.0f)
			->put("x",
				16.0f, "_dpm.width",
				VisualValueConfig::OPERATION_ADD,
				1.0f / 6.0f * 1.0f, "parent.props.activeAreaWidth"
			)
			->put("y",
				1.0f, "props.height",
				VisualValueConfig::OPERATION_ADD,
				-42.0f, "_dpm.height"
			)
			->put("alpha", 0.4f)
			->put("tintColor", Color("#000000FF"))
		))
		->appendDirect_chain(new MenuItemConfig_Visual("dlblGridOffsetY", "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Regular-AaNSS-14-_dpFont_"), "y###.#"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Regular)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.5f)
			->put("anchorY", 0.0f)
			->put("x",
				16.0f, "_dpm.width",
				VisualValueConfig::OPERATION_ADD,
				1.0f / 6.0f * 3.0f, "parent.props.activeAreaWidth"
			)
			->put("y",
				1.0f, "props.height",
				VisualValueConfig::OPERATION_ADD,
				-42.0f, "_dpm.height"
			)
			->put("alpha", 0.4f)
			->put("tintColor", Color("#000000FF"))
		))
		->appendDirect_chain(new MenuItemConfig_Visual("dlblGridOffsetZ", "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Regular-AaNSS-14-_dpFont_"), "z###.#"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Regular)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.5f)
			->put("anchorY", 0.0f)
			->put("x",
				16.0f, "_dpm.width",
				VisualValueConfig::OPERATION_ADD,
				1.0f / 6.0f * 5.0f, "parent.props.activeAreaWidth"
			)
			->put("y",
				1.0f, "props.height",
				VisualValueConfig::OPERATION_ADD,
				-42.0f, "_dpm.height"
			)
			->put("alpha", 0.4f)
			->put("tintColor", Color("#000000FF"))
		))

		->appendDirect_chain(new MenuItemConfig_ComposedVisual("changeBtnsHolder", (new ArrayList<MenuItemConfig*>())
			->appendDirect_chain(new MenuItemConfig_Visual("layer", "_local.parent.layer", VCC_Group::New(), (new VisualPropertiesConfig())
				->putAnimating("_visibleFloat", (new MenuVisualAnimationProperty())
					->putState("_EditorType:edit_", 1.0f, EaseConfig::LINEAR)
					->putState("_", 0.0f, EaseConfig::LINEAR)
				)
				->putAnimating("alpha", (new MenuVisualAnimationProperty())
					->putState("_EditorType:edit_", 1.0f, EaseConfig::LINEAR)
					->putState("_", 0.0f, EaseConfig::LINEAR)
				)
			))

			->appendDirect_chain(new MenuItemConfig_Visual("btnGridOffsetXDecrement", "layer", VCC_SpriteButton::New(
				"visual2D/taGeneral.atlas", "rpg3D/gw/entityEditor/view/settingsDrawerA/btnLeft_",
				&SoundConfigsList::UserInterface_Btn_Down->managedPath->path, 0.5f, nullptr, 0.5f, nullptr, 0.5f
			), (new VisualPropertiesConfig())
				->put("width", 24.0f, "_dpm.width")
				->put("height", 24.0f, "_dpm.height")
				->put("touchWidthOverride", std::make_shared<float>(32.0f), "_dpm.width")
				->put("touchHeightOverride", std::make_shared<float>(32.0f), "_dpm.height")
				->put("anchorX", 0.5f)
				->put("anchorY", 0.5f)
				->put("x",
					16.0f - 16.0f, "_dpm.width",
					VisualValueConfig::OPERATION_ADD,
					1.0f / 6.0f * 1.0f, "parent.parent.props.activeAreaWidth"
				)
				->put("y", 21.0f, "_dpm.height")
				->put("tintColor", Color("#000000FF"))
				->put("alpha", 0.4f)
			))
			->appendDirect_chain(new MenuItemConfig_Visual("btnGridOffsetXIncrement", "layer", VCC_SpriteButton::New(
				"visual2D/taGeneral.atlas", "rpg3D/gw/entityEditor/view/settingsDrawerA/btnRight_",
				&SoundConfigsList::UserInterface_Btn_Down->managedPath->path, 0.5f, nullptr, 0.5f, nullptr, 0.5f
			), (new VisualPropertiesConfig())
				->put("width", 24.0f, "_dpm.width")
				->put("height", 24.0f, "_dpm.height")
				->put("touchWidthOverride", std::make_shared<float>(32.0f), "_dpm.width")
				->put("touchHeightOverride", std::make_shared<float>(32.0f), "_dpm.height")
				->put("anchorX", 0.5f)
				->put("anchorY", 0.5f)
				->put("x",
					16.0f + 16.0f, "_dpm.width",
					VisualValueConfig::OPERATION_ADD,
					1.0f / 6.0f * 1.0f, "parent.parent.props.activeAreaWidth"
				)
				->put("y", 21.0f, "_dpm.height")
				->put("tintColor", Color("#000000FF"))
				->put("alpha", 0.4f)
			))
			->appendDirect_chain(new MenuItemConfig_Visual("btnGridOffsetYDecrement", "layer", VCC_SpriteButton::New(
				"visual2D/taGeneral.atlas", "rpg3D/gw/entityEditor/view/settingsDrawerA/btnLeft_",
				&SoundConfigsList::UserInterface_Btn_Down->managedPath->path, 0.5f, nullptr, 0.5f, nullptr, 0.5f
			), (new VisualPropertiesConfig())
				->put("width", 24.0f, "_dpm.width")
				->put("height", 24.0f, "_dpm.height")
				->put("touchWidthOverride", std::make_shared<float>(32.0f), "_dpm.width")
				->put("touchHeightOverride", std::make_shared<float>(32.0f), "_dpm.height")
				->put("anchorX", 0.5f)
				->put("anchorY", 0.5f)
				->put("x",
					16.0f - 16.0f, "_dpm.width",
					VisualValueConfig::OPERATION_ADD,
					1.0f / 6.0f * 3.0f, "parent.parent.props.activeAreaWidth"
				)
				->put("y", 21.0f, "_dpm.height")
				->put("tintColor", Color("#000000FF"))
				->put("alpha", 0.4f)
			))
			->appendDirect_chain(new MenuItemConfig_Visual("btnGridOffsetYIncrement", "layer", VCC_SpriteButton::New(
				"visual2D/taGeneral.atlas", "rpg3D/gw/entityEditor/view/settingsDrawerA/btnRight_",
				&SoundConfigsList::UserInterface_Btn_Down->managedPath->path, 0.5f, nullptr, 0.5f, nullptr, 0.5f
			), (new VisualPropertiesConfig())
				->put("width", 24.0f, "_dpm.width")
				->put("height", 24.0f, "_dpm.height")
				->put("touchWidthOverride", std::make_shared<float>(32.0f), "_dpm.width")
				->put("touchHeightOverride", std::make_shared<float>(32.0f), "_dpm.height")
				->put("anchorX", 0.5f)
				->put("anchorY", 0.5f)
				->put("x",
					16.0f + 16.0f, "_dpm.width",
					VisualValueConfig::OPERATION_ADD,
					1.0f / 6.0f * 3.0f, "parent.parent.props.activeAreaWidth"
				)
				->put("y", 21.0f, "_dpm.height")
				->put("tintColor", Color("#000000FF"))
				->put("alpha", 0.4f)
			))
			->appendDirect_chain(new MenuItemConfig_Visual("btnGridOffsetZDecrement", "layer", VCC_SpriteButton::New(
				"visual2D/taGeneral.atlas", "rpg3D/gw/entityEditor/view/settingsDrawerA/btnLeft_",
				&SoundConfigsList::UserInterface_Btn_Down->managedPath->path, 0.5f, nullptr, 0.5f, nullptr, 0.5f
			), (new VisualPropertiesConfig())
				->put("width", 24.0f, "_dpm.width")
				->put("height", 24.0f, "_dpm.height")
				->put("touchWidthOverride", std::make_shared<float>(32.0f), "_dpm.width")
				->put("touchHeightOverride", std::make_shared<float>(32.0f), "_dpm.height")
				->put("anchorX", 0.5f)
				->put("anchorY", 0.5f)
				->put("x",
					16.0f - 16.0f, "_dpm.width",
					VisualValueConfig::OPERATION_ADD,
					1.0f / 6.0f * 5.0f, "parent.parent.props.activeAreaWidth"
				)
				->put("y", 21.0f, "_dpm.height")
				->put("tintColor", Color("#000000FF"))
				->put("alpha", 0.4f)
			))
			->appendDirect_chain(new MenuItemConfig_Visual("btnGridOffsetZIncrement", "layer", VCC_SpriteButton::New(
				"visual2D/taGeneral.atlas", "rpg3D/gw/entityEditor/view/settingsDrawerA/btnRight_",
				&SoundConfigsList::UserInterface_Btn_Down->managedPath->path, 0.5f, nullptr, 0.5f, nullptr, 0.5f
			), (new VisualPropertiesConfig())
				->put("width", 24.0f, "_dpm.width")
				->put("height", 24.0f, "_dpm.height")
				->put("touchWidthOverride", std::make_shared<float>(32.0f), "_dpm.width")
				->put("touchHeightOverride", std::make_shared<float>(32.0f), "_dpm.height")
				->put("anchorX", 0.5f)
				->put("anchorY", 0.5f)
				->put("x",
					16.0f + 16.0f, "_dpm.width",
					VisualValueConfig::OPERATION_ADD,
					1.0f / 6.0f * 5.0f, "parent.parent.props.activeAreaWidth"
				)
				->put("y", 21.0f, "_dpm.height")
				->put("tintColor", Color("#000000FF"))
				->put("alpha", 0.4f)
			))
		))

		->appendDirect_chain(new MenuItemConfig_Visual("hDivider", "layer", VCC_Sprite::New("visual2D/taGeneral.atlas", "general/divider_horizontal_black"), (new VisualPropertiesConfig())
			->put("width",
				1.0f, "parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				-16.0f * 2.0f, "_dpm.width"
			)
			->put("height", AppStyles::DIVIDER_THICKNESS_DPM, "_dpm.width")
			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("x", 16.0f, "_dpm.width")
			->put("y", 0.0f, "_dpm.height")
			->put("alpha", AppStyles::DIVIDER_DARK_ALPHA)
		))
	);
}

MenuItemConfig* EEVSDA_Body::new_viewItemConfig_settingsDrawerA_body_voxelSize() {
	return new MenuItemConfig_ComposedVisual("voxelSize", (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("props", (new VisualPropertiesConfig())
			->put("height", 55.0f, "_dpm.height")
			->put("y",
				1.0f, "parent.gridOffset.props.y",
				VisualValueConfig::OPERATION_ADD,
				-1.0f, "_self.height"
			)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("layer", "_local.parent.layer", VCC_Group::New(), (new VisualPropertiesConfig())
			->put("x", 0.0f, "_dpm.width")
			->put("y", 1.0f, "props.y")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("lblTitle", "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Regular-AaNSS-16-_dpFont_"), "Voxel size"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Regular)
			->put("fontSize", 16, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("x", 16.0f, "_dpm.width")
			->put("y", 32.0f, "_dpm.height")
			->put("alpha", 0.7f)
			->put("tintColor", Color("#000000FF"))
		))

		->appendDirect_chain(new MenuItemConfig_Visual("dlblSubtitle", "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Regular-AaNSS-14-_dpFont_"), "Unit cube size is ##"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Regular)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("x", 16.0f, "_dpm.width")
			->put("y", 13.0f, "_dpm.height")
			->put("alpha", 0.4f)
			->put("tintColor", Color("#000000FF"))
		))

		->appendDirect_chain(new MenuItemConfig_Visual("btnDecreaseVoxelSize", "layer", VCC_SpriteButton::New(
			"visual2D/taGeneral.atlas", "rpg3D/gw/entityEditor/view/settingsDrawerA/btnLeft_",
			&SoundConfigsList::UserInterface_Btn_Down->managedPath->path, 0.5f, nullptr, 0.5f, nullptr, 0.5f
		), (new VisualPropertiesConfig())
			->putAnimating("_visibleFloat", (new MenuVisualAnimationProperty())
				->putState("_EditorType:edit_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
			->putAnimating("alpha", (new MenuVisualAnimationProperty())
				->putState("_EditorType:edit_", 0.4f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
			->put("width", 24.0f, "_dpm.width")
			->put("height", 24.0f, "_dpm.height")
			->put("touchWidthOverride", std::make_shared<float>(32.0f), "_dpm.width")
			->put("touchHeightOverride", std::make_shared<float>(32.0f), "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				1.0f, "parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				-16.0f - 24.0f / 2.0f - 24.0f / 2.0f - 8.0f - 24.0f / 2.0f, "_dpm.width"
			)
			->put("y", 27.0f, "_dpm.height")
			->put("tintColor", Color("#000000FF"))
		))
		->appendDirect_chain(new MenuItemConfig_Visual("btnIncreaseVoxelSize", "layer", VCC_SpriteButton::New(
			"visual2D/taGeneral.atlas", "rpg3D/gw/entityEditor/view/settingsDrawerA/btnRight_",
			&SoundConfigsList::UserInterface_Btn_Down->managedPath->path, 0.5f, nullptr, 0.5f, nullptr, 0.5f
		), (new VisualPropertiesConfig())
			->putAnimating("_visibleFloat", (new MenuVisualAnimationProperty())
				->putState("_EditorType:edit_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
			->putAnimating("alpha", (new MenuVisualAnimationProperty())
				->putState("_EditorType:edit_", 0.4f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
			->put("width", 24.0f, "_dpm.width")
			->put("height", 24.0f, "_dpm.height")
			->put("touchWidthOverride", std::make_shared<float>(32.0f), "_dpm.width")
			->put("touchHeightOverride", std::make_shared<float>(32.0f), "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				1.0f, "parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				-16.0f - 24.0f / 2.0f, "_dpm.width"
			)
			->put("y", 27.0f, "_dpm.height")
			->put("tintColor", Color("#000000FF"))
		))

		->appendDirect_chain(new MenuItemConfig_Visual("hDivider", "layer", VCC_Sprite::New("visual2D/taGeneral.atlas", "general/divider_horizontal_black"), (new VisualPropertiesConfig())
			->put("width",
				1.0f, "parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				-16.0f * 2.0f, "_dpm.width"
			)
			->put("height", AppStyles::DIVIDER_THICKNESS_DPM, "_dpm.width")
			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("x", 16.0f, "_dpm.width")
			->put("y", 0.0f, "_dpm.height")
			->put("alpha", AppStyles::DIVIDER_DARK_ALPHA)
		))
	);
}

MenuItemConfig* EEVSDA_Body::new_viewItemConfig_settingsDrawerA_body_carryOffsetZ() {
	return new MenuItemConfig_ComposedVisual("carryOffsetZ", (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("props", (new VisualPropertiesConfig())
			->put("height", 55.0f, "_dpm.height")
			->put("y",
				1.0f, "parent.voxelSize.props.y",
				VisualValueConfig::OPERATION_ADD,
				-1.0f, "_self.height"
			)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("layer", "_local.parent.layer", VCC_Group::New(), (new VisualPropertiesConfig())
			->put("x", 0.0f, "_dpm.width")
			->put("y", 1.0f, "props.y")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("lblTitle", "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Regular-AaNSS-16-_dpFont_"), "Carry height"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Regular)
			->put("fontSize", 16, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("x", 16.0f, "_dpm.width")
			->put("y", 32.0f, "_dpm.height")
			->put("alpha", 0.7f)
			->put("tintColor", Color("#000000FF"))
		))

		->appendDirect_chain(new MenuItemConfig_Visual("dlblSubtitle", "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Regular-AaNSS-14-_dpFont_"), "Offset_z is ##"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Regular)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("x", 16.0f, "_dpm.width")
			->put("y", 13.0f, "_dpm.height")
			->put("alpha", 0.4f)
			->put("tintColor", Color("#000000FF"))
		))
		
		->appendDirect_chain(new MenuItemConfig_Visual("btnDecreaseCarryOffsetZ", "layer", VCC_SpriteButton::New(
			"visual2D/taGeneral.atlas", "rpg3D/gw/entityEditor/view/settingsDrawerA/btnDown_",
			&SoundConfigsList::UserInterface_Btn_Down->managedPath->path, 0.5f, nullptr, 0.5f, nullptr, 0.5f
		), (new VisualPropertiesConfig())
			->putAnimating("_visibleFloat", (new MenuVisualAnimationProperty())
				->putState("_EditorType:edit_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
			->putAnimating("alpha", (new MenuVisualAnimationProperty())
				->putState("_EditorType:edit_", 0.4f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
			->put("width", 24.0f, "_dpm.width")
			->put("height", 24.0f, "_dpm.height")
			->put("touchWidthOverride", std::make_shared<float>(32.0f), "_dpm.width")
			->put("touchHeightOverride", std::make_shared<float>(32.0f), "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				1.0f, "parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				-16.0f - 24.0f / 2.0f - 24.0f / 2.0f - 8.0f - 24.0f / 2.0f, "_dpm.width"
			)
			->put("y", 27.0f, "_dpm.height")
			->put("tintColor", Color("#000000FF"))
		))
		->appendDirect_chain(new MenuItemConfig_Visual("btnIncreaseCarryOffsetZ", "layer", VCC_SpriteButton::New(
			"visual2D/taGeneral.atlas", "rpg3D/gw/entityEditor/view/settingsDrawerA/btnUp_",
			&SoundConfigsList::UserInterface_Btn_Down->managedPath->path, 0.5f, nullptr, 0.5f, nullptr, 0.5f
		), (new VisualPropertiesConfig())
			->putAnimating("_visibleFloat", (new MenuVisualAnimationProperty())
				->putState("_EditorType:edit_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
			->putAnimating("alpha", (new MenuVisualAnimationProperty())
				->putState("_EditorType:edit_", 0.4f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
			->put("width", 24.0f, "_dpm.width")
			->put("height", 24.0f, "_dpm.height")
			->put("touchWidthOverride", std::make_shared<float>(32.0f), "_dpm.width")
			->put("touchHeightOverride", std::make_shared<float>(32.0f), "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				1.0f, "parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				-16.0f - 24.0f / 2.0f, "_dpm.width"
			)
			->put("y", 27.0f, "_dpm.height")
			->put("tintColor", Color("#000000FF"))
		))

		->appendDirect_chain(new MenuItemConfig_Visual("hDivider", "layer", VCC_Sprite::New("visual2D/taGeneral.atlas", "general/divider_horizontal_black"), (new VisualPropertiesConfig())
			->put("width",
				1.0f, "parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				-16.0f * 2.0f, "_dpm.width"
			)
			->put("height", AppStyles::DIVIDER_THICKNESS_DPM, "_dpm.width")
			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("x", 16.0f, "_dpm.width")
			->put("y", 0.0f, "_dpm.height")
			->put("alpha", AppStyles::DIVIDER_DARK_ALPHA)
		))
	);
}

MenuItemConfig* EEVSDA_Body::new_viewItemConfig_settingsDrawerA_body_carryOriginIndicator() {
	return new MenuItemConfig_ComposedVisual("carryOriginIndicator", (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("props", (new VisualPropertiesConfig())
			->put("height", 55.0f, "_dpm.height")
			->put("y",
				1.0f, "parent.carryOffsetZ.props.y",
				VisualValueConfig::OPERATION_ADD,
				-1.0f, "_self.height"
			)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("layer", "_local.parent.layer", VCC_Group::New(), (new VisualPropertiesConfig())
			->put("x", 0.0f, "_dpm.width")
			->put("y", 1.0f, "props.y")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("lblTitle", "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Regular-AaNSS-16-_dpFont_"), "Carry origin"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Regular)
			->put("fontSize", 16, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("x", 16.0f, "_dpm.width")
			->put("y", 32.0f, "_dpm.height")
			->put("alpha", 0.7f)
			->put("tintColor", Color("#000000FF"))
		))

		->appendDirect_chain(new MenuItemConfig_Visual("lblTitle2", "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Regular-AaNSS-14-_dpFont_"), "Show carry position"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Regular)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("x", 16.0f, "_dpm.width")
			->put("y", 13.0f, "_dpm.height")
			->put("alpha", 0.4f)
			->put("tintColor", Color("#000000FF"))
		))

		->appendDirect_chain(new MenuItemConfig_SmartItem("tbtnShowCarryOrigin", "_local.layer", "_in_", [](IAppMetrics* appMetrics, IAppAssets* appAssets) -> ComposedStiledVisual * { return new base::ToggleSwitch2D(appMetrics, appAssets); }, (new VisualPropertiesConfig())
			->put("x",
				1.0f, "parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				-16.0f, "_dpm.width"
			)
			->put("y", 27.0f, "_dpm.height")

			->put("anchorX", 1.0f)
			->put("anchorY", 0.5f)

			->put("toggleOffSoundConfig_id", &SoundConfigsList::UserInterface_Btn_Down->managedPath->path)
			->put("toggleOffSound_volume", 0.5f)
			->put("toggleOnSoundConfig_id", &SoundConfigsList::UserInterface_Btn_Down->managedPath->path)
			->put("toggleOnSound_volume", 0.5f)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("hDivider", "layer", VCC_Sprite::New("visual2D/taGeneral.atlas", "general/divider_horizontal_black"), (new VisualPropertiesConfig())
			->put("width",
				1.0f, "parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				-16.0f * 2.0f, "_dpm.width"
			)
			->put("height", AppStyles::DIVIDER_THICKNESS_DPM, "_dpm.width")
			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("x", 16.0f, "_dpm.width")
			->put("y", 0.0f, "_dpm.height")
			->put("alpha", AppStyles::DIVIDER_DARK_ALPHA)
		))
	);
}

EEVSDA_Body::EEVSDA_Body(
	IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList,
	IAppUtils* appUtils)
	: super(
		app, viewItemConfigs, viewEaseDurationsSList,
		appUtils
	)
{
	touchListener.cb_onCursorDown = std::bind(&EEVSDA_Body::v2d_onCursorDown, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
	
	toggleSwitchListener.cb_onSelectedStateChanged = std::bind(&EEVSDA_Body::v2d_onSelectedStateChanged, this, std::placeholders::_1);
}

bool EEVSDA_Body::v2d_onCursorDown(
	IInteractiveDrawable2D * target,
	int cursorIndex, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos, Vector2& localPos)
{
	// Touch events are ignored while the menu is animating.
	if (!isActive() || isAnimating()) {
		return false;
	}

	float xx = 0.0f;
	float yy = 0.0f;
	float zz = 0.0f;
	float ff = 0.0f;

	if (dynamic_cast<void*>(target) == dynamic_cast<void*>(taBg)) {
		//void
	}

	else if ((dynamic_cast<void*>(target) == dynamic_cast<void*>(btnGridOffsetXDecrement) && Math::set(xx, -0.5f))
		|| (dynamic_cast<void*>(target) == dynamic_cast<void*>(btnGridOffsetXIncrement) && Math::set(xx, 0.5f))
		|| (dynamic_cast<void*>(target) == dynamic_cast<void*>(btnGridOffsetYDecrement) && Math::set(yy, -0.5f))
		|| (dynamic_cast<void*>(target) == dynamic_cast<void*>(btnGridOffsetYIncrement) && Math::set(yy, 0.5f))
		|| (dynamic_cast<void*>(target) == dynamic_cast<void*>(btnGridOffsetZDecrement) && Math::set(zz, -0.5f))
		|| (dynamic_cast<void*>(target) == dynamic_cast<void*>(btnGridOffsetZIncrement) && Math::set(zz, 0.5f)))
	{
		Vector3* gOffset = entityToEdit->peekEditingVoxelsVisual()->getVoxelsGrid()->getVoxelGridOffset();
		xx += gOffset->x;
		yy += gOffset->y;
		zz += gOffset->z;

		// Clip values to bounds.
		xx = Math::clamp(xx, ValBounds::gridOffset_x_min, ValBounds::gridOffset_x_max);
		yy = Math::clamp(yy, ValBounds::gridOffset_y_min, ValBounds::gridOffset_y_max);
		zz = Math::clamp(zz, ValBounds::gridOffset_z_min, ValBounds::gridOffset_z_max);

		entityToEdit->setVoxelsGridOffset(xx, yy, zz, true);
	}
	
	else if ((dynamic_cast<void*>(target) == dynamic_cast<void*>(btnDecreaseVoxelSize) && Math::set(ff, -0.01f))
		|| (dynamic_cast<void*>(target) == dynamic_cast<void*>(btnIncreaseVoxelSize) && Math::set(ff, 0.01f)))
	{
		float voxelSize = entityToEdit->peekEditingVoxelsVisual()->getVoxelsGrid()->getVoxelSize();
		ff += voxelSize;

		// Clip values to bounds.
		ff = Math::clamp(ff, ValBounds::voxelSize_min, ValBounds::voxelSize_max);

		entityToEdit->setEditEntity_voxelSize(ff, true);
	}
	
	else if ((dynamic_cast<void*>(target) == dynamic_cast<void*>(btnDecreaseCarryOffsetZ) && Math::set(ff, -0.01f))
		|| (dynamic_cast<void*>(target) == dynamic_cast<void*>(btnIncreaseCarryOffsetZ) && Math::set(ff, 0.01f)))
	{
		float haulOffsetZM = entityToEdit_stackable->getHaulOffsetZM();
		ff += haulOffsetZM;

		// Clip values to bounds.
		ff = Math::clamp(ff, ValBounds::haulOffsetZM_min, ValBounds::haulOffsetZM_max);

		entityToEdit->setEditEntity_stackable_haulOffsetZM(ff, true);
	}
	
	else {
		throw LogicException(LOC);
	}

	return true;
}

void EEVSDA_Body::v2d_onSelectedStateChanged(base::IToggleControl2D* target) {
	if (dynamic_cast<void*>(target) == dynamic_cast<void*>(tbtnShowOrigin)) {
		if (!isActive()) {
			return;
		}

		entityToEdit->setIsEntityOriginIndicatorActive(target->getIsOn());
	} else if (dynamic_cast<void*>(target) == dynamic_cast<void*>(tbtnShowCarryOrigin)) {
		if (!isActive()) {
			return;
		}
		
		entityToEdit->setIsEntityStackableCarryPosIndicatorActive(target->getIsOn());
	} else {
		throw LogicException(LOC);
	}
}

void EEVSDA_Body::createVisuals(Map1D<std::string, void*> * createConfigMap) {
	super::createVisuals(createConfigMap);

	dlblGSizeX = getItemMustExistAs<base::BitmapLabel*>("settingsDrawerA.body.gridSize.dlblGSizeX");
	dlblGSizeY = getItemMustExistAs<base::BitmapLabel*>("settingsDrawerA.body.gridSize.dlblGSizeY");
	dlblGSizeZ = getItemMustExistAs<base::BitmapLabel*>("settingsDrawerA.body.gridSize.dlblGSizeZ");
	
	dlblGridOffsetX = getItemMustExistAs<base::BitmapLabel*>("settingsDrawerA.body.gridOffset.dlblGridOffsetX");
	dlblGridOffsetY = getItemMustExistAs<base::BitmapLabel*>("settingsDrawerA.body.gridOffset.dlblGridOffsetY");
	dlblGridOffsetZ = getItemMustExistAs<base::BitmapLabel*>("settingsDrawerA.body.gridOffset.dlblGridOffsetZ");
	
	(taBg = getItemMustExistAs<IInteractiveDrawable2D*>("settingsDrawerA.body.taBg"))->addTouchListener(&touchListener);

	(tbtnShowOrigin = getItemMustExistAs<base::ToggleSwitch2D*>("settingsDrawerA.body.origin.tbtnShowOrigin"))->setEventListener(&toggleSwitchListener);

	(btnGridOffsetXDecrement = getItemMustExistAs<base::SpriteButton2D*>("settingsDrawerA.body.gridOffset.changeBtnsHolder.btnGridOffsetXDecrement"))->addTouchListener(&touchListener);
	(btnGridOffsetXIncrement = getItemMustExistAs<base::SpriteButton2D*>("settingsDrawerA.body.gridOffset.changeBtnsHolder.btnGridOffsetXIncrement"))->addTouchListener(&touchListener);
	(btnGridOffsetYDecrement = getItemMustExistAs<base::SpriteButton2D*>("settingsDrawerA.body.gridOffset.changeBtnsHolder.btnGridOffsetYDecrement"))->addTouchListener(&touchListener);
	(btnGridOffsetYIncrement = getItemMustExistAs<base::SpriteButton2D*>("settingsDrawerA.body.gridOffset.changeBtnsHolder.btnGridOffsetYIncrement"))->addTouchListener(&touchListener);
	(btnGridOffsetZDecrement = getItemMustExistAs<base::SpriteButton2D*>("settingsDrawerA.body.gridOffset.changeBtnsHolder.btnGridOffsetZDecrement"))->addTouchListener(&touchListener);
	(btnGridOffsetZIncrement = getItemMustExistAs<base::SpriteButton2D*>("settingsDrawerA.body.gridOffset.changeBtnsHolder.btnGridOffsetZIncrement"))->addTouchListener(&touchListener);

	voxelSize_dlblSubtitle = getItemMustExistAs<base::BitmapLabel*>("settingsDrawerA.body.voxelSize.dlblSubtitle");
	(btnDecreaseVoxelSize = getItemMustExistAs<base::SpriteButton2D*>("settingsDrawerA.body.voxelSize.btnDecreaseVoxelSize"))->addTouchListener(&touchListener);
	(btnIncreaseVoxelSize = getItemMustExistAs<base::SpriteButton2D*>("settingsDrawerA.body.voxelSize.btnIncreaseVoxelSize"))->addTouchListener(&touchListener);
	
	carryOffsetZ_dlblSubtitle = getItemMustExistAs<base::BitmapLabel*>("settingsDrawerA.body.carryOffsetZ.dlblSubtitle");
	(btnDecreaseCarryOffsetZ = getItemMustExistAs<base::SpriteButton2D*>("settingsDrawerA.body.carryOffsetZ.btnDecreaseCarryOffsetZ"))->addTouchListener(&touchListener);
	(btnIncreaseCarryOffsetZ = getItemMustExistAs<base::SpriteButton2D*>("settingsDrawerA.body.carryOffsetZ.btnIncreaseCarryOffsetZ"))->addTouchListener(&touchListener);

	(tbtnShowCarryOrigin = getItemMustExistAs<base::ToggleSwitch2D*>("settingsDrawerA.body.carryOriginIndicator.tbtnShowCarryOrigin"))->setEventListener(&toggleSwitchListener);

	if (entityToEdit != nullptr) {
		syncIsPossible_gridSize();
		syncIsPossible_gridOffset();
		syncIsPossible_voxelSize();
		syncIsPossible_stackable_haulOffsetZM();

		tbtnShowOrigin->setIsOn(
			entityToEdit->getIsEntityOriginIndicatorActive()/*isOn*/,
			true/*doUpdate*/, false/*doAnimation*/, false/*playSound*/, false/*dispatchEvent*/
		);

		tbtnShowCarryOrigin->setIsOn(
			entityToEdit->getIsEntityStackableCarryPosIndicatorActive()/*isOn*/,
			true/*doUpdate*/, false/*doAnimation*/, false/*playSound*/, false/*dispatchEvent*/
		);
	}
}

void EEVSDA_Body::onEntityToEditChanged() {
	super::onEntityToEditChanged();

	if (entityToEdit != nullptr) {
		syncIsPossible_gridSize();
		syncIsPossible_gridOffset();
		syncIsPossible_voxelSize();
		syncIsPossible_stackable_haulOffsetZM();

		tbtnShowOrigin->setIsOn(
			entityToEdit->getIsEntityOriginIndicatorActive()/*isOn*/,
			true/*doUpdate*/, false/*doAnimation*/, false/*playSound*/, false/*dispatchEvent*/
		);

		tbtnShowCarryOrigin->setIsOn(
			entityToEdit->getIsEntityStackableCarryPosIndicatorActive()/*isOn*/,
			true/*doUpdate*/, false/*doAnimation*/, false/*playSound*/, false/*dispatchEvent*/
		);
	}
}

void EEVSDA_Body::onTranslateVoxelsGrid(float x, float y, float z, bool updatesHistory) {
	super::onTranslateVoxelsGrid(x, y, z, updatesHistory);

	syncIsPossible_gridOffset();
}

void EEVSDA_Body::onEditEntity_voxelSize_changed(float voxelSize, float old_voxelSize, bool updatesHistory) {
	super::onEditEntity_voxelSize_changed(voxelSize, old_voxelSize, updatesHistory);

	syncIsPossible_voxelSize();
}

void EEVSDA_Body::onEditEntity_stackable_haulOffsetZM_changed(float haulOffsetZM, float old_haulOffsetZM, bool updatesHistory) {
	super::onEditEntity_stackable_haulOffsetZM_changed(haulOffsetZM, old_haulOffsetZM, updatesHistory);

	syncIsPossible_stackable_haulOffsetZM();
}

void EEVSDA_Body::onVoxelsGridSizeChange(Vector3Int& gSizeBefore, Vector3Int& gSizeAfter) {
	super::onVoxelsGridSizeChange(gSizeBefore, gSizeAfter);

	syncIsPossible_gridSize();
}

void EEVSDA_Body::syncIsPossible_gridSize() {
	if (entityToEdit == nullptr || dlblGSizeX == nullptr) {
		return;
	}

	Vector3Int* gSize = entityToEdit->peekEditingVoxelsVisual()->getVoxelsGrid()->peekGrid()->getCount();

	if (!isInitialDataSync_gSize || gSize->x != syncData_gSize.x) {
		syncData_gSize.x = gSize->x;

		dlblGSizeX->setTextDirect("x" + std::to_string(syncData_gSize.x));
	}
	if (!isInitialDataSync_gSize || gSize->y != syncData_gSize.y) {
		syncData_gSize.y = gSize->y;

		dlblGSizeY->setTextDirect("y" + std::to_string(syncData_gSize.y));
	}
	if (!isInitialDataSync_gSize || gSize->z != syncData_gSize.z) {
		syncData_gSize.z = gSize->z;

		dlblGSizeZ->setTextDirect("z" + std::to_string(syncData_gSize.z));
	}
	
	isInitialDataSync_gSize = true;
}

void EEVSDA_Body::syncIsPossible_gridOffset() {
	if (entityToEdit == nullptr || dlblGridOffsetX == nullptr) {
		return;
	}

	Vector3* gOffset = entityToEdit->peekEditingVoxelsVisual()->getVoxelsGrid()->getVoxelGridOffset();

	if (!isInitialDataSync_gOffset || gOffset->x != syncData_gOffset.x) {
		syncData_gOffset.x = gOffset->x;

		dlblGridOffsetX->setTextDirect("x" + StringUtil::floatToFixedPrecisionString(syncData_gOffset.x, 1));

		btnGridOffsetXDecrement->setEnabled(Math::compare(syncData_gOffset.x, ValBounds::gridOffset_x_min) > 0);
		btnGridOffsetXIncrement->setEnabled(Math::compare(syncData_gOffset.x, ValBounds::gridOffset_x_max) < 0);
	}
	if (!isInitialDataSync_gOffset || gOffset->y != syncData_gOffset.y) {
		syncData_gOffset.y = gOffset->y;

		dlblGridOffsetY->setTextDirect("y" + StringUtil::floatToFixedPrecisionString(syncData_gOffset.y, 1));

		btnGridOffsetYDecrement->setEnabled(Math::compare(syncData_gOffset.y, ValBounds::gridOffset_y_min) > 0);
		btnGridOffsetYIncrement->setEnabled(Math::compare(syncData_gOffset.y, ValBounds::gridOffset_y_max) < 0);
	}
	if (!isInitialDataSync_gOffset || gOffset->z != syncData_gOffset.z) {
		syncData_gOffset.z = gOffset->z;

		dlblGridOffsetZ->setTextDirect("z" + StringUtil::floatToFixedPrecisionString(syncData_gOffset.z, 1));

		btnGridOffsetZDecrement->setEnabled(Math::compare(syncData_gOffset.z, ValBounds::gridOffset_z_min) > 0);
		btnGridOffsetZIncrement->setEnabled(Math::compare(syncData_gOffset.z, ValBounds::gridOffset_z_max) < 0);
	}

	isInitialDataSync_gOffset = true;
}

void EEVSDA_Body::syncIsPossible_voxelSize() {
	if (entityToEdit == nullptr || voxelSize_dlblSubtitle == nullptr) {
		return;
	}

	float voxelSize = entityToEdit->peekEditingVoxelsVisual()->getVoxelsGrid()->getVoxelSize();

	if (!isInitialDataSync_voxelSize || voxelSize != syncData_voxelSize) {
		syncData_voxelSize = voxelSize;

		voxelSize_dlblSubtitle->setTextDirect("Unit cube size is " + StringUtil::floatToFixedPrecisionString(syncData_voxelSize, 2));

		btnDecreaseVoxelSize->setEnabled(Math::compare(syncData_voxelSize, ValBounds::voxelSize_min) > 0);
		btnIncreaseVoxelSize->setEnabled(Math::compare(syncData_voxelSize, ValBounds::voxelSize_max) < 0);
	}

	isInitialDataSync_voxelSize = true;
}

void EEVSDA_Body::syncIsPossible_stackable_haulOffsetZM() {
	if (entityToEdit == nullptr || carryOffsetZ_dlblSubtitle == nullptr) {
		return;
	}

	float haulOffsetZM = entityToEdit_stackable->getHaulOffsetZM();

	if (!isInitialDataSync_stackable_haulOffsetZM || haulOffsetZM != syncData_stackable_haulOffsetZM) {
		syncData_stackable_haulOffsetZM = haulOffsetZM;

		carryOffsetZ_dlblSubtitle->setTextDirect("Offset_z is " + StringUtil::floatToFixedPrecisionString(syncData_stackable_haulOffsetZM, 2));

		btnDecreaseCarryOffsetZ->setEnabled(Math::compare(syncData_stackable_haulOffsetZM, ValBounds::haulOffsetZM_min) > 0);
		btnIncreaseCarryOffsetZ->setEnabled(Math::compare(syncData_stackable_haulOffsetZM, ValBounds::haulOffsetZM_max) < 0);
	}

	isInitialDataSync_stackable_haulOffsetZM = true;
}

EEVSDA_Body::~EEVSDA_Body() {
	//void
}
