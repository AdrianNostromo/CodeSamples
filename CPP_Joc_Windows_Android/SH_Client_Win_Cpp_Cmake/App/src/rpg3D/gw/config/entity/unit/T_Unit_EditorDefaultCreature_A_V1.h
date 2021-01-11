#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/template/EntityTemplate.h>
#include <string>
#include <worldGame3D/gw/entity/template/visual3D/Visual3DTemplate.h>
#include <rpg3D/gw/config/entity/EntityTags.h>
#include <base/visual3d/util/Drawable3DCreateConfigBase.h>
#include <base/visual3d/util/Drawable3DCreateConfig_Group.h>
#include <base/visual3d/util/Drawable3DCreateConfig_VoxelGrid.h>
#include <base/math/Vector3Int.h>
#include <base/list/Array3D.h>

namespace rpg3D {
class T_Unit_EditorDefaultCreature_A_V1 {
	pub static std::string DATA_KEY_voxelGridZXY;
	pub static std::string DATA_KEY_materials;

	pub static float default_voxelSize;
	pub static float default_stackable_haulOffsetZM;
	pub static Vector3 default_voxelGridOffset;
	pub static Vector3 default_fixedSizeAABBToushShape_min;
	pub static Vector3 default_fixedSizeAABBToushShape_max;

	pub static Array3D<signed char>* chicken_voxelGridZXY;
	pub static ArrayList<Visual3DModelMaterial*>* chicken_materialsList;

	pub static Array3D<signed char>* flatPad_voxelGridZXY;
	pub static ArrayList<Visual3DModelMaterial*>* flatPad_materialsList;

	priv static int templateVariationIdCounter;
	pub static EntityTemplate* getNew(
		float voxelSize, Vector3& voxelGridOffset,
		Vector3& fixedSizeAABBToushShape_min, Vector3& fixedSizeAABBToushShape_max,
		float stackable_haulOffsetZM);

};
};
