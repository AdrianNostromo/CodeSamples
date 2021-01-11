#include "A_Priority.h"

using namespace worldGame3D;

A_Priority::A_Priority(
	int type, std::string name,
	bool isSyncedToServer,
	int persistenceDurationMS,
	int priority, 
	int groupsBitMap,
	int preRequirementsGroupsBitMask,
	int replaceGroupsBitMask)
	: super(
		type, name,
		isSyncedToServer,
		persistenceDurationMS,
		groupsBitMap,
		preRequirementsGroupsBitMask,
		replaceGroupsBitMask),
	priority(priority)
{
	//void
}

int A_Priority::getPriority() {
	return priority;
}

A_Priority::~A_Priority() {
	//void
}
