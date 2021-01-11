#include "MIGUIVAT_Selector.h"
#include <base/menu/util/MenuItemConfig_ComposedVisual.h>
#include <base/menu/util/MenuItemConfig_Data.h>
#include "MIGUIV_DirectionalPad.h"
#include <base/menu/util/MenuItemConfig_Visual.h>
#include <base/visual2D/util/VCC_Touchable.h>
#include <base/visual2D/util/VCC_Group.h>
#include <base/visual2D/util/VCC_Sprite.h>
#include <base/visual2D/util/VCC_TouchArea.h>
#include <base/menu/util/MenuItemConfig_SmartItem.h>
#include <base/statics/StaticsInit.h>
#include <graphics/visual2d/drawable/Sprite2D.h>
#include <base/util/AppStyles.h>
#include <base/app/IAppAssets.h>
#include <base/assets/IAssetsManager.h>
#include <rpg3D/gw/entity/module/toolsHandler/IToolsHandlerModule.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/Tool.h>
#include <rpg3D/config/sounds/SoundConfigsList.h>
#include <base/ease/Ease.h>
#include <base/visual2D/util/VCC_BitmapLabel.h>
#include <graphics/visual2d/drawable/IBitmapLabel.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>

using namespace rpg3D;

MenuItemConfig* MIGUIVAT_Selector::new_viewItemConfig_activeTool_selector() {
	return new MenuItemConfig_ComposedVisual("selector", (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("props", (new VisualPropertiesConfig())
			//->put("width", 100.0f, "_dpm.width")
			//->put("height", 100.0f, "_dpm.height")
		))
		->appendDirect_chain(new MenuItemConfig_Visual("layer", "parent.layer", VCC_Group::New(), (new VisualPropertiesConfig())
			//void
		))

		->appendDirect_chain(new MenuItemConfig_Visual("taScreen", "layer", VCC_TouchArea::New(), (new VisualPropertiesConfig())
			->put("width", 1.0f, "parent.props.width")
			->put("height", 1.0f, "parent.props.height")
			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("isTouchFallthroughEnabled", true)
			->putAnimating("visible", (new MenuVisualAnimationProperty())
				->putState("_toolSelectorStatus:on_._toolSelectorMinimode:off_", true)
				->putState("_", false)
			)
		))

		->appendDirect_chain(new MenuItemConfig_SmartItem("slToolsSelect", "layer", "_in_", [](IAppMetrics* appMetrics, IAppAssets* appAssets) -> ComposedStiledVisual* { return new base::ScrollList2D(appMetrics, appAssets); }, (new VisualPropertiesConfig())
			->putAnimating("x", (new MenuVisualAnimationProperty())
				->putState("_toolSelectorMinimode:off_",
					1.0f, "parent.props.width",
					VisualValueConfig::OPERATION_ADD,
					-16.0f, "_dpm.width",
					EaseConfig::ACCELERATION
				)
				->putState("_toolSelectorMinimode:on_",
					1.0f, "parent.props.width",
					VisualValueConfig::OPERATION_ADD,
					-4.0f, "_dpm.width",
					EaseConfig::ACCELERATION
				)
			)
			->put("y", 72.0f + 16.0f, "_dpm.width")

			->put("anchorX", 1.0f)
			->put("anchorY", 0.0f)

			->put("entryActivateSoundConfig_id", &SoundConfigsList::UserInterface_Btn_Down->managedPath->path)
			->put("entryActivateSound_volume", 0.5f)

			->put("entriesAlignmentH", base::List2D::AlignmentH::Left)
			->put("entriesAlignmentV", base::List2D::AlignmentV::Bottom)

			->put("entriesSpacingX", 0.0f)
			->putAnimating("entriesSpacingY", (new MenuVisualAnimationProperty())
				->putState("_toolSelectorStatus:off_", 12.0f, EaseConfig::ACCELERATION)
				->putState("_toolSelectorMinimode:off_", 16.0f, EaseConfig::ACCELERATION)
				->putState("_toolSelectorMinimode:on_", 12.0f, EaseConfig::ACCELERATION)
			)

			->put("listPaddingTop", 0.0f, "_dpm.width")
			->put("listPaddingBottom", 0.0f, "_dpm.width")
			->put("listPaddingLeft", 0.0f, "_dpm.width")
			->put("listPaddingRight", 0.0f, "_dpm.width")

			//->put("rowsCount", 1)// Horizontal list.
			->put("columnsCount", 1)// Vertical list

			->put("invertListGrowAxisX", false)
			->put("invertListGrowAxisY", false)

			->put("invertEntriesDrawOrder", false)

			->put("autoListSizeToContent", true)
			//->put("autoListSizeMaxWidth", 56.0f, "_dpm.width")
			->put("autoListSizeMaxHeight", 
				1.0f, "parent.props.height",
				VisualValueConfig::OPERATION_ADD,
				-((72.0f + 16.0f/*bottom screen part*/) + (4.0f + 48.0f + 4.0f/*top screen part*/)), "_dpm.height"
			)
			//->put("manualListSizeWidth", 300.0f, "_dpm.width")
			//->put("manualListSizeHeight", 56.0f, "_dpm.height")

			->put("bgTintColor", Color("#000000FF"))
			->put("bgAlpha", 0.0f)

			->put("preScrollDeltaThreshold", 5.0f, "_dpm.width")
			->put("touchDeselectionScrollDeltaThreshold", 15.0f, "_dpm.width")

			->put("animatedMinScrollSpeedS", 50.0f, "_dpm.width")
			->put("animatedMaxScrollSpeedS", 600.0f, "_dpm.width")
			->put("animatedMaxScrollSpeedAtDistance", 100.0f, "_dpm.width")
			->put("animatedScrollEaseFunction", &Ease::easeOutCubic)
		))
	);
}

