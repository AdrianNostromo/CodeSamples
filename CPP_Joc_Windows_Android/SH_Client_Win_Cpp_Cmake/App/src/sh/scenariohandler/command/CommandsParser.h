#pragma once

#include <base/gh.h>
#include <base/list/ArrayList.h>
#include <string>
#include <base/sp.h>

namespace sh {
	class ICommand;
};

namespace sh {
class CommandsParser {pub dCtor(CommandsParser);
	pub static sp<ArrayList<sp<ICommand>>> ParseAllCommands(ArrayList<std::string>& commandParts);
	priv static sp<ICommand> ParseSingleCommand(ArrayList<std::string>& commandParts, int& cPartIndex);

};
};
