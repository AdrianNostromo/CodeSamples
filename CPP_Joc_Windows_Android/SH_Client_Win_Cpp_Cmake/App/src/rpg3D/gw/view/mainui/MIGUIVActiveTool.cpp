#include "MIGUIVActiveTool.h"
#include <base/menu/util/MenuItemConfig_ComposedVisual.h>
#include <base/menu/util/MenuItemConfig_Data.h>
#include "MIGUIV_DirectionalPad.h"
#include <base/menu/util/MenuItemConfig_Visual.h>
#include <base/visual2D/util/VCC_Touchable.h>
#include <base/visual2D/util/VCC_Group.h>
#include <base/visual2D/util/VCC_Sprite.h>
#include <base/visual2D/util/VCC_TouchArea.h>

using namespace rpg3D;

MenuItemConfig* MIGUIVActiveTool::new_viewItemConfig_activeTool() {
	return new MenuItemConfig_ComposedVisual("activeTool", (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("props", (new VisualPropertiesConfig())
			->put("width", 1.0f, "parent.size.width")
			->put("height", 1.0f, "parent.size.height")
		))
		->appendDirect_chain(new MenuItemConfig_Visual("layer", "parent.layer", VCC_Group::New(), (new VisualPropertiesConfig())
			//void
		))

		->appendDirect_chain(new_viewItemConfig_activeTool_selector())
		->appendDirect_chain(new_viewItemConfig_activeTool_indicator())
		->appendDirect_chain(new_viewItemConfig_activeTool_secondaryControls())
		->appendDirect_chain(new_viewItemConfig_activeTool_listControls())
	);
}

MIGUIVActiveTool::MIGUIVActiveTool(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, base::IGameWorld* gw)
	: super(app, viewItemConfigs, viewEaseDurationsSList, gw)
{
	//void
}

MIGUIVActiveTool::~MIGUIVActiveTool() {
	//void
}
