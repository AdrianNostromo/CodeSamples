#pragma once

#include <base/gh.h>
#include "OGPMSS_Base.h"

namespace base {
    class GWEvent;
};
namespace rpg3D {
    class IOGPZoneFlags;
};

namespace rpg3D {
class OGPMobsSpawnSystem : public OGPMSS_Base {priv typedef OGPMSS_Base super;pub dCtor(OGPMobsSpawnSystem);
    priv int mobTeam = -1;
    priv std::shared_ptr<ArrayList<worldGame3D::WorldEntitySpawnConfig>> mobsSpawnList = nullptr;

    priv bool isMobsSpawnStarted = false;
    priv bool isMobsSpawnFinished = false;
    // This includes mob projectiles, summons, structures, everything from their team.
    priv int activeMobsCount = 0;

    priv IOGPZoneFlags* gpZoneFlags = nullptr;

    pub explicit OGPMobsSpawnSystem(base::IGameWorld* gw);
    prot void createBMain() override;

    pub void initMobsSpawnInformation(int mobTeam, std::shared_ptr<ArrayList<worldGame3D::WorldEntitySpawnConfig>> mobsSpawnList) final;

    priv void spawnZoneMobs();

    priv void onGWEvent(IEventListener& eventListener, base::GWEvent& event);

    pub ~OGPMobsSpawnSystem() override;
};
};
