#include "DV_Body.h"
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

MenuItemConfig* DV_Body::new_menuItemConfigs_contentArea_bodyArea() {
	return new MenuItemConfig_ComposedVisual("bodyArea", (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("template", (new VisualPropertiesConfig())
			->put("paddingBottom", 20.0f, "_dpm.width")
			->put("firstLineHeight", 13.0f, "_dpm.width")
			->put("additionalLineHeight", 18.0f, "_dpm.width")
		))

		->appendDirect_chain(new MenuItemConfig_Data("props", (new VisualPropertiesConfig())
			->put("width", 1.0f, "parent.bodyAreaSize.width")
			->put("height", 1.0f, "parent.bodyAreaSize.height")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("layer", "parent.layer", VCC_Group::New(), (new VisualPropertiesConfig())
			->put("x", 0.0f)
			->put("y", 1.0f, "parent.controlsAreaSize.height")
			->putAnimating("alpha", (new MenuVisualAnimationProperty())
				->putState("_isBodyExists_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
			->putAnimating("_visibleFloat", (new MenuVisualAnimationProperty())
				->putState("_isBodyExists_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("lblText", "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Regular-AaNSS-16-_dpFont_"), "Dialog description."
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Regular)
			->put("fontSize", 16, "_dpm_font")
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
				0.0f, "_dpm.width"
			)
			->put("tintColor", Color("#000000FF"))
			->put("alpha", AppStyles::DARK_PRIMARY_TEXT_ALPHA)
			->put("linesCountOverride", -1)// Enable unlimited lines wrap.
		))
	);
}

DV_Body::DV_Body(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, IHandlerAutoDisposal* handlerAutoDisposal)
	: super(app, viewItemConfigs, viewEaseDurationsSList, handlerAutoDisposal)
{
	//void
}

void DV_Body::initializeDialogAreas() {
	super::initializeDialogAreas();

	float bodyAreaHeight = 0.0f;

	if (this->dialogConfig->dynamicBodyTextItem != nullptr) {
		putIsBodyExists(true, false);

		if (this->dialogConfig->dynamicBodyTextItem->linesCount < 1) {
			throw LogicException(LOC);
		}

		float firstLineHeight = getItemPropertyValue("contentArea.bodyArea.template.firstLineHeight", nullptr)->getDirectAs_float();
		float additionalLineHeight = getItemPropertyValue("contentArea.bodyArea.template.additionalLineHeight", nullptr)->getDirectAs_float();
		float paddingBottom = getItemPropertyValue("contentArea.bodyArea.template.paddingBottom", nullptr)->getDirectAs_float();

		bodyAreaHeight = firstLineHeight;
		if (this->dialogConfig->dynamicBodyTextItem->linesCount > 1) {
			bodyAreaHeight += (this->dialogConfig->dynamicBodyTextItem->linesCount - 1) * additionalLineHeight;
		}

		bodyAreaHeight += paddingBottom;

		base::BitmapLabel* lblText = getItemMustExistAs< base::BitmapLabel*>("contentArea.bodyArea.lblText");

		lblText->setTextDirect(this->dialogConfig->dynamicBodyTextItem->text);
	}

	getDataMap()->putDirectB("bodyAreaHeight", base::IWrappedValue::new_float(bodyAreaHeight));
}

DV_Body::~DV_Body() {
	//void
}
