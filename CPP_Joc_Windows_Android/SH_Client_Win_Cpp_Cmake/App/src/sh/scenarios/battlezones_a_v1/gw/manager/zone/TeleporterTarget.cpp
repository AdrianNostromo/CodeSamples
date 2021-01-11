#include "TeleporterTarget.h"

using namespace battlezones_a_v1;

TeleporterTarget::TeleporterTarget(std::string& zonesGroupId, std::string& zoneName, std::shared_ptr<std::string>& spawnLocationEntityId)
	: zonesGroupId(zonesGroupId), zoneName(zoneName), spawnLocationEntityId(spawnLocationEntityId)
{
	//void
}

TeleporterTarget::~TeleporterTarget() {
	//void
}
