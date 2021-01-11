#include "TEEPl_Base.h"
#include "config/ToolConfigEEPlacer.h"

using namespace rpg3D;

TEEPl_Base::TEEPl_Base(ToolConfigEEPlacer* config, std::shared_ptr<ExtraData> extraData)
	: super(config, extraData,
		true/*usesAutoToolModeActivationSound*/),
	config(config)
{
	//void
}

TEEPl_Base::~TEEPl_Base() {
	//void
}
