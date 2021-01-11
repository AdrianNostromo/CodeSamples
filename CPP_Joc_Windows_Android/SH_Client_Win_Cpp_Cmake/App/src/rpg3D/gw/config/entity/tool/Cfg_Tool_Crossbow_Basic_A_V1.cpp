#include "Cfg_Tool_Crossbow_Basic_A_V1.h"
#include <base/list/Array3D.h>
#include <base/visual3d/util/Drawable3DCreateConfig_Group.h>
#include <base/visual3d/material/Visual3DModelMaterial_Color.h>
#include <rpg3D/config/sounds/SoundConfigsList.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/rangeInertialProjectile/config/ToolConfigRangeInertialProjectile.h>
#include <base/visual3d/util/Drawable3DCreateConfig_VoxelGrid.h>

using namespace rpg3D;

ToolConfigBase* Cfg_Tool_Crossbow_Basic_A_V1::getOrCreateTemplate() {
	std::string id = "Tool_Crossbow_Basic_A_V1";

	{
		rpg3D::ToolConfigBase* t = ToolConfigBase::FromId(id);
		if (t != nullptr) {
			return t;
		}
	}

	rpg3D::ToolConfigRangeInertialProjectile* cT = new rpg3D::ToolConfigRangeInertialProjectile(
        id,
		rpg3D::ToolConfigBase::FiltersPriority::manual/*filtersPriority*/,
		true/*supportsContinuousReactivation*/,
		std::make_shared<std::string>("visual2D/taGeneral.atlas"), std::make_shared<std::string>("sh/tools/weapons/crossbow_Basic_A_V1"),
		"Crossbow"/*shortName_partA*/, ""/*shortName_partB*/,
        0.75f/*cooldownS*/, 0.0f/*channelingDurationS*/,
		0.2f/*projectileLaunchPhysicsImpulse*/,
		msp<std::string>("_projectileSpawnParent_")/*projectilePreviewParent*/,
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
                     ->appendDirect_chain(new Drawable3DCreateConfig_Group(
	                     "_projectileSpawnParent_",
	                     new std::string("_ROOT_"),
	                     // Position is above the player with a 1 voxel spacing currently.
	                     new Vector3(2.0f * 0.04f, 0.0f * 0.04f, 0.0f * 0.04f),
	                     nullptr,
	                     nullptr// scale should not be applied here.
                     ))
                     ->appendDirect_chain(buildBase())
			)
        ),
        &SoundConfigsList::WoodBow_Fire_A_V1->managedPath->path, 0.25f
    );

	cT->projectileSpawnConfigsList.append_emplace("_projectileSpawnParent_"/*projectileSpawnParentId*/, 0.0f/*spawnAngZDeg*/, 2.0f/*maxSpawnAngZDegError*/);

	return cT;
}

Drawable3DCreateConfig_VoxelGrid* Cfg_Tool_Crossbow_Basic_A_V1::buildBase() {
	const signed char O = -1;
	const signed char W = 0; // Wood
	const signed char S = 1; // String

	Array3D<signed char>* padVoxelGrid = new Array3D<signed char>(
		4, 7, 1,
		false, true, true,
		new signed char[4 * 7 * 1] {
			S,W,O,O,
			S,O,W,O,
			S,O,O,W,
			S,O,O,W,
			S,O,O,W,
			S,O,W,O,
			S,W,O,O
		}
	);

	ArrayList<Visual3DModelMaterial*>* materialsList = new ArrayList<Visual3DModelMaterial*>();
	materialsList->appendDirect(new Visual3DModelMaterial_Color(
		new Color("#5E4A27FF")
	));
	materialsList->appendDirect(new Visual3DModelMaterial_Color(
		new Color("#D8D8D8FF")
	));

	return new Drawable3DCreateConfig_VoxelGrid(
		"_VoxelGridGroup_",//id
		new std::string("_ROOT_"),//parentId

		nullptr,// pos
		// Note rot is dynamic.
		new Quaternion(Vector3::X, 90.0f),
		nullptr,// scale

		0.04f,
		new Vector3(-1.0f, -(7.0f - 1.0f) / 2.0f, 0),

		padVoxelGrid, nullptr,
		materialsList, nullptr
	);
}
