#include "Cfg_Tool_ChickenBeak_A_V1.h"
#include <base/list/Array3D.h>
#include <base/visual3d/util/Drawable3DCreateConfig_Group.h>
#include <base/visual3d/material/Visual3DModelMaterial_Color.h>
#include <rpg3D/config/sounds/SoundConfigsList.h>
#include <base/physics3D/config/body/PBodyConfig.h>
#include <base/physics3D/config/shape/PhysicsShapeConfigCylinder.h>
#include <rpg3D/gw/physics3D/util/PhysicsUtil.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/melleArea/config/ToolConfigMelleArea.h>
#include <base/visual3d/util/Drawable3DCreateConfig_VoxelGrid.h>

using namespace rpg3D;

ToolConfigBase* Cfg_Tool_ChickenBeak_A_V1::getOrCreateTemplate() {
	std::string id = "Tool_ChickenBeak_A_V1";

	rpg3D::ToolConfigBase* t = ToolConfigBase::FromId(id);
	if (t != nullptr) {
		return t;
	}

	t = new rpg3D::ToolConfigMelleArea(
        id,
		rpg3D::ToolConfigBase::FiltersPriority::manual/*filtersPriority*/,
		true/*supportsContinuousReactivation*/,
		nullptr/*icon_atlasId*/, nullptr/*icon_regionId*/,
		"ChickenBeak"/*shortName_partA*/, ""/*shortName_partB*/,
        0.75f/*cooldownS*/, 0.0f/*channelingDurationS*/,
		rpg3D::ToolConfigMelleArea::ImpactAcknowledgeType::OnlyFirstContact/*impactAcknowledgeType*/, -1.0f/*reImpactOnPersistentContactCooldownS*/,
		nullptr/*toolVisualCreateConfig*/,
        &SoundConfigsList::Sword_Activate_A_V1->managedPath->path/*activationSoundConfig_id*/, 0.25f/*activationSoundConfig_volume*/,
        &SoundConfigsList::Sword_Impact_A_V1->managedPath->path/*impactSoundConfig_id*/, 0.25f/*impactSoundConfig_volume*/, 0.2f/*impactSoundConfig_overlapCooldownS*/,
		new rpg3D::PBodyConfig(
			rpg3D::PBodyConfig::UssageBitGroups::None,
			rpg3D::PBodyConfig::DynamicsType::Kinematic/*dynamicsType*/,
			nullptr/*physicsBodyOffsetPos*/, nullptr/*physicsBodyOffsetRot*/,
			new physics3D::PhysicsShapeConfigCylinder(
				rpg3D::PhysicsUtil::DEFAULT_COLLISSION_MARGIN/*collisionMargin*/,
				Vector3::Z/*mainAxis*/,
				Vector3(0.13f, 0.13f, 0.15f)/*halfExtents*/
			)/*pShapeConfig*/,
			0.0f/*kinematic*//*mass*/, 0.0f/*friction*/, 0.0f/*rollingFriction*/,
			Vector3{ 1, 1, 1 }/*enabledRotationAxes*/,
			0.0f/*linearDamping*/, 0.0f/*angularDamping*/,
			nullptr/*customGravity*/

		)/*triggerPBodyConfig*/,
		0.15f/*triggerDurationS*/,
		Vector3{ 0.0f, 0.0f, 0.0f }/*trigger_pBody_pos_start*/, Vector3{ 0.25f, 0.0f, 0.0f }/*trigger_pBody_pos_end*/,
		1.0f/*trigger_pBody_scale_start*/, 1.3f/*trigger_pBody_scale_end*/,
		// 12 units will be impacted maximum, each time the damage will be 0.2f less untill a lower limit of 0.2f.
		12/*maxTargetImpactsCount*/, 0.2f/*payloadPercentDecreasePerTargetImpact*/, 0.2f/*minPayloadsValuesMultiplier*/
    );

	return t;
}
