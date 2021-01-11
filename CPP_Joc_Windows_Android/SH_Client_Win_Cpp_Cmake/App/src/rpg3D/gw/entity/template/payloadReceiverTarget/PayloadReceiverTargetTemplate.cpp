#include <rpg3D/gw/entity/module/payloadReceiverTarget/PayloadReceiverTargetModule.h>
#include "PayloadReceiverTargetTemplate.h"

using namespace rpg3D;

PayloadReceiverTargetTemplate::PayloadReceiverTargetTemplate()
	: super("payloadReceiverTarget", IPayloadReceiverTargetModule::TYPE)
{
	//void
}

IEntityModule* PayloadReceiverTargetTemplate::createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) {
	PayloadReceiverTargetTemplate* t = dynamic_cast<PayloadReceiverTargetTemplate*>(_t);
	if(!t) {
		throw LogicException(LOC);
	}

	PayloadReceiverTargetModule* mod = new PayloadReceiverTargetModule(entity, t);

	return mod;
}

PayloadReceiverTargetTemplate::~PayloadReceiverTargetTemplate() {
	//void
}
