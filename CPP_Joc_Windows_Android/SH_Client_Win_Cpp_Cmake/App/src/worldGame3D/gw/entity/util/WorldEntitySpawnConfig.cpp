#include "WorldEntitySpawnConfig.h"

using namespace worldGame3D;

WorldEntitySpawnConfig::WorldEntitySpawnConfig(
	Vector3& pos, Quaternion& rot,
	std::string& entityTemplateNSpace, std::string& entityTemplateId,
	sp<int> team,
	std::shared_ptr<ArrayList<std::shared_ptr<rpg3D::EffectPayload>>>& impactPayloadsList,
	rpg3D::ILogicBlock* logicBlock, void* logicBlockInterfaceRawPtr)
	: pos(pos), rot(rot), 
	entityTemplateNSpace(entityTemplateNSpace), entityTemplateId(entityTemplateId),
	team(team),
	impactPayloadsList(impactPayloadsList),
	logicBlock(logicBlock), logicBlockInterfaceRawPtr(logicBlockInterfaceRawPtr)
{
	//void
}

WorldEntitySpawnConfig::~WorldEntitySpawnConfig() {
	//void
}
