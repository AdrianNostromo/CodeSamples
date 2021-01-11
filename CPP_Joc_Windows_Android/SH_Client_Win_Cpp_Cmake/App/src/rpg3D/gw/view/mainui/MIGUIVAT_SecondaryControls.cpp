#include "MIGUIVAT_SecondaryControls.h"
#include <base/menu/util/MenuItemConfig_ComposedVisual.h>
#include <base/menu/util/MenuItemConfig_Data.h>
#include <base/menu/util/MenuItemConfig_Visual.h>
#include <base/visual2D/util/VCC_Touchable.h>
#include <base/visual2D/util/VCC_Group.h>
#include <base/visual2D/util/VCC_Sprite.h>
#include <base/visual2D/util/VCC_TouchArea.h>
#include <base/visual2D/util/VCC_SpriteButton.h>
#include <rpg3D/config/sounds/SoundConfigsList.h>
#include <graphics/visual2d/drawable/SpriteButton2D.h>
#include <base/log/GlobalAccessLogHandler.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/entity/event/EntityEvent.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/event/ToolEvent.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/Tool.h>
#include <base/event/dispatcher/TypedEventDispatcher1D.h>
#include <rpg3D/gw/entity/module/toolsHandler/IToolsHandlerModule.h>
#include <base/app/IAppAssets.h>
#include <base/assets/IAssetsManager.h>
#include <graphics/visual2d/drawable/Sprite2D.h>

using namespace rpg3D;

