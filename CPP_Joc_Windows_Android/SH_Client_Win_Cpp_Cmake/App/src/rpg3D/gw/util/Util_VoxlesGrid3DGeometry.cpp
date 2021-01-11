#include "Util_VoxlesGrid3DGeometry.h"
#include <base/exceptions/LogicException.h>
#include <base/exceptions/DataException.h>
#include <graphics/visual3d/drawable/VoxlesGrid3DGeometry.h>
#include <base/util/StringUtil.h>
#include <base/visual3d/material/Visual3DModelMaterial_Color.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <rpg3D/gw/entity/module/stackable/IStackableModule.h>
#include <rpg3D/gw/entity/template/stackable/StackableTemplate.h>
#include <worldGame3D/gw/entity/template/touchable3D/Touchable3DTemplate.h>
#include <worldGame3D/gw/entity/module/touchable3D/Touchable3DModule_AABBBounds.h>
#include <rpg3D/gw/config/entity/EntityTags.h>

using namespace rpg3D;

std::string Util_VoxlesGrid3DGeometry::UnitType::creature = "creature";
std::string Util_VoxlesGrid3DGeometry::UnitType::hero_dummy = "hero_dummy";

const int Util_VoxlesGrid3DGeometry::DataFormatVersion_1 = 1;
const int Util_VoxlesGrid3DGeometry::DataFormatVersion_Current = DataFormatVersion_1;

std::shared_ptr<std::stringstream> Util_VoxlesGrid3DGeometry::serialize(
	IWorldEntity* entity, VoxlesGrid3DGeometry* obj,
	bool saveEntityGenerals, bool saveMaterials, bool saveVoxelsGrid)
{
	if (DataFormatVersion_Current == DataFormatVersion_1) {
		return serialize_v1(
			entity, obj,
			saveEntityGenerals, saveMaterials, saveVoxelsGrid
		);
	} else {
		// Unhandled data_format version.
		throw LogicException(LOC);
	}
}

std::shared_ptr<std::stringstream> Util_VoxlesGrid3DGeometry::serialize_v1(
	IWorldEntity* entity, VoxlesGrid3DGeometry* obj,
	bool saveEntityGenerals, bool saveMaterials, bool saveVoxelsGrid)
{
	std::shared_ptr<std::stringstream> ss = std::make_shared<std::stringstream>();
	*ss << "data_format_version " << DataFormatVersion_1 << "\r\n";

	if (saveEntityGenerals) {
		std::string unit_type;
		if (entity->getTemplate()->hasTag(rpg3D::EntityTags::EDITABLE_CREATURE)) {
			unit_type = UnitType::creature;
		}
		else if (entity->getTemplate()->hasTag(rpg3D::EntityTags::EDITABLE_PLAYR_UNIT_DUMMY)) {
			unit_type = UnitType::hero_dummy;
		}
		else {
			throw LogicException(LOC);
		}
		*ss << "unit_type " << unit_type << "\r\n";
		
		rpg3D::IStackableModule* entity_stackable = entity->getComponentAs< rpg3D::IStackableModule*>(true/*mustExist*/);
		*ss << "stackable_haulOffsetZM " << entity_stackable->getHaulOffsetZM() << "\r\n";

		Touchable3DModule_AABBBounds* entity_touchable = entity->getComponentAs< Touchable3DModule_AABBBounds*>(true/*mustExist*/);
		Vector3* entity_touchable_bounds_min = entity_touchable->peekLocalBounds_min();
		Vector3* entity_touchable_bounds_max = entity_touchable->peekLocalBounds_max();
		*ss << "touchable_AABBShape_min " << entity_touchable_bounds_min->x << " " << entity_touchable_bounds_min->y << " " << entity_touchable_bounds_min->z << "\r\n";
		*ss << "touchable_AABBShape_max " << entity_touchable_bounds_max->x << " " << entity_touchable_bounds_max->y << " " << entity_touchable_bounds_max->z << "\r\n";

		*ss << "voxel_size " << obj->getVoxelSize() << "\r\n";
		*ss << "grid_offset " << obj->getVoxelGridOffset()->x << " " << obj->getVoxelGridOffset()->y << " " << obj->getVoxelGridOffset()->z << "\r\n";
	}

	if (saveMaterials) {
		ArrayList<VoxlesGrid3DGeometry::Material*>* materialsList = obj->peekMaterialsList();
		*ss << "materials_count " << materialsList->count() << "\r\n";
		*ss << "materials_start" << "\r\n";
		for (int i = 0; i < materialsList->count(); i++) {
			VoxlesGrid3DGeometry::Material* material = materialsList->getDirect(i);

			// Handle nullptr materials.
			if (material != nullptr) {
				*ss << "mat " << "color " << material->color.serialize() << "\r\n";
			}
			else {
				*ss << "mat " << "NULL" << "\r\n";
			}
		}
		*ss << "materials_end" << "\r\n";
	}

	if (saveVoxelsGrid) {
		Array3D<VoxlesGrid3DGeometry::Voxel>* grid = obj->peekGrid();
		*ss << "voxels_grid_size " << std::to_string(grid->getCountA()) << " " << std::to_string(grid->getCountB()) << " " << std::to_string(grid->getCountC()) << "\r\n";
		*ss << "voxels_start" << "\r\n";
		for (int z = grid->getCountC() - 1; z >= 0; z--) {
			*ss << "voxels_plane_start" << "\r\n";

			for (int y = grid->getCountB() - 1; y >= 0; y--) {
				for (int x = 0; x < grid->getCountA(); x++) {
					VoxlesGrid3DGeometry::Voxel* voxel = grid->getPointer(x, y, z);

					if (x == 0) {
						*ss << std::to_string(voxel->matIndex);
					} else {
						*ss << " " << std::to_string(voxel->matIndex);
					}
				}

				*ss << "\r\n";
			}

			*ss << "voxels_plane_end" << "\r\n";
		}
		*ss << "voxels_end" << "\r\n";
	}

	return ss;
}

