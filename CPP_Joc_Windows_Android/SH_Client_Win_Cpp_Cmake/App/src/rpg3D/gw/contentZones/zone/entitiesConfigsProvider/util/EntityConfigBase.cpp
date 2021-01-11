#include "EntityConfigBase.h"

using namespace rpg3D;

int EntityConfigBase::NewType() {
	static int TYPE_COUNTER = 0;

	TYPE_COUNTER++;
	return TYPE_COUNTER;
}

EntityConfigBase::EntityConfigBase(int type, std::shared_ptr<std::string> instanceId)
	: type(type), instanceId(instanceId)
{
	//void
}

int EntityConfigBase::getType() {
	return type;
}

EntityConfigBase::~EntityConfigBase() {
	//void
}
