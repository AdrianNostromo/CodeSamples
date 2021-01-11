#include "TEERS_Base.h"
#include "config/ToolConfigEERemoverSingle.h"

using namespace rpg3D;

TEERS_Base::TEERS_Base(ToolConfigEERemoverSingle* config, std::shared_ptr<ExtraData> extraData)
	: super(config, extraData,
		true/*usesAutoToolModeActivationSound*/),
	config(config)
{
	//void
}

TEERS_Base::~TEERS_Base() {
	//void
}
