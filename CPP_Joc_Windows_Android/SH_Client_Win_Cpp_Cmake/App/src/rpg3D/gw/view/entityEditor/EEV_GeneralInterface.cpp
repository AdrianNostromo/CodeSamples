#include "EEV_GeneralInterface.h"
#include <base/menu/util/MenuItemConfig_ComposedVisual.h>
#include <base/menu/util/MenuItemConfig_Data.h>
#include <base/menu/util/MenuItemConfig_Visual.h>
#include <base/visual2D/util/VCC_Group.h>
#include <base/visual2D/util/VCC_TouchArea.h>
#include <base/visual2D/util/VCC_Sprite.h>
#include <base/visual2D/util/VCC_SpriteButton.h>
#include <base/visual2D/VisualUtil2D.h>
#include <base/log/GlobalAccessLogHandler.h>
#include <base/app/IAppAssets.h>
#include <rpg3D/config/sounds/SoundConfigsList.h>
#include <base/sensor/hapticFeedback/util/HapticFeedbackType.h>
#include <rpg3D/gw/view/entityEditor/util/IListenerEntityEditorView.h>
#include <rpg3D/gw/entity/module/editableVoxelised/IEditableVoxelisedM_Edit.h>
#include <rpg3D/gw/entity/template/editableVoxelised/EditableVoxelisedTemplate.h>
#include <graphics/visual2d/drawable/SpriteButton2D.h>

using namespace rpg3D;

