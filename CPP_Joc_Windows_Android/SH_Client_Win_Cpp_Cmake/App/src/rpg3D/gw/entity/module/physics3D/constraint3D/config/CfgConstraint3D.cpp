#include "CfgConstraint3D.h"

using namespace rpg3D;

int CfgConstraint3D::NewType() {
	static int COUNTER = 0;

	COUNTER++;

	return COUNTER;
}

CfgConstraint3D::CfgConstraint3D(int type)
	: type(type)
{
	//void
}

CfgConstraint3D::~CfgConstraint3D() {
	//void
}
