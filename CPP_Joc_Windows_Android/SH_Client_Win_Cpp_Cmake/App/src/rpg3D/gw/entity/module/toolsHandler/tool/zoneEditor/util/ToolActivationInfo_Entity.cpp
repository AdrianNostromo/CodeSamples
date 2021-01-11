#include "ToolActivationInfo_Entity.h"

using namespace rpg3D;

const int ToolActivationInfo_Entity::TYPE = super::NewType();

ToolActivationInfo_Entity::ToolActivationInfo_Entity(int cursorIndex, std::shared_ptr<base::WrappedWorldEntity> wEntity)
	: super(TYPE, cursorIndex),
	wEntity(wEntity)
{
	rawSubclassPointer = this;
}

ToolActivationInfo_Entity::~ToolActivationInfo_Entity() {
	//void
}