ArrayList<MenuItemConfig*>* MIGUIVAT_Selector::listItemConfig_tool = base::StaticsInit::AddCbGeneral<ArrayList<MenuItemConfig*>*>(1, []() {
	listItemConfig_tool = (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("size", (new VisualPropertiesConfig())
			->put("width", 36.0f, "_dpm.width")
			->put("height", 36.0f, "_dpm.width")
		))
		->appendDirect_chain(new MenuItemConfig_Data("data", (new VisualPropertiesConfig())
			->put("colorChipPosY_normal", 0.5f, "size.height")
			->put("colorChipPosY_active",
				1.0f, "_self.colorChipPosY_normal",
				VisualValueConfig::OPERATION_ADD,
				3.0f, "_dpm.width"
			)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("layer", nullptr, VCC_Group::New(), (new VisualPropertiesConfig())
			->putAnimating("alpha", (new MenuVisualAnimationProperty())
				->putState("_in_", 1.0f, EaseConfig::ACCELERATION)
				->putState("_out_", 0.0f, EaseConfig::ACCELERATION)
			)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("touchArea", "layer", VCC_TouchArea::New(), (new VisualPropertiesConfig())
			->put("width", 52.0f, "_dpm.width")
			->put("height", 52.0f, "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x", 0.5f, "size.width")
			->put("y", 0.5f, "size.height")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("bg", "layer", VCC_Sprite::New(
			"visual2D/taUtilLinear.atlas", "bg/simple_white_FFFFFF"
		), (new VisualPropertiesConfig())
			->put("width", 36.0f, "_dpm.width")
			->put("height", 36.0f, "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x", 0.5f, "size.width")
			->put("y", 0.5f, "size.height")
			->put("tintColor", Color("#FFFFFFFF"))
			->put("alpha", 0.8f)
			->putAnimating("rotationD", (new MenuVisualAnimationProperty())
				->putState("_active_", 45.0f, EaseConfig::DECELERATION)
				->putState("_", 0.0f, EaseConfig::DECELERATION)
			)
		))
		->appendDirect_chain(new MenuItemConfig_Visual("imgToolIcon", "layer", VCC_Sprite::New(), (new VisualPropertiesConfig())
			->put("width", 24.0f, "_dpm.width")
			->put("height", 24.0f, "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x", 0.5f, "size.width")
			->put("y", 0.5f, "size.height")
			->put("alpha", 1.0f)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("nameBg", "layer", VCC_Sprite::New(
			"visual2D/taUtilLinear.atlas", "bg/simple_white_FFFFFF"
		), (new VisualPropertiesConfig())
			->put("width", 50.0f, "_dpm.width")
			->putAnimating("height", (new MenuVisualAnimationProperty())
				->putState("_nameLabels:bothLines_", 26.0f, "_dpm.height", EaseConfig::LINEAR)
				->putState("_name:on_", 13.0f, "_dpm.height", EaseConfig::LINEAR)
				->putState("_", 0.0f, "_dpm.height", EaseConfig::LINEAR)
			)
			->put("anchorX", 0.5f)
			->put("anchorY", 0.0f)
			->put("x", 0.5f, "size.width")
			->put("y", 
				0.5f, "size.height",
				VisualValueConfig::OPERATION_ADD,
				-26.0f, "_dpm.height"
			)
			->put("tintColor", Color("#FFFFFFFF"))
			->putAnimating("alpha", (new MenuVisualAnimationProperty())
				->putState("_active_._name:on_", 0.8f, EaseConfig::ACCELERATION)
				->putState("_", 0.0f, EaseConfig::ACCELERATION)
			)
			->putAnimating("_visibleFloat", (new MenuVisualAnimationProperty())
				->putState("_active_._name:on_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
		))
		->appendDirect_chain(new MenuItemConfig_Visual("dlblNameA", "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Regular-AaNSS-14-_dpFont_"), "NameA"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Regular)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.5f)
			->put("anchorY", 1.0f)
			->put("x", 0.5f, "size.width")
			->put("y",
				0.5f, "size.height",
				VisualValueConfig::OPERATION_ADD,
				-1.0f, "_dpm.height"
			)
			->put("tintColor", Color("#000000FF"))
			->putAnimating("alpha", (new MenuVisualAnimationProperty())
				->putState("_active_._nameLabels:bothLines_", AppStyles::ICON_DARK_ALPHA, EaseConfig::ACCELERATION)
				->putState("_", 0.0f, EaseConfig::ACCELERATION)
			)
			->putAnimating("_visibleFloat", (new MenuVisualAnimationProperty())
				->putState("_active_._nameLabels:bothLines_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
		))
		->appendDirect_chain(new MenuItemConfig_Visual("dlblNameB", "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Regular-AaNSS-14-_dpFont_"), "NameB"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Regular)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.5f)
			->put("anchorY", 0.0f)
			->put("x", 0.5f, "size.width")
			->put("y",
				0.5f, "size.height",
				VisualValueConfig::OPERATION_ADD,
				-26.0f + 1.0f, "_dpm.height"
			)
			->put("tintColor", Color("#000000FF"))
			->putAnimating("alpha", (new MenuVisualAnimationProperty())
				->putState("_active_._name:on_", AppStyles::ICON_DARK_ALPHA, EaseConfig::ACCELERATION)
				->putState("_", 0.0f, EaseConfig::ACCELERATION)
			)
			->putAnimating("_visibleFloat", (new MenuVisualAnimationProperty())
				->putState("_active_._name:on_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
		));
});

ArrayList<StateChangeDurations*>* MIGUIVAT_Selector::listItemConfig_animationDurations = base::StaticsInit::AddCbGeneral<ArrayList<StateChangeDurations*>*>(1, []() {
	listItemConfig_animationDurations = (new ArrayList<StateChangeDurations*>())
		->appendDirect_chain(new StateChangeDurations("_in_", "_in_", 0.15f))
		->appendDirect_chain(new StateChangeDurations("_out_", "_in_", AppStyles::ANIMATION_DURATION_S_DEFAULT))
		->appendDirect_chain(new StateChangeDurations("_in_", "_out_", AppStyles::ANIMATION_DURATION_S_DEFAULT));
});

MIGUIVAT_Selector::MIGUIVAT_Selector(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, base::IGameWorld* gw)
	: super(app, viewItemConfigs, viewEaseDurationsSList, gw)
{
	touchListener.cb_onCursorDown = std::bind(&MIGUIVAT_Selector::v2d_onCursorDown, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);

	scrollListEventListener.cb_onEntryTouched = std::bind(&MIGUIVAT_Selector::slToolsSelect_onEntryTouched, this, std::placeholders::_1, std::placeholders::_2);
}

bool MIGUIVAT_Selector::v2d_onCursorDown(
	IInteractiveDrawable2D* target,
	int cursorIndex, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos, Vector2& localPos)
{
	// Touch events are ignored while the menu is animating.
	if (!isActive()) {
		return false;
	}

	if (dynamic_cast<void*>(target) == dynamic_cast<void*>(taScreen)) {
		hideToolSelectorIfActive();
	} else {
		throw LogicException(LOC);
	}

	return false;
}

void MIGUIVAT_Selector::slToolsSelect_onEntryTouched(base::List2D* list, base::ListEntry2D* listItem) {
	// Touch events are ignored while the menu is animating.
	if (!isActive()) {
		return;
	}

	if (dynamic_cast<void*>(list) == dynamic_cast<void*>(slToolsSelect)) {
		if (listItem != activeListEntry) {
			if (selectedFocusEntity_toolsHandler != nullptr) {
				std::shared_ptr<base::IWrappedValue> val_entryTool = listItem->getUserDataMap()->getDirectB("entryTool");
				sp<Tool> entryTool = val_entryTool != nullptr ? val_entryTool->getDirectAs_Custom<sp<Tool>>(-1) : nullptr;

				if (entryTool == nullptr) {
					selectedFocusEntity_toolsHandler->deselectAllToolsAtSlot(0);
				} else {
					bool isTooInEntity = selectedFocusEntity_toolsHandler->peekToolsList()->containsReference(entryTool);
					if (!isTooInEntity) {
						// Tool no longer on the target entity.
						throw LogicException(LOC);
					}

					selectedFocusEntity_toolsHandler->selectTool(entryTool, 0);
				}
			}
		}

		if (isSelectoListActive && !isSelectoListMiniMode) {
			hideToolSelectorIfActive();
		}
	} else {
		throw LogicException(LOC);
	}
}

void MIGUIVAT_Selector::createVisuals(Map1D<std::string, void*>* createConfigMap) {
	super::createVisuals(createConfigMap);

	(taScreen = getItemMustExistAs< IInteractiveDrawable2D*>("activeTool.selector.taScreen"))->addTouchListener(&touchListener);
	taScreen->setIsVisible(false);

	slToolsSelect = getItemMustExistAs< base::ScrollList2D* >("activeTool.selector.slToolsSelect");
	slToolsSelect->setListener(&scrollListEventListener);
	slToolsSelect->gotoShowState(computeStateFlags("_out_"), false, nullptr, false);
}

void MIGUIVAT_Selector::onSelectedFocusEntityChange_pre() {
	super::onSelectedFocusEntityChange_pre();

	hideToolSelectorIfActive();
}

void MIGUIVAT_Selector::onSelectedFocusEntityChanged(IWorldEntity* selectedFocusEntity, ArrayList<std::shared_ptr<IEventListener>>& selectedFocusEntity_autoListenersList) {
	super::onSelectedFocusEntityChanged(selectedFocusEntity, selectedFocusEntity_autoListenersList);

	if (selectedFocusEntity_toolsHandler != nullptr) {
		selectedFocusEntity->addEventListener(
			IToolsHandlerModule::EVENT_toolsHandler_slot_occupied->type, IToolsHandlerModule::EVENT_toolsHandler_slot_unoccupied->type,
			std::bind(&MIGUIVAT_Selector::selectedFocusEntity_onEvent, this, std::placeholders::_1, std::placeholders::_2),
			selectedFocusEntity_autoListenersList
		);
		selectedFocusEntity->addEventListener(
			IToolsHandlerModule::EVENT_toolsHandler_toolAdded->type, IToolsHandlerModule::EVENT_toolsHandler_toolRemoved->type,
			std::bind(&MIGUIVAT_Selector::selectedFocusEntity_onEvent, this, std::placeholders::_1, std::placeholders::_2),
			selectedFocusEntity_autoListenersList
		);

		hideToolSelectorIfActive();
	}
}

void MIGUIVAT_Selector::selectedFocusEntity_onEvent(IEventListener& eventListener, base::EntityEvent& event) {
	if (event.type == IToolsHandlerModule::EVENT_toolsHandler_slot_occupied->type || event.type == IToolsHandlerModule::EVENT_toolsHandler_slot_unoccupied->type) {
		if (selectedFocusEntity_toolsHandler != nullptr) {
			// If the list is active, find the new active entry and activate.
			if (isSelectoListActive) {
				ArrayList<base::ListEntry2D*>* entriesList = slToolsSelect->peekEntriesList();
				bool isAnyToolSelected = false;
				for (int i = 0; i < entriesList->count() - 1; i++) {// Don't process the last entry, that is the to_tool_entry.
					base::ListEntry2D* listEntry = entriesList->getDirect(i);

					std::shared_ptr<base::IWrappedValue> val_entryTool = listEntry->getUserDataMap()->getDirectB("entryTool");
					sp<Tool> entryTool = val_entryTool != nullptr ? val_entryTool->getDirectAs_Custom<sp<Tool>>(-1) : nullptr;
					
					if (entryTool->getIsToolSelected()/*selected_tool*/) {
						isAnyToolSelected = true;

						if (listEntry != activeListEntry) {
							if (activeListEntry != nullptr) {
								activeListEntry->setIsActive(false, true);
								activeListEntry = nullptr;
							}

							activeListEntry = listEntry;
							activeListEntry->setIsActive(true, true);
						}
					}
				}
				if (!isAnyToolSelected) {
					// No tool selected, make sure the empty slot is selected.
					base::ListEntry2D* listEntry = entriesList->getDirect(entriesList->count() - 1);

					std::shared_ptr<base::IWrappedValue> val_entryTool = listEntry->getUserDataMap()->getDirectB("entryTool");
					sp<Tool> entryTool = val_entryTool != nullptr ? val_entryTool->getDirectAs_Custom<sp<Tool>>(-1) : nullptr;

					if (listEntry != activeListEntry) {
						if (activeListEntry != nullptr) {
							activeListEntry->setIsActive(false, true);
							activeListEntry = nullptr;
						}

						activeListEntry = listEntry;
						activeListEntry->setIsActive(true, true);
					}
				}
			}
		}
	}else if (event.type == IToolsHandlerModule::EVENT_toolsHandler_toolAdded->type) {
		if (isSelectoListActive) {
			isSlectorEntriesDirty = true;
		}
	}else if (event.type == IToolsHandlerModule::EVENT_toolsHandler_toolRemoved->type) {
		if (isSelectoListActive) {
			isSlectorEntriesDirty = true;
		}
	}

	else {
		throw LogicException(LOC);
	}
}

base::ListEntry2D* MIGUIVAT_Selector::createNewListEntry(
	std::string& atlasId, std::string& regionId, bool isActive, bool doAnimation, sp<Tool> tool,
	std::string& toolName_short, std::string& toolNameModifier_short)
{
	base::ListEntry2D* listEntry = slToolsSelect->createNewEntry(listItemConfig_tool, listItemConfig_animationDurations, -1, nullptr);

	std::shared_ptr<base::IWrappedValue> entryTool = nullptr;
	if (tool != nullptr) {
		entryTool = std::static_pointer_cast<base::IWrappedValue>(std::make_shared<base::WrappedValue<sp<Tool>>>(tool, -1));
	}
	listEntry->getUserDataMap()->putDirectB("entryTool", entryTool);
	
	Sprite2D* imgToolIcon = listEntry->getItemMustExistAs<Sprite2D*>("imgToolIcon");
	TextureRegion* region = appAssets->getEmbeddedAssetsManager()->getTextureAtlasRegion(atlasId, regionId);
	if (region == nullptr) {
		throw LogicException(LOC);
	}
	imgToolIcon->setTextureRegion(region);

	base::IBitmapLabel* dlblNameA = listEntry->getItemMustExistAs<base::IBitmapLabel*>("dlblNameA");
	base::IBitmapLabel* dlblNameB = listEntry->getItemMustExistAs<base::IBitmapLabel*>("dlblNameB");

	std::string mStateFlags;
	if (!toolName_short.empty()) {
		if (!toolNameModifier_short.empty()) {
			dlblNameA->setTextDirect(toolName_short);
			dlblNameB->setTextDirect(toolNameModifier_short);

			mStateFlags = "_name:on_nameLabels:bothLines_";
		} else {
			dlblNameA->setTextDirect("");
			dlblNameB->setTextDirect(toolName_short);

			mStateFlags = "_name:on_nameLabels:bottomLine_";
		}
	}else {
		dlblNameA->setTextDirect("");
		dlblNameB->setTextDirect("");

		mStateFlags = "_name:off_nameLabels_name:none_";
	}
	
	listEntry->setManualStateFlags(mStateFlags, false/*doAnimation*/);

	if (listEntry->getIsActive() != isActive) {
		listEntry->setIsActive(isActive, doAnimation);
		activeListEntry = listEntry;
	}

	listEntry->show(false);

	return listEntry;
}

bool MIGUIVAT_Selector::getIsToolSelectorActive() {
	return isSelectoListActive;
}

bool MIGUIVAT_Selector::getIsToolSelectorActiveAndMinimode() {
	if (isSelectoListActive && isSelectoListMiniMode) {
		return true;
	}

	return false;
}

void MIGUIVAT_Selector::setIsToolSelectorActive(bool isActive, bool isMinimode) {
	if (isSelectoListActive != isActive) {
		if (!isActive) {
			hideToolSelectorIfActive();
		} else {
			if (isSelectoListMiniMode != isMinimode) {
				this->isSelectoListMiniMode = isMinimode;

				if (!this->isSelectoListMiniMode) {
					// To avoid the list going from minimode_hidden to normalMode_shown, do a update with the new flags.
					updateShowStateIfChanged(false, nullptr);
				}
			}

			showToolSelector();
		}
	} else if(isSelectoListActive) {
		if (isSelectoListMiniMode != isMinimode) {
			this->isSelectoListMiniMode = isMinimode;

			updateShowStateIfChanged(true, nullptr);
		}
	}
}

void MIGUIVAT_Selector::showToolSelector() {
	if (isSelectoListActive) {
		return;
	}
	isSelectoListActive = true;

	updateSelectorEntries();
	
	updateShowStateIfChanged(true, nullptr);
}

void MIGUIVAT_Selector::hideToolSelectorIfActive() {
	if (!isSelectoListActive) {
		return;
	}
	isSelectoListActive = false;

	if (activeListEntry != nullptr) {
		//activeListEntry->setIsActive(false, true);
		activeListEntry = nullptr;
	}

	slToolsSelect->gotoShowState(computeStateFlags("_out_"), true, nullptr, false);

	updateShowStateIfChanged(true, nullptr);
}

void MIGUIVAT_Selector::updateSelectorEntries() {
	// Always update when called, even if isSlectorEntriesDirty==false;
	isSlectorEntriesDirty = false;

	slToolsSelect->removeAllEntries();

	if (selectedFocusEntity_toolsHandler != nullptr) {
		bool isAnyToolSelected = false;

		ArrayList<sp<Tool>>* toolsList = selectedFocusEntity_toolsHandler->peekToolsList_userSelectable();
		for (int i = 0; i < toolsList->count(); i++) {
			sp<Tool> tool = toolsList->getDirect(i, nullptr);

			if (tool->config->icon_atlasId == nullptr || tool->config->icon_regionId == nullptr) {
				// All manual tools must ha a icon.
				throw LogicException(LOC);
			}

			bool isToolSelected = tool->getIsToolSelected();
			if (isToolSelected) {
				isAnyToolSelected = true;
			}
			base::ListEntry2D* listEntry = createNewListEntry(
				*tool->config->icon_atlasId, *tool->config->icon_regionId,
				isToolSelected, false/*doAnimation*/,
				tool,
				tool->config->shortName_partA, tool->config->shortName_partB
			);
		}

		// Append another entry for the no_tool option.
		bool isToolActive = isAnyToolSelected == false ? true : false;
		std::string atlasId = "visual2D/taGeneral.atlas";
		std::string regionId = "sh/tools/none";
		std::string toolName_short = "";
		std::string toolNameModifier_short = "";
		base::ListEntry2D* listEntry = createNewListEntry(
			atlasId, regionId,
			isToolActive, false/*doAnimation*/,
			nullptr,
			toolName_short, toolNameModifier_short
		);

		slToolsSelect->gotoShowState(computeStateFlags("_in_"), true, nullptr, false);
	}
}

void MIGUIVAT_Selector::onGameLoop_localInput(float deltaS, int deltaMS, float gameTimeS) {
	super::onGameLoop_localInput(deltaS, deltaMS, gameTimeS);

	if (isSlectorEntriesDirty && isSelectoListActive) {
		updateSelectorEntries();
	}
}

std::string MIGUIVAT_Selector::computeStateFlags(std::string baseState) {
	std::string state = super::computeStateFlags(baseState);

	if (!isSelectoListActive) {
		state += "_toolSelectorStatus:off_";
	}else {
		state += "_toolSelectorStatus:on_";
	}

	if (isSelectoListMiniMode) {
		state += "_toolSelectorMinimode:on_";
	} else {
		state += "_toolSelectorMinimode:off_";
	}

	return state;
}

MIGUIVAT_Selector::~MIGUIVAT_Selector() {
	//void
}
