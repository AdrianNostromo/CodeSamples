#pragma once

#include <base/gh.h>
#include <memory>
#include <sstream>
#include <rpg3D/gw/util/ParseData_VoxlesGrid3DGeometry.h>
#include <base/visual3d/material/Visual3DModelMaterial.h>

class VoxlesGrid3DGeometry;
class IWorldEntity;

namespace rpg3D {
class Util_VoxlesGrid3DGeometry {
	pub class UnitType {
		pub static std::string creature;
		pub static std::string hero_dummy;
	};

	priv static const int DataFormatVersion_1;
	priv static const int DataFormatVersion_Current;

	pub static std::shared_ptr<std::stringstream> serialize(
		IWorldEntity* entity, VoxlesGrid3DGeometry* obj,
		bool saveEntityGenerals, bool saveMaterials, bool saveVoxelsGrid);
	priv static std::shared_ptr<std::stringstream> serialize_v1(
		IWorldEntity* entity, VoxlesGrid3DGeometry* obj,
		bool saveEntityGenerals, bool saveMaterials, bool saveVoxelsGrid);

	pub static std::shared_ptr <ParseData_VoxlesGrid3DGeometry> parse(
		std::string& data,
		bool loadEntityGenerals, bool loadMaterials, bool loadVoxelsGrid);
	priv static std::shared_ptr <ParseData_VoxlesGrid3DGeometry> parse_v1(
		std::shared_ptr<ArrayList<std::string>> linesList, int& lineIndex,
		bool loadEntityGenerals, bool loadMaterials, bool loadVoxelsGrid);
	priv static std::shared_ptr<ArrayList<Visual3DModelMaterial*>> parse_v1_materials(std::shared_ptr<ArrayList<std::string>> linesList, int& lineIndex);
	priv static std::shared_ptr < Array3D<signed char>> parse_v1_voxels(std::shared_ptr<ArrayList<std::string>> linesList, int& lineIndex);

};
};
