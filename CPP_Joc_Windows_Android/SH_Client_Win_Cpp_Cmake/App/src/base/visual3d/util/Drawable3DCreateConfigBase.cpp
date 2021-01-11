#include "Drawable3DCreateConfigBase.h"

Drawable3DCreateConfigBase::Drawable3DCreateConfigBase(
        std::string id, std::string* parentId,
        Vector3* pos, Quaternion* rot, Vector3* scale)
        : id(id), parentId(parentId), pos(pos), rot(rot), scale(scale)
{
    //void
}

Drawable3DCreateConfigBase::~Drawable3DCreateConfigBase() {
    //void
}
