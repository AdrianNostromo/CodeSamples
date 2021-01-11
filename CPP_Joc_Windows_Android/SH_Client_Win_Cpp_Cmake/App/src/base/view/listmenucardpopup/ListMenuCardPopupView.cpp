#include "ListMenuCardPopupView.h"
#include <base/statics/StaticsInit.h>
#include <base/util/AppStyles.h>
#include <base/menu/util/MenuItemConfig_Data.h>
#include <base/menu/util/MenuItemConfig_Visual.h>
#include <base/menu/util/MenuItemConfig_ComposedVisual.h>
#include <base/visual2D/util/VCC_Group.h>
#include <base/visual2D/util/VCC_Sprite.h>
#include <base/visual2D/util/VCC_TouchArea.h>
#include <base/util/AppStyles.h>
#include <base/log/GlobalAccessLogHandler.h>
#include <base/app/IAppMetrics.h>

using namespace base;

ArrayList<MenuItemConfig*>* ListMenuCardPopupView::viewItemConfigs = base::StaticsInit::AddCbGeneral<ArrayList<MenuItemConfig*>*>(1, []() {
	viewItemConfigs = (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("props", (new VisualPropertiesConfig())
				->put("width", 1.0f, "_appSize.width")
				->put("height", 1.0f, "_appSize.height")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("layer", "_root.visualsHolder", VCC_Group::New(), (new VisualPropertiesConfig())
				->putAnimating("alpha", (new MenuVisualAnimationProperty())
						->putState("in", 1.0f, EaseConfig::DECELERATION)
						->putState("out", 0.0f, EaseConfig::ACCELERATION)
				)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("touchAreaFullScreen", "layer", VCC_TouchArea::New(), (new VisualPropertiesConfig())
				->put("width", 1.0f, "_local.props.width")
				->put("height", 1.0f, "_local.props.height")
				->put("anchorX", 0.0f)
				->put("anchorY", 0.0f)
				->put("x", 0.0f)
				->put("y", 0.0f)
			//->put("isTouchFallthroughEnabled", true)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("screenDimmerFullScreen", "layer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "bg/simple_white_FFFFFF"), (new VisualPropertiesConfig())
				->put("width", 1.0f, "_local.props.width")
				->put("height", 1.0f, "_local.props.height")
				->put("anchorX", 0.0f)
				->put("anchorY", 0.0f)
				->put("x", 0.0f)
				->put("y", 0.0f)
				->put("tintColor", Color("#000000FF"))
				->putAnimating("alpha", (new MenuVisualAnimationProperty())
						->putState("_in_._isFullScreenDimmer_", AppStyles::SCREEN_DIMMER_ALPHA, EaseConfig::LINEAR)
						->putState("out", 0.0f, EaseConfig::LINEAR)
				)
				->putAnimating("_visibleFloat", (new MenuVisualAnimationProperty())
						->putState("_in_._isFullScreenDimmer_", 1.0f, EaseConfig::LINEAR)
						->putState("out", 0.0f, EaseConfig::LINEAR)
				)
		))

		->appendDirect_chain(new MenuItemConfig_ComposedVisual("contentArea", (new ArrayList<MenuItemConfig*>())
			->appendDirect_chain(new MenuItemConfig_Visual("layer", "_local.parent.layer", VCC_Group::New(), new VisualPropertiesConfig())
				// Dynamic position
			)

			->appendDirect_chain(new_menuItemConfigs_contentArea_optionsList())

			->appendDirect_chain(new MenuItemConfig_ComposedVisual("shadowHolder", (new ArrayList<MenuItemConfig*>())
				->appendDirect_chain(new MenuItemConfig_Data("props", (new VisualPropertiesConfig())
					->put("width", 1.0f, "_local.parent.optionsList.listWidth")
					->put("height", 1.0f, "_local.parent.optionsList.listHeight")
				))
				->appendDirect_chain(new MenuItemConfig_Data("shadowOffset", (new VisualPropertiesConfig())
					->put("x", 0.0f)
					->put("y", 0.0f)
				))
				->appendDirect_chain(new MenuItemConfig_Visual("layer", "_local.parent.layer", VCC_Group::New(), (new VisualPropertiesConfig())
					->put("x", 0.5f, "_local.props.width")
					->put("y", -0.5f, "_local.props.height")
				))
				->appendDirect_chain(AppStyles::new_ITEM_CONFIG_shadow_CARD())
			))
		));
});

ArrayList<StateChangeDurations*>* ListMenuCardPopupView::viewAnimationDurations = base::StaticsInit::AddCbGeneral<ArrayList<StateChangeDurations*>*>(1, []() {
	viewAnimationDurations = (new ArrayList<StateChangeDurations*>())
		->appendDirect_chain(new StateChangeDurations("_out_", "_in_", AppStyles::ANIMATION_DURATION_S_DEFAULT))
		->appendDirect_chain(new StateChangeDurations("_in_", "_out_", AppStyles::ANIMATION_DURATION_S_DEFAULT));
	});

ListMenuCardPopupView::ListMenuCardPopupView(IApp* app, IHandlerAutoDisposal* handlerAutoDisposal)
	: super(app, viewItemConfigs, viewAnimationDurations, handlerAutoDisposal)
{
	touchListener.cb_onCursorDown = std::bind(&ListMenuCardPopupView::v2d_onCursorDown, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
}

bool ListMenuCardPopupView::v2d_onCursorDown(
	IInteractiveDrawable2D* target,
	int cursorIndex, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos, Vector2& localPos)
{
	// Touch events are ignored while the menu is animating.
	if (!isActive()) {
		return false;
	}

	if (dynamic_cast<void*>(target) == dynamic_cast<void*>(touchAreaFullScreen)) {
		helperCallCloseRequestedEvent();
	} else {
		GlobalAccessLogHandler::devLogHandler->post_debug("Error", "Touch event received for a unknown target.");

		return false;
	}

	return false;
}

void ListMenuCardPopupView::helperCallCloseRequestedEvent() {
	if (listener != nullptr) {
		listener->onCloseRequested(this);
	}
}

void ListMenuCardPopupView::createVisuals(Map1D<std::string, void*>* createConfigMap) {
	super::createVisuals(createConfigMap);

	(touchAreaFullScreen = getItemMustExistAs<IInteractiveDrawable2D*>("_root.touchAreaFullScreen"))->addTouchListener(&touchListener);
}

void ListMenuCardPopupView::onSelectedStateChanged() {
	super::onSelectedStateChanged();

	if (!isActive()) {
		//void
	} else {
		//void
	}
}

void ListMenuCardPopupView::syncVisuals(bool syncAbsoluteValues, bool syncAnimatingValues, std::string startStateID, std::string endStateID, float stateCompletionPercent) {
	super::syncVisuals(syncAbsoluteValues, syncAnimatingValues, startStateID, endStateID, stateCompletionPercent);

	if (viewConfig != nullptr) {
		IDrawableNode2D* contentArea_layer = getItemMustExistAs< IDrawableNode2D * >("_root.contentArea.layer" );

		contentArea_layer->setPosition(
			viewConfig->posXDip * appMetrics->getScreen()->getScreenDensityMultiplier()->x, 
			viewConfig->posYDip * appMetrics->getScreen()->getScreenDensityMultiplier()->y
		);
	}
}

ListMenuCardPopupView::~ListMenuCardPopupView() {
	//void
}
