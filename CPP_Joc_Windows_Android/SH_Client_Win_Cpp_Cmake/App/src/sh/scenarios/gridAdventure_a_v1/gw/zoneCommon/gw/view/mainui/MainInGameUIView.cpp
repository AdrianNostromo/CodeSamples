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
#include <rpg3D/gw/entity/module/LivingEntityModule.h>
#include <rpg3D/gw/entity/module/inventory/IInventoryModule.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>

using namespace gridAdventure_a_v1;

ArrayList<MenuItemConfig*>* MainInGameUIView::viewItemConfigs = base::StaticsInit::AddCbGeneral<ArrayList<MenuItemConfig*>*>(1, []() {
	viewItemConfigs = (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(super::viewItemConfigs)
		->appendDirect_chain(new_viewItemConfig_topInfo());
});

MenuItemConfig* MainInGameUIView::new_viewItemConfig_topInfo() {
	return new MenuItemConfig_ComposedVisual("topInfo", (new ArrayList<MenuItemConfig*>())
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
				   (-2.0f - 96.0f), "_dpm.width"
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
			"visual2D/taGeneral.atlas",
			"sh/ui/icons/iconHearth_alignedDown"
		), (new VisualPropertiesConfig())
			->put("width", 14.0f, "_dpm.width")
			->put("height", 14.0f, "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x", 0.5f, "props.width",
				VisualValueConfig::OPERATION_ADD,
				(-2.0f - 96.0f) + 16.0f, "_dpm.width"
			)
			->put("y",
				1.0f, "props.height",
				VisualValueConfig::OPERATION_ADD,
				-4.0f - 16.0f, "_dpm.height"
			)
			->put("alpha", AppStyles::ICON_BLACK_ALPHA)
			->put("tintColor", Color("#CE4B17FF"))
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
				(-2.0f - 96.0f) + 16.0f + 16.0f + 16.0f, "_dpm.width"
			)
			->put("y",
				1.0f, "props.height",
				VisualValueConfig::OPERATION_ADD,
				-4.0f - 16.0f, "_dpm.height"
			)
			->put("alpha", AppStyles::ICON_BLACK_ALPHA)
			->put("tintColor", Color("#CE4B17FF"))
		))

		->appendDirect_chain(new MenuItemConfig_Visual("goldBg", "layer", VCC_Sprite::New(
			"visual2D/taUtilLinear.atlas",
			"bg/simple_white_FFFFFF"
		), (new VisualPropertiesConfig())
			->put("width", 96.0f, "_dpm.width")
			->put("height", 32.0f, "_dpm.height")
			->put("anchorX", 0.0f)
			->put("anchorY", 1.0f)
			->put("x", 0.5f, "props.width",
				VisualValueConfig::OPERATION_ADD,
				(2.0f), "_dpm.width"
			)
			->put("y",
				1.0f, "props.height",
				VisualValueConfig::OPERATION_ADD,
				-4.0f, "_dpm.height"
			)
			->put("alpha", 0.2f)
			->put("tintColor", Color("#000000FF"))
		))
		->appendDirect_chain(new MenuItemConfig_Visual("goldIcon", "layer", VCC_Sprite::New(
			"visual2D/taGeneral.atlas",
			"sh/ui/icons/iconGold"
		), (new VisualPropertiesConfig())
			->put("width", 14.0f, "_dpm.width")
			->put("height", 14.0f, "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x", 0.5f, "props.width",
				VisualValueConfig::OPERATION_ADD,
				(2.0f) + 16.0f, "_dpm.width"
			)
			->put("y",
				1.0f, "props.height",
				VisualValueConfig::OPERATION_ADD,
				-4.0f - 16.0f, "_dpm.height"
			)
			->put("alpha", AppStyles::ICON_BLACK_ALPHA)
			->put("tintColor", Color("#EFD82EFF"))
		))
		->appendDirect_chain(new MenuItemConfig_Visual("goldLbl", "layer", VCC_BitmapLabel::New(
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
				(2.0f) + 16.0f + 16.0f + 16.0f, "_dpm.width"
			)
			->put("y",
				1.0f, "props.height",
				VisualValueConfig::OPERATION_ADD,
				-4.0f - 16.0f, "_dpm.height"
			)
			->put("alpha", AppStyles::ICON_BLACK_ALPHA)
			->put("tintColor", Color("#EFD82EFF"))
		))
	);
}

