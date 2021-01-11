#include "Visual3DModule.h"

using namespace worldGame3D;

Visual3DModule::Visual3DModule(
    IWorldEntity* entity,
    Visual3DTemplate* t,
    ArrayList<IDrawableNode3D*>* drawablesList, Map1D<std::string, IDrawableNode3D*>* drawablesMap, IDrawableNode3D* rootDrawable
)
    :super(
		entity,
		t,
		drawablesList, drawablesMap, rootDrawable)
{
	//void
}

Visual3DModule::~Visual3DModule() {
    //void
}
