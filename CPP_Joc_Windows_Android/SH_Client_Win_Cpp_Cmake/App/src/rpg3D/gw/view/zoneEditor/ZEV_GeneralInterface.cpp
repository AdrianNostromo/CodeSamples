#include "ZEV_GeneralInterface.h"
#include <base/menu/util/MenuItemConfig_ComposedVisual.h>
#include <base/menu/util/MenuItemConfig_Data.h>
#include <base/menu/util/MenuItemConfig_Visual.h>
#include <base/visual2D/util/VCC_Group.h>
#include <base/visual2D/util/VCC_TouchArea.h>
#include <base/visual2D/util/VCC_Sprite.h>
#include <base/visual2D/util/VCC_SpriteButton.h>
#include <rpg3D/config/sounds/SoundConfigsList.h>
#include <graphics/visual2d/drawable/SpriteButton2D.h>
#include <rpg3D/gw/view/zoneEditor/util/IListenerZoneEditorView.h>

using namespace rpg3D;

MenuItemConfig* ZEV_GeneralInterface::new_viewItemConfig_topArea() {
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

		->appendDirect_chain(new MenuItemConfig_Visual("btnBack", "layer", VCC_SpriteButton::New("visual2D/taGeneral.atlas", "rpg3D/gw/zoneEditor/view/toolbarMain/btnBack_", &SoundConfigsList::UserInterface_Btn_Down->managedPath->path, 0.5f, nullptr, 0.5f, nullptr, 0.5f), (new VisualPropertiesConfig())
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
	);
}

ZEV_GeneralInterface::ZEV_GeneralInterface(
	IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList,
	IAppUtils* appUtils)
	: super(
		app, viewItemConfigs, viewEaseDurationsSList,
		appUtils
	)
{
	touchListener.cb_onCursorDown = std::bind(&ZEV_GeneralInterface::v2d_onCursorDown, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
}

bool ZEV_GeneralInterface::v2d_onCursorDown(
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
			listener->onBtn(this, IListenerZoneEditorView::ButtonIds::CLOSE);
		}
	}

	else {
		throw LogicException(LOC);
	}

	return true;
}

void ZEV_GeneralInterface::createVisuals(Map1D<std::string, void*>* createConfigMap) {
	super::createVisuals(createConfigMap);

	(btnBack = getItemMustExistAs<base::SpriteButton2D*>("topArea.btnBack"))->addTouchListener(&touchListener);
}

std::string ZEV_GeneralInterface::computeStateFlags(std::string baseState) {
	std::string state = super::computeStateFlags(baseState);

	//void

	return state;
}

ZEV_GeneralInterface::~ZEV_GeneralInterface() {
	//void
}
