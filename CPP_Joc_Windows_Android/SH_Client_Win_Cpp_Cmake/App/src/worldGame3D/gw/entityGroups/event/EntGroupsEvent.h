#pragma once

#include <base/gh.h>
#include <string>
#include <rpg3D/gw/entity/module/toolsHandler/tool/ITool.h>

class IWorldEntity;
namespace worldGame3D {
	class IEntsGroup;
};

namespace worldGame3D {
class EntGroupsEvent {pub dCtor(EntGroupsEvent);
	priv static int typeCounter;

	pub int type;

	pub std::string name;

	pub IEntsGroup* entsGroup = nullptr;
	pub IWorldEntity* entity = nullptr;

    pub explicit EntGroupsEvent(std::string name);

	pub void clearData();

	pub EntGroupsEvent* setData(IEntsGroup* entsGroup, IWorldEntity* entity);
	
	pub virtual ~EntGroupsEvent();
};
};
