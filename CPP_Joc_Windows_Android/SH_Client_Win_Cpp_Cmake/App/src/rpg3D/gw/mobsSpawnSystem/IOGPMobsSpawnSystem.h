#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/gamepart/IGamePart.h>
#include <string>

namespace worldGame3D {
	class WorldEntitySpawnConfig;
};

namespace rpg3D {
class IOGPMobsSpawnSystem : virtual public IGamePart {priv typedef IGamePart super;
	pub static int TYPE;
    DEC_componentBasics();

	pub static std::string FLAG_isZoneCleared;

	pub virtual void initMobsSpawnInformation(int mobTeam, std::shared_ptr<ArrayList<worldGame3D::WorldEntitySpawnConfig>> mobsSpawnList) = 0;

    pub ~IOGPMobsSpawnSystem() override = default;
};
};
