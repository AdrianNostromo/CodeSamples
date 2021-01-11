#include "TZES_Base.h"
#include "config/ToolConfigZESun.h"

using namespace rpg3D;

TZES_Base::TZES_Base(ToolConfigZESun* config, std::shared_ptr<ExtraData> extraData)
	: super(config, extraData,
		true/*usesAutoToolModeActivationSound*/),
	config(config)
{
	//void
}

TZES_Base::~TZES_Base() {
	//void
}
