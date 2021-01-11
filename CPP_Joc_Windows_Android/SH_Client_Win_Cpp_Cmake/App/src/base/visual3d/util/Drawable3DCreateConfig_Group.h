#pragma once

#include "Drawable3DCreateConfigBase.h"

class Drawable3DCreateConfig_Group : public Drawable3DCreateConfigBase {
public:
    Drawable3DCreateConfig_Group(
            std::string id, std::string* parentId,
            Vector3* pos, Quaternion* rot, Vector3* scale);
    ~Drawable3DCreateConfig_Group();
};
