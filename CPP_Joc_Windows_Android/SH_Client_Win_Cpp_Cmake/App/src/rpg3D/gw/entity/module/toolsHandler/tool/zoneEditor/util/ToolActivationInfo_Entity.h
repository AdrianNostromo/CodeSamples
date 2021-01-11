#pragma once

#include <base/gh.h>
#include "ToolActivationInfo_Base.h"
#include <base/math/Vector3.h>
#include <memory>

class IWorldEntity;
namespace base {
    class WrappedWorldEntity;
};

namespace rpg3D {
class ToolActivationInfo_Entity : public ToolActivationInfo_Base {priv typedef ToolActivationInfo_Base super;pub dCtor(ToolActivationInfo_Entity);
    pub static const int TYPE;

    pub std::shared_ptr<base::WrappedWorldEntity> wEntity = nullptr;

    pub Vector3 groundPos;

    pub explicit ToolActivationInfo_Entity(int cursorIndex, std::shared_ptr<base::WrappedWorldEntity> wEntity);

    pub ~ToolActivationInfo_Entity() override;
};
};
