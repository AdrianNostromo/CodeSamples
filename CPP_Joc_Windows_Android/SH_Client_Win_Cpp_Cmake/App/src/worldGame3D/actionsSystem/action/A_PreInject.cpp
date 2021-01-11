#include "A_PreInject.h"

using namespace worldGame3D;

A_PreInject::A_PreInject(
	int type, std::string name,
	bool isSyncedToServer,
	int persistenceDurationMS,
	int priority, 
	int groupsBitMap,
	int preRequirementsGroupsBitMask,
	int replaceGroupsBitMask,
	int overridesGroupsBitMask)
	: super(
		type, name,
		isSyncedToServer,
		persistenceDurationMS,
		priority, 
		groupsBitMap,
		preRequirementsGroupsBitMask,
		replaceGroupsBitMask,
		overridesGroupsBitMask)
{
	//void
}

ArrayList<worldGame3D::IAction::CfgPreInjectedAction>* A_PreInject::getChildDependencyActionsToPreInjectList() {
	return &childDependencyActionsToPreInjectList;
}

A_PreInject::~A_PreInject() {
	//void
}