std::shared_ptr<ParseData_VoxlesGrid3DGeometry> Util_VoxlesGrid3DGeometry::parse(
	std::string& data,
	bool loadEntityGenerals, bool loadMaterials, bool loadVoxelsGrid) {
	// Split to lines.
	std::shared_ptr<ArrayList<std::string>> linesList = StringUtil::split2(data, "\n", "\r", false);
	// 10 Is a minimum number for 1 voxel and  material.
	if (!loadMaterials && !loadVoxelsGrid) {
		if (linesList->count() != 7) {
			throw DataException();
		}
	}else if (linesList->count() < 10) {
		throw DataException();
	}

	int lineIndex = 0;

	// Get the data format.
	int data_format_versionS = StringUtil::getTuple2Value_int(linesList->getReference(lineIndex++), " ", "data_format_version");

	if (data_format_versionS == DataFormatVersion_1) {
		return parse_v1(linesList, lineIndex,
			loadEntityGenerals, loadMaterials, loadVoxelsGrid
		);
	} else {
		// Unhandled data_format version.
		throw DataException();
	}
}

std::shared_ptr<ParseData_VoxlesGrid3DGeometry> Util_VoxlesGrid3DGeometry::parse_v1(
	std::shared_ptr<ArrayList<std::string>> linesList, int& lineIndex,
	bool loadEntityGenerals, bool loadMaterials, bool loadVoxelsGrid)
{
	std::shared_ptr<ParseData_VoxlesGrid3DGeometry> ret = std::make_shared<ParseData_VoxlesGrid3DGeometry>();

	if (loadEntityGenerals) {
		ret->unit_type = StringUtil::getTuple2Value_string(linesList->getReference(lineIndex++), " ", "unit_type");
		if (ret->unit_type != UnitType::creature && ret->unit_type != UnitType::hero_dummy) {
			throw DataException();
		}
		
		ret->stackable_haulOffsetZM = StringUtil::getTuple2Value_float(linesList->getReference(lineIndex++), " ", "stackable_haulOffsetZM");

		std::shared_ptr<ArrayList<std::string>> partsList = StringUtil::split(linesList->getReference(lineIndex++), " ", false);
		int partIndex = 0;
		if (partsList->count() != 4) {
			throw DataException();
		}
		if (partsList->getReference(partIndex++) != "touchable_AABBShape_min") {
			throw DataException();
		}
		ret->touchable_AABBShape_min.x = StringUtil::parseFloat(partsList->getReference(partIndex++));
		ret->touchable_AABBShape_min.y = StringUtil::parseFloat(partsList->getReference(partIndex++));
		ret->touchable_AABBShape_min.z = StringUtil::parseFloat(partsList->getReference(partIndex++));

		partsList = StringUtil::split(linesList->getReference(lineIndex++), " ", false);
		partIndex = 0;
		if (partsList->count() != 4) {
			throw DataException();
		}
		if (partsList->getReference(partIndex++) != "touchable_AABBShape_max") {
			throw DataException();
		}
		ret->touchable_AABBShape_max.x = StringUtil::parseFloat(partsList->getReference(partIndex++));
		ret->touchable_AABBShape_max.y = StringUtil::parseFloat(partsList->getReference(partIndex++));
		ret->touchable_AABBShape_max.z = StringUtil::parseFloat(partsList->getReference(partIndex++));

		ret->voxel_size = StringUtil::getTuple2Value_float(linesList->getReference(lineIndex++), " ", "voxel_size");

		partsList = StringUtil::split(linesList->getReference(lineIndex++), " ", false);
		partIndex = 0;
		if (partsList->count() != 4) {
			throw DataException();
		}
		if (partsList->getReference(partIndex++) != "grid_offset") {
			throw DataException();
		}
		ret->grid_offset.x = StringUtil::parseFloat(partsList->getReference(partIndex++));
		ret->grid_offset.y = StringUtil::parseFloat(partsList->getReference(partIndex++));
		ret->grid_offset.z = StringUtil::parseFloat(partsList->getReference(partIndex++));
	}

	if (loadMaterials) {
		ret->materialsList = parse_v1_materials(linesList, lineIndex);
	}

	if (loadVoxelsGrid) {
		ret->grid = parse_v1_voxels(linesList, lineIndex);
	}

	return ret;
}

