#pragma once

#include <base/gh.h>
#include <rpg3D/gw/entity/module/controller/logicBlock/LogicBlock.h>
#include "ILBGeneralActiveTools.h"
#include <rpg3D/gw/entity/module/toolsHandler/tool/ITool.h>
#include <rpg3D/gw/entity/module/controller/logicBlock/util/WrappedTool.h>

namespace rpg3D {
	class IUserControlledModule;
};

namespace rpg3D {
namespace playerUnit {
class LBGAT_Base : public rpg3D::LogicBlock, virtual public ILBGeneralActiveTools {priv typedef rpg3D::LogicBlock super;pub dCtor(LBGAT_Base);
	// These are in order according to their filterPriority value.
	prot ArrayList<std::shared_ptr<util::WrappedTool>> hookedToolsListOrdered{};

	prot rpg3D::IUserControlledModule* userControlledModule = nullptr;

	pub explicit LBGAT_Base();

	prot void onIsActiveChanged(bool isActive_new, worldGame3D::IGameWorldGameParts* gwGameParts) override;

	prot void hookTool(sp<rpg3D::ITool> tool);
	prot virtual void onToolHooked(std::shared_ptr<util::WrappedTool> wTool);
	prot void releaseAllTools();
	prot void releaseWTool(std::shared_ptr<util::WrappedTool> wTool);
	prot void releaseHookedTool(sp<rpg3D::ITool> tool);

    pub void connectToGameParts(worldGame3D::IGameWorldGameParts* gwGameParts) override;

	prot std::shared_ptr<util::WrappedTool> getWToolFromTool(sp<rpg3D::ITool> tool);
	prot std::shared_ptr<util::WrappedTool> getWToolFromRawTool(rpg3D::ITool* rawTool);

	prot void managedConnectToolFilters(std::shared_ptr<util::WrappedTool> wTool);
	prot void managedDisconnectToolFilters(std::shared_ptr<util::WrappedTool> wTool);
	
	prot virtual bool shouldManageConnectTool(std::shared_ptr<util::WrappedTool> wTool);
	
	prot void syncToolsFilters();

    pub ~LBGAT_Base() override;
};
};
};
