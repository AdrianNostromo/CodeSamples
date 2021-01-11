#pragma once

#include <base/gh.h>
#include "../../zoneCommon/config/ContentZoneCommon.h"
#include <base/list/Array3D.h>
#include <base/list/ArrayList.h>
#include <memory>

class Visual3DModelMaterial;
class EntityTemplate;
class IWorldEntity;

namespace randDungeons_a_v1 {
namespace Main {
class ContentZoneMain : public common::ContentZoneCommon {priv typedef common::ContentZoneCommon super;pub dCtor(ContentZoneMain);
	pub static std::string NAME;
	pub static std::shared_ptr<worldGame3D::ContentZone> FactoryFunction();
	
    pub explicit ContentZoneMain();
	
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

    pub ~ContentZoneMain() override;
};
};
};
