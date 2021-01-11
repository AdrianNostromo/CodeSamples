#pragma once

#include <base/gh.h>
#include "../../czCommon/config/CZ_Common.h"
#include <base/list/Array3D.h>
#include <base/list/ArrayList.h>
#include <memory>
#include <base/list/Array2D.h>
#include <base/list/ListDL.h>
#include <base/memory/SharedPointer.h>

class Visual3DModelMaterial;
class EntityTemplate;
class IWorldEntity;
class IEventListener;
namespace base {
	class GWEvent;
};
namespace rpg3D {
	class ZoneContentData;
};
namespace graphics {
	class TilingInformation;
};

namespace zoneAlert_a_v1 {
namespace czTownA {
class CZ_TownA : public czCommon::CZ_Common {priv typedef czCommon::CZ_Common super;pub dCtor(CZ_TownA);
	pub static std::string NAME;
	pub static std::shared_ptr<worldGame3D::ContentZone> FactoryFunction();
	
	priv ArrayList<std::shared_ptr<IEventListener>> localAutoListenersList{};

    pub explicit CZ_TownA();

	priv void onEvent(IEventListener& eventListener, base::GWEvent& event);

	priv static sp<rpg3D::ZoneContentData> CreateDefaultZoneContentData();

	prot void onZoneActivated(std::shared_ptr<worldGame3D::ZoneStartExtraData>& zoneStartExtraData, base::IGameWorld* gw) override;
	prot void onZoneDeactivate_pre(base::IGameWorld* gw) override;

    pub ~CZ_TownA() override;
};
};
};
