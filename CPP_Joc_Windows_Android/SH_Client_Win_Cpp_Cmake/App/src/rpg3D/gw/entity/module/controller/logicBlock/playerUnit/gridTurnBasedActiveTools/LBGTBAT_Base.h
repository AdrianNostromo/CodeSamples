#pragma once

#include <base/gh.h>
#include "ILBGridTurnBasedActiveTools.h"
#include <rpg3D/gw/entity/module/toolsHandler/tool/ITool.h>
#include <rpg3D/gw/entity/module/controller/logicBlock/util/WrappedTool.h>
#include "../generalActiveTool/LBGeneralActiveTools.h"

namespace rpg3D {
	class IToolsHandlerModule;
};

namespace rpg3D {
namespace playerUnit {
class LBGTBAT_Base : public LBGeneralActiveTools, virtual public ILBGridTurnBasedActiveTools {priv typedef LBGeneralActiveTools super;pub dCtor(LBGTBAT_Base);
	prot rpg3D::IToolsHandlerModule* toolsHandler = nullptr;

	pub explicit LBGTBAT_Base();

	prot void onIsActiveChanged(bool isActive_new, worldGame3D::IGameWorldGameParts* gwGameParts) override;

    pub ~LBGTBAT_Base() override;
};
};
};