std::shared_ptr<ArrayList<Visual3DModelMaterial*>> Util_VoxlesGrid3DGeometry::parse_v1_materials(std::shared_ptr<ArrayList<std::string>> linesList, int& lineIndex) {
	std::shared_ptr<ArrayList<Visual3DModelMaterial*>> materialsList = std::make_shared<ArrayList<Visual3DModelMaterial*>>();

	int materials_count = StringUtil::getTuple2Value_int(linesList->getReference(lineIndex++), " ", "materials_count");
	
	StringUtil::validateTuple1(linesList->getReference(lineIndex++), "materials_start");
	for (int i = 0; i < materials_count; i++) {
		std::string& line = linesList->getReference(lineIndex++);
		
		std::shared_ptr<ArrayList<std::string>> partsList = StringUtil::split(line, " ", false);
		int partIndex = 0;

		if (partsList->count() < 2) {
			throw DataException();
		}
		StringUtil::validateTuple1(partsList->getReference(partIndex++), "mat");
		std::string& part_matType = partsList->getReference(partIndex++);
		if (part_matType == "color") {
			if (partsList->count() < 3) {
				throw DataException();
			}
			
			materialsList->appendDirect(new Visual3DModelMaterial_Color(
				new Color(partsList->getReference(partIndex++))
			));
		} else if (part_matType == "NULL") {
			materialsList->appendDirect(nullptr);
		}
	}
	StringUtil::validateTuple1(linesList->getReference(lineIndex++), "materials_end");

	return materialsList;
}

std::shared_ptr < Array3D<signed char>> Util_VoxlesGrid3DGeometry::parse_v1_voxels(std::shared_ptr<ArrayList<std::string>> linesList, int& lineIndex) {
	int partIndex = 0;
	std::shared_ptr<ArrayList<std::string>> partsList = StringUtil::split(linesList->getReference(lineIndex++), " ", false);
	if (partsList->count() != 4) {
		throw DataException();
	}

	if (partsList->getReference(partIndex++) != "voxels_grid_size") {
		throw DataException();
	}
	Vector3Int voxelsGridSize{};
	voxelsGridSize.x = StringUtil::parseInt(partsList->getReference(partIndex++));
	voxelsGridSize.y = StringUtil::parseInt(partsList->getReference(partIndex++));
	voxelsGridSize.z = StringUtil::parseInt(partsList->getReference(partIndex++));

	std::shared_ptr < Array3D<signed char>> grid = std::make_shared< Array3D<signed char>>(
		voxelsGridSize.x,
		voxelsGridSize.y,
		voxelsGridSize.z
	);

	StringUtil::validateTuple1(linesList->getReference(lineIndex++), "voxels_start");
	for (int z = grid->getCountC() - 1; z >= 0; z--) {
		StringUtil::validateTuple1(linesList->getReference(lineIndex++), "voxels_plane_start");

		for (int y = grid->getCountB() - 1; y >= 0; y--) {
			int partIndex = 0;
			std::shared_ptr<ArrayList<std::string>> partsList = StringUtil::split(linesList->getReference(lineIndex++), " ", false);

			if (partsList->count() != grid->getCountA()) {
				throw DataException();
			}

			for (int x = 0; x < grid->getCountA(); x++) {
				grid->setDirect(x, y, z, StringUtil::parseInt(partsList->getReference(partIndex++)));
			}
		}

		StringUtil::validateTuple1(linesList->getReference(lineIndex++), "voxels_plane_end");
	}

	StringUtil::validateTuple1(linesList->getReference(lineIndex++), "voxels_end");

	return grid;
}
