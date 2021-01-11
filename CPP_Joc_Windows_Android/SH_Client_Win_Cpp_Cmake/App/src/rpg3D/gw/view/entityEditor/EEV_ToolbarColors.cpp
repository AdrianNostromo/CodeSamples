#include "EEV_ToolbarColors.h"
#include <base/menu/util/MenuItemConfig_Data.h>
#include <base/menu/util/MenuItemConfig_Visual.h>
#include <base/menu/util/MenuItemConfig_SmartItem.h>
#include <base/visual2D/util/VCC_Group.h>
#include <base/visual2D/util/VCC_TouchArea.h>
#include <base/visual2D/util/VCC_Sprite.h>
#include <base/visual2D/util/VCC_BitmapLabel.h>
#include <base/menu/util/MenuItemConfig_ComposedVisual.h>
#include <base/util/AppStyles.h>
#include <base/statics/StaticsInit.h>
#include <graphics/visual2d/drawable/Sprite2D.h>
#include <base/app/config/AppConfig.h>
#include <base/view/colorSelector/ColorSelectorView.h>
#include <base/view/dialog/DialogView.h>
#include <base/view/colorSelector/IColorSelectorView.h>
#include <rpg3D/gw/entity/module/editableVoxelised/IEditableVoxelisedM_Edit.h>
#include <rpg3D/gw/entity/module/voxelizedEditor/IVoxelizedEditorM_Edit.h>
#include <rpg3D/config/sounds/SoundConfigsList.h>
#include <base/view/dialog/IDialogView.h>
#include <base/ease/Ease.h>
#include <base/viewsHandler/IViewsHandler.h>
#include <base/app/IApp.h>

using namespace rpg3D;

// This limit is arbitrary. Can be increased some if needed (I feel like this should be kept small-ish).
const int EEV_ToolbarColors::VOXEL_MODEL_MAX_VERTEX_COLORS_COUNT = 20;

const int EEV_ToolbarColors::LIST_ENTRY_TYPE_COLOR = 1;
const int EEV_ToolbarColors::LIST_ENTRY_TYPE_ADD_NEW_COLOR = 2;

const int EEV_ToolbarColors::COLOR_SELECTOR_BUTTON_ID_ADD = 1;
const int EEV_ToolbarColors::COLOR_SELECTOR_BUTTON_ID_CANCEL = 2;
const int EEV_ToolbarColors::COLOR_SELECTOR_BUTTON_ID_UPDATE = 3;
const int EEV_ToolbarColors::COLOR_SELECTOR_BUTTON_ID_DELETE = 4;

