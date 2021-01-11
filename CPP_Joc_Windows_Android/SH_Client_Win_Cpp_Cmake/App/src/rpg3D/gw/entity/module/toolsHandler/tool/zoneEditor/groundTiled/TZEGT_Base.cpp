#include "TZEGT_Base.h"
#include "config/ToolConfigZEGroundTiled.h"

using namespace rpg3D;

TZEGT_Base::TZEGT_Base(ToolConfigZEGroundTiled* config, std::shared_ptr<ExtraData> extraData)
	: super(config, extraData,
		true/*usesAutoToolModeActivationSound*/),
	config(config)
{
	//void
}

TZEGT_Base::~TZEGT_Base() {
	//void
}
