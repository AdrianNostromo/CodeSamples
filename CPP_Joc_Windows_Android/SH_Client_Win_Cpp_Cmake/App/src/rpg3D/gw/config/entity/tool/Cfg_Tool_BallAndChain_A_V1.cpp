#include "Cfg_Tool_BallAndChain_A_V1.h"
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

ToolConfigBase* Cfg_Tool_BallAndChain_A_V1::getOrCreateTemplate() {
	std::string id = "Tool_BallAndChain_A_V1";

	rpg3D::ToolConfigBase* t = ToolConfigBase::FromId(id);
	if (t != nullptr) {
		return t;
	}

	t = new rpg3D::ToolConfigMelleArea(
        id,
		rpg3D::ToolConfigBase::FiltersPriority::manual/*filtersPriority*/,
		true/*supportsContinuousReactivation*/,
		std::make_shared<std::string>("visual2D/taGeneral.atlas"), std::make_shared<std::string>("sh/tools/weapons/ballAndChain_A_V1"),
		"BallAndChain"/*shortName_partA*/, ""/*shortName_partB*/,
        0.75f/*cooldownS*/, 0.0f/*channelingDurationS*/,
		rpg3D::ToolConfigMelleArea::ImpactAcknowledgeType::OnlyFirstContact/*impactAcknowledgeType*/, -1.0f/*reImpactOnPersistentContactCooldownS*/,
		new Visual3DCreateConfig(
	        (new ArrayList<ArrayList<Drawable3DCreateConfigBase*>*>())
		        ->appendDirect_chain((new ArrayList<Drawable3DCreateConfigBase*>())
                     ->appendDirect_chain(new Drawable3DCreateConfig_Group(
	                     "_ROOT_",
	                     nullptr,
	                     nullptr,
	                     nullptr,
	                     nullptr
                     ))
                     ->appendDirect_chain(buildBase())
			)
        ),
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
		10.15f/*triggerDurationS*/,//asd_t;// 0.15f
		Vector3{ 0.0f, 0.0f, 0.0f }/*trigger_pBody_pos_start*/, Vector3{ 0.55f, 0.0f, 0.0f }/*trigger_pBody_pos_end*/,
		1.0f/*trigger_pBody_scale_start*/, 1.3f/*trigger_pBody_scale_end*/,
		// 12 units will be impacted maximum, each time the damage will be 0.2f less untill a lower limit of 0.2f.
		12/*maxTargetImpactsCount*/, 0.2f/*payloadPercentDecreasePerTargetImpact*/, 0.2f/*minPayloadsValuesMultiplier*/
    );

	return t;
}

Drawable3DCreateConfig_VoxelGrid* Cfg_Tool_BallAndChain_A_V1::buildBase() {
	const signed char O = -1;
	const signed char W = 0; // Wood
	const signed char M = 1; // Metal

	Vector3Int gSize{9, 5, 1};
	Array3D<signed char>* padVoxelGrid = new Array3D<signed char>(
		gSize.x, gSize.y, gSize.z,
		false, true, true,
		new signed char[gSize.x * gSize.y * gSize.z] {
			O,O,O,W,O,O,O,O,O,
			W,W,W,W,M,M,M,M,O,
			W,W,W,W,M,M,M,M,M,
			W,W,W,W,M,M,M,M,O,
			O,O,O,W,O,O,O,O,O,
		}
	);

	ArrayList<Visual3DModelMaterial*>* materialsList = new ArrayList<Visual3DModelMaterial*>();
	materialsList->appendDirect(new Visual3DModelMaterial_Color(
		new Color("#5E4A27FF")
	));
	materialsList->appendDirect(new Visual3DModelMaterial_Color(
		new Color("#888888FF")
	));

	Quaternion* q1 = new Quaternion{};
	q1->setEulerAnglesZYXDeg(0.0f/*yawZDeg*/, 0.0f/*pitchYDeg*/, 90.0f/*rollXDeg*/);

	return new Drawable3DCreateConfig_VoxelGrid(
		"_VoxelGridGroup_",//id
		new std::string("_ROOT_"),//parentId

		nullptr,// pos
		// Note rot is dynamic.
		q1,
		nullptr,// scale

		0.04f,
		new Vector3(0.0f/*forward_on_x*/, -2.0f, 0.0f/*to_the_side_outward*/),

		padVoxelGrid, nullptr,
		materialsList, nullptr
	);
}
