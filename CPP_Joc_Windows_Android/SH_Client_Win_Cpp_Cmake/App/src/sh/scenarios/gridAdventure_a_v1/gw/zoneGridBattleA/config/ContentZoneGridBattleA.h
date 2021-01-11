#pragma once

#include <base/gh.h>
#include "../../zoneCommon/config/ContentZoneCommon.h"
#include <base/list/Array2D.h>
#include <base/list/Array3D.h>
#include <base/list/ArrayList.h>
#include <memory>
#include <base/math/Bounds2DInt.h>
#include <base/MM.h>

class Visual3DModelMaterial;
class EntityTemplate;
class IWorldEntity;
namespace rpg3D {
	class ILogicBlock;
};

namespace gridAdventure_a_v1 {
namespace GridBattleA {
class ContentZoneGridBattleA : public common::ContentZoneCommon {priv typedef common::ContentZoneCommon super;pub dCtor(ContentZoneGridBattleA);
	pub static std::string NAME;
	pub static std::shared_ptr<worldGame3D::ContentZone> FactoryFunction();
	
    pub explicit ContentZoneGridBattleA();
	
	prot void onZoneActivatedPre(std::shared_ptr<worldGame3D::ZoneStartExtraData>& zoneStartExtraData, base::IGameWorld* gw) override;
	prot void onZoneActivated(std::shared_ptr<worldGame3D::ZoneStartExtraData>& zoneStartExtraData, base::IGameWorld* gw) override;
	prot void onZoneActivatedB(std::shared_ptr<worldGame3D::ZoneStartExtraData>& zoneStartExtraData, base::IGameWorld* gw) override;
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
		base::IGameWorld* gw,
		const int* teamId,
		float* worldItemGoldAmount, float* itemHealAmount, float* itemDamageAmount, float* projectileDamage=nullptr,
		rpg3D::ILogicBlock* logicBlock=nullptr, void* logicBlockInterfaceRawPtr=nullptr);

    pub ~ContentZoneGridBattleA() override;
};
};
};
