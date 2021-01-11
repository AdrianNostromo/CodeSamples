#pragma once

#include <base/gh.h>
#include "../../zoneCommon/config/ContentZoneCommon.h"
#include <base/list/Array3D.h>
#include <base/list/ArrayList.h>
#include <memory>
#include <base/math/Vector2.h>
#include <base/math/Vector3.h>
#include <base/math/Vector2Int.h>
#include "../dungeonContent/DungeonContent.h"
#include <base/object/WrappedValue.h>

class Visual3DModelMaterial;
class EntityTemplate;
class IWorldEntity;
namespace base {
	class IDebugEntryUser;
};

namespace randDungeons_a_v1 {
namespace RDungeonA {
class ContentZoneRDungeonA : public common::ContentZoneCommon {priv typedef common::ContentZoneCommon super;pub dCtor(ContentZoneRDungeonA);
	pub static std::string NAME;
	pub static std::shared_ptr<worldGame3D::ContentZone> FactoryFunction();
	
	priv static bool IS_DEBUG_VIEW_LOG;

	priv std::shared_ptr<base::IDebugEntryUser> dynamicDebugEntry = nullptr;
	priv std::shared_ptr<base::WrappedValue<int>> dynamicDebugEntry_levelIndexI = nullptr;
	priv std::shared_ptr<base::WrappedValue<int>> dynamicDebugEntry_seedI = nullptr;

    pub explicit ContentZoneRDungeonA();
	
	prot void onZoneActivated(std::shared_ptr<worldGame3D::ZoneStartExtraData>& zoneStartExtraData, base::IGameWorld* gw) override;
	prot void onZoneDeactivate_pre(base::IGameWorld* gw) override;

    pub ~ContentZoneRDungeonA() override;
};
};
};
