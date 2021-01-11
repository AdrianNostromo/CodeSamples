#pragma once

#include <base/gh.h>
#include <string>

class IWorldEntity;

namespace base {
class GWEvent {pub dCtor(GWEvent);
	priv static int typeCounter;

	pub int type;

	pub std::string name;

	pub void* voidData = nullptr;
	pub IWorldEntity* entityData = nullptr;

	pub float deltaS;
	pub int deltaMS;
	pub float gameTimeS;

	pub explicit GWEvent(std::string name);

	pub void clearData();

	pub void* getVoidData();
	pub GWEvent* setVoidData(void* voidData);

	pub GWEvent* setLoopData(float deltaS, int deltaMS, float gameTimeS);

	pub IWorldEntity* getEntityData();
	pub GWEvent* setEntityData(IWorldEntity* entityData);

	pub virtual ~GWEvent();
};
};
