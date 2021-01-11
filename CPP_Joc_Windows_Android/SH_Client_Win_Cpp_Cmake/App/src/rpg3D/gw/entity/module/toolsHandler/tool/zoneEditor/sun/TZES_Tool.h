#pragma once

#include <base/gh.h>
#include "TZES_Base.h"
#include <base/math/Vector3Int.h>

namespace rpg3D {
class TZES_Tool : public TZES_Base {priv typedef TZES_Base super;pub dCtor(TZES_Tool);
	priv static const int TouchMode_TOOL;

    pub explicit TZES_Tool(ToolConfigZESun* config, std::shared_ptr<ExtraData> extraData);

	prot int computeTouchModeUnderPoint(ToolActivationInfo_Base* touchData) override;

	pub bool onFilterInteraction(int filterSource, std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>& interaction) override;

	prot void onIsOperatingChanged(bool isOperating) override;

	prot void tickActiveTouchMode(
		int touchMode, float deltaS,
		ToolActivationInfo_Base* touchData) override;

    pub ~TZES_Tool() override;
};
};
