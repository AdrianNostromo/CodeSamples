#pragma once

#include <base/gh.h>
#include "GPPLH_Base.h"
#include <memory>
#include <string>
#include <base/math/Vector3.h>

class EntityTemplate;
class IWorldEntity;
namespace base {
	class GWEvent;
};

namespace randDungeons_a_v1 {
namespace RDungeonA {
class GPPLH_Platforms : public GPPLH_Base {priv typedef GPPLH_Base super;pub dCtor(GPPLH_Platforms);
	pub explicit GPPLH_Platforms(base::IGameWorld* gw);

	priv void onEvent(IEventListener& eventListener, base::GWEvent& event);

    pub ~GPPLH_Platforms() override;
};
};
};
