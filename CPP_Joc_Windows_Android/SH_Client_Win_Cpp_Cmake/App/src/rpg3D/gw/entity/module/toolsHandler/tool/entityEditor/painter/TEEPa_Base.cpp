#include "TEEPa_Base.h"
#include "config/ToolConfigEEPainter.h"

using namespace rpg3D;

TEEPa_Base::TEEPa_Base(ToolConfigEEPainter* config, std::shared_ptr<ExtraData> extraData)
	: super(config, extraData,
		true/*usesAutoToolModeActivationSound*/),
	config(config)
{
	//void
}

TEEPa_Base::~TEEPa_Base() {
	//void
}
