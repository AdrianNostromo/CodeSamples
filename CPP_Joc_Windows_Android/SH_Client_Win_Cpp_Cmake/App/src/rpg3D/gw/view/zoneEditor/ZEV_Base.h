#pragma once

#include <base/gh.h>
#include <base/view/UIView.h>
#include "IZoneEditorView_Edit.h"

class IAppUtils;
namespace base {
	class IBlockingInputLayer;
};

namespace rpg3D {
class ZEV_Base : public UIView, public virtual IZoneEditorView_Edit {priv typedef UIView super;pub dCtor(ZEV_Base);
	prot IAppUtils* appUtils;

	prot IZoneEditorM_Edit* entityEditor = nullptr;
	prot IListenerZoneEditorView* listener = nullptr;

	priv base::IBlockingInputLayer* blockingStreamLayer = nullptr;

    pub explicit ZEV_Base(
		IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList,
		IAppUtils* appUtils);

	pub void setHandlerAndListener(IZoneEditorM_Edit* entityEditor, IListenerZoneEditorView* listener) override;

	prot base::IBlockingInputLayer* getBlockingStreamLayer();
	prot void setBlockingStreamLayer(base::IBlockingInputLayer* blockingStreamLayer);

    pub ~ZEV_Base() override;
};
};
