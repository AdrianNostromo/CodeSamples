#pragma once

#include <base/gh.h>
#include <string>
#include <rpg3D/gw/entity/module/toolsHandler/tool/ITool.h>

class IWorldEntity;

namespace worldGame3D {
namespace gw {
class CapabilityEvent {pub dCtor(CapabilityEvent);
	priv static int typeCounter;

	pub int type;

	pub std::string name;

	pub unsigned char capabilityBitIndex = 0;
	pub bool state = false;

    pub explicit CapabilityEvent(std::string name);

	pub void clearData();

	pub CapabilityEvent* setData(unsigned char capabilityBitIndex, bool state);
	
	pub virtual ~CapabilityEvent();
};
};
};
