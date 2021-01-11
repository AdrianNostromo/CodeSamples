#pragma once

#include <base/gh.h>
#include <string>

class IWorldEntity;

namespace base {
class LoopEvent {pub dCtor(LoopEvent);
	priv static int typeCounter;

	pub int type;

	pub std::string name;

	pub float deltaS;
    pub int deltaMS;
    
	pub float appTimeS;

	pub explicit LoopEvent(std::string name);

	pub void clearData();

	pub LoopEvent* setLoopData(float deltaS, int deltaMS, float appTimeS);

	pub virtual ~LoopEvent();
};
};
