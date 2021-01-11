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
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <rpg3D/config/sounds/SoundConfigsList.h>
#include <base/visual2D/util/VCC_SpriteButton.h>
#include <base/app/config/AppConfig.h>
#include <rpg3D/gw/playPhases/IGCPlayPhases.h>
#include <worldGame3D/gw/IGameWorld.h>

using namespace squareTactics_a_v1;

ArrayList<MenuItemConfig*>* MainInGameUIView::viewItemConfigs = base::StaticsInit::AddCbGeneral<ArrayList<MenuItemConfig*>*>(1, []() {
	viewItemConfigs = (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(super::viewItemConfigs)
		->appendDirect_chain(new_viewItemConfig_hp());
	});

MenuItemConfig* MainInGameUIView::new_viewItemConfig_hp() {
	return new MenuItemConfig_ComposedVisual("hp", (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("props", (new VisualPropertiesConfig())
			->put("width", 1.0f, "parent.size.width")
			->put("height", 1.0f, "parent.size.height")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("layer", "parent.layer", VCC_Group::New(), (new VisualPropertiesConfig())
			->put("x", 0.0f)
			->put("y", 0.0f)
			->putAnimating("alpha", (new MenuVisualAnimationProperty())
				->putState("_selectedFocusEntity:true_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
			->putAnimating("_visibleFloat", (new MenuVisualAnimationProperty())
				->putState("_selectedFocusEntity:true_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("hpBg", "layer", VCC_Sprite::New(
			"visual2D/taUtilLinear.atlas",
			"bg/simple_white_FFFFFF"
		), (new VisualPropertiesConfig())
			->put("width", 96.0f, "_dpm.width")
			->put("height", 32.0f, "_dpm.height")
			->put("anchorX", 0.0f)
			->put("anchorY", 1.0f)
			->put("x", 0.5f, "props.width",
				  VisualValueConfig::OPERATION_ADD,
				   -48.0f, "_dpm.width"
			)
			->put("y",
				1.0f, "props.height",
				VisualValueConfig::OPERATION_ADD,
				-4.0f, "_dpm.height"
			)
			->put("alpha", 0.2f)
			->put("tintColor", Color("#000000FF"))
		))
		->appendDirect_chain(new MenuItemConfig_Visual("hpIcon", "layer", VCC_Sprite::New(
			"visual2D/taUtilLinear.atlas",
			"bg/simple_white_FFFFFF"
		), (new VisualPropertiesConfig())
			->put("width", 14.0f, "_dpm.width")
			->put("height", 14.0f, "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x", 0.5f, "props.width",
				  VisualValueConfig::OPERATION_ADD,
				-48.0f + 22.0f, "_dpm.width"
			)
			->put("y",
				1.0f, "props.height",
				VisualValueConfig::OPERATION_ADD,
				-4.0f - 16.0f, "_dpm.height"
			)
			->put("alpha", 0.6f)
			->put("tintColor", Color("#FF2222FF"))
			->put("rotationD", 45.0f)
		))
		->appendDirect_chain(new MenuItemConfig_Visual("hpLbl", "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Medium-AaNSS-16-_dpFont_"), "##/##"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 16, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.0f)
			->put("anchorY", 0.5f)
			->put("x", 0.5f, "props.width",
				  VisualValueConfig::OPERATION_ADD,
				-48.0f + 42.0f, "_dpm.width"
			)
			->put("y",
				1.0f, "props.height",
				VisualValueConfig::OPERATION_ADD,
				-4.0f - 16.0f, "_dpm.height"
			)
			->put("alpha", 0.6f)
			->put("tintColor", Color("#FF2222FF"))
		))

		->appendDirect_chain(new MenuItemConfig_Visual("bgEndPlayPhase", "layer", VCC_Sprite::New(
			"visual2D/taUtilLinear.atlas",
			"bg/simple_white_FFFFFF"
		), (new VisualPropertiesConfig())
			->put("width", 48.0f, "_dpm.width")
			->put("height", 48.0f, "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x", 0.5f, "props.width")
			->put("y",
				1.0f, "props.height",
				VisualValueConfig::OPERATION_ADD,
				(-4.0f - 32.0f) - 10.0f - 24.0f, "_dpm.height"
			)
			->put("alpha", 0.8f)
			->put("tintColor", Color("#FFFFFFFF"))
		))
		->appendDirect_chain(new MenuItemConfig_Visual("btnEndPlayPhase", "layer", VCC_SpriteButton::New(
			"visual2D/taGeneral.atlas", "sh/squareTactics/mainUI/iconEndPlayPhase",
			&rpg3D::SoundConfigsList::UserInterface_Btn_Down->managedPath->path, 0.5f,
			nullptr, 0.0f,
			nullptr, 0.0f
		), (new VisualPropertiesConfig())
			->put("width", 24.0f, "_dpm.width")
			->put("height", 24.0f, "_dpm.height")
			->put("touchWidthOverride", std::make_shared<float>(48.0f), "_dpm.width")
			->put("touchHeightOverride", std::make_shared<float>(48.0f), "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x", 0.5f, "props.width")
			->put("y",
				1.0f, "props.height",
				VisualValueConfig::OPERATION_ADD,
				(-4.0f - 32.0f) - 10.0f - 24.0f, "_dpm.height"
			)
			->put("alpha", 1.0f)
			->put("tintColor", &AppConfig::APP_MAIN_COLOR)
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
	
	hpLbl = getItemMustExistAs<base::BitmapLabel*>("hp.hpLbl");
	hpLbl->setTextDirect(std::to_string(currentPlayerEntityHp) + "/" + std::to_string(maxPlayerEntityHp));

	(btnEndPlayPhase = getItemOptionalAs<IInteractiveDrawable2D*>("hp.btnEndPlayPhase"))->addTouchListener(&touchListener);
	bgEndPlayPhase = getItemOptionalAs<IInteractiveDrawable2D*>("hp.bgEndPlayPhase");
}

void MainInGameUIView::visualLoop(float deltaS, int deltaMS) {
	super::visualLoop(deltaS, deltaMS);

	bgEndPlayPhase->setRotationD(bgEndPlayPhase->getRotationD() + 120.0f * deltaS);
}

bool MainInGameUIView::onCursorDown(
	IInteractiveDrawable2D* target,
	int cursorIndex, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos, Vector2& localPos)
{
	if (!isActive()) {
		return false;
	}

	if (dynamic_cast<void*>(target) == dynamic_cast<void*>(btnEndPlayPhase)) {
		// Show the side panel.
		rpg3D::playPhases::IGCPlayPhases* gcPlayPhases = gw->getComponentAs<rpg3D::playPhases::IGCPlayPhases*>(false/*mustExist*/);
		// The gcPlayPhases is optional to avoid bugs that can occur on zones changes when the gcPlayPhases is disposed/created.
		if (gcPlayPhases != nullptr) {
			gcPlayPhases->onUserInput_endCurrentPlayPhase();
		}
	}

	else {
		//"Unhandled touch target.";

		throw LogicException(LOC);
	}

	return false;
}

void MainInGameUIView::onSelectedStateChanged() {
	super::onSelectedStateChanged();

	if(isActive()) {
		hpLbl->setTextDirect(std::to_string(currentPlayerEntityHp) + "/" + std::to_string(maxPlayerEntityHp));
	}
}

void MainInGameUIView::onSelectedFocusEntityChange_pre() {
	super::onSelectedFocusEntityChange_pre();

	if(getSelectedFocusEntity() != nullptr) {
		// Remove listeners and data.
		playerEntityListenersList.clear();
		livingEntity = nullptr;

		currentPlayerEntityHp = 0;
		// Don't update bacause on player entity death the lbl will become "0" before the view hide finishes.
		//maxPlayerEntityHp = 0;

		hpLbl->setTextDirect(std::to_string(currentPlayerEntityHp) + "/" + std::to_string(maxPlayerEntityHp));
	}
}

void MainInGameUIView::onSelectedFocusEntityChanged(IWorldEntity* selectedFocusEntity, ArrayList<std::shared_ptr<IEventListener>>& selectedFocusEntity_autoListenersList) {
	super::onSelectedFocusEntityChanged(selectedFocusEntity, selectedFocusEntity_autoListenersList);

	if(selectedFocusEntity != nullptr) {
		// Add listener for current_hp change.
		selectedFocusEntity->addEventListener(
			rpg3D::LivingEntityModule::EVENT_Common_currentHP_Change->type,
			std::bind(&MainInGameUIView::onPlayerEntityEvent, this, std::placeholders::_1, std::placeholders::_2),
			playerEntityListenersList
		);

		// Get the current and max entity hp;
		livingEntity = selectedFocusEntity->getComponentAs<rpg3D::ILivingEntityModule*>(true/*mustExist*/);
		currentPlayerEntityHp = livingEntity->getCurrentHP();
		maxPlayerEntityHp = livingEntity->getT()->maxHP;

		if(isActive()) {
			hpLbl->setTextDirect(std::to_string(currentPlayerEntityHp) + "/" + std::to_string(maxPlayerEntityHp));
		}
	}
}

void MainInGameUIView::onPlayerEntityEvent(IEventListener& eventListener, base::EntityEvent& event) {
	if (event.type == rpg3D::LivingEntityModule::EVENT_Common_currentHP_Change->type) {
		currentPlayerEntityHp = livingEntity->getCurrentHP();

		if(isActive()) {
			hpLbl->setTextDirect(std::to_string(currentPlayerEntityHp) + "/" + std::to_string(maxPlayerEntityHp));
		}
	}else {
		throw LogicException(LOC);
	}
}

MainInGameUIView::~MainInGameUIView() {
    //void
}
