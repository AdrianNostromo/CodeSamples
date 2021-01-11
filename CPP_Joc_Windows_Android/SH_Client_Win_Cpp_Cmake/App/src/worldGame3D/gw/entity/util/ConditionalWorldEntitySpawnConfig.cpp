#include "ConditionalWorldEntitySpawnConfig.h"
#include <base/container/flags/TestFlagEntry.h>

using namespace worldGame3D;

ConditionalWorldEntitySpawnConfig::ConditionalWorldEntitySpawnConfig(
	std::string& entityTemplateNSpace, std::string& entityTemplateId,
	Vector3& pos, Quaternion& rot,
	std::shared_ptr<std::string> instanceId)
	: entityTemplateNSpace(entityTemplateNSpace), entityTemplateId(entityTemplateId),
	pos(pos), rot(rot), 
	instanceId(instanceId)
{
	//void
}

ConditionalWorldEntitySpawnConfig::~ConditionalWorldEntitySpawnConfig() {
	//void
}
