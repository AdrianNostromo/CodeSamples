#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/template/BasicModuleTemplate.h>

namespace rpg3D {
class ProjectileTemplate : public BasicModuleTemplate {priv typedef BasicModuleTemplate super;pub dCtor(ProjectileTemplate);
	pub enum ImpactAcknowledgeType { EachContact, OnlyFirstContact };

	// This can be overridden where needed. The current value is mostly used to for mobs to have an unlimited impacts.
	// -1; Unlimited contacts.// Use this for projectile_mobs.
	// >=1; Limited contacts count.// Use 1 for normal single hit projectiles.
	pub int maxImpactsCount;
	// This is used to remove projectiles when they hit a wall or bounce off it. This also makes projectile_mobs not be removed when they hit a wall.
	// -1; Unlimited contacts.// Use this for projectile_mobs.
	// >=1; Limited contacts count.// Use 1 for normal single hit projectiles.
	pub int maxEnvironmentImpactsCount;
	
	pub ImpactAcknowledgeType impactAcknowledgeType;
	// >= 0.0f; Re-impact enabled
	// <0.0f; No re-impacting.
	//asd_1;// implement in code;
	pub float reImpactOnPersistentContactCooldownS;

	pub bool freezeMovementOnEnvironmentImpact;
	pub bool disablePayloadImpactsOnEnvironmentImpact;

	pub std::string* impactSoundConfig_id;
	pub float impactSoundConfig_volume;
	pub float impactSoundConfig_overlapCooldownS;

	// eg. 0.1f will decreate the payload values by 0.1f for each impact until the minimum is reached.
	pub float payloadPercentDecreasePerTargetImpact;
	// The above multiplier is clipped by this.
	// eg. 0.2f will make the minimum payload multiplier be 0.2f;
	pub float minPayloadsValuesMultiplier;

	explicit ProjectileTemplate(
		int maxImpactsCount, int maxEnvironmentImpactsCount,
		ImpactAcknowledgeType impactAcknowledgeType, float reImpactOnPersistentContactCooldownS,
		bool freezeMovementOnEnvironmentImpact, bool disablePayloadImpactsOnEnvironmentImpact,
		std::string* impactSoundConfig_id, float impactSoundConfig_volume, float impactSoundConfig_overlapCooldownS,
		float payloadPercentDecreasePerTargetImpact, float minPayloadsValuesMultiplier);

	pub IEntityModule* createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) override;

	pub ~ProjectileTemplate() override;
};
};
