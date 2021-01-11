#include "SV_Controls.h"
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
#include "util/DynamicTextButtonConfig.h"
#include <graphics/visual2d/drawable/ISprite2D.h>
#include <base/view/event/ViewEvent.h>
#include "util/Config.h"

using namespace base;
using namespace base::snackbar;

MenuItemConfig* SV_Controls::new_menuItemConfigs_contentArea_controlsArea() {
	return new MenuItemConfig_ComposedVisual("controlsArea", (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("template", (new VisualPropertiesConfig())
			->put("btnPaddingLeftRightTopBottom", 8.0f, "_dpm.width")
			->put("btnExclusiveMarginBottom", 8.0f, "_dpm.width")
		))

		->appendDirect_chain(new MenuItemConfig_Data("props", (new VisualPropertiesConfig())
			->put("width", 1.0f, "parent.props.width")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("layer", "parent.layer", VCC_Group::New(), (new VisualPropertiesConfig())
			->put("x", 0.0f)
			->put("y", 0.0f)
			->putAnimating("alpha", (new MenuVisualAnimationProperty())
				->putState("_isControlExists:true_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
			->putAnimating("_visibleFloat", (new MenuVisualAnimationProperty())
				->putState("_isControlExists:true_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
		))

		// Example visual indicator for the touch area.
		//->appendDirect_chain(new MenuItemConfig_Visual("controlBg", "layer", VCC_Sprite::New(
		//	"visual2D/taUtilLinear.atlas",
		//	"bg/simple_white_FFFFFF"
		//), (new VisualPropertiesConfig())
		//	// width is dynamicly updated.
		//	// ->put("width", 74f, "_dpm.width")
		//	->put("height", 36.0f, "_dpm.width")
		//	->put("anchorX", 1.0f)
		//	->put("anchorY", 0.5f)
		//	->put("x",
		//		1.0f, "props.width",
		//		VisualValueConfig::OPERATION_ADD,
		//		-8.0f, "_dpm.width"
		//	)
		//	// y is programatic;
		//	//->put("y", 6.0f, "_dpm.width")
		//	->put("alpha", 0.2f)
		//	->put("tintColor", Color("#FF0000FF"))
		//))
		->appendDirect_chain(new MenuItemConfig_Visual("taBtn", "layer", VCC_TouchArea::New(
			&SoundConfigsList::UserInterface_Btn_Down->managedPath->path, 0.5f, base::HapticFeedbackType::NONE, nullptr, 0.5f
		), (new VisualPropertiesConfig())
			// width is dynamicly updated.
			// ->put("width", 74f, "_dpm.width")
			->put("height", 36.0f, "_dpm.width")
			->put("anchorX", 1.0f)
			->put("anchorY", 0.5f)
			->put("x",
				1.0f, "props.width",
				VisualValueConfig::OPERATION_ADD,
				-8.0f, "_dpm.width"
			)
			// y is programatic;
			//->put("y", 6.0f, "_dpm.width")
		))
		->appendDirect_chain(new MenuItemConfig_Visual("lblBtn", "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Medium-AaNSS-14-_dpFont_"), "UPDATE"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 1.0f)
			->put("anchorY", 0.5f)
			->put("x",
				1.0f, "props.width",
				VisualValueConfig::OPERATION_ADD,
				-8.0f - 8.0f, "_dpm.width"
			)
			// y is programatic;
			//->put("y", 6.0f, "_dpm.width")

			// alpha and tintColor are programatic;
			//->put("alpha", 1.0f)
			//->put("tintColor", Color("#FFFFFFFF"))
		))
	);
}

SV_Controls::SV_Controls(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, IHandlerAutoDisposal* handlerAutoDisposal, ISnackbarViewsInOrderDisplayHandler* viewInOrderDisplayHandler)
	: super(app, viewItemConfigs, viewEaseDurationsSList, handlerAutoDisposal, viewInOrderDisplayHandler)
{
	touchListener.cb_onCursorDown = std::bind(&SV_Controls::v2d_onCursorDown, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
}

bool SV_Controls::v2d_onCursorDown(
	IInteractiveDrawable2D* target,
	int cursorIndex, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos, Vector2& localPos)
{
	// Touch events are ignored while the menu is animating.
	if (!isActive()) {
		return false;
	}

	if (dynamic_cast<void*>(target) == dynamic_cast<void*>(controlTouchArea)) {
		DynamicTextButtonConfig* dynamicButtonConfig = target->getUserDataMap()->getDirectB("dynamicButtonConfig")->getDirectAs_Custom< DynamicTextButtonConfig*>(-1);
		dynamicButtonActivated(dynamicButtonConfig);
	}
	
	else {
		throw LogicException(LOC);
	}

	return false;
}

void SV_Controls::dynamicButtonActivated(DynamicTextButtonConfig* dynamicButtonConfig) {
	events.dispatchEvent(*ViewEvent_DynamicButtonActivated->setUserData_voidP(this, dynamicButtonConfig));
}

void SV_Controls::createVisuals(Map1D<std::string, void*>* createConfigMap) {
	super::createVisuals(createConfigMap);

	(controlTouchArea = getItemMustExistAs<IInteractiveDrawable2D*>("contentArea.controlsArea.taBtn"))->addTouchListener(&touchListener);
	controlLbl = getItemMustExistAs<base::BitmapLabel*>("contentArea.controlsArea.lblBtn");
	controlBg = getItemOptionalAs<ISprite2D*>("contentArea.controlsArea.controlBg");
}

void SV_Controls::initializeSnackbarContent() {
	super::initializeSnackbarContent();

	if (config->dynamicButtonConfig != nullptr) {
		isControlExists = true;

		DynamicTextButtonConfig& dinamicTextButtonConfig = *config->dynamicButtonConfig;

		initDynamicBtn(&dinamicTextButtonConfig, controlTouchArea, controlLbl);
	}
}

void SV_Controls::initDynamicBtn(DynamicTextButtonConfig* dynamicButtonConfig, IInteractiveDrawable2D* taBtn, base::BitmapLabel* lblBtn) {
	if (dynamicButtonConfig == nullptr) {
		throw LogicException(LOC);
	}

	taBtn->getUserDataMap()->putDirectB("dynamicButtonConfig", base::IWrappedValue::new_Custom(dynamicButtonConfig, -1));

	taBtn->setIsVisible(true);
	lblBtn->setIsVisible(true);

	if (lblBtn->getText() != dynamicButtonConfig->text) {
		lblBtn->setTextDirect(dynamicButtonConfig->text);
	}

	lblBtn->setTintColor(dynamicButtonConfig->tintColor);
	lblBtn->setAlpha(dynamicButtonConfig->alpha);
}

void SV_Controls::syncVisuals(bool syncAbsoluteValues, bool syncAnimatingValues, std::string startStateID, std::string endStateID, float stateCompletionPercent) {
	super::syncVisuals(syncAbsoluteValues, syncAnimatingValues, startStateID, endStateID, stateCompletionPercent);

	//void
}

std::string SV_Controls::computeStateFlags(std::string baseState) {
	std::string state = super::computeStateFlags(baseState);

	if (isControlExists) {
		state += "_isControlExists:true_";
	} else {
		state += "_isControlExists:false_";
	}

	return state;
}

SV_Controls::~SV_Controls() {
	//void
}
