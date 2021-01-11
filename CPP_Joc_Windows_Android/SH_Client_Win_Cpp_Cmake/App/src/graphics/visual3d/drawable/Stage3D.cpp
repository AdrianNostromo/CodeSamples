#include "Stage3D.h"

Stage3D::Stage3D()
	: super()
{
    setRoot(this);
}

int Stage3D::getGlobalTransformPreupdateIndexCounter() {
	return globalTransformPreupdateIndexCounter;
}

void Stage3D::incrementGlobalTransformPreupdateIndexCounter() {
	this->globalTransformPreupdateIndexCounter++;
}

int Stage3D::syncStageDrawSpecs(
	const int parentInvalidationBitMap,
	graphics::IRenderer& renderer)
{
	return syncDrawSpecsWhereNeeded(
		parentInvalidationBitMap,
		renderer
	);
}

Stage3D::~Stage3D() {
	//void
}
