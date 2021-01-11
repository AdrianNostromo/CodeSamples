#include <rpg3D/gw/entity/module/toolsHandler/tool/rangeInertialProjectile/ToolRangeInertialProjectile.h>
#include "ToolConfigRangeInertialProjectile.h"
#include <memory>
#include <base/list/Array1D.h>
#include <base/assets/IAssetsManager.h>
#include <base/app/IAppAssets.h>

using namespace rpg3D;

std::string ToolConfigRangeInertialProjectile::TAG_weapon = "weapon";
std::string ToolConfigRangeInertialProjectile::TAG_bow = "bow";
std::string ToolConfigRangeInertialProjectile::TAG_range = "range";

ToolConfigRangeInertialProjectile::ToolConfigRangeInertialProjectile(
	std::string id,
	int filtersPriority,
	bool supportsContinuousReactivation,
	std::shared_ptr<std::string> icon_atlasId, std::shared_ptr<std::string> icon_regionId,
	std::string shortName_partA, std::string shortName_partB,
	float cooldownS, float channelingDurationS,
	float projectileLaunchPhysicsImpulse,
	sp<std::string> projectilePreviewParentId,
	Visual3DCreateConfig* toolVisualCreateConfig,
	std::string* activationSoundConfig_id, float activationSoundConfig_volume)
	: super(
		id,
		std::make_shared<Array1D<std::string>>(3, new std::string[3] {TAG_weapon, TAG_bow, TAG_range}),
		filtersPriority, true/*isUserSelectable*/, true/*isDirectional*/, supportsContinuousReactivation, channelingDurationS,
		icon_atlasId, icon_regionId,
		shortName_partA, shortName_partB,
		cooldownS
	),
	projectileLaunchPhysicsImpulse(projectileLaunchPhysicsImpulse),
	projectilePreviewParentId(projectilePreviewParentId), toolVisualCreateConfig(toolVisualCreateConfig),
	activationSoundConfig_id(activationSoundConfig_id), activationSoundConfig_volume(activationSoundConfig_volume)
{
	//void
}

sp<Tool> ToolConfigRangeInertialProjectile::createNewTool(IAppAssets* appAssets) {
	//return super.createNewTool();

	sp<ToolRangeInertialProjectile> tool = msp<ToolRangeInertialProjectile>(this, nullptr);
	tool->reservedCreate(appAssets->getEmbeddedAssetsManager());

	return tool;
}

ToolConfigRangeInertialProjectile::~ToolConfigRangeInertialProjectile() {
	//void
}
