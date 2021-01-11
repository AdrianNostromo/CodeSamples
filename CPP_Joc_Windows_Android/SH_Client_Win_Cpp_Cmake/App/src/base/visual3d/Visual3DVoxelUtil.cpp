#include "Visual3DVoxelUtil.h"

using namespace base;

Visual3DVoxelUtil::Visual3DVoxelUtil() {
	//void
}

std::shared_ptr<PosRotSize3D> Visual3DVoxelUtil::drawablePositionalToGlobalPositional(Matrix4 srcTransform) {
	return drawablePositionalToGlobalPositional(
		srcTransform,
		1.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 0.0f
	);
}

std::shared_ptr<PosRotSize3D> Visual3DVoxelUtil::drawablePositionalToGlobalPositional(
	Matrix4& srcTransform,
	float subScaleX, float subScaleY, float subScaleZ,
	float posX, float posY, float posZ)
{
	Matrix4 voxelLocalTransform{};

	if (subScaleX != 1.0f || subScaleY != 1.0f || subScaleZ != 1.0f) {
		voxelLocalTransform.scale(
			subScaleX,
			subScaleY,
			subScaleZ
		);
	}
	if (posX != 0.0f || posY != 0.0f || posZ != 0.0f) {
		voxelLocalTransform.translate(
			posX,
			posY,
			posZ
		);
	}

	Matrix4 voxelGlobalTransform{ srcTransform };
	voxelGlobalTransform.mul(voxelLocalTransform);

	Vector3 posW{};
	voxelGlobalTransform.getTranslation(posW);

	Vector3 scale{};
	voxelGlobalTransform.getScale(scale);

	Quaternion rot{};
	voxelGlobalTransform.getRotation(rot, true);

	// Get the transform for the
	std::shared_ptr<PosRotSize3D> positional = std::make_shared<PosRotSize3D> (
		posW,
		rot,
		scale
	);

	return positional;
}

Visual3DVoxelUtil::~Visual3DVoxelUtil() {
	//void
}
