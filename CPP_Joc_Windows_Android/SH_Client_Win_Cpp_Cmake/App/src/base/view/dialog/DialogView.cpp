#include "DialogView.h"
#include <base/statics/StaticsInit.h>
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

using namespace base;

MenuItemConfig* DialogView::new_menuItemConfigs_contentArea() {
	return new MenuItemConfig_ComposedVisual("contentArea", (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("titleAreaSize", (new VisualPropertiesConfig())
			->put("width", 1.0f, "_dataMap.width")
			->put("height", 1.0f, "_dataMap.titleAreaHeight")
		))
		->appendDirect_chain(new MenuItemConfig_Data("bodyAreaSize", (new VisualPropertiesConfig())
			->put("width", 1.0f, "_dataMap.width")
			->put("height", 1.0f, "_dataMap.bodyAreaHeight")
		))
		->appendDirect_chain(new MenuItemConfig_Data("controlsAreaSize", (new VisualPropertiesConfig())
			->put("width", 1.0f, "_dataMap.width")
			->put("height", 1.0f, "_dataMap.controlsAreaHeight")
		))
		->appendDirect_chain(new MenuItemConfig_Data("props", (new VisualPropertiesConfig())
			->put("width", 1.0f, "titleAreaSize.width")
			->put("height", new VisualValueConfig(
				1.0f, "titleAreaSize.height",
				VisualValueConfig::OPERATION_ADD,
				new VisualValueConfig(
					1.0f, "bodyAreaSize.height",
					VisualValueConfig::OPERATION_ADD,
					new VisualValueConfig(
						1.0f, "controlsAreaSize.height"
					)
				)
			))
		))

		->appendDirect_chain(new MenuItemConfig_Visual("layer", "parent.layer", VCC_Group::New(), (new VisualPropertiesConfig())
			->put("x", 
				0.5f, "_appSize.width",
				VisualValueConfig::OPERATION_SUBTRACT,
				0.5f, "props.width"
			)
			// Animating version not used for x.
			//->putAnimating("x", (new MenuVisualAnimationProperty())
			//	->putState("_out_",
			//			-1.0f, "props.width",
			//			AppStyles::EASE_FUNCTION_ACCELERATION_ID, AppStyles::EASE_FUNCTION_ACCELERATION_CONFIG
			//	)
			//	->putState("_",
			//		0.5f, "_appSize.width",
			//		VisualValueConfig::OPERATION_SUBTRACT,
			//		0.5f, "props.width",
			//		EaseConfig::DECELERATION
			//	)
			//)
			->putAnimating("y", (new MenuVisualAnimationProperty())
				->putState("_out_",
					1.0f, "_appSize.height",
					EaseConfig::LINEAR
				)
				->putState("_",
					0.5f, "_appSize.height",
					VisualValueConfig::OPERATION_SUBTRACT,
					0.5f, "props.height",
					EaseConfig::LINEAR
				)
			)
		))

		->appendDirect_chain(new MenuItemConfig_ComposedVisual("shadowHolder", (new ArrayList<MenuItemConfig*>())
			->appendDirect_chain(new MenuItemConfig_Data("props", (new VisualPropertiesConfig())
				->put("width", 1.0f, "parent.props.width")
				->put("height", 1.0f, "parent.props.height")
			))
			->appendDirect_chain(new MenuItemConfig_Data("shadowOffset", (new VisualPropertiesConfig())
				->put("x", 0.0f)
				->put("y", 0.0f)
			))
			->appendDirect_chain(new MenuItemConfig_Visual("layer", "parent.layer", VCC_Group::New(), (new VisualPropertiesConfig())
				->put("x", 0.5f, "props.width")
				->put("y", 0.5f, "props.height")

			))
			->appendDirect_chain(AppStyles::new_ITEM_CONFIG_shadow_DIALOG())
		))

		->appendDirect_chain(new MenuItemConfig_Visual("touchAreaContentBg", "layer", VCC_TouchArea::New(), (new VisualPropertiesConfig())
			->putAnimating("width", (new MenuVisualAnimationProperty())
				->putState("_", 1.0f, "props.width", EaseConfig::LINEAR)
			)
			->putAnimating("height", (new MenuVisualAnimationProperty())
				->putState("_", 1.0f, "props.height", EaseConfig::LINEAR)
			)
			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("x", 0.0f)
			->put("y", 0.0f)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("bg", "layer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "bg/simple_white_FFFFFF"), (new VisualPropertiesConfig())
			->put("width", 1.0f, "props.width")
			->put("height", 1.0f, "props.height")
			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("x", 0.0f)
			->put("y", 0.0f)
		))

		->appendDirect_chain(new_menuItemConfigs_contentArea_titleArea())
		->appendDirect_chain(new_menuItemConfigs_contentArea_bodyArea())
		->appendDirect_chain(new_menuItemConfigs_contentArea_controlsArea())
	);
};

ArrayList<MenuItemConfig*>* DialogView::viewItemConfigs = base::StaticsInit::AddCbGeneral<ArrayList<MenuItemConfig*>*>(1, []() {
	viewItemConfigs = (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("props", (new VisualPropertiesConfig())
			->put("width", 1.0f, "_appSize.width")
			->put("height", 1.0f, "_appSize.height")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("layer", "_root.visualsHolder", VCC_Group::New(), (new VisualPropertiesConfig())
			//void
		))

		->appendDirect_chain(new MenuItemConfig_Visual("screenDimmerFullScreen", "layer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "bg/simple_white_FFFFFF"), (new VisualPropertiesConfig())
			->put("width", 1.0f, "props.width")
			->put("height", 1.0f, "props.height")
			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("x", 0.0f)
			->put("y", 0.0f)
			->put("tintColor", Color("#000000FF"))
			->putAnimating("alpha", (new MenuVisualAnimationProperty())
				->putState("in", AppStyles::SCREEN_DIMMER_ALPHA, EaseConfig::LINEAR)
				->putState("out", 0.0f, EaseConfig::LINEAR)
			)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("touchAreaFullScreen", "layer", VCC_TouchArea::New(), (new VisualPropertiesConfig())
			->put("width", 1.0f, "props.width")
			->put("height", 1.0f, "props.height")
			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("x", 0.0f)
			->put("y", 0.0f)
		))

		->appendDirect_chain(new_menuItemConfigs_contentArea());

});

ArrayList<StateChangeDurations*>* DialogView::viewAnimationDurations = base::StaticsInit::AddCbGeneral<ArrayList<StateChangeDurations*>*>(1, []() {
	viewAnimationDurations = (new ArrayList<StateChangeDurations*>())
		->appendDirect_chain(new StateChangeDurations("_out_", "_in_", AppStyles::ANIMATION_DURATION_S_DEFAULT))
		->appendDirect_chain(new StateChangeDurations("_in_", "_out_", AppStyles::ANIMATION_DURATION_S_DEFAULT))
		->appendDirect_chain(new StateChangeDurations("_in_", "_in_", AppStyles::ANIMATION_DURATION_S_DEFAULT));
});

DialogView::DialogView(IApp* app, IHandlerAutoDisposal* autoDisposalHandler)
	: super(app, viewItemConfigs, viewAnimationDurations, autoDisposalHandler)
{
	touchListener.cb_onCursorDown = std::bind(&DialogView::v2d_onCursorDown, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
}

bool DialogView::v2d_onCursorDown(
	IInteractiveDrawable2D* target,
	int cursorIndex, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos, Vector2& localPos)
{
	// Touch events are ignored while the menu is animating.
	if (!isActive()) {
		return false;
	}

	if (dynamic_cast<void*>(target) == dynamic_cast<void*>(touchAreaFullScreen)) {
		if (listener != nullptr) {
			listener->onCloseRequested(this);
		}
	} else if (dynamic_cast<void*>(target) == dynamic_cast<void*>(contentArea_touchAreaContentBg)) {
		if (listener != nullptr) {
			listener->onContentAreaTouched(this);
		}
	} else {
		throw LogicException(LOC);
	}

	return false;
}

void DialogView::createVisuals(Map1D<std::string, void*>* createConfigMap) {
	super::createVisuals(createConfigMap);

	(touchAreaFullScreen = getItemMustExistAs< IInteractiveDrawable2D * >("touchAreaFullScreen"))->addTouchListener(&touchListener);
	(contentArea_touchAreaContentBg = getItemMustExistAs< IInteractiveDrawable2D * >("contentArea.touchAreaContentBg"))->addTouchListener(&touchListener);
}

void DialogView::onSelectedStateChanged() {
	super::onSelectedStateChanged();

	if (!isActive()) {
		//void
	} else {
		//void
	}
}

DialogView::~DialogView() {
	//void
}
