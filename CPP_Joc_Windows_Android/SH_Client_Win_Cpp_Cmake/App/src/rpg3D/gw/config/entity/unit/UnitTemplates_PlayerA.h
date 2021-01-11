#pragma once

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
class UnitTemplates_PlayerA {
public:
	static ArrayList<Visual3DModelMaterial*>* defaultMaterialsList;
	
	static std::string DATA_KEY_voxelGridZXY;
    static std::string DATA_KEY_materials;

    static Array3D<signed char>* defaultVoxelGridZXY;

public:
	static EntityTemplate* GetNew();

};
};
