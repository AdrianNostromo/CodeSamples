#include "Action.h"

using namespace worldGame3D;

Action::Action(
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

Action::~Action() {
	//void
}
