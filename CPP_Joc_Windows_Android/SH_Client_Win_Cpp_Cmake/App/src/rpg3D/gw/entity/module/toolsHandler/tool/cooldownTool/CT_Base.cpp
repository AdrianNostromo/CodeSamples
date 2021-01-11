#include "CT_Base.h"
#include <rpg3D/gw/entity/module/toolsHandler/tool/config/ToolConfigPersistentCooldown.h>

using namespace rpg3D;

CT_Base::CT_Base(ToolConfigPersistentCooldown* config)
	: super(config)
{
	//void
}

ToolConfigPersistentCooldown* CT_Base::getLocalConfig() {
	return config->castAs<ToolConfigPersistentCooldown*>();
}

CT_Base::~CT_Base() {
	//void
}
