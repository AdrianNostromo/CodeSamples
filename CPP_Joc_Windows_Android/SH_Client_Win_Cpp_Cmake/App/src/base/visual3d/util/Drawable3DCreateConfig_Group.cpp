#include "Drawable3DCreateConfig_Group.h"

Drawable3DCreateConfig_Group::Drawable3DCreateConfig_Group(
        std::string id, std::string* parentId,
        Vector3* pos, Quaternion* rot, Vector3* scale)
        :Drawable3DCreateConfigBase(id, parentId, pos, rot, scale)
{
    //void
}

Drawable3DCreateConfig_Group::~Drawable3DCreateConfig_Group() {
    //void
}
