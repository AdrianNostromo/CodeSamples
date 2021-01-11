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
};
namespace graphics {
	class TilingInformation;
};

namespace zoneAlert_a_v1 {
namespace czGridBattleA {
class CZ_GridBattleA : public czCommon::CZ_Common {priv typedef czCommon::CZ_Common super;pub dCtor(CZ_GridBattleA);
	pub static std::string NAME;
	pub static std::shared_ptr<worldGame3D::ContentZone> FactoryFunction();

	priv ArrayList<std::shared_ptr<IEventListener>> localAutoListenersList{};

    pub explicit CZ_GridBattleA();

	priv void onEvent(IEventListener& eventListener, base::GWEvent& event);

	priv static sp<rpg3D::ZoneContentData> CreateDefaultZoneContentData();
	prot void onZoneActivated(std::shared_ptr<worldGame3D::ZoneStartExtraData>& zoneStartExtraData, base::IGameWorld* gw) override;
	prot void onZoneDeactivate_pre(base::IGameWorld* gw) override;

	priv void createGroundPatch(
		float x, float y, float z,
		Array3D<signed char>* voxelsGrid,
		ArrayList<Visual3DModelMaterial*>* materialsList,
		base::IGameWorld* gw);
	IWorldEntity* createConfigEntity(
		float x, float y, float z,
		float rotZ,
		EntityTemplate* entityTemplate,
		std::shared_ptr<std::string> instanceId, std::shared_ptr<std::string> platformActivationScript,
		base::IGameWorld* gw);

    pub ~CZ_GridBattleA() override;
};
};
};