MainInGameUIView::MainInGameUIView(IApp* app, base::IGameWorld* gw)
    : super(app, viewItemConfigs, viewAnimationDurations, gw)
{
    //void
}

void MainInGameUIView::createVisuals(Map1D<std::string, void*>* createConfigMap) {
	super::createVisuals(createConfigMap);

	hpLbl = getItemMustExistAs<base::BitmapLabel*>("topInfo.hpLbl");
	hpLbl->setTextDirect(std::to_string(currentPlayerEntityHp) + "/" + std::to_string(maxPlayerEntityHp));
	
	goldLbl = getItemMustExistAs<base::BitmapLabel*>("topInfo.goldLbl");
	goldLbl->setTextDirect(std::to_string(goldAmount));
}

void MainInGameUIView::onSelectedStateChanged() {
	super::onSelectedStateChanged();

	if(isActive()) {
		hpLbl->setTextDirect(std::to_string(currentPlayerEntityHp) + "/" + std::to_string(maxPlayerEntityHp));

		goldLbl->setTextDirect(std::to_string(goldAmount));
	}
}

void MainInGameUIView::onSelectedFocusEntityChange_pre() {
	super::onSelectedFocusEntityChange_pre();

	if(getSelectedFocusEntity() != nullptr) {
		// Remove listeners and data.
		playerEntityListenersList.clear();
		livingEntity = nullptr;
		inventory = nullptr;

		currentPlayerEntityHp = 0;
		
		goldAmount = 0;
		// Don't update bacause on player entity death the lbl will become "0" before the view hide finishes.
		//maxPlayerEntityHp = 0;

		hpLbl->setTextDirect(std::to_string(currentPlayerEntityHp) + "/" + std::to_string(maxPlayerEntityHp));

		goldLbl->setTextDirect(std::to_string(goldAmount));
	}
}

void MainInGameUIView::onSelectedFocusEntityChanged(IWorldEntity* selectedFocusEntity, ArrayList<std::shared_ptr<IEventListener>>& selectedFocusEntity_autoListenersList) {
	super::onSelectedFocusEntityChanged(selectedFocusEntity, selectedFocusEntity_autoListenersList);

	if(selectedFocusEntity != nullptr) {
		// Add listener for current_hp change.
		selectedFocusEntity->addEventListener(
			rpg3D::LivingEntityModule::EVENT_Common_currentHP_Change->type, rpg3D::IInventoryModule::EVENT_inventory_goldAmountChanged->type,
			std::bind(&MainInGameUIView::onPlayerEntityEvent, this, std::placeholders::_1, std::placeholders::_2),
			playerEntityListenersList
		);

		// Get the current and max entity hp;
		livingEntity = selectedFocusEntity->getComponentAs<rpg3D::ILivingEntityModule*>(true/*mustExist*/);
		inventory = selectedFocusEntity->getComponentAs<rpg3D::IInventoryModule*>(true/*mustExist*/);

		currentPlayerEntityHp = livingEntity->getCurrentHP();
		maxPlayerEntityHp = livingEntity->getT()->maxHP;
		
		goldAmount = inventory->getGoldAmount();

		if(isActive()) {
			hpLbl->setTextDirect(std::to_string(currentPlayerEntityHp) + "/" + std::to_string(maxPlayerEntityHp));

			goldLbl->setTextDirect(std::to_string(goldAmount));
		}
	}
}

void MainInGameUIView::onPlayerEntityEvent(IEventListener& eventListener, base::EntityEvent& event) {
	if (event.type == rpg3D::LivingEntityModule::EVENT_Common_currentHP_Change->type) {
		currentPlayerEntityHp = livingEntity->getCurrentHP();

		if(isActive()) {
			hpLbl->setTextDirect(std::to_string(currentPlayerEntityHp) + "/" + std::to_string(maxPlayerEntityHp));
		}
	} else if (event.type == rpg3D::IInventoryModule::EVENT_inventory_goldAmountChanged->type) {
		goldAmount = inventory->getGoldAmount();

		if (isActive()) {
			goldLbl->setTextDirect(std::to_string(goldAmount));
		}
	} else {
		throw LogicException(LOC);
	}
}

MainInGameUIView::~MainInGameUIView() {
    //void
}
