#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/template/BasicModuleTemplate.h>
#include <base/math/Vector3.h>

// This is also used as a SwordTarget.
namespace rpg3D {
class PayloadReceiverTargetTemplate : public BasicModuleTemplate {priv typedef BasicModuleTemplate super;pub dCtor(PayloadReceiverTargetTemplate);
    pub explicit PayloadReceiverTargetTemplate();

	pub IEntityModule* createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) override;

	pub ~PayloadReceiverTargetTemplate() override;
};
};
