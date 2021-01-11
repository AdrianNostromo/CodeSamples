#pragma once

#include <base/gh.h>
#include <graphics/visual3d/drawable/VoxlesGrid3DGeometry.h>
#include <base/visual3d/material/Visual3DModelMaterial.h>

namespace rpg3D {
class ParseData_VoxlesGrid3DGeometry {pub dCtor(ParseData_VoxlesGrid3DGeometry);
	pub std::string unit_type = "";

	pub std::shared_ptr<ArrayList<Visual3DModelMaterial*>> materialsList = nullptr;
	pub std::shared_ptr < Array3D<signed char>> grid = nullptr;

	pub float stackable_haulOffsetZM = 0.0f;
	pub Vector3 touchable_AABBShape_min{};
	pub Vector3 touchable_AABBShape_max{};

	pub float voxel_size = 0.0f;
	pub Vector3 grid_offset{};

    pub explicit ParseData_VoxlesGrid3DGeometry();

    pub virtual ~ParseData_VoxlesGrid3DGeometry();
};
};
