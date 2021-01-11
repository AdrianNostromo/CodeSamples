#include "EC_TiledXY.h"

using namespace rpg3D;

const int EC_TiledXY::TYPE = super::NewType();

EC_TiledXY::EC_TiledXY(std::shared_ptr<std::string> instanceId,
	Vector3& pos, Quaternion& rot,
	std::shared_ptr<graphics::TilingInformation_GridXY> tilingInformation)
	: super(TYPE, instanceId),
	pos(pos), rot(rot),
	tilingInformation(tilingInformation)
{
	rawSubclassPointer = this;
}

EC_TiledXY::~EC_TiledXY() {
	//void
}
