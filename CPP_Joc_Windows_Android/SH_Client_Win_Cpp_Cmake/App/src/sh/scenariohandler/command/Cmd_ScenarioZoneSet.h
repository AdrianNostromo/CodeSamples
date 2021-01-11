#pragma once

#include <base/gh.h>
#include "Cmd_Base.h"
#include <string>
#include <base/sp.h>
#include <base/list/ArrayList.h>

namespace sh {
class Cmd_ScenarioZoneSet : public Cmd_Base {priv typedef Cmd_Base super;pub dCtor(Cmd_ScenarioZoneSet);
    pub bool isScenarioComponent = false;
    pub std::string scenarioName = "";
    pub bool restartScenario = false;

    pub bool isZoneComponent = false;
    // "", "same", "default", "ZONE_NAME"
    pub std::string zoneName = "";
    pub std::string spawnTarget = "";
    pub bool restartZone = false;

    pub bool keepTeleporterOffset = false;

    pub explicit Cmd_ScenarioZoneSet();

    pub void run(SH_SquareHeads* scenariosHandler, Vector3& sourceTeleporterOffset) final;

    pub static sp<ICommand> ParseCommand(ArrayList<std::string>& commandParts, int& cPartIndex);

    pub ~Cmd_ScenarioZoneSet() override;
};
};
