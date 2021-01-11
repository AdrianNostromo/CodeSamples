#include "EC_PosRot.h"

using namespace rpg3D;

const int EC_PosRot::TYPE = super::NewType();

EC_PosRot::EC_PosRot(
	std::shared_ptr<std::string> instanceId,
	Vector3& pos, Quaternion& rot)
	: super(TYPE, instanceId),
	pos(pos), rot(rot)
{
	rawSubclassPointer = this;
}

EC_PosRot::~EC_PosRot() {
	//void
}
