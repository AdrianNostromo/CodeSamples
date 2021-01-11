#include "EntityAction.h"
#include <base/exceptions/LogicException.h>
#include <base/log/GlobalAccessLogHandler.h>
#include <base/app/config/AppConfig.h>

using namespace worldGame3D;

// This is used for non-intrussive semi-optional actions (mouse_turn).
const int EntityAction::Priorities::Low = 1;
// Normal actions (move, stackable_pickup).
const int EntityAction::Priorities::Normal = 2;
const int EntityAction::Priorities::NormalPlus1 = 3;
const int EntityAction::Priorities::High = 4;
// This is used for stops generally.
const int EntityAction::Priorities::UltaHight = 5;

EntityAction::EntityAction(
	int type, std::string name,
	bool isSyncedToServer,
	int persistenceDurationMS,
	int localEntityId, int syncedEntityId,
	int priority, 
	int groupsBitMap,
	int preRequirementsGroupsBitMask,
	int replaceGroupsBitMask,
	int overridesGroupsBitMask)
	: super(
		type, name,
		isSyncedToServer,
		persistenceDurationMS,
		localEntityId, syncedEntityId,
		priority, 
		groupsBitMap,
		preRequirementsGroupsBitMask,
		replaceGroupsBitMask,
		overridesGroupsBitMask)
{
	//void
}

EntityAction::~EntityAction() {
	reservedDisposeIfNeeded();// Note1001
}
