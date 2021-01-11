#include "SnackbarView.h"
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
#include <graphics/visual2d/drawable/ISprite2D.h>
#include <graphics/visual2d/drawable/BitmapLabel.h>
#include <base/app/IAppMetrics.h>
#include <base/view/event/ViewEvent.h>
#include "util/Config.h"
#include <base/visual2D/util/VCC_Sprite9Slice2D.h>
#include <graphics/visual2d/drawable/ISprite9Slice2D.h>
#include <base/math/Bounds2D.h>

using namespace base;
using namespace base::snackbar;

#define BODY_SHADOW_SIZE_PIXELS 8.0f

ArrayList<MenuItemConfig*>* SnackbarView::viewItemConfigs = base::StaticsInit::AddCbGeneral<ArrayList<MenuItemConfig*>*>(1, []() {
	viewItemConfigs = (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("props", (new VisualPropertiesConfig())
			->put("width", 1.0f, "_appSize.width")
			->put("height", 1.0f, "_appSize.height")

			->put("bottomTextPadding", 18.0f, "_dpm.height")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("layer", "_root.visualsHolder", VCC_Group::New(), (new VisualPropertiesConfig())
			->putAnimating("_visibleFloat", (new MenuVisualAnimationProperty())
				->putState("_in_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
		))

		->appendDirect_chain(new_menuItemConfigs_contentArea());
});

MenuItemConfig* SnackbarView::new_menuItemConfigs_contentArea() {
	return new MenuItemConfig_ComposedVisual("contentArea", (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("props", (new VisualPropertiesConfig())
			->put("width",
				new VisualValueConfig(
					654.0f, "_dpm.width",
					VisualValueConfig::OPERATION_MIN,
					new VisualValueConfig(
						1.0f, "_appSize.width",
						VisualValueConfig::OPERATION_ADD,
						-8.0f * 2.0f, "_dpm.width"
					)
				)
			)
			// height is programatic.
		))

		->appendDirect_chain(new MenuItemConfig_Visual("layer", "parent.layer", VCC_Group::New(), (new VisualPropertiesConfig())
			->put("x",
				0.5f, "_appSize.width",
				VisualValueConfig::OPERATION_ADD,
				-0.5f, "props.width"
			)
			->put("y", 8.0f, "_dpm.height")
			->putAnimating("alpha", (new MenuVisualAnimationProperty())
				->putState("_in_", 1.0f, EaseConfig::ACCELERATION)
				->putState("_out_", 0.0f, EaseConfig::ACCELERATION)
			)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("bodyOutterShadow", "layer", VCC_Sprite9Slice2D::New(
			"visual2D/taUtilLinear.atlas",
			"shadow/square_4dp_303030_303030_20",
			//"bg/TestImg_9Slice",
			false/*drawCenterRegion*/,
			Slice9SizePercents{
				30/*x0*/, 30/*x1*/, 30/*x2*/,
				30/*y0*/, 30/*y1*/, 30/*y2*/
			}/*sliceSizePercents*/
		), (new VisualPropertiesConfig())
			->put("width", 
				1.0f, "props.width",
				VisualValueConfig::OPERATION_ADD,
				BODY_SHADOW_SIZE_PIXELS * 2.0f, "_dpm.width"
			)
			//->put("height", 200.0f, "_dpm.height") Programatic
			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			// Programatic
			//->put("innerBoundsMinX", 60.0f, "_dpm.width")
			//->put("innerBoundsMinY", 60.0f, "_dpm.height")
			//->put("innerBoundsMaxX", 340.0f, "_dpm.width")
			//->put("innerBoundsMaxY", 340.0f, "_dpm.height")
			->put("x", -BODY_SHADOW_SIZE_PIXELS, "_dpm.width")
			->put("y", -BODY_SHADOW_SIZE_PIXELS, "_dpm.height")
			->put("alpha", 0.4f)
			//->put("tintColor", Color("#000000FF"))
		))
		->appendDirect_chain(new MenuItemConfig_Visual("touchAreaContentBg", "layer", VCC_TouchArea::New(), (new VisualPropertiesConfig())
			->put("width", 1.0f, "props.width")
			// Programatic
			//->put("height", 1.0f, "props.height")
			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("x", 0.0f)
			->put("y", 0.0f)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("bg", "layer", VCC_Sprite::New(
			"visual2D/taUtilLinear.atlas", "bg/simple_white_FFFFFF"
		), (new VisualPropertiesConfig())
			->put("width", 1.0f, "props.width")
			//->put("height", 1.0f, "props.height")//programatic
			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("x", 0.0f)
			->put("y", 0.0f)
			->put("tintColor", Color("#323232ff"))
			->put("alpha", 0.95f)
		))

		->appendDirect_chain(new_menuItemConfigs_contentArea_textArea())
		->appendDirect_chain(new_menuItemConfigs_contentArea_controlsArea())
	);
};

ArrayList<StateChangeDurations*>* SnackbarView::viewAnimationDurations = base::StaticsInit::AddCbGeneral<ArrayList<StateChangeDurations*>*>(1, []() {
	viewAnimationDurations = (new ArrayList<StateChangeDurations*>())
		->appendDirect_chain(new StateChangeDurations("_out_", "_in_", AppStyles::ANIMATION_DURATION_S_DEFAULT))
		->appendDirect_chain(new StateChangeDurations("_in_", "_out_", AppStyles::ANIMATION_DURATION_S_DEFAULT))
		->appendDirect_chain(new StateChangeDurations("_in_", "_in_", AppStyles::ANIMATION_DURATION_S_DEFAULT));
});

SnackbarView::SnackbarView(IApp* app, IHandlerAutoDisposal* autoDisposalHandler, ISnackbarViewsInOrderDisplayHandler* viewInOrderDisplayHandler)
	: super(app, viewItemConfigs, viewAnimationDurations, autoDisposalHandler, viewInOrderDisplayHandler)
{
	touchListener.cb_onCursorDown = std::bind(&SnackbarView::v2d_onCursorDown, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
}

bool SnackbarView::v2d_onCursorDown(
	IInteractiveDrawable2D* target,
	int cursorIndex, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos, Vector2& localPos)
{
	// Touch events are ignored while the menu is animating.
	if (!isActive()) {
		return false;
	}

	if (dynamic_cast<void*>(target) == dynamic_cast<void*>(contentArea_touchAreaContentBg)) {
		events.dispatchEvent(*ViewEvent_ContentAreaTouched->setUserData_voidP(this, nullptr));
	}
	
	else {
		throw LogicException(LOC);
	}

	return false;
}

void SnackbarView::createVisuals(Map1D<std::string, void*>* createConfigMap) {
	super::createVisuals(createConfigMap);
	
	(contentArea_touchAreaContentBg = getItemMustExistAs< IInteractiveDrawable2D * >("contentArea.touchAreaContentBg"))->addTouchListener(&touchListener);
	contentArea_bg = getItemMustExistAs<ISprite2D*>("contentArea.bg");
	bodyOutterShadow = getItemAs<base::ISprite9Slice2D*>("contentArea.bodyOutterShadow", true/*mustExist*/);
}

void SnackbarView::syncVisuals(bool syncAbsoluteValues, bool syncAnimatingValues, std::string startStateID, std::string endStateID, float stateCompletionPercent) {
	super::syncVisuals(syncAbsoluteValues, syncAnimatingValues, startStateID, endStateID, stateCompletionPercent);

	float contentAreaWidth = getItemPropertyValue("contentArea.props.width", nullptr)->getDirectAs_float();
	float dpmToPixelsW = appMetrics->getScreen()->getScreenDensityMultiplier()->x;
	float dpmToPixelsH = appMetrics->getScreen()->getScreenDensityMultiplier()->y;

	bool isControlExistsAndOnExclusiveLine = false;
	if (config != nullptr && config->dynamicButtonConfig != nullptr && config->dynamicButtonConfig->hasExclusiveLine) {
		isControlExistsAndOnExclusiveLine = true;
	}

	float btnPaddingLeftRightTopBottom = getItemPropertyValue("contentArea.controlsArea.template.btnPaddingLeftRightTopBottom", nullptr)->getDirectAs_float();

	if (isControlExistsAndOnExclusiveLine) {
		// Control on exclusive line.
		float btnExclusiveMarginBottom = getItemPropertyValue("contentArea.controlsArea.template.btnExclusiveMarginBottom", nullptr)->getDirectAs_float();

		controlLbl->setY(btnExclusiveMarginBottom + controlTouchArea->getHeight() * 0.5f);
		controlLbl->validateGlyphsAndMetricsIfNeeded();

		controlTouchArea->setY(controlLbl->getY());
		controlTouchArea->setWidth(controlLbl->getWidth() + btnPaddingLeftRightTopBottom * 2.0f);
		if (controlBg != nullptr) {
			controlBg->setY(controlTouchArea->getY());
			controlBg->setWidth(controlTouchArea->getWidth());
		}

		contentArea_textArea_lblText->setY(btnExclusiveMarginBottom + controlTouchArea->getHeight() + 18.0f * dpmToPixelsH);
		contentArea_textArea_lblText->setWrappingWidth(contentAreaWidth - ((16.0f/*left_padding_dpm*/ + 16.0f/*right_padding_dpm*/) * dpmToPixelsW));
		contentArea_textArea_lblText->validateGlyphsAndMetricsIfNeeded();

		float contentAreaHeight = 
			btnExclusiveMarginBottom + controlTouchArea->getHeight() + 18.0f * dpmToPixelsH 
			// This is the distance from the text pos_y to he baseline of the last line.
			+ contentArea_textArea_lblText->computeLineHeight() * (contentArea_textArea_lblText->getLinesCount() - 1)
			// 30 pixels are from the last line baseline to the body_upper_part.
			+ 30 * dpmToPixelsH;
		contentArea_bg->setHeight(contentAreaHeight);
		bodyOutterShadow->setHeight(contentAreaHeight + BODY_SHADOW_SIZE_PIXELS * 2.0f * dpmToPixelsH);
		Bounds2D shadowInnerBounds{
			BODY_SHADOW_SIZE_PIXELS * dpmToPixelsH/*xMin*/, BODY_SHADOW_SIZE_PIXELS * dpmToPixelsH/*yMin*/,
			BODY_SHADOW_SIZE_PIXELS * dpmToPixelsH + contentArea_bg->getWidth()/*xMax*/, BODY_SHADOW_SIZE_PIXELS * dpmToPixelsH + contentArea_bg->getHeight()/*yMax*/
		};
		bodyOutterShadow->setInnerBounds(shadowInnerBounds);
		contentArea_touchAreaContentBg->setHeight(contentAreaHeight);
	} else if(isControlExists) {
		// Control on the right.
		controlLbl->validateGlyphsAndMetricsIfNeeded();
		controlTouchArea->setWidth(controlLbl->getWidth() + btnPaddingLeftRightTopBottom * 2.0f);
		if (controlBg != nullptr) {
			controlBg->setWidth(controlTouchArea->getWidth());
		}

		contentArea_textArea_lblText->setY((6.0f + 8.0f) * dpmToPixelsH);
		contentArea_textArea_lblText->setWrappingWidth(contentAreaWidth - (16.0f/*left_margin*/ + 8.0f + 8.0f) * dpmToPixelsW - controlTouchArea->getWidth());
		contentArea_textArea_lblText->validateGlyphsAndMetricsIfNeeded();

		float contentAreaHeight =
			(6.0f + 8.0f) * dpmToPixelsH
			// This is the distance from the text pos_y to he baseline of the last line.
			+ contentArea_textArea_lblText->computeLineHeight() * (contentArea_textArea_lblText->getLinesCount() - 1)
			+ 30 * dpmToPixelsH;
		contentArea_bg->setHeight(contentAreaHeight);
		bodyOutterShadow->setHeight(contentAreaHeight + BODY_SHADOW_SIZE_PIXELS * 2.0f * dpmToPixelsH);
		Bounds2D shadowInnerBounds{
			BODY_SHADOW_SIZE_PIXELS * dpmToPixelsH/*xMin*/, BODY_SHADOW_SIZE_PIXELS * dpmToPixelsH/*yMin*/,
			BODY_SHADOW_SIZE_PIXELS * dpmToPixelsH + contentArea_bg->getWidth()/*xMax*/, BODY_SHADOW_SIZE_PIXELS * dpmToPixelsH + contentArea_bg->getHeight()/*yMax*/
		};
		bodyOutterShadow->setInnerBounds(shadowInnerBounds);
		contentArea_touchAreaContentBg->setHeight(contentAreaHeight);

		controlLbl->setY(contentAreaHeight * 0.5f);
		controlTouchArea->setY(controlLbl->getY());
		if (controlBg != nullptr) {
			controlBg->setY(controlTouchArea->getY());
		}
	} else {
		// No control.
		contentArea_textArea_lblText->setY((6.0f + 8.0f) * dpmToPixelsH);
		contentArea_textArea_lblText->setWrappingWidth(contentAreaWidth - (16.0f/*left_margin*/ + 16.0f/*right_margin*/) * dpmToPixelsW);
		contentArea_textArea_lblText->validateGlyphsAndMetricsIfNeeded();

		float contentAreaHeight =
			(6.0f + 8.0f) * dpmToPixelsH
			// This is the distance from the text pos_y to he baseline of the last line.
			+ contentArea_textArea_lblText->computeLineHeight() * (contentArea_textArea_lblText->getLinesCount() - 1)
			+ 30 * dpmToPixelsH;
		contentArea_bg->setHeight(contentAreaHeight);
		bodyOutterShadow->setHeight(contentAreaHeight + BODY_SHADOW_SIZE_PIXELS * 2.0f * dpmToPixelsH);
		Bounds2D shadowInnerBounds{
			BODY_SHADOW_SIZE_PIXELS * dpmToPixelsH/*xMin*/, BODY_SHADOW_SIZE_PIXELS * dpmToPixelsH/*yMin*/,
			BODY_SHADOW_SIZE_PIXELS * dpmToPixelsH + contentArea_bg->getWidth()/*xMax*/, BODY_SHADOW_SIZE_PIXELS * dpmToPixelsH + contentArea_bg->getHeight()/*yMax*/
		};
		bodyOutterShadow->setInnerBounds(shadowInnerBounds);
		contentArea_touchAreaContentBg->setHeight(contentAreaHeight);
	}
}

void SnackbarView::onSelectedStateChanged() {
	super::onSelectedStateChanged();

	if (!isActive()) {
		//void
	} else {
		//void
	}
}

SnackbarView::~SnackbarView() {
	//void
}
