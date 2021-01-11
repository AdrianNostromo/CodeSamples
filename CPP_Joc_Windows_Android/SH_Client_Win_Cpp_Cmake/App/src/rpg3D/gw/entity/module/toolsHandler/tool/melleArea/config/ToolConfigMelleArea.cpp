#include <rpg3D/gw/entity/module/toolsHandler/tool/melleArea/ToolMelleArea.h>
#include "ToolConfigMelleArea.h"
#include <memory>
#include <base/list/Array1D.h>
#include <base/assets/IAssetsManager.h>
#include <base/app/IAppAssets.h>
#include <base/physics3D/config/body/PBodyConfig.h>

using namespace rpg3D;

std::string ToolConfigMelleArea::TAG_weapon = "weapon";
std::string ToolConfigMelleArea::TAG_sword = "sword";
std::string ToolConfigMelleArea::TAG_melle = "melle";

ToolConfigMelleArea::ToolConfigMelleArea(
	std::string id,
	int filtersPriority, 
	bool supportsContinuousReactivation,
	std::shared_ptr<std::string> icon_atlasId, std::shared_ptr<std::string> icon_regionId,
	std::string shortName_partA, std::string shortName_partB,
	float cooldownS, float channelingDurationS,
	ImpactAcknowledgeType impactAcknowledgeType, float reImpactOnPersistentContactCooldownS,
	Visual3DCreateConfig* toolVisualCreateConfig,
	std::string* activationSoundConfig_id, float activationSoundConfig_volume,
	std::string* impactSoundConfig_id, float impactSoundConfig_volume, float impactSoundConfig_overlapCooldownS,
	rpg3D::PBodyConfig* triggerPBodyConfig,
	float triggerDurationS,
	const Vector3 trigger_pBody_pos_start, const Vector3 trigger_pBody_pos_end,
	float trigger_pBody_scale_start, float trigger_pBody_scale_end,
	int maxTargetImpactsCount, float payloadPercentDecreasePerTargetImpact, float minPayloadsValuesMultiplier)
	: super(
		id,
		std::make_shared<Array1D<std::string>>(3, new std::string[3] {TAG_weapon, TAG_sword, TAG_melle}),
		filtersPriority, true/*isUserSelectable*/, true/*isDirectional*/, supportsContinuousReactivation, channelingDurationS,
		icon_atlasId, icon_regionId,
		shortName_partA, shortName_partB,
		cooldownS
	),
	impactAcknowledgeType(impactAcknowledgeType), reImpactOnPersistentContactCooldownS(reImpactOnPersistentContactCooldownS),
	toolVisualCreateConfig(toolVisualCreateConfig),
	activationSoundConfig_id(activationSoundConfig_id), activationSoundConfig_volume(activationSoundConfig_volume),
	impactSoundConfig_id(impactSoundConfig_id), impactSoundConfig_volume(impactSoundConfig_volume), impactSoundConfig_overlapCooldownS(impactSoundConfig_overlapCooldownS),
	triggerPBodyConfig(triggerPBodyConfig),
	triggerDurationS(triggerDurationS),
	trigger_pBody_pos_start(trigger_pBody_pos_start), trigger_pBody_pos_end(trigger_pBody_pos_end),
	trigger_pBody_scale_start(trigger_pBody_scale_start), trigger_pBody_scale_end(trigger_pBody_scale_end),
	maxTargetImpactsCount(maxTargetImpactsCount), payloadPercentDecreasePerTargetImpact(payloadPercentDecreasePerTargetImpact), minPayloadsValuesMultiplier(minPayloadsValuesMultiplier)
{
	//void
}

sp<Tool> ToolConfigMelleArea::createNewTool(IAppAssets* appAssets) {
	//return super.createNewTool();

	sp<ToolMelleArea> tool = msp<ToolMelleArea>(this, nullptr);
	tool->reservedCreate(appAssets->getEmbeddedAssetsManager());

	return tool;
}

ToolConfigMelleArea::~ToolConfigMelleArea() {
	if (triggerPBodyConfig != nullptr) {
		delete triggerPBodyConfig;
		triggerPBodyConfig = nullptr;
	}
}
