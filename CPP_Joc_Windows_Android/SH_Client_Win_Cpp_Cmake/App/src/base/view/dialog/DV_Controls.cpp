#include "DV_Controls.h"
#include <base/menu/util/MenuItemConfig_Data.h>
#include <base/menu/util/MenuItemConfig_ComposedVisual.h>
#include <base/menu/util/MenuItemConfig_Visual.h>
#include <base/menu/util/VisualPropertiesConfig.h>
#include <base/visual2D/util/VisualCreateConfig.h>
#include <base/visual2D/util/VCC_Group.h>
#include <base/visual2D/util/VCC_Sprite.h>
#include <base/visual2D/util/VCC_BitmapLabel.h>
#include <base/visual2D/util/VCC_TouchArea.h>
#include <base/config/sounds/SoundConfigsList.h>
#include <graphics/visual2d/drawable/BitmapLabel.h>
#include <base/sensor/hapticFeedback/util/HapticFeedbackType.h>
#include <base/sensor/hapticFeedback/util/HapticFeedbackType.h>

using namespace base;

MenuItemConfig* DV_Controls::new_menuItemConfigs_contentArea_controlsArea() {
	return new MenuItemConfig_ComposedVisual("controlsArea", (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("template", (new VisualPropertiesConfig())
				->put("paddingTop", 8.0f, "_dpm.width")
				->put("paddingBottom", 8.0f, "_dpm.width")
				->put("controlsRowHeight", 36.0f, "_dpm.width")

				->put("paddingRight", 8.0f, "_dpm.width")
				->put("interItemsHorizontalSpacing", 8.0f, "_dpm.width")
				->put("labelTouchMargin", 8.0f, "_dpm.width")
		))

		->appendDirect_chain(new MenuItemConfig_Data("props", (new VisualPropertiesConfig())
				->put("width", 1.0f, "parent.controlsAreaSize.width")
				->put("height", 1.0f, "parent.controlsAreaSize.height")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("layer", "parent.layer", VCC_Group::New(), (new VisualPropertiesConfig())
				->put("x", 0.0f)
				->put("y", 0.0f)
				->putAnimating("alpha", (new MenuVisualAnimationProperty())
						->putState("_isControlsExists_", 1.0f, EaseConfig::LINEAR)
						->putState("_", 0.0f, EaseConfig::LINEAR)
				)
				->putAnimating("_visibleFloat", (new MenuVisualAnimationProperty())
						->putState("_isControlsExists_", 1.0f, EaseConfig::LINEAR)
						->putState("_", 0.0f, EaseConfig::LINEAR)
				)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("lblBtnA", "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Medium-AaNSS-14-_dpFont_"), "UPDATE"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			// x is dynamicly updated.
			//->put("x",
			//        1.0f, "props.width",
			//        VisualValueConfig::OPERATION_SUBTRACT,
			//        51.0f, "_dpm.width"
			//)
			//->put("y", 33.0f, "_dpm.width")
		))
		->appendDirect_chain(new MenuItemConfig_Visual("taBtnA", "layer", VCC_TouchArea::New(
			&SoundConfigsList::UserInterface_Btn_Down->managedPath->path, 0.5f, base::HapticFeedbackType::NONE, nullptr, 0.5f
		), (new VisualPropertiesConfig())
			// x is dynamicly updated.
			// ->put("width", 74f, "_dpm.width")
			->put("height", 36.0f, "_dpm.width")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			// x is dynamicly updated.
			//->put("x",
			//        1f, "props.width",
			//        VisualValueConfig::OPERATION_SUBTRACT,
			//        51f, "_dpm.width"
			//)
			//->put("y", 33f, "_dpm.width")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("lblBtnB", "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Medium-AaNSS-14-_dpFont_"), "UPDATE"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			// x is dynamicly updated.
			//->put("x",
			//        1.0f, "props.width",
			//        VisualValueConfig::OPERATION_SUBTRACT,
			//        51.0f, "_dpm.width"
			//)
			//->put("y", 33f, "_dpm.width")
		))
		->appendDirect_chain(new MenuItemConfig_Visual("taBtnB", "layer", VCC_TouchArea::New(&SoundConfigsList::UserInterface_Btn_Down->managedPath->path, 0.5f, base::HapticFeedbackType::NONE, nullptr, 0.5f), (new VisualPropertiesConfig())
			// x is dynamicly updated.
			// ->put("width", 74f, "_dpm.width")
			->put("height", 36.0f, "_dpm.width")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			// x is dynamicly updated.
			//->put("x",
			//        1.0f, "props.width",
			//        VisualValueConfig::OPERATION_SUBTRACT,
			//        51.0f, "_dpm.width"
			//)
			//->put("y", 33.0f, "_dpm.width")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("lblBtnC", "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Medium-AaNSS-14-_dpFont_"), "UPDATE"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			// x is dynamicly updated.
			//->put("x",
			//        1.0f, "props.width",
			//        VisualValueConfig::OPERATION_SUBTRACT,
			//        51.0f, "_dpm.width"
			//)
			//->put("y", 33.0f, "_dpm.width")
		))
		->appendDirect_chain(new MenuItemConfig_Visual("taBtnC", "layer", VCC_TouchArea::New(&SoundConfigsList::UserInterface_Btn_Down->managedPath->path, 0.5f, base::HapticFeedbackType::NONE, nullptr, 0.5f), (new VisualPropertiesConfig())
			// x is dynamicly updated.
			// ->put("width", 74f, "_dpm.width")
			->put("height", 36.0f, "_dpm.width")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			// x is dynamicly updated.
			//->put("x",
			//        1.0f, "props.width",
			//        VisualValueConfig::OPERATION_SUBTRACT,
			//        51.0f, "_dpm.width"
			//)
			//->put("y", 33.0f, "_dpm.width")
		))
	);
}

DV_Controls::DV_Controls(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, IHandlerAutoDisposal* handlerAutoDisposal)
	: super(app, viewItemConfigs, viewEaseDurationsSList, handlerAutoDisposal)
{
	touchListener.cb_onCursorDown = std::bind(&DV_Controls::v2d_onCursorDown, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
}

bool DV_Controls::v2d_onCursorDown(
	IInteractiveDrawable2D* target,
	int cursorIndex, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos, Vector2& localPos)
{
	// Touch events are ignored while the menu is animating.
	if (!isActive()) {
		return false;
	}

	bool isTaBtn = false;
	for (int i = 0; i < cuntrolButtonTouchAreas.count(); i++) {
		IInteractiveDrawable2D* entry = cuntrolButtonTouchAreas.getDirect(i);
		if (dynamic_cast<void*>(target) == dynamic_cast<void*>(entry)) {
			isTaBtn = true;

			break;
		}
	}
	if (isTaBtn) {
		DynamicTextButtonConfig* dynamicButtonConfig = target->getUserDataMap()->getDirectB("dynamicButtonConfig")->getDirectAs_Custom< DynamicTextButtonConfig*>(-1);
		dynamicButtonActivated(dynamicButtonConfig);
	} else {
		throw LogicException(LOC);
	}

	return false;
}

void DV_Controls::dynamicButtonActivated(DynamicTextButtonConfig* dynamicButtonConfig) {
	if (listener != nullptr) {
		listener->onDynamicButtonActivated(this, dynamicButtonConfig);
	}
}

void DV_Controls::createVisuals(Map1D<std::string, void*>* createConfigMap) {
	super::createVisuals(createConfigMap);

	cuntrolButtonTouchAreas.appendDirect(getItemMustExistAs<IInteractiveDrawable2D*>("contentArea.controlsArea.taBtnA"));
	cuntrolButtonTouchAreas.appendDirect(getItemMustExistAs<IInteractiveDrawable2D*>("contentArea.controlsArea.taBtnB"));
	cuntrolButtonTouchAreas.appendDirect(getItemMustExistAs<IInteractiveDrawable2D*>("contentArea.controlsArea.taBtnC"));

	for (int i = 0; i < cuntrolButtonTouchAreas.count(); i++) {
		IInteractiveDrawable2D* entry = cuntrolButtonTouchAreas.getDirect(i);

		entry->addTouchListener(&touchListener);
	}

	cuntrolButtonLbls.appendDirect(getItemMustExistAs<base::BitmapLabel*>("contentArea.controlsArea.lblBtnA"));
	cuntrolButtonLbls.appendDirect(getItemMustExistAs<base::BitmapLabel*>("contentArea.controlsArea.lblBtnB"));
	cuntrolButtonLbls.appendDirect(getItemMustExistAs<base::BitmapLabel*>("contentArea.controlsArea.lblBtnC"));
}

base::BitmapLabel* DV_Controls::getControlLbl(int controlIndex) {
	return cuntrolButtonLbls.getDirect(controlIndex);
}

void DV_Controls::initializeDialogAreas() {
	super::initializeDialogAreas();

	int controlsLinesCount = 0;
	if (this->dialogConfig->dynamicButtonConfigs != nullptr) {
		putIsControlsExists(true, false);

		for (int i = 0; i < this->dialogConfig->dynamicButtonConfigs->count(); i++) {
			DynamicTextButtonConfig* dinamicTextButtonConfig = this->dialogConfig->dynamicButtonConfigs->getPointer(i);

			if (this->dialogConfig->isOneButtonControlPerLine) {
				controlsLinesCount++;
			} else {
				controlsLinesCount = 1;
			}
		}
	}

	float paddingTop = getItemPropertyValue("contentArea.controlsArea.template.paddingTop", nullptr)->getDirectAs_float();
	float paddingBottom = getItemPropertyValue("contentArea.controlsArea.template.paddingBottom", nullptr)->getDirectAs_float();
	float controlsRowHeight = getItemPropertyValue("contentArea.controlsArea.template.controlsRowHeight", nullptr)->getDirectAs_float();

	float controlsAreaHeight = 0.0f;
	if (controlsLinesCount == 1) {
		controlsAreaHeight += paddingTop;
	}
	controlsAreaHeight += controlsLinesCount * controlsRowHeight;
	controlsAreaHeight += paddingBottom;

	getDataMap()->putDirectB("controlsAreaHeight", base::IWrappedValue::new_float(controlsAreaHeight));

	// Initialize the control buttons.
	int btnIndex = 0;
	if (this->dialogConfig->dynamicButtonConfigs != nullptr) {
		for (int i = 0; i < dialogConfig->dynamicButtonConfigs->count(); i++) {
			DynamicTextButtonConfig* dBtnConfig = dialogConfig->dynamicButtonConfigs->getPointer(i);

			base::BitmapLabel* btnLbl = cuntrolButtonLbls.getDirect(btnIndex);
			IInteractiveDrawable2D* btnTA = cuntrolButtonTouchAreas.getDirect(btnIndex);

			initDynamicBtn(dBtnConfig, btnTA, btnLbl);

			btnIndex++;
		}
	}
	for (int i = btnIndex; i < cuntrolButtonLbls.count(); i++) {
		base::BitmapLabel* lblBtn = cuntrolButtonLbls.getDirect(i);
		IInteractiveDrawable2D* taBtn = cuntrolButtonTouchAreas.getDirect(i);

		lblBtn->setIsVisible(false);

		taBtn->setIsVisible(false);
		taBtn->getUserDataMap()->putDirectB("dynamicButtonConfig", nullptr);
	}
}

void DV_Controls::initDynamicBtn(DynamicTextButtonConfig* dynamicButtonConfig, IInteractiveDrawable2D* taBtn, base::BitmapLabel* lblBtn) {
	if (dynamicButtonConfig == nullptr) {
		throw LogicException(LOC);
	}

	taBtn->getUserDataMap()->putDirectB("dynamicButtonConfig", base::IWrappedValue::new_Custom(dynamicButtonConfig, -1));

	taBtn->setIsVisible(true);
	lblBtn->setIsVisible(true);

	if (lblBtn->getText() != dynamicButtonConfig->text) {
		lblBtn->setTextDirect(dynamicButtonConfig->text);
		// This is reuired for the below access of the width property.
		lblBtn->validateGlyphsAndMetricsIfNeeded();
	}

	float labelTouchMargin = getItemPropertyValue("contentArea.controlsArea.template.labelTouchMargin", nullptr)->getDirectAs_float();

	float touchAreaWidth = lblBtn->getWidth() + labelTouchMargin * 2.0f;

	taBtn->setWidth(touchAreaWidth);

	lblBtn->setTintColor(dynamicButtonConfig->tintColor);
	lblBtn->setAlpha(dynamicButtonConfig->alpha);
}

void DV_Controls::syncVisuals(bool syncAbsoluteValues, bool syncAnimatingValues, std::string startStateID, std::string endStateID, float stateCompletionPercent) {
	super::syncVisuals(syncAbsoluteValues, syncAnimatingValues, startStateID, endStateID, stateCompletionPercent);

	syncDynamicButtonsPositions();
}

void DV_Controls::syncDynamicButtonsPositions() {
	float paddingRight = getItemPropertyValue("contentArea.controlsArea.template.paddingRight", nullptr)->getDirectAs_float();
	float paddingBottom = getItemPropertyValue("contentArea.controlsArea.template.paddingBottom", nullptr)->getDirectAs_float();
	float interItemsHorizontalSpacing = getItemPropertyValue("contentArea.controlsArea.template.interItemsHorizontalSpacing", nullptr)->getDirectAs_float();
	float controlsRowHeight = getItemPropertyValue("contentArea.controlsArea.template.controlsRowHeight", nullptr)->getDirectAs_float();
	float controlsAreaWidth = getItemPropertyValue("contentArea.controlsAreaSize.width", nullptr)->getDirectAs_float();

	float rightWallPosX = controlsAreaWidth - paddingRight;
	float posY = paddingBottom + controlsRowHeight / 2.0f;

	for (int i = 0; i < cuntrolButtonTouchAreas.count(); i++) {
		IInteractiveDrawable2D* taBtn = cuntrolButtonTouchAreas.getDirect(i);
		base::BitmapLabel* lblBtn = cuntrolButtonLbls.getDirect(i);

		if (taBtn->getUserDataMap()->getDirectB("dynamicButtonConfig", nullptr) != nullptr) {
			float posX = rightWallPosX - taBtn->getWidth() / 2.0f;

			taBtn->setPosition(posX, posY);
			lblBtn->setPosition(posX, posY);

			if (dialogConfig->isOneButtonControlPerLine) {
				posY += controlsRowHeight;
			} else {
				rightWallPosX = rightWallPosX - taBtn->getWidth() - interItemsHorizontalSpacing;
			}
		}
	}
}

DV_Controls::~DV_Controls() {
	//void
}
