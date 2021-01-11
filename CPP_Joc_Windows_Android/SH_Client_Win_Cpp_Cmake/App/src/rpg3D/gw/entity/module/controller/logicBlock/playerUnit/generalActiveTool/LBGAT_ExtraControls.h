#pragma once

#include <base/gh.h>
#include "LBGAT_Base.h"

namespace rpg3D {
	class IToolsHandlerModule;
};

namespace rpg3D {
namespace playerUnit {
class LBGAT_ExtraControls : public LBGAT_Base {priv typedef LBGAT_Base super; pub dCtor(LBGAT_ExtraControls);
	priv ArrayList<std::shared_ptr<IEventListener>> localAutoListenersList{};

	prot rpg3D::IToolsHandlerModule* toolsHandler = nullptr;

	// This is used in edit mode to disable tools like stackable_pickup but keep movement and turn tools.
	priv bool isExtraAutoToolsEnabled = false;

    pub explicit LBGAT_ExtraControls();
	
	prot void onToolHooked(std::shared_ptr<util::WrappedTool> wTool) override;

	prot bool shouldManageConnectTool(std::shared_ptr<util::WrappedTool> wTool) override;

	prot void onIsActiveChanged(bool isActive_new, worldGame3D::IGameWorldGameParts* gwGameParts) override;
	
	priv void onEvent(IEventListener& eventListener, base::EntityEvent& event);

	priv void setIsExtraAutoToolsEnabled(bool isExtraAutoToolsEnabled);

    pub ~LBGAT_ExtraControls() override;
};
};
};
