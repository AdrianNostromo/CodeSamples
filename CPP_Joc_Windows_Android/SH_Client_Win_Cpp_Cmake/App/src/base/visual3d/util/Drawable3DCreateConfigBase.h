#pragma once

#include <string>
#include <base/math/Vector3.h>
#include <base/math/Quaternion.h>

class Drawable3DCreateConfigBase {
public:
    std::string id;
    std::string* parentId;

    Vector3* pos;
    Quaternion* rot;
    Vector3* scale;

public:
    Drawable3DCreateConfigBase(
            std::string id, std::string* parentId,
            Vector3* pos, Quaternion* rot, Vector3* scale);
    virtual ~Drawable3DCreateConfigBase();


};
