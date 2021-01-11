#include "TEECPi_Base.h"
#include "config/ToolConfigEEColorPicker.h"

using namespace rpg3D;

TEECPi_Base::TEECPi_Base(ToolConfigEEColorPicker* config, std::shared_ptr<ExtraData> extraData)
	: super(config, extraData,
		false/*usesAutoToolModeActivationSound*/),
	config(config)
{
	//void
}

TEECPi_Base::~TEECPi_Base() {
	//void
}
