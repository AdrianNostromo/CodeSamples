#pragma once

#include <string>
#include <memory>

namespace battlezones_a_v1 {
class TeleporterTarget {
public:
	std::string zonesGroupId;
	std::string zoneName;

	std::shared_ptr<std::string> spawnLocationEntityId;

public:
	TeleporterTarget(TeleporterTarget const&) = delete;
	TeleporterTarget(TeleporterTarget &&) = default;
	TeleporterTarget& operator=(TeleporterTarget const&) = delete;
	TeleporterTarget& operator=(TeleporterTarget &&) = default;

	explicit TeleporterTarget(std::string& zonesGroupId, std::string& zoneName, std::shared_ptr<std::string>& spawnLocationEntityId);

	virtual ~TeleporterTarget();
};
};
