#include "CfgConstraint3D6Dof.h"

using namespace rpg3D;

int CfgConstraint3D6Dof::TYPE = super::NewType();

CfgConstraint3D6Dof::CfgConstraint3D6Dof(
	btTransform& offsetInActorA, btTransform& offsetInActorB,
	bool useLinearReferenceFrameA)
	: super(TYPE),
	offsetInActorA(offsetInActorA), offsetInActorB(offsetInActorB),
	useLinearReferenceFrameA(useLinearReferenceFrameA)
{
	rawSubclassPointer = this;
}

CfgConstraint3D6Dof::~CfgConstraint3D6Dof() {
	//void
}
