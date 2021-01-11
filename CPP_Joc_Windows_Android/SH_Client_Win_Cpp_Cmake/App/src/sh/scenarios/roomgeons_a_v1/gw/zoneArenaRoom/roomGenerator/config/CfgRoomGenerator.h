#pragma once

#include <base/gh.h>
#include <string>
#include <functional>
#include <memory>
#include <worldGame3D/gw/entity/template/ETemplateIdentifier.h>
#include <graphics/visual3d/drawable/voxelsGrid/tiling/TilingInformation.h>
#include <base/math/Vector2Int.h>

class EntityTemplate;
class Visual3DModelMaterial;

namespace roomgeons_a_v1 {
namespace ArenaRoom {
class CfgRoomGenerator {pub dCtor(CfgRoomGenerator);
	// This is the size of the visual tiles that are used to create the room.
	// Value is in centimeters.
	// {60, 100}; Templates for the specified size must exist.
	pub int tileSizeCm = 100;

	pub bool isFirstRoomInSequence = false;
	pub bool isLastRoomInSequence = false;

	pub Vector2Int roomPrefabsCount{};

	pub std::function<EntityTemplate *(int size_dm)>
		templateFactory_door = nullptr;

	pub base::ETemplateIdentifier templateIdentifier_teleporter_exitStairs{};
	pub base::ETemplateIdentifier templateIdentifier_teleporter_stairsDown{};
	pub base::ETemplateIdentifier templateIdentifier_teleporter_stairsUp{};

	pub base::ETemplateIdentifier templateIdentifier_mob{};

    pub explicit CfgRoomGenerator();

    pub virtual ~CfgRoomGenerator();
};
};
};
