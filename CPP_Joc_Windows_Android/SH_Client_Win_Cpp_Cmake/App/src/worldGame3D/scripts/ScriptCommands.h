#pragma once

#include <base/gh.h>
#include <string>
#include <memory>

namespace worldGame3D {
class ScriptCommands {
	/*
	* Ussage examples.
	worldGame3D::ScriptCommands::cmd_setScenarioAndZone + worldGame3D::ScriptCommands::commandPartsDelimiter
	+ worldGame3D::ScriptCommands::sameScenario + worldGame3D::ScriptCommands::commandPartsDelimiter + restart
	+ Main::ContentZoneMain::NAME

	worldGame3D::ScriptCommands::cmd_setScenarioAndZone + worldGame3D::ScriptCommands::commandPartsDelimiter
	+ startarea_a_v1::ScenarioFactory::scenarioTemplate->id

	worldGame3D::ScriptCommands::cmd_setZone + worldGame3D::ScriptCommands::commandPartsDelimiter
	+ Main::ContentZoneMain::NAME*/

    pub static std::string commandPartsDelimiter;

    pub static std::string cmd_setScenarioAndZone;
    pub static std::string cmd_setZone;

    pub static std::string sameScenario;
    pub static std::string restart;
    pub static std::string spawnTarget;

    pub static std::string commandsDelimiter;

	// These are used in the new system.
	pub static std::string delimiter_commands;

	pub static std::string delimiter_commandParts;

	// Command template is:
	// cmd action <action_target> <modifier...>
	/// Some commands can have multiple commands in the same cmd class (eg. scenario and zone change are processed in the same place).
	
	// scenario <set [same/SCENARIO_NAME]> <restart>
	pub static std::string cmd_scenario;
	// zone <set [same/ZONE_NAME]> <spawnTarget SPAWN_TARGET> <restart>
	pub static std::string cmd_zone;

	pub static std::string action_restart;
	pub static std::string action_set;

	pub static std::string target_same;
	pub static std::string target_spawnTarget;

	pub static std::string modifier_restart;
	pub static std::string modifier_keepTeleporterOffset;

};
};
