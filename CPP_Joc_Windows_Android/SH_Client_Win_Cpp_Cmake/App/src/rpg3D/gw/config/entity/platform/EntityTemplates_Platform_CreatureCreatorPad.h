#pragma once

#include <worldGame3D/gw/entity/template/EntityTemplate.h>
#include <base/visual3d/util/Drawable3DCreateConfig_VoxelGrid.h>

namespace rpg3D {
class EntityTemplates_Platform_CreatureCreatorPad {
public:
	static std::string V_ID_Base;
	static std::string V_ID_Base_Disabled;
	static std::string V_ID_Icon;

public:
	static EntityTemplate* getNew();

private:
	static Drawable3DCreateConfig_VoxelGrid* buildBase();
	static Drawable3DCreateConfig_VoxelGrid* buildBase_disabled();
	static Drawable3DCreateConfig_VoxelGrid* buildIcon();

};
};
