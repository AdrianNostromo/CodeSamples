#pragma once

#include <base/gh.h>
#include "../../czCommon/config/CZ_Common.h"
#include <base/list/Array3D.h>
#include <base/list/ArrayList.h>
#include <memory>

class Visual3DModelMaterial;
class EntityTemplate;
class IWorldEntity;
namespace rpg3D {
	class ZoneContentData;
	class IGPGrid2DContent;

	namespace playPhases {
		class IGCPlayPhases;
	};
};
namespace graphics {
	class TilingInformation;
};
namespace squareTactics_a_v1 {
	class IGPPartyCandidates;
};

namespace squareTactics_a_v1 {
namespace czGridBattleA {
class CZ_GridBattleA : public czCommon::CZ_Common {priv typedef czCommon::CZ_Common super;pub dCtor(CZ_GridBattleA);
	pub static std::string NAME;
	pub static std::shared_ptr<worldGame3D::ContentZone> FactoryFunction();

	priv ArrayList<std::shared_ptr<IEventListener>> localAutoListenersList{};

	priv IGPPartyCandidates* gpPartyCandidates = nullptr;
	priv rpg3D::playPhases::IGCPlayPhases* gcTurnBasedPlay = nullptr;
	priv rpg3D::IGPGrid2DContent* gpGrid2DContent = nullptr;

	priv bool cachedPrevousStackableGWCapability = false;

    pub explicit CZ_GridBattleA();

	priv void onEvent(IEventListener& eventListener, base::GWEvent& event);

	priv static sp<rpg3D::ZoneContentData> CreateDefaultZoneContentData();
	prot void onZoneActivatedPre(std::shared_ptr<worldGame3D::ZoneStartExtraData>& zoneStartExtraData, base::IGameWorld* gw) override;
	prot void onZoneActivated(std::shared_ptr<worldGame3D::ZoneStartExtraData>& zoneStartExtraData, base::IGameWorld* gw) override;
	prot void onZoneActivatedB(std::shared_ptr<worldGame3D::ZoneStartExtraData>& zoneStartExtraData, base::IGameWorld* gw) override;
	prot void onZoneDeactivate_pre(base::IGameWorld* gw) override;

    pub ~CZ_GridBattleA() override;
};
};
};
