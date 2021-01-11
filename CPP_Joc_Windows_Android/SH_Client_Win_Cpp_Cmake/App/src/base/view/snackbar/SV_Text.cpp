#include "SV_Text.h"
#include <base/menu/util/MenuItemConfig_Data.h>
#include <base/menu/util/MenuItemConfig_ComposedVisual.h>
#include <base/menu/util/MenuItemConfig_Visual.h>
#include <base/menu/util/VisualPropertiesConfig.h>
#include <base/visual2D/util/VisualCreateConfig.h>
#include <base/visual2D/util/VCC_Group.h>
#include <base/visual2D/util/VCC_Sprite.h>
#include <base/visual2D/util/VCC_BitmapLabel.h>
#include <base/visual2D/util/VCC_TouchArea.h>
#include <base/util/AppStyles.h>
#include <graphics/visual2d/drawable/BitmapLabel.h>
#include "util/Config.h"

using namespace base;
using namespace base::snackbar;

MenuItemConfig* SV_Text::new_menuItemConfigs_contentArea_textArea() {
	return new MenuItemConfig_ComposedVisual("textArea", (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("props", (new VisualPropertiesConfig())
			->put("width", 1.0f, "parent.props.width")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("layer", "parent.layer", VCC_Group::New(), (new VisualPropertiesConfig())
			->put("x", 16.0f, "_dpm.width")
			//->put("y", 1.0f, "parent.controlsAreaSize.height")//programatic
		))

		->appendDirect_chain(new MenuItemConfig_Visual("lblText", "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Regular-AaNSS-16-_dpFont_"), "Snackbar description."
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Regular)
			->put("fontSize", 16, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("width", 
				1.0f, "props.width",
				VisualValueConfig::OPERATION_ADD,
				-16.0f * 2.0f, "_dpm.width"
			)
			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("x", 0.0f, "_dpm.width")
			//->put("y", 0.0f, "_dpm.width")
			->put("tintColor", AppStyles::LIGHT_TEXT_COLOR)
			->put("alpha", AppStyles::LIGHT_PRIMARY_TEXT_ALPHA)
			->put("linesCountOverride", -1)// Enable unlimited lines wrap.
			/*programatic 
			->put("wrappingWidth", 
				1.0f, "props.width",
				VisualValueConfig::OPERATION_ADD,
				-16.0f * 2.0f, "_dpm.width"
			)*/
		))
	);
}

SV_Text::SV_Text(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, IHandlerAutoDisposal* handlerAutoDisposal, ISnackbarViewsInOrderDisplayHandler* viewInOrderDisplayHandler)
	: super(app, viewItemConfigs, viewEaseDurationsSList, handlerAutoDisposal, viewInOrderDisplayHandler)
{
	//void
}

void SV_Text::initializeSnackbarContent() {
	super::initializeSnackbarContent();

	contentArea_textArea_lblText->setTextReference(config->text);
}

void SV_Text::createVisuals(Map1D<std::string, void*>* createConfigMap) {
	super::createVisuals(createConfigMap);

	contentArea_textArea_lblText = getItemMustExistAs<base::BitmapLabel*>("contentArea.textArea.lblText");
}

void SV_Text::syncVisuals(bool syncAbsoluteValues, bool syncAnimatingValues, std::string startStateID, std::string endStateID, float stateCompletionPercent) {
	super::syncVisuals(syncAbsoluteValues, syncAnimatingValues, startStateID, endStateID, stateCompletionPercent);

	//void
}

SV_Text::~SV_Text() {
	//void
}
