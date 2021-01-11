#include "MainInGameUIView.h"
#include <base/util/AppStyles.h>
#include <base/menu/util/MenuItemConfig_Visual.h>
#include <base/menu/util/MenuItemConfig_Data.h>
#include <base/visual2D/util/VCC_Group.h>
#include <base/visual2D/util/VCC_Sprite.h>
#include <base/visual2D/util/VCC_TouchArea.h>
#include <base/exceptions/LogicException.h>
#include <functional>
#include <base/visual2D/util/VCC_BitmapLabel.h>
#include <base/menu/util/MenuItemConfig_ComposedVisual.h>
#include <base/statics/StaticsInit.h>
#include <graphics/visual2d/drawable/BitmapLabel.h>
#include <graphics/visual2d/drawable/DrawableNode2D.h>

using namespace towerDefense_a_v1;
using namespace towerDefense_a_v1::MazeTD;

ArrayList<MenuItemConfig*>* MainInGameUIView::viewItemConfigs = base::StaticsInit::AddCbGeneral<ArrayList<MenuItemConfig*>*>(1, []() {
	viewItemConfigs = (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(super::viewItemConfigs)
		->appendDirect_chain(new_viewItemConfig_top());
});

MenuItemConfig* MainInGameUIView::new_viewItemConfig_top() {
	return new MenuItemConfig_ComposedVisual("top", (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("props", (new VisualPropertiesConfig())
			->put("width", 1.0f, "parent.size.width")
			->put("height", 1.0f, "parent.size.height")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("layer", "parent.layer", VCC_Group::New(), (new VisualPropertiesConfig())
			->put("x", 0.0f)
			->put("y", 1.0f, "props.height")
			->putAnimating("alpha", (new MenuVisualAnimationProperty())
				->putState("_selectedFocusEntity:true_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
			->putAnimating("_visibleFloat", (new MenuVisualAnimationProperty())
				->putState("_selectedFocusEntity:true_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("livesBg", "layer", VCC_Sprite::New(
			"visual2D/taUtilLinear.atlas",
			"bg/simple_white_FFFFFF"
		), (new VisualPropertiesConfig())
			->put("width", 48.0f, "_dpm.width")
			->put("height", 48.0f, "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x", 0.5f, "props.width",
				VisualValueConfig::OPERATION_ADD,
				-2.0f - 24.0f, "_dpm.width"
			)
			->put("y", -4.0f - 24.0f, "_dpm.height")
			->put("alpha", AppStyles::BG_DARK_ALPHA)
			->put("tintColor", Color("#000000FF"))
		))
		->appendDirect_chain(new MenuItemConfig_Visual("livesIcon", "layer", VCC_Sprite::New(
			"visual2D/taGeneral.atlas",
			"sh/ui/icons/iconHearth_alignedDown"
		), (new VisualPropertiesConfig())
			->put("width", 14.0f, "_dpm.width")
			->put("height", 14.0f, "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.0f)
			->put("x", 0.5f, "props.width",
				VisualValueConfig::OPERATION_ADD,
				-2.0f - 24.0f, "_dpm.width"
			)
			->put("y", -4.0f - 24.0f + 3.0f, "_dpm.height")
			->put("alpha", AppStyles::ICON_BLACK_ALPHA)
			->put("tintColor", Color("#CE4B17FF"))
		))
		->appendDirect_chain(new MenuItemConfig_Visual("livesLbl", "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Medium-AaNSS-14-_dpFont_"), "##"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.5f)
			->put("anchorY", 1.0f)
			->put("x", 0.5f, "props.width",
				VisualValueConfig::OPERATION_ADD,
				-2.0f - 24.0f, "_dpm.width"
			)
			->put("y", -4.0f - 24.0f - 3.0f, "_dpm.height")
			->put("alpha", AppStyles::LIGHT_SECONDARY_TEXT_ALPHA)
			->put("tintColor", Color("#FFFFFFFF"))
		))

		->appendDirect_chain(new MenuItemConfig_Visual("goldBg", "layer", VCC_Sprite::New(
			"visual2D/taUtilLinear.atlas",
			"bg/simple_white_FFFFFF"
		), (new VisualPropertiesConfig())
			->put("width", 48.0f, "_dpm.width")
			->put("height", 48.0f, "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x", 0.5f, "props.width",
				VisualValueConfig::OPERATION_ADD,
				2.0f + 24.0f, "_dpm.width"
			)
			->put("y", -4.0f - 24.0f, "_dpm.height")
			->put("alpha", AppStyles::BG_DARK_ALPHA)
			->put("tintColor", Color("#000000FF"))
		))
		->appendDirect_chain(new MenuItemConfig_Visual("goldIcon", "layer", VCC_Sprite::New(
			"visual2D/taGeneral.atlas",
			"sh/ui/icons/iconGold"
		), (new VisualPropertiesConfig())
			->put("width", 14.0f, "_dpm.width")
			->put("height", 14.0f, "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.0f)
			->put("x", 0.5f, "props.width",
				VisualValueConfig::OPERATION_ADD,
				2.0f + 24.0f, "_dpm.width"
			)
			->put("y", -4.0f - 24.0f + 3.0f, "_dpm.height")
			->put("alpha", AppStyles::ICON_BLACK_ALPHA)
			->put("tintColor", Color("#EFD82EFF"))
		))
		->appendDirect_chain(new MenuItemConfig_Visual("goldLbl", "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Medium-AaNSS-14-_dpFont_"), "###"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.5f)
			->put("anchorY", 1.0f)
			->put("x", 0.5f, "props.width",
				VisualValueConfig::OPERATION_ADD,
				2.0f + 24.0f, "_dpm.width"
			)
			->put("y", -4.0f - 24.0f - 3.0f, "_dpm.height")
			->put("alpha", AppStyles::LIGHT_SECONDARY_TEXT_ALPHA)
			->put("tintColor", Color("#FFFFFFFF"))
		))

		->appendDirect_chain(new MenuItemConfig_Visual("layerStartWave", "layer", VCC_Group::New(), (new VisualPropertiesConfig())
			->put("x", 0.5f, "props.width")
			->put("y", -4.0f - 48.0f - 14.0f - 24.0f, "_dpm.height")
			->putAnimating("scaleX", (new MenuVisualAnimationProperty())
				->putState("_isWaveStarterBtnActive:true_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
			->putAnimating("scaleY", (new MenuVisualAnimationProperty())
				->putState("_isWaveStarterBtnActive:true_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
			->putAnimating("alpha", (new MenuVisualAnimationProperty())
				->putState("_isWaveStarterBtnActive:true_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
			->putAnimating("_visibleFloat", (new MenuVisualAnimationProperty())
				->putState("_isWaveStarterBtnActive:true_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
		))
		->appendDirect_chain(new MenuItemConfig_Visual("startWaveBg", "layerStartWave", VCC_Sprite::New(
			"visual2D/taUtilLinear.atlas",
			"bg/simple_white_FFFFFF"
		), (new VisualPropertiesConfig())
			->put("width", 48.0f, "_dpm.width")
			->put("height", 48.0f, "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x", 0.0f)
			->put("y", 0.0f)
			->put("alpha", AppStyles::BG_LIGHT_ALPHA)
			->put("tintColor", Color("#FFFFFFFF"))
		))
		->appendDirect_chain(new MenuItemConfig_Visual("startWaveIcon", "layerStartWave", VCC_Sprite::New(
			"visual2D/taGeneral.atlas",
			"sh/towerDefense/ui/gw/mainUI/iconStartWave"
		), (new VisualPropertiesConfig())
			->put("width", 24.0f, "_dpm.width")
			->put("height", 24.0f, "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x", 0.0f)
			->put("y", 0.0f)
			->put("alpha", AppStyles::ICON_BLACK_ALPHA)
			->put("tintColor", Color("#4285F4FF"))
		))
		->appendDirect_chain(new MenuItemConfig_Visual("startWaveTA", "layerStartWave", VCC_TouchArea::New(), (new VisualPropertiesConfig())
			->put("width", 58.0f, "_dpm.width")
			->put("height", 58.0f, "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x", 0.0f)
			->put("y", 0.0f)
		))
	);
}

MainInGameUIView::MainInGameUIView(IApp* app, base::IGameWorld* gw)
    : super(app, viewItemConfigs, viewAnimationDurations, gw)
{
	touchListener.cb_onCursorDown = std::bind(&MainInGameUIView::onCursorDown, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
}

void MainInGameUIView::createVisuals(Map1D<std::string, void*>* createConfigMap) {
	super::createVisuals(createConfigMap);

	goldLbl = getItemMustExistAs<base::BitmapLabel*>("top.goldLbl");
	livesLbl = getItemMustExistAs<base::BitmapLabel*>("top.livesLbl");
	layerStartWave = getItemMustExistAs<DrawableNode2D*>("top.layerStartWave");
	startWaveBg = getItemMustExistAs<DrawableNode2D*>("top.startWaveBg");

	(startWaveTA = getItemOptionalAs<TouchArea2D*>("top.startWaveTA"))->addTouchListener(&touchListener);

	goldLbl->setTextDirect(std::to_string(currentGold));
	livesLbl->setTextDirect(std::to_string(currentLives));

	startWaveBg->setRotationD(45.0f);
}

void MainInGameUIView::setCb_onStartNextWaveRequest(std::function<void(
	IMainInGameUIView * target)> cb_onStartNextWaveRequest)
{
	this->cb_onStartNextWaveRequest = cb_onStartNextWaveRequest;
}

void MainInGameUIView::setCurrentGold(int currentGold) {
	if (this->currentGold == currentGold) {
		return;
	}
	this->currentGold = currentGold;

	goldLbl->setTextDirect(std::to_string(currentGold));
}

void MainInGameUIView::setCurrentLives(int currentLives) {
	if (this->currentLives == currentLives) {
		return;
	}
	this->currentLives = currentLives;
	
	livesLbl->setTextDirect(std::to_string(currentLives));
}

void MainInGameUIView::setIsWaveStarterBtnActive(bool isWaveStarterBtnActive) {
	if (this->isWaveStarterBtnActive == isWaveStarterBtnActive) {
		return;
	}
	this->isWaveStarterBtnActive = isWaveStarterBtnActive;

	if (this->isWaveStarterBtnActive) {
		startWaveBg->setRotationD(45.0f);
	}

	updateShowStateIfChanged(true, nullptr);
}

bool MainInGameUIView::onCursorDown(
	IInteractiveDrawable2D* target,
	int cursorIndex, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos, Vector2& localPos)
{
	if (!isActive()) {
		return false;
	}

	if (dynamic_cast<void*>(target) == dynamic_cast<void*>(startWaveTA)) {
		// Show the side panel.

		if (cb_onStartNextWaveRequest) {
			cb_onStartNextWaveRequest(this);
		}
	}

	else {
		//"Unhandled touch target.";

		throw LogicException(LOC);
	}

	return false;
}

void MainInGameUIView::visualLoop(float deltaS, int deltaMS) {
	super::visualLoop(deltaS, deltaMS);

	if (layerStartWave->getIsVisible()) {
		startWaveBg->setRotationD(startWaveBg->getRotationD() - 180.0f * deltaS);
	}
}

std::string MainInGameUIView::computeStateFlags(std::string baseState) {
	std::string state = super::computeStateFlags(baseState);

	if (!isWaveStarterBtnActive) {
		state += "_isWaveStarterBtnActive:false_";
	} else {
		state += "_isWaveStarterBtnActive:true_";
	}

	return state;
}

MainInGameUIView::~MainInGameUIView() {
    //void
}
