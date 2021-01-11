#pragma once

#include <base/list/ArrayList.h>
#include <base/list/Array2D.h>
#include <base/math/Vector2Int.h>

namespace randDungeons_a_v1 {
namespace RDungeonA {
struct Result_RoomElementsPopulator {
	pub enum SideElementType { None = 0, Wall = 1, Door = 2 };
	pub class RoomGCellSides {
		// Left wall;
		pub static const int xNegative;
		// Right wall;
		pub static const int xPositive;
		// Bottom wall;
		pub static const int yNegative;
		// Top wall;
		pub static const int yPositive;
	};
	pub struct CombinedSideElementInfo {
		pub Vector2Int tilesGridPos;
		pub int sideIndex;

		pub SideElementType type;
		pub int tilesLen;

		// This is used only for doors.
		// >=0; Door;
		// <0; Not door;
		pub int doorOtherSideRoomIndex;

		pub explicit CombinedSideElementInfo(Vector2Int& tilesGridPos, int sideIndex, SideElementType type, int tilesLen, int doorOtherSideRoomIndex)
			: tilesGridPos(tilesGridPos), sideIndex(sideIndex), type(type), tilesLen(tilesLen), doorOtherSideRoomIndex(doorOtherSideRoomIndex)
		{};
	};

	pub struct TeleporterInfo {
		pub enum Type { EntryPointStairs, ExitStairs, StairsDown, StairsUp };
		Vector2Int roomGPos;
		Type type;

		pub explicit TeleporterInfo(Vector2Int& roomGPos, Type type)
			: roomGPos(roomGPos), type(type)
		{};
	};
	pub struct MobInfo {
		pub enum Type { GeneralMob };
		Vector2Int roomGPos;
		Type type;

		pub explicit MobInfo(Vector2Int& roomGPos, Type type)
			: roomGPos(roomGPos), type(type)
		{};
	};

    pub ArrayList<CombinedSideElementInfo> finalSideElementOrigins{};
	pub Array2D<signed char> floorIndexedTilingGrid{0, 0};

	pub ArrayList<TeleporterInfo> teleportersList{};
	
	pub ArrayList<MobInfo> mobsList{};

};
};
};
