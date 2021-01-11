#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/module/IEntityModule.h>
#include <base/math/Vector3.h>
#include <base/list/ArrayList.h>
#include <memory>
namespace rpg3D {
	class EffectPayload;
};

namespace rpg3D {
class IWorldItemModule : public virtual IEntityModule {priv typedef IComponent super;
	pub static int TYPE;
	DEC_componentBasics();

	pub virtual ArrayList<std::shared_ptr<rpg3D::EffectPayload>>* peekPayloadsList() = 0;

	pub virtual void addPayload(std::shared_ptr<rpg3D::EffectPayload> payload) = 0;
	pub virtual void addPayloadsList(ArrayList<std::shared_ptr<rpg3D::EffectPayload>>* payloadsList) = 0;

	pub virtual void onItemCollected() = 0;

    pub ~IWorldItemModule() override = default;
};
};
