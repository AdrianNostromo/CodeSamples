#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/gamepart/IGamePart.h>
#include <base/memory/SharedPointer.h>

class IWorldEntity;
namespace worldGame3D {
	class ConditionalWorldEntitySpawnConfig;
};

namespace rpg3D {
class IOGPConditionalEntities : virtual public IGamePart {priv typedef IGamePart super;
	pub static int TYPE;
    DEC_componentBasics();

	pub virtual IWorldEntity* addConditionalEntitySpawnInfo(sp<worldGame3D::ConditionalWorldEntitySpawnConfig> spawnInfo) = 0;

    pub ~IOGPConditionalEntities() override = default;
};
};