MenuItemConfig* MIGUIVAT_SecondaryControls::new_viewItemConfig_activeTool_secondaryControls() {
	return new MenuItemConfig_ComposedVisual("secondaryControls", (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("props", (new VisualPropertiesConfig())
			//void
		))
		->appendDirect_chain(new MenuItemConfig_Visual("layer", "parent.layer", VCC_Group::New(), (new VisualPropertiesConfig())
			->putAnimating("alpha", (new MenuVisualAnimationProperty())
				->putState("_isToolSecondaryControls:true_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
			->putAnimating("_visibleFloat", (new MenuVisualAnimationProperty())
				->putState("_isToolSecondaryControls:true_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("bg_secondaryControl_0", "layer", VCC_Sprite::New(
			"visual2D/taUtilLinear.atlas", "bg/simple_white_FFFFFF"
		), (new VisualPropertiesConfig())
			->put("width", 48.0f, "_dpm.width")
			->put("height", 48.0f, "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				1.0f, "parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				-72.0f - 16.0f - (48.0f + 4.0f) * 0.0f/*index_from_right*/ - 48.0f / 2.0f, "_dpm.width"
			)
			->put("y", 4.0f + 24.0f + (48.0f + 4.0f) * 0.0f/*index_from_bottom*/, "_dpm.height")
			->put("alpha", 0.2f)
			->put("tintColor", Color("#000000FF"))
		))
		->appendDirect_chain(new MenuItemConfig_Visual("btn_secondaryControl_0", "layer", VCC_SpriteButton::New(
			"visual2D/taUtilLinear.atlas"/*atlasPath*/, "bg/simple_white_FFFFFF"/*buttonAssetsPrefix*/,
			&SoundConfigsList::UserInterface_Btn_Down->managedPath->path, 0.5f, nullptr, 0.5f, nullptr, 0.5f), (new VisualPropertiesConfig())
			->put("width", 48.0f, "_dpm.width")
			->put("height", 48.0f, "_dpm.height")
			->put("touchWidthOverride", std::make_shared<float>(52.0f), "_dpm.width")
			->put("touchHeightOverride", std::make_shared<float>(52.0f), "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				1.0f, "parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				-72.0f - 16.0f - (48.0f + 4.0f) * 0.0f/*index_from_right*/ - 48.0f / 2.0f, "_dpm.width"
			)
			->put("y", 4.0f + 24.0f + (48.0f + 4.0f) * 0.0f/*index_from_bottom*/, "_dpm.height")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("bg_secondaryControl_1", "layer", VCC_Sprite::New(
			"visual2D/taUtilLinear.atlas", "bg/simple_white_FFFFFF"
		), (new VisualPropertiesConfig())
			->put("width", 48.0f, "_dpm.width")
			->put("height", 48.0f, "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				1.0f, "parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				-72.0f - 16.0f - (48.0f + 4.0f) * 1.0f/*index_from_right*/ - 48.0f / 2.0f, "_dpm.width"
			)
			->put("y", 4.0f + 24.0f + (48.0f + 4.0f) * 0.0f/*index_from_bottom*/, "_dpm.height")
			->put("alpha", 0.2f)
			->put("tintColor", Color("#000000FF"))
		))
		->appendDirect_chain(new MenuItemConfig_Visual("btn_secondaryControl_1", "layer", VCC_SpriteButton::New(
			"visual2D/taUtilLinear.atlas"/*atlasPath*/, "bg/simple_white_FFFFFF"/*buttonAssetsPrefix*/,
			&SoundConfigsList::UserInterface_Btn_Down->managedPath->path, 0.5f, nullptr, 0.5f, nullptr, 0.5f), (new VisualPropertiesConfig())
			->put("width", 48.0f, "_dpm.width")
			->put("height", 48.0f, "_dpm.height")
			->put("touchWidthOverride", std::make_shared<float>(52.0f), "_dpm.width")
			->put("touchHeightOverride", std::make_shared<float>(52.0f), "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				1.0f, "parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				-72.0f - 16.0f - (48.0f + 4.0f) * 1.0f/*index_from_right*/ - 48.0f / 2.0f, "_dpm.width"
			)
			->put("y", 4.0f + 24.0f + (48.0f + 4.0f) * 0.0f/*index_from_bottom*/, "_dpm.height")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("bg_secondaryControl_2", "layer", VCC_Sprite::New(
			"visual2D/taUtilLinear.atlas", "bg/simple_white_FFFFFF"
		), (new VisualPropertiesConfig())
			->put("width", 48.0f, "_dpm.width")
			->put("height", 48.0f, "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				1.0f, "parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				-72.0f - 16.0f - (48.0f + 4.0f) * 2.0f/*index_from_right*/ - 48.0f / 2.0f, "_dpm.width"
			)
			->put("y", 4.0f + 24.0f + (48.0f + 4.0f) * 0.0f/*index_from_bottom*/, "_dpm.height")
			->put("alpha", 0.2f)
			->put("tintColor", Color("#000000FF"))
		))
		->appendDirect_chain(new MenuItemConfig_Visual("btn_secondaryControl_2", "layer", VCC_SpriteButton::New(
			"visual2D/taUtilLinear.atlas"/*atlasPath*/, "bg/simple_white_FFFFFF"/*buttonAssetsPrefix*/,
			&SoundConfigsList::UserInterface_Btn_Down->managedPath->path, 0.5f, nullptr, 0.5f, nullptr, 0.5f), (new VisualPropertiesConfig())
			->put("width", 48.0f, "_dpm.width")
			->put("height", 48.0f, "_dpm.height")
			->put("touchWidthOverride", std::make_shared<float>(52.0f), "_dpm.width")
			->put("touchHeightOverride", std::make_shared<float>(52.0f), "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				1.0f, "parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				-72.0f - 16.0f - (48.0f + 4.0f) * 2.0f/*index_from_right*/ - 48.0f / 2.0f, "_dpm.width"
			)
			->put("y", 4.0f + 24.0f + (48.0f + 4.0f) * 0.0f/*index_from_bottom*/, "_dpm.height")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("bg_secondaryControl_3", "layer", VCC_Sprite::New(
			"visual2D/taUtilLinear.atlas", "bg/simple_white_FFFFFF"
		), (new VisualPropertiesConfig())
			->put("width", 48.0f, "_dpm.width")
			->put("height", 48.0f, "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				1.0f, "parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				-72.0f - 16.0f - (48.0f + 4.0f) * 3.0f/*index_from_right*/ - 48.0f / 2.0f, "_dpm.width"
			)
			->put("y", 4.0f + 24.0f + (48.0f + 4.0f) * 0.0f/*index_from_bottom*/, "_dpm.height")
			->put("alpha", 0.2f)
			->put("tintColor", Color("#000000FF"))
		))
		->appendDirect_chain(new MenuItemConfig_Visual("btn_secondaryControl_3", "layer", VCC_SpriteButton::New(
			"visual2D/taUtilLinear.atlas"/*atlasPath*/, "bg/simple_white_FFFFFF"/*buttonAssetsPrefix*/,
			&SoundConfigsList::UserInterface_Btn_Down->managedPath->path, 0.5f, nullptr, 0.5f, nullptr, 0.5f), (new VisualPropertiesConfig())
			->put("width", 48.0f, "_dpm.width")
			->put("height", 48.0f, "_dpm.height")
			->put("touchWidthOverride", std::make_shared<float>(52.0f), "_dpm.width")
			->put("touchHeightOverride", std::make_shared<float>(52.0f), "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				1.0f, "parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				-72.0f - 16.0f - (48.0f + 4.0f) * 3.0f/*index_from_right*/ - 48.0f / 2.0f, "_dpm.width"
			)
			->put("y", 4.0f + 24.0f + (48.0f + 4.0f) * 0.0f/*index_from_bottom*/, "_dpm.height")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("bg_secondaryControl_4", "layer", VCC_Sprite::New(
			"visual2D/taUtilLinear.atlas", "bg/simple_white_FFFFFF"
		), (new VisualPropertiesConfig())
			->put("width", 48.0f, "_dpm.width")
			->put("height", 48.0f, "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				1.0f, "parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				-72.0f - 16.0f - (48.0f + 4.0f) * 0.0f/*index_from_right*/ - 48.0f / 2.0f, "_dpm.width"
			)
			->put("y", 4.0f + 24.0f + (48.0f + 4.0f) * 1.0f/*index_from_bottom*/, "_dpm.height")
			->put("alpha", 0.2f)
			->put("tintColor", Color("#000000FF"))
		))
		->appendDirect_chain(new MenuItemConfig_Visual("btn_secondaryControl_4", "layer", VCC_SpriteButton::New(
			"visual2D/taUtilLinear.atlas"/*atlasPath*/, "bg/simple_white_FFFFFF"/*buttonAssetsPrefix*/,
			&SoundConfigsList::UserInterface_Btn_Down->managedPath->path, 0.5f, nullptr, 0.5f, nullptr, 0.5f), (new VisualPropertiesConfig())
			->put("width", 48.0f, "_dpm.width")
			->put("height", 48.0f, "_dpm.height")
			->put("touchWidthOverride", std::make_shared<float>(52.0f), "_dpm.width")
			->put("touchHeightOverride", std::make_shared<float>(52.0f), "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				1.0f, "parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				-72.0f - 16.0f - (48.0f + 4.0f) * 0.0f/*index_from_right*/ - 48.0f / 2.0f, "_dpm.width"
			)
			->put("y", 4.0f + 24.0f + (48.0f + 4.0f) * 1.0f/*index_from_bottom*/, "_dpm.height")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("bg_secondaryControl_5", "layer", VCC_Sprite::New(
			"visual2D/taUtilLinear.atlas", "bg/simple_white_FFFFFF"
		), (new VisualPropertiesConfig())
			->put("width", 48.0f, "_dpm.width")
			->put("height", 48.0f, "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				1.0f, "parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				-72.0f - 16.0f - (48.0f + 4.0f) * 1.0f/*index_from_right*/ - 48.0f / 2.0f, "_dpm.width"
			)
			->put("y", 4.0f + 24.0f + (48.0f + 4.0f) * 1.0f/*index_from_bottom*/, "_dpm.height")
			->put("alpha", 0.2f)
			->put("tintColor", Color("#000000FF"))
		))
		->appendDirect_chain(new MenuItemConfig_Visual("btn_secondaryControl_5", "layer", VCC_SpriteButton::New(
			"visual2D/taUtilLinear.atlas"/*atlasPath*/, "bg/simple_white_FFFFFF"/*buttonAssetsPrefix*/,
			&SoundConfigsList::UserInterface_Btn_Down->managedPath->path, 0.5f, nullptr, 0.5f, nullptr, 0.5f), (new VisualPropertiesConfig())
			->put("width", 48.0f, "_dpm.width")
			->put("height", 48.0f, "_dpm.height")
			->put("touchWidthOverride", std::make_shared<float>(52.0f), "_dpm.width")
			->put("touchHeightOverride", std::make_shared<float>(52.0f), "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				1.0f, "parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				-72.0f - 16.0f - (48.0f + 4.0f) * 1.0f/*index_from_right*/ - 48.0f / 2.0f, "_dpm.width"
			)
			->put("y", 4.0f + 24.0f + (48.0f + 4.0f) * 1.0f/*index_from_bottom*/, "_dpm.height")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("bg_secondaryControl_6", "layer", VCC_Sprite::New(
			"visual2D/taUtilLinear.atlas", "bg/simple_white_FFFFFF"
		), (new VisualPropertiesConfig())
			->put("width", 48.0f, "_dpm.width")
			->put("height", 48.0f, "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				1.0f, "parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				-72.0f - 16.0f - (48.0f + 4.0f) * 2.0f/*index_from_right*/ - 48.0f / 2.0f, "_dpm.width"
			)
			->put("y", 4.0f + 24.0f + (48.0f + 4.0f) * 1.0f/*index_from_bottom*/, "_dpm.height")
			->put("alpha", 0.2f)
			->put("tintColor", Color("#000000FF"))
		))
		->appendDirect_chain(new MenuItemConfig_Visual("btn_secondaryControl_6", "layer", VCC_SpriteButton::New(
			"visual2D/taUtilLinear.atlas"/*atlasPath*/, "bg/simple_white_FFFFFF"/*buttonAssetsPrefix*/,
			&SoundConfigsList::UserInterface_Btn_Down->managedPath->path, 0.5f, nullptr, 0.5f, nullptr, 0.5f), (new VisualPropertiesConfig())
			->put("width", 48.0f, "_dpm.width")
			->put("height", 48.0f, "_dpm.height")
			->put("touchWidthOverride", std::make_shared<float>(52.0f), "_dpm.width")
			->put("touchHeightOverride", std::make_shared<float>(52.0f), "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				1.0f, "parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				-72.0f - 16.0f - (48.0f + 4.0f) * 2.0f/*index_from_right*/ - 48.0f / 2.0f, "_dpm.width"
			)
			->put("y", 4.0f + 24.0f + (48.0f + 4.0f) * 1.0f/*index_from_bottom*/, "_dpm.height")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("bg_secondaryControl_7", "layer", VCC_Sprite::New(
			"visual2D/taUtilLinear.atlas", "bg/simple_white_FFFFFF"
		), (new VisualPropertiesConfig())
			->put("width", 48.0f, "_dpm.width")
			->put("height", 48.0f, "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				1.0f, "parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				-72.0f - 16.0f - (48.0f + 4.0f) * 3.0f/*index_from_right*/ - 48.0f / 2.0f, "_dpm.width"
			)
			->put("y", 4.0f + 24.0f + (48.0f + 4.0f) * 1.0f/*index_from_bottom*/, "_dpm.height")
			->put("alpha", 0.2f)
			->put("tintColor", Color("#000000FF"))
		))

		->appendDirect_chain(new MenuItemConfig_Visual("btn_secondaryControl_7", "layer", VCC_SpriteButton::New(
			"visual2D/taUtilLinear.atlas"/*atlasPath*/, "bg/simple_white_FFFFFF"/*buttonAssetsPrefix*/,
			&SoundConfigsList::UserInterface_Btn_Down->managedPath->path, 0.5f, nullptr, 0.5f, nullptr, 0.5f), (new VisualPropertiesConfig())
			->put("width", 48.0f, "_dpm.width")
			->put("height", 48.0f, "_dpm.height")
			->put("touchWidthOverride", std::make_shared<float>(52.0f), "_dpm.width")
			->put("touchHeightOverride", std::make_shared<float>(52.0f), "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				1.0f, "parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				-72.0f - 16.0f - (48.0f + 4.0f) * 3.0f/*index_from_right*/ - 48.0f / 2.0f, "_dpm.width"
			)
			->put("y", 4.0f + 24.0f + (48.0f + 4.0f) * 1.0f/*index_from_bottom*/, "_dpm.height")
		))
	);
}

MIGUIVAT_SecondaryControls::MIGUIVAT_SecondaryControls(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, base::IGameWorld* gw)
	: super(app, viewItemConfigs, viewEaseDurationsSList, gw)
{
	touchListener.cb_onCursorDown = std::bind(&MIGUIVAT_SecondaryControls::v2d_onCursorDown, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
}

void MIGUIVAT_SecondaryControls::createVisuals(Map1D<std::string, void*>* createConfigMap) {
	super::createVisuals(createConfigMap);

	const int controlsCount = 8;
	for (int i = 0; i < controlsCount; i++) {
		std::string id = std::string("activeTool.secondaryControls.bg_secondaryControl_") + std::to_string(i);
		Sprite2D* bg = getItemMustExistAs<Sprite2D*>(id);

		id = std::string("activeTool.secondaryControls.btn_secondaryControl_") + std::to_string(i);
		base::SpriteButton2D* btn = getItemMustExistAs<base::SpriteButton2D*>(id);

		bg->setIsVisible(false);
		btn->setIsVisible(false);

		secondaryControlsArray.append_emplace(bg, btn);

		btn->addTouchListener(&touchListener);
	}
}

bool MIGUIVAT_SecondaryControls::v2d_onCursorDown(
	IInteractiveDrawable2D* target,
	int cursorIndex, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos, Vector2& localPos)
{
	// Touch events are ignored while the menu is animating.
	if (!isActive() || isAnimating() || !isToolSecondaryControls) {
		return false;
	}

	//asd_01;// check taht it is main_btn and is down;

	ControlEntry* secondaryControlUI = static_cast<ControlEntry*>(target->getExtraData_voidP());

	if (secondaryControlUI->toolInstanceId >= 0 && secondaryControlUI->controlUid >= 0) {
		if (listenerToolExtraActivation != nullptr) {
			listenerToolExtraActivation->onToolExtraActivation_secondaryControl_Down_MainBtn(secondaryControlUI->toolInstanceId, secondaryControlUI->controlUid);
		}
	}

	else {
		throw LogicException(LOC);
	}

	return true;
}

void MIGUIVAT_SecondaryControls::onSelectedFocusEntityChange_pre() {
	super::onSelectedFocusEntityChange_pre();

	isToolSecondaryControls = false;
	selectedManualTool_autoListenersList.clear();
	selectedManualTool = nullptr;
	syncSecondaryEntries();

	updateShowStateIfChanged(true, nullptr);
}

void MIGUIVAT_SecondaryControls::onSelectedFocusEntityChanged(IWorldEntity* selectedFocusEntity, ArrayList<std::shared_ptr<IEventListener>>& selectedFocusEntity_autoListenersList) {
	super::onSelectedFocusEntityChanged(selectedFocusEntity, selectedFocusEntity_autoListenersList);

	if (selectedFocusEntity_toolsHandler != nullptr) {
		selectedFocusEntity->addEventListener(
			IToolsHandlerModule::EVENT_toolsHandler_slot_occupied->type, IToolsHandlerModule::EVENT_toolsHandler_slot_unoccupied->type,
			std::bind(&MIGUIVAT_SecondaryControls::selectedFocusEntity_onEvent, this, std::placeholders::_1, std::placeholders::_2),
			selectedFocusEntity_autoListenersList
		);

		if (selectedFocusEntity_toolsHandler->getSelectedToolsCount_userSelectable() > 1) {
			//Currently only 1 active user selectable tool may be displayed with this u.i.
			throw LogicException(LOC);
		}
		if (selectedFocusEntity_toolsHandler->getSelectedToolsCount_userSelectable() == 1) {
			selectedManualTool = selectedFocusEntity_toolsHandler->getSelectedToolsList_userSelectable().getFirst()->data;
		}
		if (selectedManualTool != nullptr && selectedManualTool->getExtraActivationSecondaryControlsList() != nullptr && selectedManualTool->getExtraActivationSecondaryControlsList()->count() > 0) {
			isToolSecondaryControls = true;

			syncSecondaryEntries();

			selectedManualTool->getEvents().addEventListener(
				rpg3D::ITool::ToolEvent_extraActivationSecondaryControl_onConfigChanged->type,
				std::bind(&MIGUIVAT_SecondaryControls::selectedManualTool_extraActivationSecondaryControls, this, std::placeholders::_1, std::placeholders::_2),
				selectedManualTool_autoListenersList
			);
		} else {
			isToolSecondaryControls = false;

			syncSecondaryEntries();
		}
	}

	updateShowStateIfChanged(true, nullptr);
}

void MIGUIVAT_SecondaryControls::selectedFocusEntity_onEvent(IEventListener& eventListener, base::EntityEvent& event) {
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
			if (selectedManualTool != nullptr && selectedManualTool->getExtraActivationSecondaryControlsList() != nullptr && selectedManualTool->getExtraActivationSecondaryControlsList()->count() > 0) {
				isToolSecondaryControls = true;

				syncSecondaryEntries();

				selectedManualTool->getEvents().addEventListener(
					rpg3D::ITool::ToolEvent_extraActivationSecondaryControl_onConfigChanged->type,
					std::bind(&MIGUIVAT_SecondaryControls::selectedManualTool_extraActivationSecondaryControls, this, std::placeholders::_1, std::placeholders::_2),
					selectedManualTool_autoListenersList
				);
			} else {
				isToolSecondaryControls = false;
				selectedManualTool_autoListenersList.clear();

				syncSecondaryEntries();
			}
		}

		updateShowStateIfChanged(true, nullptr);
	}

	else {
		throw LogicException(LOC);
	}
}

void MIGUIVAT_SecondaryControls::selectedManualTool_extraActivationSecondaryControls(IEventListener& eventListener, rpg3D::ToolEvent& event) {
	if (event.type == rpg3D::ITool::ToolEvent_extraActivationSecondaryControl_onConfigChanged->type) {
		if (event.extraActivationSecondaryControl == nullptr) {
			throw LogicException(LOC);
		}

		if (selectedManualTool == nullptr || !isToolSecondaryControls || selectedManualTool.get() != event.tool) {
			// The tool is not active or u.i. not in sync with the tool.
			return;
		}

		ControlEntry* controlEntry = getEntryWithUid(event.extraActivationSecondaryControl->uid);
		if (controlEntry == nullptr) {
			// If this occurs, maybe don't throw.
			throw LogicException(LOC);
		}
		updateEntryConfig(*controlEntry, event.extraActivationSecondaryControl);
	}

	else {
		throw LogicException(LOC);
	}
}

void MIGUIVAT_SecondaryControls::syncSecondaryEntries() {
	int i = 0;
	if (isToolSecondaryControls) {
		if (selectedManualTool->getExtraActivationSecondaryControlsList() != nullptr) {
			selectedManualTool->getExtraActivationSecondaryControlsList()->enumerateInOrderDirect([this, &i](TreeAVL<rpg3D::ITool::ExtraActivationSecondaryControl>::Node* node, rpg3D::ITool::ExtraActivationSecondaryControl& value, bool& stopEnumeration)->void {
				// Set the entryUid to use the fast entry fetch logic above.
				ControlEntry& secondaryControlUI = secondaryControlsArray.getReference(i);

				secondaryControlUI.toolInstanceId = selectedManualTool->getInstanceId();
				secondaryControlUI.controlUid = value.uid;

				updateEntryConfig(secondaryControlUI, &value);

				if (!secondaryControlUI.btn->getIsVisible()) {
					secondaryControlUI.bg->setIsVisible(true);
					secondaryControlUI.btn->setIsVisible(true);
				}

				secondaryControlUI.btn->setExtraData_voidP(&secondaryControlUI);

				i++;
			});
		}
	}

	// Make the remaining buttons not visible.
	for (/*void*/; i < secondaryControlsArray.count(); i++) {
		ControlEntry& secondaryControlUI = secondaryControlsArray.getReference(i);

		if (!secondaryControlUI.btn->getIsVisible()) {
			// The remaining btn-s are not visible.
			break;
		}

		secondaryControlUI.bg->setIsVisible(false);
		secondaryControlUI.btn->setIsVisible(false);
	}
}

inline void MIGUIVAT_SecondaryControls::updateEntryConfig(ControlEntry& controlEntry, void*/* rpg3D::ITool::ExtraActivationSecondaryControl* */ extraActivationSecondaryControl) {
	rpg3D::ITool::ExtraActivationSecondaryControl* cExtraActivationSecondaryControl = static_cast<rpg3D::ITool::ExtraActivationSecondaryControl*>(extraActivationSecondaryControl);
	controlEntry.btn->setBtnStatesTexInfo(cExtraActivationSecondaryControl->atlasId, cExtraActivationSecondaryControl->regionID);
}

MIGUIVAT_SecondaryControls::ControlEntry* MIGUIVAT_SecondaryControls::getEntryWithUid(int entryUid) {
	for (int i=0; i < secondaryControlsArray.count(); i++) {
		ControlEntry* secondaryControlUI = secondaryControlsArray.getPointer(i);

		if (secondaryControlUI->controlUid == entryUid) {
			return secondaryControlUI;
		}
	}

	return nullptr;
}

std::string MIGUIVAT_SecondaryControls::computeStateFlags(std::string baseState) {
	std::string state = super::computeStateFlags(baseState);

	if (isToolSecondaryControls) {
		state += "_isToolSecondaryControls:true_";
	} else {
		state += "_isToolSecondaryControls:false_";
	}

	return state;
}

MIGUIVAT_SecondaryControls::~MIGUIVAT_SecondaryControls() {
	//void
}
