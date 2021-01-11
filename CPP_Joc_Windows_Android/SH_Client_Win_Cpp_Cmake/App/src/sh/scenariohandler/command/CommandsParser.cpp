#include "CommandsParser.h"
#include <sh/scenariohandler/command/ICommand.h>
#include <sh/scenariohandler/command/Cmd_ScenarioZoneSet.h>
#include <worldGame3D/scripts/ScriptCommands.h>
#include <base/exceptions/LogicException.h>

using namespace sh;

sp<ArrayList<sp<ICommand>>> CommandsParser::ParseAllCommands(ArrayList<std::string>& commandParts) {
	sp<ArrayList<sp<ICommand>>> commandsList = new ArrayList<sp<ICommand>>();

	int cPartIndex = 0;
	while (true) {
		sp<ICommand> commandObject = ParseSingleCommand(commandParts, cPartIndex);
		if (commandObject == nullptr) {
			break;
		}

		commandsList->appendDirect(commandObject);
	}

	return commandsList;
}

sp<ICommand> CommandsParser::ParseSingleCommand(ArrayList<std::string>& commandParts, int& cPartIndex) {
	using sc = worldGame3D::ScriptCommands;

	sp<ICommand> cmd = nullptr;

	std::string* cPart = commandParts.getPointer(cPartIndex++, false/*mustExist*/);
	if (cPart == nullptr) {
		//void
	}else if (*cPart == sc::cmd_scenario || *cPart == sc::cmd_zone) {
		cPartIndex--;

		cmd = Cmd_ScenarioZoneSet::ParseCommand(commandParts, cPartIndex);
	} else {
		throw LogicException(LOC);
	}

	return cmd;
}