MenuItemConfig* EEV_ToolbarColors::new_menuItemConfigs_topArea_toolbarColors() {
	return new MenuItemConfig_ComposedVisual("toolbarColors", (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("props", (new VisualPropertiesConfig())
			->put("width", 1.0f, "parent.props.width")
			->put("height", 56.0f, "_dpm.height")
		))
		->appendDirect_chain(new MenuItemConfig_Visual("layer", "parent.layer", VCC_Group::New(), (new VisualPropertiesConfig())
			->put("x", 0.0f, "_dpm.width")
			->put("y", 56.0f, "_dpm.height")

			->putAnimating("_visibleFloat", (new MenuVisualAnimationProperty())
				->putState("_colorsBar:true_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
		))

		->appendDirect_chain(new MenuItemConfig_SmartItem("colorsHScrollList", "layer", "_in_", [](IAppMetrics* appMetrics, IAppAssets* appAssets) -> ComposedStiledVisual * { return new base::ScrollList2D(appMetrics, appAssets); }, (new VisualPropertiesConfig())
			->put("x", 0.0f, "_dpm.width")
			->put("y", 0.0f, "_dpm.width")

			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)

			->put("entryActivateSoundConfig_id", &SoundConfigsList::UserInterface_Btn_Down->managedPath->path)
			->put("entryActivateSound_volume", 0.5f)

			->put("entriesAlignmentH", base::List2D::AlignmentH::Left)
			->put("entriesAlignmentV", base::List2D::AlignmentV::Bottom)

			->put("entriesSpacingX", 0.0f)
			->put("entriesSpacingY", 0.0f)
			->put("listPaddingTop", 0.0f, "_dpm.width")
			->put("listPaddingBottom", 0.0f, "_dpm.width")
			->put("listPaddingLeft", 0.0f, "_dpm.width")
			->put("listPaddingRight", 0.0f, "_dpm.width")

			->put("rowsCount", 1)// Horizontal list.
			//->put("columnsCount", 1) // Vertical list

			->put("invertListGrowAxisX", true)
			->put("invertListGrowAxisY", false)
			
			->put("invertEntriesDrawOrder", true)

			->put("autoListSizeToContent", false)
			->put("manualListSizeWidth", 1.0f, "props.width")
			->put("manualListSizeHeight", 65.0f, "_dpm.width")

			->put("bgTintColor", Color("#FFFFFFFF"))
			->put("bgAlpha", 1.0f)
			
			->put("preScrollDeltaThreshold", 5.0f, "_dpm.width")
			->put("touchDeselectionScrollDeltaThreshold", 15.0f, "_dpm.width")

			->put("animatedMinScrollSpeedS", 50.0f, "_dpm.width")
			->put("animatedMaxScrollSpeedS", 600.0f, "_dpm.width")
			->put("animatedMaxScrollSpeedAtDistance", 100.0f, "_dpm.width")
			->put("animatedScrollEaseFunction", &Ease::easeOutCubic)
		))
	);
}

ArrayList<MenuItemConfig*>* EEV_ToolbarColors::listItemConfig_colorSelector = base::StaticsInit::AddCbGeneral<ArrayList<MenuItemConfig*>*>(1, []() {
	listItemConfig_colorSelector = (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("size", (new VisualPropertiesConfig())
			->put("width", 56.0f, "_dpm.width")
			->put("height", 56.0f, "_dpm.width")
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
			//void
		))

		->appendDirect_chain(new MenuItemConfig_Visual("touchArea", "layer", VCC_TouchArea::New(), (new VisualPropertiesConfig())
			->put("width", 1.0f, "size.width")
			->put("height", 1.0f, "size.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x", 0.5f, "size.width")
			->put("y", 0.5f, "size.height")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("imgColoChipCheckerPatternBg", "layer", VCC_Sprite::New("visual2D/taGeneral.atlas", "rpg3D/gw/entityEditor/view/toolbarColors/colorChip/checkerPatternBg"), (new VisualPropertiesConfig())
			->putAnimating("width", (new MenuVisualAnimationProperty())
				->putState("_active_", 36.0f, "_dpm.width", EaseConfig::LINEAR)
				->putState("_", 24.0f, "_dpm.width", EaseConfig::LINEAR)
			)
			->putAnimating("height", (new MenuVisualAnimationProperty())
				->putState("_active_", 36.0f, "_dpm.width", EaseConfig::LINEAR)
				->putState("_", 24.0f, "_dpm.width", EaseConfig::LINEAR)
			)
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x", 0.5f, "size.width")
			->putAnimating("y", (new MenuVisualAnimationProperty())
				->putState("_active_", 1.0f, "data.colorChipPosY_active", EaseConfig::LINEAR)
				->putState("_", 1.0f, "data.colorChipPosY_normal", EaseConfig::LINEAR)
			)
		))
		->appendDirect_chain(new MenuItemConfig_Visual("imgColorChipShadow", "layer", VCC_Sprite::New("visual2D/taGeneral.atlas", "rpg3D/gw/entityEditor/view/toolbarColors/colorChip/shadow"), (new VisualPropertiesConfig())
			->putAnimating("width", (new MenuVisualAnimationProperty())
				->putState("_active_", 36.0f, "_dpm.width", EaseConfig::LINEAR)
				->putState("_", 24.0f, "_dpm.width", EaseConfig::LINEAR)
			)
			->putAnimating("height", (new MenuVisualAnimationProperty())
				->putState("_active_", 36.0f, "_dpm.width", EaseConfig::LINEAR)
				->putState("_", 24.0f, "_dpm.width", EaseConfig::LINEAR)
			)
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x", 0.5f, "size.width")
			->putAnimating("y", (new MenuVisualAnimationProperty())
				->putState("_active_", 1.0f, "data.colorChipPosY_active", EaseConfig::LINEAR)
				->putState("_", 1.0f, "data.colorChipPosY_normal", EaseConfig::LINEAR)
			)
		))
		->appendDirect_chain(new MenuItemConfig_Visual("imgColorChip", "layer", VCC_Sprite::New("visual2D/taGeneral.atlas", "rpg3D/gw/entityEditor/view/toolbarColors/colorChip/bg"), (new VisualPropertiesConfig())
			->putAnimating("width", (new MenuVisualAnimationProperty())
				->putState("_active_", 36.0f, "_dpm.width", EaseConfig::LINEAR)
				->putState("_", 24.0f, "_dpm.width", EaseConfig::LINEAR)
			)
			->putAnimating("height", (new MenuVisualAnimationProperty())
				->putState("_active_", 36.0f, "_dpm.width", EaseConfig::LINEAR)
				->putState("_", 24.0f, "_dpm.width", EaseConfig::LINEAR)
			)
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x", 0.5f, "size.width")
			->putAnimating("y", (new MenuVisualAnimationProperty())
				->putState("_active_", 1.0f, "data.colorChipPosY_active", EaseConfig::LINEAR)
				->putState("_", 1.0f, "data.colorChipPosY_normal", EaseConfig::LINEAR)
			)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("imgActiveItemIndicator", "layer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "bg/simple_white_FFFFFF"), (new VisualPropertiesConfig())
			->put("width", 36.0f, "_dpm.width")
			->putAnimating("height", (new MenuVisualAnimationProperty())
				->putState("_active_", 6.0f, "_dpm.width", EaseConfig::LINEAR)
				->putState("_", 0.0f, "_dpm.width", EaseConfig::LINEAR)
			)
			->put("anchorX", 0.5f)
			->put("anchorY", 0.0f)
			->put("x", 0.5f, "size.width")
			->put("y", 0.0f)
			->putAnimating("_visibleFloat", (new MenuVisualAnimationProperty())
				// Use float because it will be automatically casted to boolean.
				->putState("_active_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
			->put("tintColor", Color("#111111FF"))
			->put("alpha", 0.4f)
		));
		
		/*->appendDirect_chain(new MenuItemConfig_Visual("itemsVDivider", "layer", VCC_Sprite::New("visual2D/taGeneral.atlas", "general/divider_vertical_black_000000"), (new VisualPropertiesConfig())
			->put("width", 1.0f, "_dpm.width")
			->put("height", 29.0f, "_dpm.width")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x", 1.0f, "size.width")
			->put("y", 0.5f, "size.height")
			->put("tintColor", Color("#111111FF"))
			->put("alpha", 0.4f)
		));*/
});

ArrayList<MenuItemConfig*>* EEV_ToolbarColors::listItemConfig_addColor = base::StaticsInit::AddCbGeneral<ArrayList<MenuItemConfig*>*>(1, []() {
	listItemConfig_addColor = (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("size", (new VisualPropertiesConfig())
			->put("width", 56.0f, "_dpm.width")
			->put("height", 56.0f, "_dpm.width")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("layer", nullptr, VCC_Group::New(), (new VisualPropertiesConfig())
		))

		->appendDirect_chain(new MenuItemConfig_Visual("touchArea", "layer", VCC_TouchArea::New(), (new VisualPropertiesConfig())
			->put("width", 1.0f, "size.width")
			->put("height", 1.0f, "size.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x", 0.5f, "size.width")
			->put("y", 0.5f, "size.height")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("iconAddNewColor", "layer", VCC_Sprite::New("visual2D/taGeneral.atlas", "rpg3D/gw/entityEditor/view/toolbarColors/iconAddNewColor"), (new VisualPropertiesConfig())
			->put("width", 36.0f, "_dpm.width")
			->put("height", 36.0f, "_dpm.width")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x", 0.5f, "size.width")
			->put("y", 0.5f, "size.height")
			->put("tintColor", Color("#111111FF"))
			->put("alpha", 0.4f)
		));

		/*->appendDirect_chain(new MenuItemConfig_Visual("itemsVDivider", "layer", VCC_Sprite::New("visual2D/taGeneral.atlas", "general/divider_vertical_black_000000"), (new VisualPropertiesConfig())
			->put("width", 1.0f, "_dpm.width")
			->put("height", 29.0f, "_dpm.width")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x", 1.0f, "size.width")
			->put("y", 0.5f, "size.height")
			->put("alpha", AppStyles::DIVIDER_DARK_ALPHA)
		));*/
});

ArrayList<StateChangeDurations*>* EEV_ToolbarColors::listItemConfig_animationDurations = base::StaticsInit::AddCbGeneral<ArrayList<StateChangeDurations*>*>(1, []() {
	listItemConfig_animationDurations = (new ArrayList<StateChangeDurations*>())
		->appendDirect_chain(new StateChangeDurations("_in_", "_in_", 0.15f))
		->appendDirect_chain(new StateChangeDurations("_out_", "_in_", AppStyles::ANIMATION_DURATION_S_DEFAULT))
		->appendDirect_chain(new StateChangeDurations("_in_", "_out_", AppStyles::ANIMATION_DURATION_S_DEFAULT));
});

EEV_ToolbarColors::EEV_ToolbarColors(
	IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList,
	IAppUtils* appUtils)
	: super(
		app, viewItemConfigs, viewEaseDurationsSList,
		appUtils)
{
	scrollListEventListener.cb_onEntryTouched = std::bind(&EEV_ToolbarColors::colorsHScrollList_onEntryTouched, this, std::placeholders::_1, std::placeholders::_2);

	colorSelectorEventListener.cb_onDynamicButtonActivated = std::bind(&EEV_ToolbarColors::colorSelector_onDynamicButtonActivated, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	colorSelectorEventListener.cb_onCloseRequested = std::bind(&EEV_ToolbarColors::colorSelector_onCloseRequested, this, std::placeholders::_1);

	dialogEventListener.cb_onDynamicButtonActivated = std::bind(&EEV_ToolbarColors::dialog_onDynamicButtonActivated, this, std::placeholders::_1, std::placeholders::_2);
	dialogEventListener.cb_onCloseRequested = std::bind(&EEV_ToolbarColors::dialog_onCloseRequested, this, std::placeholders::_1);
	dialogEventListener.cb_onContentAreaTouched = std::bind(&EEV_ToolbarColors::dialog_onContentAreaTouched, this, std::placeholders::_1);
}

void EEV_ToolbarColors::onEntityToEditChanged() {
	super::onEntityToEditChanged();

	if (entityToEdit == nullptr) {
		colorsHScrollList->removeAllEntries();
	} else {
		colorsHScrollList->removeAllEntries();

		// Init the new colors list.
		bool isAnyColorAdded = false;
		ArrayList<std::shared_ptr<IEditableVoxelisedM_Edit::WrappedIndexedVoxelColor>>* editingIndexedVoxelColors = entityToEdit->peekEditingIndexedVoxelColors();
		for (int i = 0; i < editingIndexedVoxelColors->size(); i++) {
			std::shared_ptr<IEditableVoxelisedM_Edit::WrappedIndexedVoxelColor> indexedVoxelColor = editingIndexedVoxelColors->getDirect(i);

			base::ListEntry2D* newEntry = createNewColorsListEntry(indexedVoxelColor->color, i);

			isAnyColorAdded = true;
		}

		base::ListEntry2D* newEntry = colorsHScrollList->createNewEntry(listItemConfig_addColor, listItemConfig_animationDurations, nullptr);
		newEntry->getUserDataMap()->putDirectB("type", base::IWrappedValue::new_int(LIST_ENTRY_TYPE_ADD_NEW_COLOR));
		newEntry->show(false);

		onActiveDrawDynamicMaterialIndexChanged();
	}
}

void EEV_ToolbarColors::setIsColorsSelectorActive(bool isColorsSelectorActive) {
	if (this->isColorsSelectorActive == isColorsSelectorActive) {
		return;
	}

	this->isColorsSelectorActive = isColorsSelectorActive;

	updateShowStateIfChanged(true, nullptr);
}

void EEV_ToolbarColors::colorsHScrollList_onEntryTouched(base::List2D* list, base::ListEntry2D* listItem) {
	// Touch events are ignored while the menu is animating.
	if (!isActive() || entityToEdit == nullptr) {
		return;
	}

	if (dynamic_cast<void*>(list) == dynamic_cast<void*>(colorsHScrollList)) {
		int type = listItem->getUserDataMap()->getDirectB("type")->getDirectAs_int();

		if (type == LIST_ENTRY_TYPE_COLOR) {
			int editorColorIndex = listItem->getUserDataMap()->getDirectB("editorColorIndex")->getDirectAs_int();

			// Implement normal activation and also double tap within 0.5s to edit or remove a color entry.
			if (activeColorListEntry != nullptr && activeColorListEntry == listItem) {
				// Show color selector to edit or remove color.
				cleanup_colorSelectorViewIfAny();
				colorSelectorView = app->getComponentAs<base::IViewsHandler*>(true/*mustExist*/)->getNewColorSelectorViewAutoDisposable();
				colorSelectorView->getUserDataMap()->putDirectB("deleteConfirmCounter", base::IWrappedValue::new_int(0));

				std::shared_ptr<IEditableVoxelisedM_Edit::WrappedIndexedVoxelColor> indexedMat = entityToEdit->peekEditingIndexedVoxelColors()->getDirect(editorColorIndex);

				std::shared_ptr<ArrayList<base::IColorSelectorView::DynamicTextButtonConfig>> buttonConfigsList = std::make_shared<ArrayList<base::IColorSelectorView::DynamicTextButtonConfig>>();

				Color col{ "#2196F3FF" };
				buttonConfigsList->append_emplace(COLOR_SELECTOR_BUTTON_ID_UPDATE, "UPDATE", col, 1.0f);
				col.set("#000000FF");
				//buttonConfigsList->append_emplace(COLOR_SELECTOR_BUTTON_ID_CANCEL, "CANCEL", col, 0.38f);
				col.set("#EA1D24FF");
				buttonConfigsList->append_emplace(COLOR_SELECTOR_BUTTON_ID_DELETE, "DELETE", col, 1.0f);
				colorSelectorView->setListener(&colorSelectorEventListener);
				colorSelectorView->show(true, nullptr, &indexedMat->color,
					buttonConfigsList,
					false, 0xFF
				);
			} else if (activeColorListEntry != listItem) {
				entityToEdit->setActiveDynamicMaterialIndex(editorColorIndex);
			}
		} else if (type == LIST_ENTRY_TYPE_ADD_NEW_COLOR) {
			if (entityToEdit->peekEditingIndexedVoxelColors()->size() >= VOXEL_MODEL_MAX_VERTEX_COLORS_COUNT) {
				showColorsCountLimitReached(VOXEL_MODEL_MAX_VERTEX_COLORS_COUNT);
			} else {
				std::shared_ptr<ArrayList<base::IColorSelectorView::DynamicTextButtonConfig>> buttonConfigsList = std::make_shared<ArrayList<base::IColorSelectorView::DynamicTextButtonConfig>>();
				
				Color col{ "#2196F3FF" };
				buttonConfigsList->append_emplace(COLOR_SELECTOR_BUTTON_ID_ADD, "ADD", col, 1.0f);
				//buttonConfigsList->append_emplace(COLOR_SELECTOR_BUTTON_ID_CANCEL, "CANCEL", 0x000000FF, 0.38f);
				
				cleanup_colorSelectorViewIfAny();
				colorSelectorView = app->getComponentAs<base::IViewsHandler*>(true/*mustExist*/)->getNewColorSelectorViewAutoDisposable();
				colorSelectorView->setListener(&colorSelectorEventListener);
				colorSelectorView->show(
					true, nullptr, &AppConfig::APP_MAIN_COLOR,
					buttonConfigsList,
					false, 0xFF
				);
			}
		} else {
			throw LogicException(LOC);
		}
	} else {
		throw LogicException(LOC);
	}
}

void EEV_ToolbarColors::colorSelector_onDynamicButtonActivated(base::IColorSelectorView* view, base::IColorSelectorView::DynamicTextButtonConfig* dynamicButtonConfig, Color& activeColor) {
	if (view != colorSelectorView) {
		throw LogicException(LOC);
	}

	if (dynamicButtonConfig->id == COLOR_SELECTOR_BUTTON_ID_ADD) {
		cleanup_colorSelectorViewIfAny();

		int colorIndex = entityToEdit->createNewMaterialFromDiffuseColor(-1, activeColor, true);

		entityToEdit->setActiveDynamicMaterialIndex(colorIndex);
	} else if (dynamicButtonConfig->id == COLOR_SELECTOR_BUTTON_ID_CANCEL) {
		cleanup_colorSelectorViewIfAny();
	} else if (dynamicButtonConfig->id == COLOR_SELECTOR_BUTTON_ID_UPDATE) {
		cleanup_colorSelectorViewIfAny();

		int editorColorIndex = activeColorListEntry->getUserDataMap()->getDirectB("editorColorIndex")->getDirectAs_int();

		entityToEdit->editVoxelMaterialDiffuseColor(editorColorIndex, activeColor, true);
	} else if (dynamicButtonConfig->id == COLOR_SELECTOR_BUTTON_ID_DELETE) {
		int editorColorIndex = activeColorListEntry->getUserDataMap()->getDirectB("editorColorIndex")->getDirectAs_int();

		std::shared_ptr<IEditableVoxelisedM_Edit::WrappedIndexedVoxelColor> wrappedMat = entityToEdit->peekEditingIndexedVoxelColors()->getDirect(editorColorIndex);
		if (wrappedMat->voxelsUsageCount > 0) {
			showColorFailedInUseDialog(wrappedMat->voxelsUsageCount);
		} else {
			int deleteConfirmCounter = view->getUserDataMap()->getDirectB("deleteConfirmCounter")->getDirectAs_int();
			static const int requiredConfimCount = 3;
			if (deleteConfirmCounter < requiredConfimCount) {
				deleteConfirmCounter++;

				if (deleteConfirmCounter == 0) {
					view->updateControlLblText(1, "DELETE");
				} else {
					view->updateControlLblText(1, "CONFIRM " + std::to_string(deleteConfirmCounter) + "/" + std::to_string(requiredConfimCount - 1));
				}

				view->getUserDataMap()->putDirectB("deleteConfirmCounter", base::IWrappedValue::new_int(deleteConfirmCounter));
				if (deleteConfirmCounter == requiredConfimCount) {
					cleanup_colorSelectorViewIfAny();

					// No need to do this because it is handled by the entity_interface.
					// entityToEdit->setActiveDynamicMaterialIndex(-1);

					entityToEdit->removeUnusedVoxelMaterial(editorColorIndex, true);
				}
			}
		}
	} else {
		throw LogicException(LOC);
	}
}

void EEV_ToolbarColors::dialog_onDynamicButtonActivated(base::IDialogView* view, base::DynamicTextButtonConfig* dynamicButtonConfig) {
	if (view != dialogView) {
		throw LogicException(LOC);
	}

	// The dialog has no buttons.
	throw LogicException(LOC);
}

void EEV_ToolbarColors::dialog_onCloseRequested(base::IDialogView* view) {
	if (view != dialogView) {
		throw LogicException(LOC);
	}

	cleanup_dialogViewIfAny();
}

void EEV_ToolbarColors::dialog_onContentAreaTouched(base::IDialogView* view) {
	if (view != dialogView) {
		throw LogicException(LOC);
	}

	cleanup_dialogViewIfAny();
}

void EEV_ToolbarColors::colorSelector_onCloseRequested(base::IColorSelectorView* view) {
	if (view != colorSelectorView) {
		throw LogicException(LOC);
	}

	cleanup_colorSelectorViewIfAny();
}

void EEV_ToolbarColors::createVisuals(Map1D<std::string, void*>* createConfigMap) {
	super::createVisuals(createConfigMap);

	colorsHScrollList = getItemMustExistAs< base::ScrollList2D * >("topArea.toolbarColors.colorsHScrollList");
	colorsHScrollList->setListener(&scrollListEventListener);
}

void EEV_ToolbarColors::onActiveDrawDynamicMaterialIndexChanged() {
	base::ListEntry2D* listEntry = (entityToEdit->peekActiveDynamicMaterialIndex() >= 0) ? colorsHScrollList->getEtryWithIndex(entityToEdit->peekActiveDynamicMaterialIndex()) : nullptr;

	if (this->activeColorListEntry == listEntry) {
		return;
	}

	if (this->activeColorListEntry != nullptr) {
		updateColorEntryActiveStateVisuals(this->activeColorListEntry, false);

		this->activeColorListEntry = nullptr;
	}

	this->activeColorListEntry = listEntry;

	if (this->activeColorListEntry != nullptr) {
		// Set active tool to the drawer.
		updateColorEntryActiveStateVisuals(this->activeColorListEntry, true);
	}
}

void EEV_ToolbarColors::onNewEditorColor(int editorColorIndex, Color& col, bool updatesHistory) {
	std::shared_ptr<IEditableVoxelisedM_Edit::WrappedIndexedVoxelColor> indexedVoxelColor = entityToEdit->peekEditingIndexedVoxelColors()->getDirect(editorColorIndex);

	createNewColorsListEntry(indexedVoxelColor->color, editorColorIndex);
}

void EEV_ToolbarColors::onEditorColorChanged(int editorColorIndex, Color& oldCol, Color& newCol, bool updatesHistory) {
	std::shared_ptr<IEditableVoxelisedM_Edit::WrappedIndexedVoxelColor> indexedVoxelColor = entityToEdit->peekEditingIndexedVoxelColors()->getDirect(editorColorIndex);

	//// Get the list entry.
	base::ListEntry2D* listEntry = colorsHScrollList->getEtryWithIndex(editorColorIndex);

	//// Update the list entry visual.
	Sprite2D* imgColorChip = listEntry->getItemMustExistAs< Sprite2D*>("imgColorChip");
	imgColorChip->setTintColor(indexedVoxelColor->color);
}

void EEV_ToolbarColors::onEditorColorRemoved(int editorColorIndex, Color& col, bool updatesHistory) {
	base::ListEntry2D* listEntry = colorsHScrollList->getEtryWithIndex(editorColorIndex);

	if (activeColorListEntry == listEntry) {
		activeColorListEntry = nullptr;
	}

	colorsHScrollList->removeEntry(listEntry);

	// Update the post entries "editorColorIndex" to the correct value.
	ArrayList<std::shared_ptr<IEditableVoxelisedM_Edit::WrappedIndexedVoxelColor>>* editingIndexedVoxelColors = entityToEdit->peekEditingIndexedVoxelColors();
	for (int i = editorColorIndex; i < editingIndexedVoxelColors->size(); i++) {
		listEntry = colorsHScrollList->getEtryWithIndex(i);

		listEntry->getUserDataMap()->putDirectB("editorColorIndex", base::IWrappedValue::new_int(i));
	}

	if (entityToEdit->peekActiveDynamicMaterialIndex() == editorColorIndex) {
		// The removed entry was the actve one.
		// If there is another entry at that index, activate the list_entry.
		// If no entry at that index, wait for different logic to activate a new material.
		onActiveDrawDynamicMaterialIndexChanged();
	}
}

/**
 *
 * Negative number place the entry at an offset from the list end plus one.
 * -1 will append at end.
 * @return
 */
base::ListEntry2D* EEV_ToolbarColors::createNewColorsListEntry(Color& diffuseColor, int editorColorIndex) {
	base::ListEntry2D* listEntry = colorsHScrollList->createNewEntry(listItemConfig_colorSelector, listItemConfig_animationDurations, editorColorIndex, nullptr);

	listEntry->getUserDataMap()->putDirectB("type", base::IWrappedValue::new_int(LIST_ENTRY_TYPE_COLOR));

	listEntry->getUserDataMap()->putDirectB("editorColorIndex", base::IWrappedValue::new_int(editorColorIndex));

	Sprite2D* imgColorChip = listEntry->getItemMustExistAs<Sprite2D*>("imgColorChip");
	imgColorChip->setTintColor(diffuseColor);

	updateColorEntryActiveStateVisuals(listEntry, false);

	listEntry->show(false);

	return listEntry;
}

void EEV_ToolbarColors::updateColorEntryActiveStateVisuals(base::ListEntry2D* listEntry, bool isActive) {
	if (listEntry != nullptr) {
		if (!isActive && listEntry->getIsActive()) {
			listEntry->setIsActive(false, true);
		} else if (isActive && !listEntry->getIsActive()) {
			listEntry->setIsActive(true, true);

			colorsHScrollList->scrollEntryIntoView(listEntry, true);
		}
	}
}

void EEV_ToolbarColors::showColorFailedInUseDialog(int voxelsUsageCount) {
	cleanup_dialogViewIfAny();
	dialogView = app->getComponentAs<base::IViewsHandler*>(true/*mustExist*/)->getNewDialogViewAutoDisposable();
	dialogView->setListener(&dialogEventListener);
	dialogView->show(
		true, nullptr,
		std::make_shared<base::IDialogView::Config>(
			250.0f /*widthDPM*/,
			std::make_shared<std::string>("Not possible") /*titleText*/, 1 /*titleTotalLinesCount*/,
			std::make_shared<base::IDialogView::DynamicTextItem>("Color is used by " + std::to_string(voxelsUsageCount) + " voxels.", 1) /*dynamicBodyTextItem*/, false /*isOneButtonControlPerLine*/,
			nullptr /*dynamicButtonConfigs*/
		)
	);
}

void EEV_ToolbarColors::showColorsCountLimitReached(int maxColorCount) {
	cleanup_dialogViewIfAny();
	dialogView = app->getComponentAs<base::IViewsHandler*>(true/*mustExist*/)->getNewDialogViewAutoDisposable();
	dialogView->setListener(&dialogEventListener);
	dialogView->show(
		true, nullptr,
		std::make_shared<base::IDialogView::Config>(
			250.0f /*widthDPM*/,
			std::make_shared<std::string>("Not possible") /*titleText*/, 1 /*titleTotalLinesCount*/,
			std::make_shared<base::IDialogView::DynamicTextItem>("Too many colors (" + std::to_string(maxColorCount) + ").", 1) /*dynamicBodyTextItem*/, false /*isOneButtonControlPerLine*/,
			nullptr /*dynamicButtonConfigs*/
			)
	);
}

void EEV_ToolbarColors::onSelectedStateChanged() {
	super::onSelectedStateChanged();

	if (!isActive()) {
		cleanup_dialogViewIfAny();
		cleanup_colorSelectorViewIfAny();
	} else {
		//void
	}
}

std::string EEV_ToolbarColors::computeStateFlags(std::string baseState) {
	std::string state = super::computeStateFlags(baseState);

	if (isColorsSelectorActive) {
		state += "_colorsBar:true_";
	} else {
		state += "_colorsBar:false_";
	}

	return state;
}

void EEV_ToolbarColors::cleanup_dialogViewIfAny() {
	if (dialogView != nullptr) {
		dialogView->setListener(nullptr);
		dialogView->hide(true, nullptr);

		dialogView = nullptr;
	}
}

void EEV_ToolbarColors::cleanup_colorSelectorViewIfAny() {
	if (colorSelectorView != nullptr) {
		colorSelectorView->setListener(nullptr);
		colorSelectorView->hide(true, nullptr);

		colorSelectorView = nullptr;
	}
}

void EEV_ToolbarColors::disposeMain() {
	cleanup_dialogViewIfAny();
	cleanup_colorSelectorViewIfAny();

	super::disposeMain();
}

EEV_ToolbarColors::~EEV_ToolbarColors() {
	//void
}