MenuItemConfig* EEV_GeneralInterface::new_viewItemConfig_topArea() {
	return new MenuItemConfig_ComposedVisual("topArea", (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("props", (new VisualPropertiesConfig())
			->put("width", 1.0f, "parent.props.width")
			//->put("height", 56.0f, "_dpm.height")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("layer", "parent.layer", VCC_Group::New(), (new VisualPropertiesConfig())
			->put("x", 0.0f)
			->putAnimating("y", (new MenuVisualAnimationProperty())
				->putState("_in_._colorsBar:true_",
					1.0f, "parent.props.height",
					VisualValueConfig::OPERATION_ADD,
					-56.0f * 2.0f, "_dpm.height",
					EaseConfig::LINEAR
				)
				->putState("_in_",
					1.0f, "parent.props.height",
					VisualValueConfig::OPERATION_ADD,
					-56.0f, "_dpm.height",
					EaseConfig::DECELERATION
				)
				->putState("_",
					1.0f, "parent.props.height",
					EaseConfig::ACCELERATION
				)
			)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("bg_btnBack", "layer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "bg/simple_white_FFFFFF"), (new VisualPropertiesConfig())
			->put("width", 48.0f, "_dpm.width")
			->put("height", 48.0f, "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->putAnimating("x", (new MenuVisualAnimationProperty())
				->putState("_in_._colorsBar:true_",
					4.0f + 48.0f + 4.0f + 24.0f, "_dpm.width",
					EaseConfig::LINEAR
				)
				->putState("_",
					4.0f + 24.0f, "_dpm.width",
					EaseConfig::LINEAR
				)
			)
			->put("y", 4.0f + 24.0f, "_dpm.height")
			->put("tintColor", Color("#000000FF"))
			->put("alpha", 0.2f)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("btnBack", "layer", VCC_SpriteButton::New("visual2D/taGeneral.atlas", "rpg3D/gw/entityEditor/view/toolbarMain/btnBack_", &SoundConfigsList::UserInterface_Btn_Down->managedPath->path, 0.5f, nullptr, 0.5f, nullptr, 0.5f), (new VisualPropertiesConfig())
			->put("width", 24.0f, "_dpm.width")
			->put("height", 24.0f, "_dpm.height")
			->put("touchWidthOverride", std::make_shared<float>(56.0f), "_dpm.width")
			->put("touchHeightOverride", std::make_shared<float>(56.0f), "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->putAnimating("x", (new MenuVisualAnimationProperty())
				->putState("_in_._colorsBar:true_",
					4.0f + 48.0f + 4.0f + 24.0f, "_dpm.width",
					EaseConfig::LINEAR
				)
				->putState("_",
					4.0f + 24.0f, "_dpm.width",
					EaseConfig::LINEAR
				)
			)
			->put("y", 4.0f + 24.0f, "_dpm.height")
			->put("tintColor", Color("#111111FF"))
			->put("alpha", 0.4f)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("bg_btnSettingsDrawerA", "layer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "bg/simple_white_FFFFFF"), (new VisualPropertiesConfig())
			->put("width", 48.0f, "_dpm.width")
			->put("height", 48.0f, "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->putAnimating("x", (new MenuVisualAnimationProperty())
				->putState("_in_._colorsBar:true_",
					1.0f, "parent.props.width",
					VisualValueConfig::OPERATION_ADD,
					-4.0f - 48.0f - 4.0f - 24.0f, "_dpm.width",
					EaseConfig::LINEAR
				)
				->putState("_",
					1.0f, "parent.props.width",
					VisualValueConfig::OPERATION_ADD,
					-4.0f - 24.0f, "_dpm.width",
					EaseConfig::LINEAR
				)
			)
			->put("y", 4.0f + 24.0f, "_dpm.height")
			->put("tintColor", Color("#000000FF"))
			->put("alpha", 0.2f)
		))
		->appendDirect_chain(new MenuItemConfig_Visual("btnSettingsDrawerA", "layer", VCC_SpriteButton::New(
			"visual2D/taGeneral.atlas", "rpg3D/gw/entityEditor/view/toolbarMain/btnSettingsDrawrA_", 
			&SoundConfigsList::UserInterface_Btn_Down->managedPath->path, 0.5f, 
			nullptr, 0.5f, 
			nullptr, 0.5f
		), (new VisualPropertiesConfig())
			->put("width", 24.0f, "_dpm.width")
			->put("height", 24.0f, "_dpm.height")
			->put("touchWidthOverride", std::make_shared<float>(44.0f), "_dpm.width")
			->put("touchHeightOverride", std::make_shared<float>(44.0f), "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->putAnimating("x", (new MenuVisualAnimationProperty())
				->putState("_in_._colorsBar:true_",
					1.0f, "parent.props.width",
					VisualValueConfig::OPERATION_ADD,
					-4.0f - 48.0f - 4.0f - 24.0f, "_dpm.width",
					EaseConfig::LINEAR
				)
				->putState("_",
					1.0f, "parent.props.width",
					VisualValueConfig::OPERATION_ADD,
					-4.0f - 24.0f, "_dpm.width",
					EaseConfig::LINEAR
				)
			)
			->put("y", 4.0f + 24.0f, "_dpm.height")
			->put("tintColor", Color("#111111FF"))
			->put("alpha", 0.4f)
		))

		->appendDirect_chain(new_menuItemConfigs_topArea_toolbarColors())
	);
}

MenuItemConfig* EEV_GeneralInterface::new_viewItemConfig_bottomArea() {
	return new MenuItemConfig_ComposedVisual("bottomArea", (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("props", (new VisualPropertiesConfig())
			->put("width", 1.0f, "parent.props.width")
			//->put("height", 56.0f, "_dpm.height")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("layer", "parent.layer", VCC_Group::New(), (new VisualPropertiesConfig())
			->put("x", 0.0f)
			->putAnimating("y", (new MenuVisualAnimationProperty())
				->putState("_in_",
					56.0f, "_dpm.height",
					EaseConfig::LINEAR
				)
				->putState("_",
					0.0f, "_dpm.height",
					EaseConfig::LINEAR
				)
			)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("bg_btnResetRotation", "layer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "bg/simple_white_FFFFFF"), (new VisualPropertiesConfig())
			->putAnimating("_visibleFloat", (new MenuVisualAnimationProperty())
				->putState("_EditorStructure:enabled_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
			->putAnimating("alpha", (new MenuVisualAnimationProperty())
				->putState("_EditorStructure:enabled_", 0.2f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
			->put("width", 48.0f, "_dpm.width")
			->put("height", 48.0f, "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				1.0f, "parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				-72.0f - 16.0f - (48.0f + 4.0f) * 2.0f - 48.0f / 2.0f, "_dpm.width"
			)
			->put("y", -4.0f - 24.0f, "_dpm.height")
			->put("tintColor", Color("#000000FF"))
		))
		->appendDirect_chain(new MenuItemConfig_Visual("btnResetRotation", "layer", VCC_SpriteButton::New("visual2D/taGeneral.atlas", "rpg3D/gw/entityEditor/view/toolbarMain/btnResetRotation_", &SoundConfigsList::UserInterface_Btn_Down->managedPath->path, 0.5f, nullptr, 0.5f, nullptr, 0.5f), (new VisualPropertiesConfig())
			->putAnimating("_visibleFloat", (new MenuVisualAnimationProperty())
				->putState("_EditorStructure:enabled_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
			->putAnimating("alpha", (new MenuVisualAnimationProperty())
				->putState("_EditorStructure:enabled_", 0.4f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
			->put("width", 24.0f, "_dpm.width")
			->put("height", 24.0f, "_dpm.height")
			->put("touchWidthOverride", std::make_shared<float>(44.0f), "_dpm.width")
			->put("touchHeightOverride", std::make_shared<float>(44.0f), "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				1.0f, "parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				-72.0f - 16.0f - (48.0f + 4.0f) * 2.0f - 48.0f / 2.0f, "_dpm.width"
			)
			->put("y", -4.0f - 24.0f, "_dpm.height")
			->put("tintColor", Color("#111111FF"))
		))

		->appendDirect_chain(new MenuItemConfig_Visual("bg_btnUndo", "layer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "bg/simple_white_FFFFFF"), (new VisualPropertiesConfig())
			->put("width", 48.0f, "_dpm.width")
			->put("height", 48.0f, "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				1.0f, "parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				-72.0f - 16.0f - (48.0f + 4.0f) * 1.0f - 48.0f / 2.0f, "_dpm.width"
			)
			->put("y", -4.0f - 24.0f, "_dpm.height")
			->put("tintColor", Color("#000000FF"))
			->put("alpha", 0.2f)
		))
		->appendDirect_chain(new MenuItemConfig_Visual("btnUndo", "layer", VCC_SpriteButton::New("visual2D/taGeneral.atlas", "rpg3D/gw/entityEditor/view/toolbarMain/btnUndo_", &SoundConfigsList::UserInterface_Btn_Down->managedPath->path, 0.5f, nullptr, 0.5f, nullptr, 0.5f), (new VisualPropertiesConfig())
			->put("width", 24.0f, "_dpm.width")
			->put("height", 24.0f, "_dpm.height")
			->put("touchWidthOverride", std::make_shared<float>(44.0f), "_dpm.width")
			->put("touchHeightOverride", std::make_shared<float>(44.0f), "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				1.0f, "parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				-72.0f - 16.0f - (48.0f + 4.0f) * 1.0f - 48.0f / 2.0f, "_dpm.width"
			)
			->put("y", -4.0f - 24.0f, "_dpm.height")
			->put("tintColor", Color("#111111FF"))
			->put("alpha", 0.4f)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("bg_btnRedo", "layer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "bg/simple_white_FFFFFF"), (new VisualPropertiesConfig())
			->put("width", 48.0f, "_dpm.width")
			->put("height", 48.0f, "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				1.0f, "parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				-72.0f - 16.0f - (48.0f + 4.0f) * 0.0f - 48.0f / 2.0f, "_dpm.width"
			)
			->put("y", -4.0f - 24.0f, "_dpm.height")
			->put("tintColor", Color("#000000FF"))
			->put("alpha", 0.2f)
		))
		->appendDirect_chain(new MenuItemConfig_Visual("btnRedo", "layer", VCC_SpriteButton::New("visual2D/taGeneral.atlas", "rpg3D/gw/entityEditor/view/toolbarMain/btnRedo_", &SoundConfigsList::UserInterface_Btn_Down->managedPath->path, 0.5f, nullptr, 0.5f, nullptr, 0.5f), (new VisualPropertiesConfig())
			->put("width", 24.0f, "_dpm.width")
			->put("height", 24.0f, "_dpm.height")
			->put("touchWidthOverride", std::make_shared<float>(44.0f), "_dpm.width")
			->put("touchHeightOverride", std::make_shared<float>(44.0f), "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				1.0f, "parent.props.width",
				VisualValueConfig::OPERATION_ADD,
				-72.0f - 16.0f - (48.0f + 4.0f) * 0.0f - 48.0f / 2.0f, "_dpm.width"
			)
			->put("y", -4.0f - 24.0f, "_dpm.height")
			->put("tintColor", Color("#111111FF"))
			->put("alpha", 0.4f)
		))
	);
}

EEV_GeneralInterface::EEV_GeneralInterface(
	IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList,
	IAppUtils* appUtils)
	: super(
		app, viewItemConfigs, viewEaseDurationsSList,
		appUtils
	)
{
	touchListener.cb_onCursorDown = std::bind(&EEV_GeneralInterface::v2d_onCursorDown, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
}

void EEV_GeneralInterface::onEntityToEditChanged() {
	super::onEntityToEditChanged();

	if (entityToEdit != nullptr) {
		btnUndo->setEnabled(entityToEdit->getIsHistryUndoAvailable());
		btnRedo->setEnabled(entityToEdit->getIsHistryRedoAvailable());
	}

	updateShowStateIfChanged(true, nullptr);
}

bool EEV_GeneralInterface::v2d_onCursorDown(
	IInteractiveDrawable2D* target,
	int cursorIndex, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos, Vector2& localPos)
{
	// Touch events are ignored while the menu is animating.
	if (!isActive() || isAnimating()) {
		return false;
	}
	
	if (dynamic_cast<void*>(target) == dynamic_cast<void*>(btnBack)) {
		if (listener != nullptr) {
			listener->onBtn(this, IListenerEntityEditorView::ButtonIds::CLOSE);
		}
	} else if (dynamic_cast<void*>(target) == dynamic_cast<void*>(btnSettingsDrawerA)) {
		toggleSettingsDrawerA();
	} else if (dynamic_cast<void*>(target) == dynamic_cast<void*>(btnResetRotation)) {
		if (entityToEdit != nullptr) {
			entityToEdit->resetEditEntityRotation();
		}
	} else if (dynamic_cast<void*>(target) == dynamic_cast<void*>(btnUndo)) {
		if (entityToEdit != nullptr) {
			entityToEdit->historyUndo();
		}
	} else if (dynamic_cast<void*>(target) == dynamic_cast<void*>(btnRedo)) {
		if (entityToEdit != nullptr) {
			entityToEdit->historyRedo();
		}
	}
	
	else {
		throw LogicException(LOC);
	}

	return true;
}

void EEV_GeneralInterface::createVisuals(Map1D<std::string, void*>* createConfigMap) {
	super::createVisuals(createConfigMap);
	
	(btnBack = getItemMustExistAs<base::SpriteButton2D*>("topArea.btnBack"))->addTouchListener(&touchListener);
	(btnSettingsDrawerA = getItemMustExistAs<base::SpriteButton2D*>("topArea.btnSettingsDrawerA"))->addTouchListener(&touchListener);

	(btnResetRotation = getItemMustExistAs<base::SpriteButton2D*>("bottomArea.btnResetRotation"))->addTouchListener(&touchListener);

	(btnUndo = getItemMustExistAs<base::SpriteButton2D*>("bottomArea.btnUndo"))->addTouchListener(&touchListener);
	(btnRedo = getItemMustExistAs<base::SpriteButton2D*>("bottomArea.btnRedo"))->addTouchListener(&touchListener);
}

void EEV_GeneralInterface::onHistoryUndoAvailableChanged(bool isHistryRedoAvailable) {
	if (btnUndo != nullptr) {
		btnUndo->setEnabled(isHistryRedoAvailable);
	}
}

void EEV_GeneralInterface::onHistoryRedoAvailableChanged(bool isHistryRedoAvailable) {
	if (btnRedo != nullptr) {
		btnRedo->setEnabled(isHistryRedoAvailable);
	}
}

std::string EEV_GeneralInterface::computeStateFlags(std::string baseState) {
	std::string state = super::computeStateFlags(baseState);

	if (entityToEdit != nullptr && entityToEdit->getTemplate()->isEditorStructureEnabled == true) {
		state = state + "_EditorStructure:enabled_";
	} else {
		state = state + "_EditorStructure:disabled_";
	}

	return state;
}

EEV_GeneralInterface::~EEV_GeneralInterface() {
	//void
}
