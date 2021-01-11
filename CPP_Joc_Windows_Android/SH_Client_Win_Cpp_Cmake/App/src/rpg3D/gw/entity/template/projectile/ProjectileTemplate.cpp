#include <rpg3D/gw/entity/module/projectile/ProjectileModule.h>
#include "ProjectileTemplate.h"

using namespace rpg3D;

ProjectileTemplate::ProjectileTemplate(
	int maxImpactsCount, int maxEnvironmentImpactsCount,
	ImpactAcknowledgeType impactAcknowledgeType, float reImpactOnPersistentContactCooldownS,
	bool freezeMovementOnEnvironmentImpact, bool disablePayloadImpactsOnEnvironmentImpact,
	std::string* impactSoundConfig_id, float impactSoundConfig_volume, float impactSoundConfig_overlapCooldownS,
	float payloadPercentDecreasePerTargetImpact, float minPayloadsValuesMultiplier)
	: super("projectile", IProjectileModule::TYPE),
	maxImpactsCount(maxImpactsCount), maxEnvironmentImpactsCount(maxEnvironmentImpactsCount),
	impactAcknowledgeType(impactAcknowledgeType), reImpactOnPersistentContactCooldownS(reImpactOnPersistentContactCooldownS),
	freezeMovementOnEnvironmentImpact(freezeMovementOnEnvironmentImpact), disablePayloadImpactsOnEnvironmentImpact(disablePayloadImpactsOnEnvironmentImpact),
	impactSoundConfig_id(impactSoundConfig_id), impactSoundConfig_volume(impactSoundConfig_volume), impactSoundConfig_overlapCooldownS(impactSoundConfig_overlapCooldownS),
	payloadPercentDecreasePerTargetImpact(payloadPercentDecreasePerTargetImpact), minPayloadsValuesMultiplier(minPayloadsValuesMultiplier)
{
    //void
}

IEntityModule* ProjectileTemplate::createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) {
	ProjectileTemplate* t = dynamic_cast<ProjectileTemplate*>(_t);
	if(!t) {
		throw LogicException(LOC);
	}

	ProjectileModule* mod = new ProjectileModule(entity, t);

	return mod;
}

ProjectileTemplate::~ProjectileTemplate() {
    //void
}
