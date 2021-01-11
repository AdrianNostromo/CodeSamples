#include "MIGUIVAT_Indicator.h"
#include "MIGUIV_DirectionalPad.h"
#include <base/menu/util/MenuItemConfig_ComposedVisual.h>
#include <base/menu/util/MenuItemConfig_Data.h>
#include <base/menu/util/MenuItemConfig_Visual.h>
#include <base/visual2D/util/VCC_Touchable.h>
#include <base/visual2D/util/VCC_Group.h>
#include <base/visual2D/util/VCC_Sprite.h>
#include <base/visual2D/util/VCC_TouchArea.h>
#include <rpg3D/config/sounds/SoundConfigsList.h>
#include <rpg3D/gw/entity/module/toolsHandler/IToolsHandlerModule.h>
#include <worldGame3D/gw/entity/event/EntityEvent.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <graphics/visual2d/drawable/ISprite2D.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/Tool.h>
#include <base/app/IAppAssets.h>
#include <base/assets/IAssetsManager.h>
#include <base/sensor/hapticFeedback/util/HapticFeedbackType.h>
#include <base/util/AppStyles.h>

using namespace rpg3D;

MenuItemConfig* MIGUIVAT_Indicator::new_viewItemConfig_activeTool_indicator() {
	return new MenuItemConfig_ComposedVisual("indicator", (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("props", (new VisualPropertiesConfig())
			//void
		))
		->appendDirect_chain(new MenuItemConfig_Visual("layer", "parent.layer", VCC_Group::New(), (new VisualPropertiesConfig())
			->putAnimating("alpha", (new MenuVisualAnimationProperty())
				->putState("_selectedToolStatus:on_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
			->putAnimating("_visibleFloat", (new MenuVisualAnimationProperty())
				->putState("_selectedToolStatus:on_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
		))

		->appendDirect_chain(new_viewItemConfig_activeTool_indicator_indicatorArea())

		->appendDirect_chain(new MenuItemConfig_Visual("iconMinimodeActive", "layer", VCC_Sprite::New(
			"visual2D/taGeneral.atlas", "rpg3D/gw/mainUi/iconToolSelectorMinimodeActive"
		), (new VisualPropertiesConfig())
			->put("width", 24.0f, "_dpm.width")
			->put("height", 24.0f, "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				1.0f, "parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				-22.0f, "_dpm.width"
			)
			->put("y", 64.0f, "_dpm.width")
			->put("tintColor", Color("#000000FF"))
			->putAnimating("alpha", (new MenuVisualAnimationProperty())
				->putState("_toolSelectorStatus:on_._toolSelectorMinimode:on_", AppStyles::ICON_DARK_ALPHA, EaseConfig::ACCELERATION)
				->putState("_", 0.0f, EaseConfig::ACCELERATION)
			)
			->putAnimating("_visibleFloat", (new MenuVisualAnimationProperty())
				->putState("_toolSelectorStatus:on_._toolSelectorMinimode:on_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
		))
	);
}

MenuItemConfig* MIGUIVAT_Indicator::new_viewItemConfig_activeTool_indicator_indicatorArea() {
	return new MenuItemConfig_ComposedVisual("indicatorArea", (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("props", (new VisualPropertiesConfig())
			->putAnimating("width", (new MenuVisualAnimationProperty())
				->putState("_selectedToolStatus:onWithTool_", 72.0f, "_dpm.width", EaseConfig::LINEAR)
				->putState("_selectedToolStatus:onNoTool_._toolSelectorStatus:on_", 72.0f, "_dpm.width", EaseConfig::LINEAR)
				->putState("_", 36.0f, "_dpm.width", EaseConfig::LINEAR)
			)
			->putAnimating("height", (new MenuVisualAnimationProperty())
				->putState("_selectedToolStatus:onWithTool_", 72.0f, "_dpm.width", EaseConfig::LINEAR)
				->putState("_selectedToolStatus:onNoTool_._toolSelectorStatus:on_", 72.0f, "_dpm.width", EaseConfig::LINEAR)
				->putState("_", 36.0f, "_dpm.width", EaseConfig::LINEAR)
			)
			->putAnimating("iconScale", (new MenuVisualAnimationProperty())
				->putState("_selectedToolStatus:onWithTool_", 1.0f, EaseConfig::LINEAR)
				->putState("_selectedToolStatus:onNoTool_._toolSelectorStatus:on_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
		))
		->appendDirect_chain(new MenuItemConfig_Visual("layer", "parent.layer", VCC_Group::New(), (new VisualPropertiesConfig())
			->putAnimating("x", (new MenuVisualAnimationProperty())
				->putState("_selectedToolStatus:on_", 1.0f, "parent.parent.props.width", EaseConfig::DECELERATION)
				->putState("_",
					1.0f, "parent.parent.props.width",
					VisualValueConfig::OPERATION_ADD,
					1.0f, "props.width",
					EaseConfig::ACCELERATION
				)
			)
			->putAnimating("y", (new MenuVisualAnimationProperty())
				->putState("_selectedToolStatus:on_", 0.0f, EaseConfig::DECELERATION)
				->putState("_", -1.0f, "props.width", EaseConfig::ACCELERATION)
			)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("taBg", "layer", VCC_TouchArea::New(&SoundConfigsList::UserInterface_Btn_Down->managedPath->path, 0.5f, base::HapticFeedbackType::NONE, nullptr, 0.5f), (new VisualPropertiesConfig())
			->put("width", 1.0f, "props.width")
			->put("height", 1.0f, "props.height")
			->put("anchorX", 1.0f)
			->put("anchorY", 0.0f)
			->put("x", 0.0f)
			->put("y", 0.0f)
		))
		->appendDirect_chain(new MenuItemConfig_Visual("bg", "layer", VCC_Sprite::New(
			"visual2D/taGeneral.atlas", "rpg3D/gw/mainUi/activeToolBg_A"
		), (new VisualPropertiesConfig())
			->put("width", 1.0f, "props.width")
			->put("height", 1.0f, "props.height")
			->put("anchorX", 1.0f)
			->put("anchorY", 0.0f)
			->put("x", 0.0f)
			->put("y", 0.0f)
			->put("tintColor", Color("#FFFFFFFF"))
			->put("alpha", 0.8f)
		))
		->appendDirect_chain(new MenuItemConfig_Visual("toolIcon", "layer", VCC_Sprite::New(/*optional texture*/), (new VisualPropertiesConfig())
			->put("width",
				48.0f, "_dpm.width",
				VisualValueConfig::OPERATION_MULTIPLY,
				1.0f, "props.iconScale"
			)
			->put("height",
				48.0f, "_dpm.height",
				VisualValueConfig::OPERATION_MULTIPLY,
				1.0f, "props.iconScale"
			)
			->put("anchorX", 1.0f)
			->put("anchorY", 0.0f)
			->put("x", -4.0f, "_dpm.width")
			->put("y", 4.0f, "_dpm.width")
			->put("alpha", 1.0f)
		))
	);
}

MIGUIVAT_Indicator::MIGUIVAT_Indicator(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, base::IGameWorld* gw)
	: super(app, viewItemConfigs, viewEaseDurationsSList, gw)
{
	touchListener.cb_onCursorDown = std::bind(&MIGUIVAT_Indicator::v2d_onCursorDown, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
}

void MIGUIVAT_Indicator::createVisuals(Map1D<std::string, void*>* createConfigMap) {
	super::createVisuals(createConfigMap);

	(taBg = getItemMustExistAs< IInteractiveDrawable2D*>("activeTool.indicator.indicatorArea.taBg"))->addTouchListener(&touchListener);

	toolIcon = getItemOptionalAs<ISprite2D*>("activeTool.indicator.indicatorArea.toolIcon");

	if (selectedFocusEntity_toolsHandler != nullptr) {
		if (selectedFocusEntity_toolsHandler->getSelectedToolsCount_userSelectable() > 1) {
			//Currently only 1 active user selectable tool may be displayed with this u.i.
			throw LogicException(LOC);
		}

		if (selectedFocusEntity_toolsHandler->getSelectedToolsCount_userSelectable() == 1) {
			sp<Tool> selectedManualTool = selectedFocusEntity_toolsHandler->getSelectedToolsList_userSelectable().getFirst()->data;
			if (selectedManualTool->config->icon_atlasId != nullptr) {
				TextureRegion* region = appAssets->getEmbeddedAssetsManager()->getTextureAtlasRegion(*selectedManualTool->config->icon_atlasId, *selectedManualTool->config->icon_regionId);
				if (region == nullptr) {
					throw LogicException(LOC);
				}

				hasSelectedManualTool = true;
				toolIcon->setTextureRegion(region);
			}
		}
	}
}

bool MIGUIVAT_Indicator::v2d_onCursorDown(
	IInteractiveDrawable2D* target,
	int cursorIndex, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos, Vector2& localPos)
{
	// Touch events are ignored while the menu is animating.
	if (!isActive()) {
		return false;
	}

	if (dynamic_cast<void*>(target) == dynamic_cast<void*>(taBg)) {
		if (getIsToolSelectorActiveAndMinimode()) {
			setIsToolSelectorActive(false, true);
		}else if (getIsToolSelectorActive()) {
			setIsToolSelectorActive(true, true);
		} else {
			setIsToolSelectorActive(true, false);
		}
	} else {
		throw LogicException(LOC);
	}

	return false;
}

void MIGUIVAT_Indicator::onSelectedFocusEntityChange_pre() {
	super::onSelectedFocusEntityChange_pre();
	
	hasAnyManuallyActivatedTool = false;
	hasSelectedManualTool = false;
	toolIcon->setTextureRegion(nullptr);

	updateShowStateIfChanged(true, nullptr);
}

void MIGUIVAT_Indicator::onSelectedFocusEntityChanged(IWorldEntity* selectedFocusEntity, ArrayList<std::shared_ptr<IEventListener>>& selectedFocusEntity_autoListenersList) {
	super::onSelectedFocusEntityChanged(selectedFocusEntity, selectedFocusEntity_autoListenersList);

	if (selectedFocusEntity_toolsHandler != nullptr) {
		selectedFocusEntity->addEventListener(
			IToolsHandlerModule::EVENT_toolsHandler_slot_occupied->type, IToolsHandlerModule::EVENT_toolsHandler_slot_unoccupied->type,
			std::bind(&MIGUIVAT_Indicator::selectedFocusEntity_onEvent, this, std::placeholders::_1, std::placeholders::_2),
			selectedFocusEntity_autoListenersList
		);
		selectedFocusEntity->addEventListener(
			IToolsHandlerModule::EVENT_toolsHandler_toolAdded->type, IToolsHandlerModule::EVENT_toolsHandler_toolRemoved->type,
			std::bind(&MIGUIVAT_Indicator::selectedFocusEntity_onEvent, this, std::placeholders::_1, std::placeholders::_2),
			selectedFocusEntity_autoListenersList
		);

		if (selectedFocusEntity_toolsHandler->getSelectedToolsCount_userSelectable() > 1) {
			//Currently only 1 active user selectable tool may be displayed with this u.i.
			throw LogicException(LOC);
		}
		if (selectedFocusEntity_toolsHandler->getSelectedToolsCount_userSelectable() == 1) {
			sp<Tool> selectedManualTool = selectedFocusEntity_toolsHandler->getSelectedToolsList_userSelectable().getFirst()->data;
			if (selectedManualTool->config->icon_atlasId != nullptr) {
				TextureRegion* region = appAssets->getEmbeddedAssetsManager()->getTextureAtlasRegion(*selectedManualTool->config->icon_atlasId, *selectedManualTool->config->icon_regionId);
				if (region == nullptr) {
					throw LogicException(LOC);
				}

				hasSelectedManualTool = true;
				toolIcon->setTextureRegion(region);
			}
		}

		hasAnyManuallyActivatedTool = selectedFocusEntity_toolsHandler->getUserSelectableToolsCount() > 0 ? true : false;
	}

	updateShowStateIfChanged(true, nullptr);
}

void MIGUIVAT_Indicator::selectedFocusEntity_onEvent(IEventListener& eventListener, base::EntityEvent& event) {
	if (event.type == IToolsHandlerModule::EVENT_toolsHandler_slot_occupied->type || event.type == IToolsHandlerModule::EVENT_toolsHandler_slot_unoccupied->type) {
		if (selectedFocusEntity_toolsHandler != nullptr) {
			if (selectedFocusEntity_toolsHandler->getSelectedToolsCount_userSelectable() > 1) {
				//Currently only 1 active user selectable tool may be displayed with this u.i.
				throw LogicException(LOC);
			}

			sp<Tool> selectedManualTool = nullptr;
			if (selectedFocusEntity_toolsHandler->getSelectedToolsCount_userSelectable() == 1) {
				selectedManualTool = selectedFocusEntity_toolsHandler->getSelectedToolsList_userSelectable().getFirst()->data;
			}
			if (selectedManualTool != nullptr && selectedManualTool->config->icon_atlasId != nullptr) {
				TextureRegion* region = appAssets->getEmbeddedAssetsManager()->getTextureAtlasRegion(*selectedManualTool->config->icon_atlasId, *selectedManualTool->config->icon_regionId);
				if (region == nullptr) {
					throw LogicException(LOC);
				}

				hasSelectedManualTool = true;
				toolIcon->setTextureRegion(region);
			}else {
				hasSelectedManualTool = false;
				toolIcon->setTextureRegion(nullptr);
			}

			bool new_hasAnyManuallyActivatedTool = selectedFocusEntity_toolsHandler->getUserSelectableToolsCount() > 0 ? true : false;
			if (new_hasAnyManuallyActivatedTool != hasAnyManuallyActivatedTool) {
				hasAnyManuallyActivatedTool = new_hasAnyManuallyActivatedTool;

				updateShowStateIfChanged(true, nullptr);
			}
		}
	}

	else if (event.type == IToolsHandlerModule::EVENT_toolsHandler_toolAdded->type) {
		if (selectedFocusEntity_toolsHandler != nullptr) {
			bool new_hasAnyManuallyActivatedTool = selectedFocusEntity_toolsHandler->getUserSelectableToolsCount() > 0 ? true : false;
			if (new_hasAnyManuallyActivatedTool != hasAnyManuallyActivatedTool) {
				hasAnyManuallyActivatedTool = new_hasAnyManuallyActivatedTool;

				updateShowStateIfChanged(true, nullptr);
			}
		}
	}else if (event.type == IToolsHandlerModule::EVENT_toolsHandler_toolRemoved->type) {
		if (selectedFocusEntity_toolsHandler != nullptr) {
			bool new_hasAnyManuallyActivatedTool = selectedFocusEntity_toolsHandler->getUserSelectableToolsCount() > 0 ? true : false;
			if (new_hasAnyManuallyActivatedTool != hasAnyManuallyActivatedTool) {
				hasAnyManuallyActivatedTool = new_hasAnyManuallyActivatedTool;

				updateShowStateIfChanged(true, nullptr);
			}
		}
	}

	else {
		throw LogicException(LOC);
	}
}

std::string MIGUIVAT_Indicator::computeStateFlags(std::string baseState) {
	std::string state = super::computeStateFlags(baseState);

	if (!hasAnyManuallyActivatedTool) {
		state += "_selectedToolStatus:off_";
	} else {
		state += "_selectedToolStatus:on_";

		if (!hasSelectedManualTool) {
			state += "_selectedToolStatus:onNoTool_";
		} else {
			state += "_selectedToolStatus:onWithTool_";
		}
	}

	return state;
}

MIGUIVAT_Indicator::~MIGUIVAT_Indicator() {
	//void
}
