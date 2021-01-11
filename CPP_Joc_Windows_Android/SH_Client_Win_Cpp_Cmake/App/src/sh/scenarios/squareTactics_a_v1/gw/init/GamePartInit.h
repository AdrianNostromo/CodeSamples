#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/gamepart/GamePart.h>
#include <base/list/ArrayList.h>

class Visual3DModelMaterial;
namespace base {
	class GWEvent;
};
namespace worldGame3D {
	class IContentZone;
};

namespace squareTactics_a_v1 {
class GamePartInit : public GamePart {priv typedef GamePart super;pub dCtor(GamePartInit);
	pub explicit GamePartInit(base::IGameWorld* gw);

	priv void onGWEvent_createContent(IEventListener& eventListener, base::GWEvent& event);

	priv void createPlayerUnit(float x, float y, float z);

    pub ~GamePartInit() override;
};
};
