#include "TEERL_Base.h"
#include "config/ToolConfigEERemoverLaser.h"

using namespace rpg3D;

TEERL_Base::TEERL_Base(ToolConfigEERemoverLaser* config, std::shared_ptr<ExtraData> extraData)
	: super(config, extraData,
		true/*usesAutoToolModeActivationSound*/),
	config(config)
{
	//void
}

TEERL_Base::~TEERL_Base() {
	//void
}
