#include "CSV_PreviewArea.h"
#include <base/menu/util/MenuItemConfig_Data.h>
#include <base/menu/util/MenuItemConfig_ComposedVisual.h>
#include <base/menu/util/MenuItemConfig_Visual.h>
#include <base/menu/util/VisualPropertiesConfig.h>
#include <base/visual2D/util/VisualCreateConfig.h>
#include <base/visual2D/util/VCC_Group.h>
#include <base/visual2D/util/VCC_Sprite.h>
#include <base/visual2D/util/VCC_BitmapLabel.h>
#include <base/visual2D/util/VCC_TouchArea.h>
#include <base/util/AppStyles.h>
#include <base/visual2D/util/VCC_SpriteButton.h>
#include <base/config/sounds/SoundConfigsList.h>
#include <graphics/visual2d/drawable/Sprite2D.h>
#include <graphics/visual2d/drawable/BitmapLabel.h>

using namespace base;

MenuItemConfig* CSV_PreviewArea::new_menuItemConfigs_contentArea_previewArea() {
	return new MenuItemConfig_ComposedVisual("previewArea", (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("size", (new VisualPropertiesConfig())
			->put("width", 1.0f, "_local.parent.previewAreaSize.width")
			->put("height", 1.0f, "_local.parent.previewAreaSize.height")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("layer", "_local.parent.layer", VCC_Group::New(), (new VisualPropertiesConfig())
			->put("x", 0.0f, "_dpm.width")
			->putAnimating("y", (new MenuVisualAnimationProperty())
				->putState("_",
					1.0f, "_local.parent.composedSize.height",
					VisualValueConfig::OPERATION_SUBTRACT,
					1.0f, "_local.size.height",
					EaseConfig::LINEAR
				)
			)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("imgCheckerBg_0_0", "_local.layer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "bg/simple_white_FFFFFF"), (new VisualPropertiesConfig())
			->put("width",
				1.0f / 2.0f, "_local.size.width",
				VisualValueConfig::OPERATION_ADD,
				1.0f, nullptr
			)
			->put("height",
				1.0f / 2.0f, "_local.size.height",
				VisualValueConfig::OPERATION_ADD,
				1.0f, nullptr
			)
			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("x", (1.0f / 2.0f) * 0.0f, "_local.size.width")
			->put("y", (1.0f / 2.0f) * 0.0f, "_local.size.height")
			->put("tintColor", Color("#000000FF"))
		))
		->appendDirect_chain(new MenuItemConfig_Visual("imgCheckerBg_1_0", "_local.layer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "bg/simple_white_FFFFFF"), (new VisualPropertiesConfig())
			->put("width",
				1.0f / 2.0f, "_local.size.width",
				VisualValueConfig::OPERATION_ADD,
				0.0f, nullptr
			)
			->put("height",
				1.0f / 2.0f, "_local.size.height",
				VisualValueConfig::OPERATION_ADD,
				1.0f, nullptr
			)
			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("x", (1.0f / 2.0f) * 1.0f, "_local.size.width")
			->put("y", (1.0f / 2.0f) * 0.0f, "_local.size.height")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("imgCheckerBg_0_1", "_local.layer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "bg/simple_white_FFFFFF"), (new VisualPropertiesConfig())
			->put("width",
				1.0f / 2.0f, "_local.size.width",
				VisualValueConfig::OPERATION_ADD,
				1.0f, nullptr
			)
			->put("height",
				1.0f / 2.0f, "_local.size.height",
				VisualValueConfig::OPERATION_ADD,
				0.0f, nullptr
			)
			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("x", (1.0f / 2.0f) * 0.0f, "_local.size.width")
			->put("y", (1.0f / 2.0f) * 1.0f, "_local.size.height")
		))
		->appendDirect_chain(new MenuItemConfig_Visual("imgCheckerBg_1_1", "_local.layer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "bg/simple_white_FFFFFF"), (new VisualPropertiesConfig())
			->put("width",
				1.0f / 2.0f, "_local.size.width",
				VisualValueConfig::OPERATION_ADD,
				0.0f, nullptr
			)
			->put("height",
				1.0f / 2.0f, "_local.size.height",
				VisualValueConfig::OPERATION_ADD,
				0.0f, nullptr
			)
			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("x", (1.0f / 2.0f) * 1.0f, "_local.size.width")
			->put("y", (1.0f / 2.0f) * 1.0f, "_local.size.height")
			->put("tintColor", Color("#000000FF"))
		))

		->appendDirect_chain(new MenuItemConfig_Visual("imgPreviewColorChip", "_local.layer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "bg/simple_white_FFFFFF"), (new VisualPropertiesConfig())
			->put("width", 1.0f, "_local.size.width")
			->put("height", 1.0f, "_local.size.height")
			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("x", 0.0f)
			->put("y", 0.0f)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("lblColorCodeLeft", "_local.layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Medium-AaNSS-14-_dpFont_"), "#FFFFFFFF"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.0f)
			->put("anchorY", 0.5f)
			->put("x", 11.0f, "_dpm.width")
			->put("y", 20.0f, "_dpm.width")
			->put("tintColor", Color("#FFFFFFFF"))
			->put("alpha", AppStyles::DARK_PRIMARY_TEXT_ALPHA)
		))
		->appendDirect_chain(new MenuItemConfig_Visual("lblColorCodeRight", "_local.layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Medium-AaNSS-14-_dpFont_"), "#FFFFFFFF"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 1.0f)
			->put("anchorY", 0.5f)
			->put("x",
				1.0f, "_local.size.width",
				VisualValueConfig::OPERATION_ADD,
				-11.0f, "_dpm.width"
			)
			->put("y", 20.0f, "_dpm.width")
			->put("tintColor", Color("#000000FF"))
			->put("alpha", AppStyles::DARK_PRIMARY_TEXT_ALPHA)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("btnToggleHistoryColors", "_local.layer", VCC_SpriteButton::New("visual2D/taGeneral.atlas", "colorSelectorView/btnToggleHistoryColors_", &SoundConfigsList::UserInterface_Btn_Down->managedPath->path, 0.5f, nullptr, 0.5f, nullptr, 0.5f), (new VisualPropertiesConfig())
			->put("width", 36.0f, "_dpm.width")
			->put("height", 36.0f, "_dpm.width")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				1.0f, "_local.size.width",
				VisualValueConfig::OPERATION_SUBTRACT,
				21.0f, "_dpm.width"
			)
			->put("y",
				1.0f, "_local.size.height",
				VisualValueConfig::OPERATION_SUBTRACT,
				21.0f, "_dpm.width"
			)
			->putAnimating("rotationD", (new MenuVisualAnimationProperty())
				->putState("in._historyActive_", -180.0f, EaseConfig::LINEAR)
				->putState("out._historyActive_", -180.0f, EaseConfig::LINEAR)
				->putState("in", 0.0f, EaseConfig::LINEAR)
				->putState("out", 0.0f, EaseConfig::LINEAR)
			)
			->putAnimating("alpha", (new MenuVisualAnimationProperty())
				->putState("_isHistoryExists_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
			->putAnimating("_visibleFloat", (new MenuVisualAnimationProperty())
				->putState("_isHistoryExists_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
		))
	);
}

CSV_PreviewArea::CSV_PreviewArea(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, IHandlerAutoDisposal* handlerAutoDisposal)
	: super(app, viewItemConfigs, viewEaseDurationsSList, handlerAutoDisposal)
{
	touchListener.cb_onCursorDown = std::bind(&CSV_PreviewArea::v2d_onCursorDown, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
}

bool CSV_PreviewArea::v2d_onCursorDown(
	IInteractiveDrawable2D* target,
	int cursorIndex, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos, Vector2& localPos)
{
	// Touch events are ignored while the menu is animating.
	if (!isActive()) {
		return false;
	}

	if (dynamic_cast<void*>(target) == dynamic_cast<void*>(contentArea_previewArea_btnToggleHistoryColors)) {
		setIsHistoryActive(!getIsHistoryActive(), true);
	} else {
		throw LogicException(LOC);
	}

	return false;
}

void CSV_PreviewArea::createVisuals(Map1D<std::string, void*>* createConfigMap) {
	super::createVisuals(createConfigMap);
	
	(contentArea_previewArea_btnToggleHistoryColors = getItemMustExistAs< IInteractiveDrawable2D*>("_root.contentArea.previewArea.btnToggleHistoryColors"))->addTouchListener(&touchListener);
}

void CSV_PreviewArea::setActiveColor(Color& activeColor, bool updateSliders, bool animateSliders) {
	super::setActiveColor(activeColor, updateSliders, animateSliders);

	Sprite2D* imgPreviewColorChip = getItemMustExistAs< Sprite2D*>("_root.contentArea.previewArea.imgPreviewColorChip");
	imgPreviewColorChip->setTintColor(activeColor);

	// Update the labels.
	std::string colName = activeColor.serialize();

	getItemMustExistAs< base::BitmapLabel*>("_root.contentArea.previewArea.lblColorCodeLeft" )->setTextDirect(colName);
	getItemMustExistAs< base::BitmapLabel*>("_root.contentArea.previewArea.lblColorCodeRight" )->setTextDirect(colName);
}

CSV_PreviewArea::~CSV_PreviewArea() {
	//void
}
