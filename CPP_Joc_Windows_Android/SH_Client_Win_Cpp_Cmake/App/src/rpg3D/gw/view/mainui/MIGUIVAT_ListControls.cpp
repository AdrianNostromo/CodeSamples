#include "MIGUIVAT_ListControls.h"
#include <base/menu/util/MenuItemConfig_ComposedVisual.h>
#include <base/menu/util/MenuItemConfig_Data.h>
#include <base/menu/util/MenuItemConfig_Visual.h>
#include <base/visual2D/util/VCC_Touchable.h>
#include <base/visual2D/util/VCC_Group.h>
#include <base/visual2D/util/VCC_Sprite.h>
#include <base/visual2D/util/VCC_TouchArea.h>
#include <base/menu/util/MenuItemConfig_SmartItem.h>
#include <rpg3D/config/sounds/SoundConfigsList.h>
#include <base/ease/Ease.h>
#include <base/statics/StaticsInit.h>
#include <base/util/AppStyles.h>
#include <base/log/GlobalAccessLogHandler.h>
#include <graphics/visual2d/drawable/Sprite2D.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <rpg3D/gw/entity/module/toolsHandler/IToolsHandlerModule.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/Tool.h>
#include <base/app/IAppAssets.h>
#include <base/assets/IAssetsManager.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/event/ToolEvent.h>
#include <graphics/visual2d/drawable/List2D.h>

using namespace rpg3D;

const int MIGUIVAT_ListControls::LIST_ENTRY_TYPE_GENERAL_SELECTABLE = 1;

