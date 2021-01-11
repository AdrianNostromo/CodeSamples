#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/gamepart/GamePart.h>
#include <base/list/ArrayList.h>
#include <string>
#include <base/memory/SharedPointer.h>

class Visual3DModelMaterial;
namespace base {
	class GWEvent;
};
namespace rpg3D {
	class ProjectileConfig;
	class EffectPayload;
};
namespace worldGame3D {
	class IContentZone;
};

namespace roomgeons_a_v1 {
class GamePartInit : public GamePart {priv typedef GamePart super;pub dCtor(GamePartInit);
	priv class ToolCreationInfo {pub dCtor(ToolCreationInfo);
		pub enum ToolType { Melle, InertialProjectile };

		pub std::string configId;

		pub ToolType toolType;

		pub sp<ArrayList<std::shared_ptr<rpg3D::EffectPayload>>> melleToolsPayloadsList;

		pub rpg3D::ProjectileConfig* projectileConfig;

		pub explicit ToolCreationInfo(std::string configId, ToolType toolType, sp<ArrayList<std::shared_ptr<rpg3D::EffectPayload>>> melleToolsPayloadsList, rpg3D::ProjectileConfig* projectileConfig)
			: configId(configId), toolType(toolType), melleToolsPayloadsList(melleToolsPayloadsList), projectileConfig(projectileConfig)
		{}
	};

	pub explicit GamePartInit(base::IGameWorld* gw);

	priv void onGWEvent_createContent(IEventListener& eventListener, base::GWEvent& event);

	priv void createPlayerUnit(float x, float y, float z);

    pub ~GamePartInit() override;
};
};
