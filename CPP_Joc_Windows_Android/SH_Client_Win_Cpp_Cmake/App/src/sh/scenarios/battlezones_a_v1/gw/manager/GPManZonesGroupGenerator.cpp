#include <sh/scenarios/battlezones_a_v1/gw/zone/ZoneTemplatesList.h>
#include <sh/scenarios/battlezones_a_v1/gw/config/entity/EntityIds.h>
#include "GPManZonesGroupGenerator.h"
#include <sh/scenarios/battlezones_a_v1/gw/config/entity/EntityTemplatesList.h>
#include <sh/scenarios/battlezones_a_v1/gw/zone/ZoneTemplate_Battle_WESN_A_V1.h>
#include <rpg3D/gw/entity/module/statusEffects/statusEffect/config/CfgStatusEffectKnockback.h>
#include <rpg3D/gw/entity/module/statusEffects/statusEffect/config/CfgStatusEffectStun.h>

using namespace battlezones_a_v1;

base::GWEvent* GPManZonesGroupGenerator::GWEvent_onZonesGroupFinished = new base::GWEvent("GWEvent_onZonesGroupFinished");

GPManZonesGroupGenerator::GPManZonesGroupGenerator(base::IGameWorld* gw)
	: super(gw)
{
	//void
}

void GPManZonesGroupGenerator::generateNewZonesGroup() {
	removeZoneConfigsWithGroupId(GENERATED_ZONES_GROUP_ID);

	buildZoneConfig(
		GENERATED_ZONES_GROUP_ID, 0,
		ZoneTemplatesList::Battle_WESN_A_V1,
		true, false
	);
	buildZoneConfig(
		GENERATED_ZONES_GROUP_ID, 1,
		ZoneTemplatesList::Battle_WESN_A_V1,
		false, false
	);
	buildZoneConfig(
		GENERATED_ZONES_GROUP_ID, 2,
		ZoneTemplatesList::Battle_WESN_A_V1,
		false, false
	);
	buildZoneConfig(
		GENERATED_ZONES_GROUP_ID, 3,
		ZoneTemplatesList::Battle_WESN_A_V1,
		false, false
	);

	ZoneConfig* zoneConfig = buildZoneConfig(
		GENERATED_ZONES_GROUP_ID, 4,
		ZoneTemplatesList::Battle_WESN_A_V1,
		false, true
	);
	zoneConfig->zoneClearedEventsInstancePointers.appendDirect(GWEvent_onZonesGroupFinished);
}

ZoneConfig* GPManZonesGroupGenerator::buildZoneConfig(
	std::string& zonesGroupId, int zoneIndex,
	ZoneTemplate* zoneTemplate,
	bool isPreviousZoneNexus, bool isNextZoneNexus)
{
	ZoneConfig* zoneConfig = createNewZoneConfig(zonesGroupId, std::to_string(zoneIndex));
	zoneConfig->zoneTemplateId = zoneTemplate->id;

	//Validate if needed teleporters exist in the template.
	if(zoneTemplate->getEnvironmentEntityWithInstanceId(EntityIds::platform_zoneTeleporter_previous) == nullptr
	   || zoneTemplate->getEnvironmentEntityWithInstanceId(EntityIds::platform_zoneTeleporter_next) == nullptr)
	{
		// Template doesn't contain the needed teleporters.
		throw LogicException(LOC);
	}

	// Set the 2 teleporters as needed.
	if(!isPreviousZoneNexus) {
		zoneConfig->addTeleporterTarget(
			*EntityIds::platform_zoneTeleporter_previous,
			zonesGroupId, std::to_string(zoneIndex - 1), EntityIds::platform_zoneTeleporter_next
		);

		zoneConfig->flags.setFlag(ZoneTemplate_Battle_WESN_A_V1::FLAG_isPreviousZoneNexus, false);
	}else {
		zoneConfig->addTeleporterTarget(
			*EntityIds::platform_zoneTeleporter_previous,
			GLOBAL_ZONES_GROUP_ID, ZONE_ID_NEXUS, EntityIds::platform_zonesGroupStart
		);

		zoneConfig->flags.setFlag(ZoneTemplate_Battle_WESN_A_V1::FLAG_isPreviousZoneNexus, true);
	}

	if(!isNextZoneNexus) {
		zoneConfig->addTeleporterTarget(
			*EntityIds::platform_zoneTeleporter_next,
			zonesGroupId, std::to_string(zoneIndex + 1), EntityIds::platform_zoneTeleporter_previous
		);

		zoneConfig->flags.setFlag(ZoneTemplate_Battle_WESN_A_V1::FLAG_isNextZoneNexus, false);
	}else {
		zoneConfig->addTeleporterTarget(
			*EntityIds::platform_zoneTeleporter_next,
			GLOBAL_ZONES_GROUP_ID, ZONE_ID_NEXUS, EntityIds::defaultSpawnLocation
		);

		zoneConfig->flags.setFlag(ZoneTemplate_Battle_WESN_A_V1::FLAG_isNextZoneNexus, true);
	}

	Vector3 pos{10.0f, 10.0f, 0.0f};
	Quaternion rot{};
	ZoneConfig::UnitSpawnConfig* unitSpawnConfig = zoneConfig->addUnitSpawnConfig(
		pos, rot,
		EntityTemplatesList::Mob_General_A_V1->nSpace,
		EntityTemplatesList::Mob_General_A_V1->id
	);

	unitSpawnConfig->impactPayloadsList.appendDirect(std::make_shared<rpg3D::EffectPayload>(
		rpg3D::EffectPayload::Types::physicalDamage, rpg3D::EffectPayload::Target::OTHER_ENTITY, 1.0f
	));

	std::shared_ptr<rpg3D::CfgStatusEffectKnockback> seKnockback = std::make_shared<rpg3D::CfgStatusEffectKnockback>(
		0.8f, //durationS
		0.9f, //distance
		0.0f //jumpHeight
	);
	std::shared_ptr<rpg3D::CfgStatusEffectStun> seStun = std::make_shared<rpg3D::CfgStatusEffectStun>(-1.0f);
	seKnockback->addSubEffect(std::static_pointer_cast<rpg3D::CfgStatusEffect>(seStun), true);
	unitSpawnConfig->impactPayloadsList.appendDirect(std::make_shared<rpg3D::EffectPayload>(
		rpg3D::EffectPayload::Types::statusEffect, rpg3D::EffectPayload::Target::SELF_ENTITY, std::static_pointer_cast<rpg3D::CfgStatusEffect>(seKnockback)
	));

	return zoneConfig;
}

GPManZonesGroupGenerator::~GPManZonesGroupGenerator() {
	//void
}