MenuItemConfig* MIGUIVAT_ListControls::new_viewItemConfig_activeTool_listControls() {
	return new MenuItemConfig_ComposedVisual("listControls", (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("props", (new VisualPropertiesConfig())
			->put("width", 1.0f, "parent.props.width")
			->put("height", 1.0f, "parent.props.height")
		))
		->appendDirect_chain(new MenuItemConfig_Visual("layer", "parent.layer", VCC_Group::New(), (new VisualPropertiesConfig())
			->putAnimating("alpha", (new MenuVisualAnimationProperty())
				->putState("_isToolListControls:true_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
			->putAnimating("_visibleFloat", (new MenuVisualAnimationProperty())
				->putState("_isToolListControls:true_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
		))

		->appendDirect_chain(new MenuItemConfig_SmartItem("hScrollList", "layer", "_in_", [](IAppMetrics* appMetrics, IAppAssets* appAssets) -> ComposedStiledVisual * { return new base::ScrollList2D(appMetrics, appAssets); }, (new VisualPropertiesConfig())
			->put("x", 0.0f, "_dpm.width")
			->put("y", 1.0f, "props.height")

			->put("anchorX", 0.0f)
			->put("anchorY", 1.0f)

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

			->put("bgTintColor", Color("#000000FF"))
			->put("bgAlpha", 0.4f)
			
			->put("preScrollDeltaThreshold", 5.0f, "_dpm.width")
			->put("touchDeselectionScrollDeltaThreshold", 15.0f, "_dpm.width")

			->put("animatedMinScrollSpeedS", 50.0f, "_dpm.width")
			->put("animatedMaxScrollSpeedS", 600.0f, "_dpm.width")
			->put("animatedMaxScrollSpeedAtDistance", 100.0f, "_dpm.width")
			->put("animatedScrollEaseFunction", &Ease::easeOutCubic)
		))

		
	);
}

ArrayList<MenuItemConfig*>* MIGUIVAT_ListControls::listItemConfig_generalSelectable = base::StaticsInit::AddCbGeneral<ArrayList<MenuItemConfig*>*>(1, []() {
	listItemConfig_generalSelectable = (new ArrayList<MenuItemConfig*>())
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

		->appendDirect_chain(new MenuItemConfig_Visual("imgIcon", "layer", VCC_Sprite::New(/*void*/), (new VisualPropertiesConfig())
			->put("width", 36.0f, "_dpm.width")
			->put("height", 36.0f, "_dpm.width")
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
			->put("tintColor", AppStyles::THEME_COLOR)
			->put("alpha", 0.8f)
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

ArrayList<StateChangeDurations*>* MIGUIVAT_ListControls::listItemConfig_animationDurations = base::StaticsInit::AddCbGeneral<ArrayList<StateChangeDurations*>*>(1, []() {
	listItemConfig_animationDurations = (new ArrayList<StateChangeDurations*>())
		->appendDirect_chain(new StateChangeDurations("_in_", "_in_", 0.15f))
		->appendDirect_chain(new StateChangeDurations("_out_", "_in_", AppStyles::ANIMATION_DURATION_S_DEFAULT))
		->appendDirect_chain(new StateChangeDurations("_in_", "_out_", AppStyles::ANIMATION_DURATION_S_DEFAULT));
});

MIGUIVAT_ListControls::MIGUIVAT_ListControls(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, base::IGameWorld* gw)
	: super(app, viewItemConfigs, viewEaseDurationsSList, gw)
{
	scrollListEventListener.cb_onEntryTouched = std::bind(&MIGUIVAT_ListControls::hScrollList_onEntryTouched, this, std::placeholders::_1, std::placeholders::_2);
}

void MIGUIVAT_ListControls::createVisuals(Map1D<std::string, void*>* createConfigMap) {
	super::createVisuals(createConfigMap);

	hScrollList = getItemMustExistAs< base::ScrollList2D* >("activeTool.listControls.hScrollList");
	hScrollList->setListener(&scrollListEventListener);
}

void MIGUIVAT_ListControls::updateGeneralEntryActiveStateVisuals(base::ListEntry2D* listEntry, bool isActive) {
	if (listEntry != nullptr) {
		if (!isActive && listEntry->getIsActive()) {
			listEntry->setIsActive(false, true);
		} else if (isActive && !listEntry->getIsActive()) {
			listEntry->setIsActive(true, true);

			hScrollList->scrollEntryIntoView(listEntry, true);
		}
	}
}

void MIGUIVAT_ListControls::hScrollList_onEntryTouched(base::List2D* list, base::ListEntry2D* listItem) {
	// Touch events are ignored while the menu is animating.
	if (!isActive() || selectedManualTool == nullptr || !isToolListControls) {
		return;
	}

	//asd_01;// check that it is main_btn and is down;

	if (dynamic_cast<void*>(list) == dynamic_cast<void*>(hScrollList)) {
		int listEntryType = listItem->getUserDataMap()->getDirectB("listEntryType")->getDirectAs_int();

		if (listEntryType == LIST_ENTRY_TYPE_GENERAL_SELECTABLE) {
			if (!listItem->getIsActive()) {
				int toolInstanceId = listItem->getUserDataMap()->getDirectB("toolInstanceId")->getDirectAs_int();
				if (toolInstanceId != selectedManualTool->getInstanceId()) {
					// This is from a different tool, ignre it.
					/// This should never occur but it is from 2 different system so allow it to happen without any error dispatching.

					return;
				}

				int toolControlUid = listItem->getUserDataMap()->getDirectB("toolControlUid")->getDirectAs_int();

				if (listenerToolExtraActivation != nullptr) {
					listenerToolExtraActivation->onToolExtraActivation_listControl_Down_MainBtn(toolInstanceId, toolControlUid);
				}
			}
		} else {
			throw LogicException(LOC);
		}
	} else {
		throw LogicException(LOC);
	}
}

void MIGUIVAT_ListControls::onSelectedFocusEntityChange_pre() {
	super::onSelectedFocusEntityChange_pre();

	isToolListControls = false;
	selectedManualTool_autoListenersList.clear();
	selectedManualTool = nullptr;
	syncHScrollListEntries();

	updateShowStateIfChanged(true, nullptr);
}

void MIGUIVAT_ListControls::onSelectedFocusEntityChanged(IWorldEntity* selectedFocusEntity, ArrayList<std::shared_ptr<IEventListener>>& selectedFocusEntity_autoListenersList) {
	super::onSelectedFocusEntityChanged(selectedFocusEntity, selectedFocusEntity_autoListenersList);

	if (selectedFocusEntity_toolsHandler != nullptr) {
		selectedFocusEntity->addEventListener(
			IToolsHandlerModule::EVENT_toolsHandler_slot_occupied->type, IToolsHandlerModule::EVENT_toolsHandler_slot_unoccupied->type,
			std::bind(&MIGUIVAT_ListControls::selectedFocusEntity_onEvent, this, std::placeholders::_1, std::placeholders::_2),
			selectedFocusEntity_autoListenersList
		);

		if (selectedFocusEntity_toolsHandler->getSelectedToolsCount_userSelectable() > 1) {
			//Currently only 1 active user selectable tool may be displayed with this u.i.
			throw LogicException(LOC);
		}
		if (selectedFocusEntity_toolsHandler->getSelectedToolsCount_userSelectable() == 1) {
			selectedManualTool = selectedFocusEntity_toolsHandler->getSelectedToolsList_userSelectable().getFirst()->data;
		}
		if (selectedManualTool != nullptr && selectedManualTool->getExtraActivationListControlsList() != nullptr && selectedManualTool->getExtraActivationListControlsList()->count() > 0) {
			isToolListControls = true;

			syncHScrollListEntries();

			selectedManualTool->getEvents().addEventListener(
				rpg3D::ITool::ToolEvent_extraActivationListControls_onSelectedChanged->type,
				std::bind(&MIGUIVAT_ListControls::selectedManualTool_extraActivationListControls, this, std::placeholders::_1, std::placeholders::_2),
				selectedManualTool_autoListenersList
			);
		} else {
			isToolListControls = false;

			syncHScrollListEntries();
		}
	}

	updateShowStateIfChanged(true, nullptr);
}

void MIGUIVAT_ListControls::selectedFocusEntity_onEvent(IEventListener& eventListener, base::EntityEvent& event) {
	if (event.type == IToolsHandlerModule::EVENT_toolsHandler_slot_occupied->type || event.type == IToolsHandlerModule::EVENT_toolsHandler_slot_unoccupied->type) {
		if (selectedFocusEntity_toolsHandler != nullptr) {
			if (selectedFocusEntity_toolsHandler->getSelectedToolsCount_userSelectable() > 1) {
				//Currently only 1 active user selectable tool may be displayed with this u.i.
				throw LogicException(LOC);
			}

			if (selectedFocusEntity_toolsHandler->getSelectedToolsCount_userSelectable() == 1) {
				selectedManualTool = selectedFocusEntity_toolsHandler->getSelectedToolsList_userSelectable().getFirst()->data;
			} else {
				selectedManualTool = nullptr;
			}
			if (selectedManualTool != nullptr && selectedManualTool->getExtraActivationListControlsList() != nullptr && selectedManualTool->getExtraActivationListControlsList()->count() > 0) {
				isToolListControls = true;

				syncHScrollListEntries();

				selectedManualTool->getEvents().addEventListener(
					rpg3D::ITool::ToolEvent_extraActivationListControls_onSelectedChanged->type,
					std::bind(&MIGUIVAT_ListControls::selectedManualTool_extraActivationListControls, this, std::placeholders::_1, std::placeholders::_2),
					selectedManualTool_autoListenersList
				);
			} else {
				isToolListControls = false;
				selectedManualTool_autoListenersList.clear();

				syncHScrollListEntries();
			}
		}

		updateShowStateIfChanged(true, nullptr);
	}

	else {
		throw LogicException(LOC);
	}
}

void MIGUIVAT_ListControls::selectedManualTool_extraActivationListControls(IEventListener& eventListener, rpg3D::ToolEvent& event) {
	if (event.type == rpg3D::ITool::ToolEvent_extraActivationListControls_onSelectedChanged->type) {
		if (selectedManualTool == nullptr || !isToolListControls || selectedManualTool.get() != event.tool) {
			// The tool is not active or u.i. not in sync with the tool.
			return;
		}

		if (event.entryUid_deselected >= 0) {
			// Deselect the entry;
			base::ListEntry2D* listEntry = hScrollList->getEtryWithUid(event.entryUid_deselected);

			updateGeneralEntryActiveStateVisuals(listEntry, false/*isActive*/);
		}
		if (event.entryUid_selected >= 0) {
			// Select the entry;
			base::ListEntry2D* listEntry = hScrollList->getEtryWithUid(event.entryUid_selected);

			updateGeneralEntryActiveStateVisuals(listEntry, true/*isActive*/);
		}
	}

	else {
		throw LogicException(LOC);
	}
}

void MIGUIVAT_ListControls::syncHScrollListEntries() {
	hScrollList->removeAllEntries();

	if (isToolListControls) {
		if (selectedManualTool->getExtraActivationListControlsList() != nullptr) {
			int i = 0;
			selectedManualTool->getExtraActivationListControlsList()->enumerateInOrderDirect([this , &i](TreeAVL<rpg3D::ITool::ExtraActivationListControl>::Node* node, rpg3D::ITool::ExtraActivationListControl& value, bool& stopEnumeration)->void {
				// Set the entryUid to use the fast entry fetch logic above.
				base::ListEntry2D* listEntry = hScrollList->createNewEntry(
					listItemConfig_generalSelectable, listItemConfig_animationDurations, i, nullptr,
					value.uid
				);

				listEntry->getUserDataMap()->putDirectB("listEntryType", base::IWrappedValue::new_int(LIST_ENTRY_TYPE_GENERAL_SELECTABLE));
				listEntry->getUserDataMap()->putDirectB("toolInstanceId", base::IWrappedValue::new_int(selectedManualTool->getInstanceId()));
				listEntry->getUserDataMap()->putDirectB("toolControlUid", base::IWrappedValue::new_int(value.uid));

				Sprite2D* imgIcon = listEntry->getItemMustExistAs<Sprite2D*>("imgIcon");
				TextureRegion* region = appAssets->getEmbeddedAssetsManager()->getTextureAtlasRegion(value.atlasId, value.regionId);
				if (region == nullptr) {
					throw LogicException(LOC);
				}
				imgIcon->setTextureRegion(region);

				updateGeneralEntryActiveStateVisuals(listEntry, value.isSelected);

				listEntry->show(false);

				i++;
			});
		}
	}
}

std::string MIGUIVAT_ListControls::computeStateFlags(std::string baseState) {
	std::string state = super::computeStateFlags(baseState);
	
	if (isToolListControls) {
		state += "_isToolListControls:true_";
	} else {
		state += "_isToolListControls:false_";
	}

	return state;
}

MIGUIVAT_ListControls::~MIGUIVAT_ListControls() {
	//void
}
