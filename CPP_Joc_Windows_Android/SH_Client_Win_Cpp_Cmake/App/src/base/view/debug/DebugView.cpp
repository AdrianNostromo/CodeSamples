#include <base/util/AppStyles.h>
#include <base/menu/util/MenuItemConfig_Data.h>
#include <base/menu/util/MenuItemConfig_Visual.h>
#include <base/menu/util/VisualPropertiesConfig.h>
#include <base/visual2D/util/VisualCreateConfig.h>
#include <base/visual2D/util/VCC_Group.h>
#include <base/visual2D/util/VCC_Sprite.h>
#include <base/visual2D/util/VCC_Sprite9Slice2D.h>
#include <base/visual2D/util/VCC_BitmapLabel.h>
#include <base/visual2D/util/VCC_TouchArea.h>
#include "DebugView.h"
#include <base/statics/StaticsInit.h>
#include <graphics/visual2d/drawable/ISprite9Slice2D.h>
#include <base/math/Bounds2D.h>
#include <base/math/Slice9SizePercents.h>
#include <graphics/visual2d/drawable/ISprite2D.h>

using namespace base;

ArrayList<MenuItemConfig*>* DebugView::viewItemConfigs = base::StaticsInit::AddCbGeneral<ArrayList<MenuItemConfig*>*>(1, []() {
	viewItemConfigs = (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("size", (new VisualPropertiesConfig())
			->put("width", 1.0f, "_appSize.width")
			->put("height", 1.0f, "_appSize.height")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("layer", "_root.visualsHolder", VCC_Group::New(), (new VisualPropertiesConfig())
			->putAnimating("alpha", (new MenuVisualAnimationProperty())
				->putState("_in_", 1.0f, EaseConfig::ACCELERATION)
				->putState("_out_", 0.0f, EaseConfig::ACCELERATION)
			)
		))

		->appendDirect_chain(new_viewItemConfig_hardcodedEntries())
		->appendDirect_chain(new_viewItemConfig_bodyDynamics())

		->appendDirect_chain(new MenuItemConfig_Visual("secretDebugTa", "layer", VCC_TouchArea::New(), (new VisualPropertiesConfig())
			->put("width", 48.0f, "_dpm.width")
			->put("height", 48.0f, "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			//->put("x", 4.0f + 24.0f, "_dpm.width")
			->put("x", 4.0f + 24.0f, "_dpm.width")
			->put("y", 4.0f + 24.0f, "_dpm.height")
		))
		->appendDirect_chain(new MenuItemConfig_Visual("debugTa", "layer", VCC_TouchArea::New(), (new VisualPropertiesConfig())
			->put("width", 48.0f, "_dpm.width")
			->put("height", 48.0f, "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			//->put("x", 4.0f + 24.0f, "_dpm.width")
			->put("x", 4.0f + 24.0f, "_dpm.width")
			->put("y", 4.0f + 24.0f, "_dpm.height")
		))
		->appendDirect_chain(new MenuItemConfig_Visual("debugBg", "layer", VCC_Sprite::New(
			"visual2D/taUtilLinear.atlas",
			"bg/simple_white_FFFFFF"
		), (new VisualPropertiesConfig())
			->put("width", 48.0f, "_dpm.width")
			->put("height", 48.0f, "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			//->put("x", 24.0f + 4.0f, "_dpm.width")
			->put("x", 4.0f + 24.0f, "_dpm.width")
			->put("y", 4.0f + 24.0f, "_dpm.height")
			//->put("alpha", 0.2f)// programatic.
			->put("tintColor", Color("#000000FF"))
		))
		->appendDirect_chain(new MenuItemConfig_Visual("debugIcon", "layer", VCC_Sprite::New(
			"visual2D/taGeneral.atlas", "sh/debugView/iconDebug"
		), (new VisualPropertiesConfig())
			->put("width", 24.0f, "_dpm.width")
			->put("height", 24.0f, "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			//->put("x", 4.0f + 24.0f, "_dpm.width")
			->put("x", 4.0f + 24.0f, "_dpm.width")
			->put("y", 4.0f + 24.0f, "_dpm.height")
			->put("tintColor", Color("#00FF00FF"))
			//->put("alpha", 0.6f)// programatic.
		));
});

ArrayList<StateChangeDurations*>* DebugView::viewAnimationDurations = base::StaticsInit::AddCbGeneral<ArrayList<StateChangeDurations*>*>(1, []() {
	viewAnimationDurations = (new ArrayList<StateChangeDurations*>())
		->appendDirect_chain(new StateChangeDurations("_out_", "_in_", AppStyles::ANIMATION_DURATION_S_DEFAULT))
		->appendDirect_chain(new StateChangeDurations("_in_", "_out_", AppStyles::ANIMATION_DURATION_S_DEFAULT))
		->appendDirect_chain(new StateChangeDurations("_in_", "_in_", AppStyles::ANIMATION_DURATION_S_DEFAULT));
	});

DebugView::DebugView(IApp* app)
	: super(app, viewItemConfigs, viewAnimationDurations)
{
	touchListener.cb_onCursorDown = std::bind(&DebugView::onCursorDown, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
}

void DebugView::createContent() {
	super::createContent();

	(debugTa = getItemOptionalAs<TouchArea2D*>("debugTa"))
		->setInputInterractiveButtonsBitMap(base::Touch::ButtonCode::LeftOrTouch->bitGroup | base::Touch::ButtonCode::Right->bitGroup)
		->addTouchListener(&touchListener);
	debugBg = getItemOptionalAs<ISprite2D*>("debugBg");
	debugIcon = getItemOptionalAs<ISprite2D*>("debugIcon");

	(secretDebugTa = getItemOptionalAs<TouchArea2D*>("secretDebugTa"))->addTouchListener(&touchListener);
	secretDebugTa->setIsTouchFallthroughEnabled(true);

	// Note. NDEBUG is guaranteed by thestandard and CMake uses it.
	#ifndef NDEBUG
		// Debug mode.
		debugTa->setIsVisible(true);
		debugBg->setIsVisible(true);
		debugBg->setAlpha(0.2f);
		debugIcon->setIsVisible(true);
		debugIcon->setAlpha(0.6f);

		secretDebugTa->setIsVisible(false);

		isSecretMode = false;
		// In debug mode, there is no initial fade out until the debug btn is pressed.
		btnFadeOutStartDelayS = -1.0f;
	#else
		// Release mode. Secret hidden btn.
		debugTa->setIsVisible(false);
		debugBg->setIsVisible(false);
		debugBg->setAlpha(0.2f);
		debugIcon->setIsVisible(false);
		debugIcon->setAlpha(0.6f);

		secretDebugTa->setIsVisible(true);

		isSecretMode = true;
		secretBtnPressesCounter = 0;
		lastSecretBtnPressDeltaS = 0.0f;
	#endif
}

bool DebugView::onCursorDown(
	IInteractiveDrawable2D* target,
	int cursorIndex, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos, Vector2& localPos)
{
	if (!isActive()) {
		return false;
	}

	if (dynamic_cast<void*>(target) == dynamic_cast<void*>(debugTa)) {
		// Show the side panel.
		gotoNextPage();

		if (getActivePageIndex() == -1) {
			debugBg->setAlpha(0.2f);
			debugIcon->setAlpha(0.6f);
			btnFadeOutStartDelayS = 4.0f;
		}
	}else if (dynamic_cast<void*>(target) == dynamic_cast<void*>(secretDebugTa)) {
		secretBtnPressesCounter++;
		lastSecretBtnPressDeltaS = 0.0f;
		if (secretBtnPressesCounter >= 5) {
			isSecretMode = false;

			debugTa->setIsVisible(true);
			debugBg->setIsVisible(true);
			debugBg->setAlpha(0.2f);
			debugIcon->setIsVisible(true);
			debugIcon->setAlpha(0.6f);

			secretDebugTa->setIsVisible(false);

			// On debug enabling, don't hide the btn until it is pressed.
			btnFadeOutStartDelayS = -1.0f;
		}
	}

	else {
		//"Unhandled touch target.";

		throw LogicException(LOC);
	}

	return false;
}

void DebugView::visualLoop(float deltaS, int deltaMS) {
	super::visualLoop(deltaS, deltaMS);

	if (isSecretMode) {
		lastSecretBtnPressDeltaS += deltaS;
		if (lastSecretBtnPressDeltaS >= 1.0f) {
			// The secret btn presses are not fast enough.
			secretBtnPressesCounter = 0;
		}
	} else {
		if (btnFadeOutStartDelayS > -1.0f) {
			btnFadeOutStartDelayS -= deltaS;

			if (btnFadeOutStartDelayS <= 0.0f) {
				float cp = Math::max(0.0f, Math::min(1.0f, btnFadeOutStartDelayS * -1.0f));
				float alphaMultiplier = 1.0f - cp;

				debugBg->setAlpha(0.2f * alphaMultiplier);
				debugIcon->setAlpha(0.6f * alphaMultiplier);
			}
		}
	}
}

std::string DebugView::computeStateFlags(std::string baseState) {
	std::string state = super::computeStateFlags(baseState);

	//state += "_SOME-FLAG_";

	return state;
}

DebugView::~DebugView() {
	//void
}
