#include "MovementConfig.h"

using namespace rpg3D;

int MovementConfig::NewType() {
	static int TYPE_COUNTER = 0;

	TYPE_COUNTER++;
	return TYPE_COUNTER;
}

MovementConfig::MovementConfig(int type)
	: type(type)
{
	//void
}

int MovementConfig::getType() {
	return type;
}

MovementConfig::~MovementConfig() {
	//void
}
