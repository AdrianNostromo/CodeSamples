#include "V3DM_Base.h"
#include <worldGame3D/gw/entity/template/visual3D/Visual3DTemplate.h>

using namespace worldGame3D;

V3DM_Base::V3DM_Base(
	IWorldEntity* entity,
	Visual3DTemplate* t)
	: super(t->moduleName, entity),
	t(t)
{
	//void
}

V3DM_Base::~V3DM_Base() {
	//void
}
