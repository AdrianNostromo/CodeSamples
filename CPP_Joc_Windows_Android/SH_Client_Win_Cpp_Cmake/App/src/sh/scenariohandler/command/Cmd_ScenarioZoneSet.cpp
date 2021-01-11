#include "Cmd_ScenarioZoneSet.h"
#include <worldGame3D/scripts/ScriptCommands.h>
#include <worldGame3D/scenariohandler/util/ScenarioConfig.h>
#include <base/exceptions/LogicException.h>
#include "../SH_SquareHeads.h"
#include <worldGame3D/gw/IGameWorld.h>
#include <worldGame3D/gw/contentZones/zone/util/ZoneStartExtraData.h>

using namespace sh;

Cmd_ScenarioZoneSet::Cmd_ScenarioZoneSet()
	: super()
{
	//void
}

void Cmd_ScenarioZoneSet::run(SH_SquareHeads* scenariosHandler, Vector3& sourceTeleporterOffset) {
	using sc = worldGame3D::ScriptCommands;

	std::string* bakedScenarioId = nullptr;
	if (isScenarioComponent && scenarioName != sc::target_same) {
		if (scenarioName.length() == 0) {
			throw LogicException(LOC);
		}

		bakedScenarioId = &scenarioName;
	} else {
		// Same scenario or none provided, only the zone changes probably.
		if (scenariosHandler == nullptr || scenariosHandler->getActiveScenarioTemplate() == nullptr) {
			throw LogicException(LOC);
		}
		bakedScenarioId = &scenariosHandler->getActiveScenarioTemplate()->id;
	}

	ScenarioConfig* scenarioConfig = nullptr;
	scenarioConfig = new ScenarioConfig(nullptr/*serverConnectConfig*/);

	std::shared_ptr<worldGame3D::ZoneStartExtraData> zoneStartExtraData = nullptr;
	sp<Vector3> teleportTargetOffset = nullptr;
	if (keepTeleporterOffset) {
		if (spawnTarget.length() <= 0) {
			// The teleport target offset can be used only if there is a teleport target
			throw LogicException(LOC);
		}

		teleportTargetOffset = new Vector3(sourceTeleporterOffset);
	}

	if (spawnTarget.length() > 0) {
		zoneStartExtraData = std::make_shared<worldGame3D::ZoneStartExtraData>(
			std::make_shared<std::string>(spawnTarget), teleportTargetOffset/*teleportTargetOffset*/
		);
	}

	int r = scenariosHandler->setActiveScenario(
		*bakedScenarioId, scenarioConfig, restartScenario/*forceScenarioRestartIfSame*/,
		zoneName/*optionalZoneName*/, restartZone/*forceZoneRestartIfSame*/, zoneStartExtraData
	);
	if (r != 0) {
		throw LogicException(LOC);
	}
}

sp<ICommand> Cmd_ScenarioZoneSet::ParseCommand(ArrayList<std::string>& commandParts, int& cPartIndex) {
	sp<ICommand> retCmd = nullptr;
	Cmd_ScenarioZoneSet* cmd = nullptr;

	using sc = worldGame3D::ScriptCommands;

	// This loop is used to handle commands that contain multiple sub-command parts (eg. scenario, zone change).
	while (true) {
		std::string* cPart = commandParts.getPointer(cPartIndex++, false/*mustExist*/);
		if (cPart == nullptr) {
			break;
		} else if (*cPart == sc::cmd_scenario) {
			if (retCmd == nullptr) {
				cmd = new Cmd_ScenarioZoneSet();
				retCmd = cmd;
			}

			if (cmd->isScenarioComponent) {
				throw LogicException(LOC);
			}
			cmd->isScenarioComponent = true;

			bool isCmdRequiredPartsOccured = false;

			cPart = commandParts.getPointer(cPartIndex++, false/*mustExist*/);
			if (cPart != nullptr && *cPart == sc::action_set) {
				cPart = commandParts.getPointer(cPartIndex++, true/*mustExist*/);
				if (*cPart == sc::target_same) {
					cmd->scenarioName = *cPart;
				} else {
					cmd->scenarioName = *cPart;
				}
			} else {
				cPartIndex--;
			}

			cPart = commandParts.getPointer(cPartIndex++, false/*mustExist*/);
			if (cPart != nullptr && *cPart == sc::action_restart) {
				cmd->restartScenario = true;

				isCmdRequiredPartsOccured = true;
			} else {
				cPartIndex--;
			}

			if (!isCmdRequiredPartsOccured) {
				// Command encountered but without any of the needed parts.
				throw LogicException(LOC);
			}
		} else if (*cPart == sc::cmd_zone) {
			if (retCmd == nullptr) {
				cmd = new Cmd_ScenarioZoneSet();
				retCmd = cmd;
			}

			if (cmd->isZoneComponent) {
				throw LogicException(LOC);
			}
			cmd->isZoneComponent = true;

			bool isCmdRequiredPartsOccured = false;

			cPart = commandParts.getPointer(cPartIndex++, false/*mustExist*/);
			if (cPart != nullptr && *cPart == sc::action_set) {
				cPart = commandParts.getPointer(cPartIndex++, true/*mustExist*/);
				if (*cPart == sc::target_same) {
					cmd->zoneName = *cPart;
				} else {
					cmd->zoneName = *cPart;
				}

				isCmdRequiredPartsOccured = true;
			} else {
				cPartIndex--;
			}

			cPart = commandParts.getPointer(cPartIndex++, false/*mustExist*/);
			if (cPart != nullptr && *cPart == sc::target_spawnTarget) {
				cPart = commandParts.getPointer(cPartIndex++, true/*mustExist*/);
				cmd->spawnTarget = *cPart;
			} else {
				cPartIndex--;
			}

			cPart = commandParts.getPointer(cPartIndex++, false/*mustExist*/);
			if (cPart != nullptr && *cPart == sc::action_restart) {
				cmd->restartZone = true;

				isCmdRequiredPartsOccured = true;
			} else {
				cPartIndex--;
			}

			if (!isCmdRequiredPartsOccured) {
				// Command encountered but without any of the needed parts.
				throw LogicException(LOC);
			}
		} else if (*cPart == sc::modifier_keepTeleporterOffset) {
			cmd->keepTeleporterOffset = true;
		} else {
			throw LogicException(LOC);
		}
	}

	return retCmd;
}

Cmd_ScenarioZoneSet::~Cmd_ScenarioZoneSet() {
	//void
}
