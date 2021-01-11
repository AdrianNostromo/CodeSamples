#include "TZEA_Base.h"
#include "config/ToolConfigZEAssets.h"

using namespace rpg3D;

TZEA_Base::TZEA_Base(ToolConfigZEAssets* config, std::shared_ptr<ExtraData> extraData)
	: super(config, extraData,
		true/*usesAutoToolModeActivationSound*/),
	config(config)
{
	//void
}

TZEA_Base::~TZEA_Base() {
	//void
}
