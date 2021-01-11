#include "MSVPHeader.h"
#include <base/visual2D/util/VCC_Group.h>
#include <base/visual2D/util/VCC_TouchArea.h>
#include <base/visual2D/util/VCC_Sprite.h>
#include <base/visual2D/util/VCC_SpriteButton.h>
#include <base/visual2D/util/VCC_BitmapLabel.h>
#include <base/menu/util/MenuItemConfig_ComposedVisual.h>
#include <base/menu/util/MenuItemConfig_Data.h>
#include <base/menu/util/MenuItemConfig_Visual.h>

using namespace sh;

MenuItemConfig* MSVPHeader::new_viewItemConfig_panel_header() {
	return new MenuItemConfig_ComposedVisual("header", (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("props", (new VisualPropertiesConfig())
			->put("width", 1.0f, "parent.props.width")
			->put("height", 142.0f, "_dpm.height")
		))
		 
		->appendDirect_chain(new MenuItemConfig_Visual("layer", "parent.layer", VCC_Group::New(), (new VisualPropertiesConfig())
			->put("x", 0.0f)
			->put("y",
			    1.0f, "parent.props.height",
			    VisualValueConfig::OPERATION_SUBTRACT,
			    1.f, "props.height"
			)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("profileBg", "layer", VCC_Sprite::New("visual2D/taGeneral.atlas", "mainSidebar/profileBg"), (new VisualPropertiesConfig())
			->put("width",
				288.0f, "_dpm.width",
				VisualValueConfig::OPERATION_MAX,
				1.0f, "props.width"
			)
			->put("height", 142.0f / 288.0f, "_self.width")
			->put("anchorX", 1.0f)
			->put("anchorY", 0.0f)
			->put("x", 1.0f, "props.width")
			->put("y", 0.0f)
		))

		/*asdA29
	    menuItemConfigs_panel_header_cloudContent,
	
	    menuItemConfigs_panel_header_contentOffline,
	    menuItemConfigs_panel_header_contentConnecting,
	    menuItemConfigs_panel_header_contentLoggedIn*/
	);
}

//asdA1;
//protected static final MenuItemConfig menuItemConfigs_panel_header = new MenuItemConfig_ComposedVisual("header", new MenuItemConfig[] {

//    new MenuItemConfig_Data("props", new VisualPropertiesConfig()
//       
//    ),
//
//    new MenuItemConfig_Visual("layer", "parent.layer", VisualCreateConfig.newGroup(), new VisualPropertiesConfig()

//    ),
//    new MenuItemConfig_Visual("", "layer", VisualCreateConfig.newSprite(""), new VisualPropertiesConfig()

//    ),
//

//});

MSVPHeader::MSVPHeader(
    IApp* app,
    ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList,
    worldGame3D::IAppGame* appGame, IAppUtils* appUtils)
    : super(app, viewItemConfigs, viewEaseDurationsSList, appGame, appUtils)
{
    //void
}

MSVPHeader::~MSVPHeader() {
    //void
}
