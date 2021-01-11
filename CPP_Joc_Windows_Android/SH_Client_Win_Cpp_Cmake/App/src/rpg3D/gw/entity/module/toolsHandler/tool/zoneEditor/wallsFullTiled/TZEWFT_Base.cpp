#include "TZEWFT_Base.h"
#include "config/ToolConfigZEWallsFullTiled.h"

using namespace rpg3D;

TZEWFT_Base::TZEWFT_Base(ToolConfigZEWallsFullTiled* config, std::shared_ptr<ExtraData> extraData)
	: super(config, extraData,
		true/*usesAutoToolModeActivationSound*/),
	config(config)
{
	//void
}

TZEWFT_Base::~TZEWFT_Base() {
	//void
}
