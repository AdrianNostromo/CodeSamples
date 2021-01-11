#include "ZoneConfig.h"

using namespace battlezones_a_v1;

ZoneConfig::ZoneConfig(std::string& groupId, std::string& zoneName)
	: groupId(groupId), zoneName(zoneName)
{
	//void
}

ArrayList<ZoneConfig::UnitSpawnConfig>* ZoneConfig::getMobsSpawnsList() {
	return &mobsSpawnsList;
}

ZoneConfig::UnitSpawnConfig* ZoneConfig::addUnitSpawnConfig(Vector3& pos, Quaternion& rot, std::string& entityTemplateNSpace, std::string& entityTemplateId) {
	return mobsSpawnsList.append_emplace(pos, rot, entityTemplateNSpace, entityTemplateId);
}

TeleporterTarget* ZoneConfig::getTeleporterTarget(std::string& teleporterEntityId) {
	std::shared_ptr<TeleporterTarget> entry = teleporterTargetsMap.getDirect(teleporterEntityId, nullptr);
	if(entry != nullptr) {
		return entry.get();
	}

	return nullptr;
}

void ZoneConfig::addTeleporterTarget(
	std::string& teleporterEntityId,
	std::string& zonesGroupId, std::string zoneName, std::shared_ptr<std::string>& spawnLocationEntityId)
{
	if(getTeleporterTarget(teleporterEntityId) != nullptr) {
		throw LogicException(LOC);
	}

	std::shared_ptr<TeleporterTarget> entry = std::make_shared<TeleporterTarget>(zonesGroupId, zoneName, spawnLocationEntityId);

	teleporterTargetsMap.putReference(teleporterEntityId, entry);
}

ZoneConfig::~ZoneConfig() {
	//void
}
