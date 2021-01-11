#pragma once

#include <base/gh.h>
#include <string>
#include <functional>
#include <memory>
#include <worldGame3D/gw/entity/template/ETemplateIdentifier.h>
#include <graphics/visual3d/drawable/voxelsGrid/tiling/TilingInformation.h>
#include <base/list/ArrayList.h>
#include "../../prefab/PrefabRoom.h"

class EntityTemplate;
class Visual3DModelMaterial;

namespace randDungeons_a_v1 {
namespace PrefabRoomsDungeonA {
class CfgDungeonGenerator {pub dCtor(CfgDungeonGenerator);
	pub int zoneFloorsCount = 1;

	pub ArrayList<PrefabRoomsDungeonA::PrefabRoom>* prefabRoomsList;

	pub base::ETemplateIdentifier templateIdentifier_teleporter_exitStairs{};
	pub base::ETemplateIdentifier templateIdentifier_teleporter_stairsDown{};
	pub base::ETemplateIdentifier templateIdentifier_teleporter_stairsUp{};

	pub base::ETemplateIdentifier templateIdentifier_mob{};

    pub explicit CfgDungeonGenerator();

    pub virtual ~CfgDungeonGenerator();
};
};
};
