#include "DV_Title.h"
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
#include <graphics/visual2d/drawable/BitmapLabel.h>

using namespace base;

MenuItemConfig* DV_Title::new_menuItemConfigs_contentArea_titleArea() {
	return new MenuItemConfig_ComposedVisual("titleArea", (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("template", (new VisualPropertiesConfig())
				->put("paddingTop", 24.0f, "_dpm.width")
				->put("paddingBottom", 20.0f, "_dpm.width")
				->put("firstLineHeight", 19.0f, "_dpm.width")
				->put("additionalLineHeight", 20.0f, "_dpm.width")
		))

		->appendDirect_chain(new MenuItemConfig_Data("props", (new VisualPropertiesConfig())
				->put("width", 1.0f, "parent.titleAreaSize.width")
				->put("height", 1.0f, "parent.titleAreaSize.height")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("layer", "parent.layer", VCC_Group::New(), (new VisualPropertiesConfig())
				->put("x", 0.0f)
				->put("y",
						1.0f, "parent.controlsAreaSize.height",
						VisualValueConfig::OPERATION_ADD,
						1.0f, "parent.bodyAreaSize.height"
				)
				->putAnimating("alpha", (new MenuVisualAnimationProperty())
						->putState("_isTitleExists_", 1.0f, EaseConfig::LINEAR)
						->putState("_", 0.0f, EaseConfig::LINEAR)
				)
				->putAnimating("_visibleFloat", (new MenuVisualAnimationProperty())
						->putState("_isTitleExists_", 1.0f, EaseConfig::LINEAR)
						->putState("_", 0.0f, EaseConfig::LINEAR)
				)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("lblTitle", "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Medium-AaNSS-20-_dpFont_"), "Dialog title"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 20, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("width",
					1.0f, "props.width",
					VisualValueConfig::OPERATION_SUBTRACT,
					24.0f * 2.0f, "_dpm.width"
			)
			->put("anchorX", 0.0f)
			->put("anchorY", 1.0f)
			->put("x", 24.0f, "_dpm.width")
			->put("y",
					1.0f, "props.height",
					VisualValueConfig::OPERATION_SUBTRACT,
					24.0f, "_dpm.width"
			)
			->put("tintColor", Color("#000000FF"))
			->put("alpha", AppStyles::DARK_PRIMARY_TEXT_ALPHA)
			->put("linesCountOverride", -1)// Enable unlimited lines wrap.
		))
	);
}

DV_Title::DV_Title(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, IHandlerAutoDisposal* handlerAutoDisposal)
	: super(app, viewItemConfigs, viewEaseDurationsSList, handlerAutoDisposal)
{
	//void
}

void DV_Title::initializeDialogAreas() {
	super::initializeDialogAreas();

	float titlePaddingTop = getItemPropertyValue("contentArea.titleArea.template.paddingTop", nullptr)->getDirectAs_float();

	float titleAreaHeight = titlePaddingTop;

	if (this->dialogConfig->titleText != nullptr) {
		putIsTitleExists(true, false);

		if (this->dialogConfig->titleTotalLinesCount < 1) {
			throw LogicException(LOC);
		}

		float firstLineHeight = getItemPropertyValue("contentArea.titleArea.template.firstLineHeight", nullptr)->getDirectAs_float();
		float additionalLineHeight = getItemPropertyValue("contentArea.titleArea.template.additionalLineHeight", nullptr)->getDirectAs_float();
		float paddingBottom = getItemPropertyValue("contentArea.titleArea.template.paddingBottom", nullptr)->getDirectAs_float();

		titleAreaHeight += firstLineHeight;
		if (this->dialogConfig->titleTotalLinesCount > 1) {
			titleAreaHeight += (this->dialogConfig->titleTotalLinesCount - 1.0f) * additionalLineHeight;
		}

		titleAreaHeight += paddingBottom;

		base::BitmapLabel* lblTitle = getItemMustExistAs< base::BitmapLabel * >("contentArea.titleArea.lblTitle");

		lblTitle->setTextDirect(*this->dialogConfig->titleText);
	}

	getDataMap()->putDirectB("titleAreaHeight", base::IWrappedValue::new_float(titleAreaHeight));
}

DV_Title::~DV_Title() {
	//void
}
