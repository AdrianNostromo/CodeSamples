#include "TC_GroundOrWall.h"
#include <worldGame3D/gw/entity/template/visual3D/Visual3DTemplate.h>
#include <base/visual3d/util/Drawable3DCreateConfig_Group.h>
#include <base/visual3d/material/Visual3DModelMaterial_Color.h>
#include <base/math/Bounds2D.h>
#include <worldGame3D/gw/entity/template/frustumCUllable3D/FrustumCullable3DTemplate.h>
#include <sh/scenarios/battlezones_a_v1/gw/config/entity/EntityTemplatesList.h>
#include <sh/scenarios/battlezones_a_v1/gw/config/entity/EntityTags.h>
#include <worldGame3D/gw/entity/template/EntityTemplate.h>
#include <base/visual3d/util/Drawable3DCreateConfig_VoxelGrid.h>
#include <base/statics/StaticsInit.h>
#include "../../../../Namespace.h"
#include <rpg3D/gw/entity/template/physics3D/Physics3DTemplate.h>
#include <base/physics3D/config/shape/PhysicsShapeConfigTiledFloorGridXY.h>
#include <base/physics3D/config/shape/PhysicsShapeConfigTiledWallLinearX.h>
#include <base/physics3D/config/shape/PhysicsShapeConfigTiledWallGridXY.h>
#include <rpg3D/gw/config/physics/PhysicsCollissionConfig.h>
#include <rpg3D/gw/physics3D/util/PhysicsUtil.h>
#include <base/util/StringUtil.h>
#include <base/exceptions/LogicException.h>

using namespace rpg3D;

bool TC_GroundOrWall::IsStaticsInit = base::StaticsInit::AddCbGeneral<bool>(1, []() {
	EntityTemplate::customTemplatesFactoriesMap.putDirect(Namespace::NAMESPACE, BaseId, TC_GroundOrWall::GetOrCreateTemplateFromConfigParams);
});

std::string TC_GroundOrWall::BaseId = "TC_GroundOrWall_";
std::string TC_GroundOrWall::DATA_KEY_materials = "_materials_";

Vector2Int TC_GroundOrWall::GridSizeXY{ 1, 1 };
Vector3Int TC_GroundOrWall::TileVoxelsCount_1Layer{ GridSizeXY.x, GridSizeXY.y, 1 };
Vector3Int TC_GroundOrWall::TileVoxelsCount_2Layers{ GridSizeXY.x, GridSizeXY.y, 2 };

Array3D<signed char>* TC_GroundOrWall::getOrCreateGridGrassA() {
	static Array3D<signed char>* var = nullptr;
	if (var == nullptr) {
		if (!TileVoxelsCount_1Layer.equals(1, 1, 1)) {
			throw LogicException(LOC);
		}
		var = new Array3D<signed char>(
			TileVoxelsCount_1Layer.x, TileVoxelsCount_1Layer.y, TileVoxelsCount_1Layer.z
		);
		var->setDirect(0, 0, 0, MaterialIndices::GrassA);
	}

	return var;
}

Array3D<signed char>* TC_GroundOrWall::getOrCreateGridGrassB() {
	static Array3D<signed char>* var = nullptr;
	if (var == nullptr) {
		if (!TileVoxelsCount_1Layer.equals(1, 1, 1)) {
			throw LogicException(LOC);
		}
		var = new Array3D<signed char>(
			TileVoxelsCount_1Layer.x, TileVoxelsCount_1Layer.y, TileVoxelsCount_1Layer.z
		);
		var->setDirect(0, 0, 0, MaterialIndices::GrassB);
	}

	return var;
}

Array3D<signed char>* TC_GroundOrWall::getOrCreateGridStoneA() {
	static Array3D<signed char>* var = nullptr;
	if (var == nullptr) {
		if (!TileVoxelsCount_1Layer.equals(1, 1, 1)) {
			throw LogicException(LOC);
		}
		var = new Array3D<signed char>(
			TileVoxelsCount_1Layer.x, TileVoxelsCount_1Layer.y, TileVoxelsCount_1Layer.z
		);
		var->setDirect(0, 0, 0, MaterialIndices::StoneA);
	}

	return var;
}

Array3D<signed char>* TC_GroundOrWall::getOrCreateGridStoneB() {
	static Array3D<signed char>* var = nullptr;
	if (var == nullptr) {
		if (!TileVoxelsCount_1Layer.equals(1, 1, 1)) {
			throw LogicException(LOC);
		}
		var = new Array3D<signed char>(
			TileVoxelsCount_1Layer.x, TileVoxelsCount_1Layer.y, TileVoxelsCount_1Layer.z
		);
		var->setDirect(0, 0, 0, MaterialIndices::StoneB);
	}

	return var;
}

Array3D<signed char>* TC_GroundOrWall::getOrCreateGridStoneC() {
	static Array3D<signed char>* var = nullptr;
	if (var == nullptr) {
		if (!TileVoxelsCount_1Layer.equals(1, 1, 1)) {
			throw LogicException(LOC);
		}
		var = new Array3D<signed char>(
			TileVoxelsCount_1Layer.x, TileVoxelsCount_1Layer.y, TileVoxelsCount_1Layer.z
		);
		var->setDirect(0, 0, 0, MaterialIndices::StoneC);
	}

	return var;
}

Array3D<signed char>* TC_GroundOrWall::getOrCreateGridStoneD() {
	static Array3D<signed char>* var = nullptr;
	if (var == nullptr) {
		if (!TileVoxelsCount_1Layer.equals(1, 1, 1)) {
			throw LogicException(LOC);
		}
		var = new Array3D<signed char>(
			TileVoxelsCount_1Layer.x, TileVoxelsCount_1Layer.y, TileVoxelsCount_1Layer.z
		);
		var->setDirect(0, 0, 0, MaterialIndices::StoneD);
	}

	return var;
}

Array3D<signed char>* TC_GroundOrWall::getOrCreateGrid_None_None() {
	static Array3D<signed char>* var = nullptr;
	if (var == nullptr) {
		if (!TileVoxelsCount_2Layers.equals(1, 1, 2)) {
			throw LogicException(LOC);
		}
		var = new Array3D<signed char>(
			TileVoxelsCount_2Layers.x, TileVoxelsCount_2Layers.y, TileVoxelsCount_2Layers.z
		);
		var->setDirect(0, 0, 1, MaterialIndices::Empty);
		var->setDirect(0, 0, 0, MaterialIndices::Empty);
	}

	return var;
}

Array3D<signed char>* TC_GroundOrWall::getOrCreateGridGrassADirtA() {
	static Array3D<signed char>* var = nullptr;
	if (var == nullptr) {
		if (!TileVoxelsCount_2Layers.equals(1, 1, 2)) {
			throw LogicException(LOC);
		}
		var = new Array3D<signed char>(
			TileVoxelsCount_2Layers.x, TileVoxelsCount_2Layers.y, TileVoxelsCount_2Layers.z
		);
		var->setDirect(0, 0, 1, MaterialIndices::GrassA);
		var->setDirect(0, 0, 0, MaterialIndices::DirtA);
	}

	return var;
}

Array3D<signed char>* TC_GroundOrWall::getOrCreateGridGrassBDirtA() {
	static Array3D<signed char>* var = nullptr;
	if (var == nullptr) {
		if (!TileVoxelsCount_2Layers.equals(1, 1, 2)) {
			throw LogicException(LOC);
		}
		var = new Array3D<signed char>(
			TileVoxelsCount_2Layers.x, TileVoxelsCount_2Layers.y, TileVoxelsCount_2Layers.z
			);
		var->setDirect(0, 0, 1, MaterialIndices::GrassB);
		var->setDirect(0, 0, 0, MaterialIndices::DirtA);
	}

	return var;
}

ArrayList<Visual3DModelMaterial*>* TC_GroundOrWall::getOrCreateMaterials() {
	static ArrayList<Visual3DModelMaterial*>* materialsList = nullptr;
	if (materialsList == nullptr) {
		materialsList = new ArrayList<Visual3DModelMaterial*>();
		materialsList->appendDirect(new Visual3DModelMaterial_Color(new Color("#78C214FF")));// grassA
		materialsList->appendDirect(new Visual3DModelMaterial_Color(new Color("#70B80DFF")));// grassB
		materialsList->appendDirect(new Visual3DModelMaterial_Color(new Color("#B8B7BCFF")));// stoneA
		materialsList->appendDirect(new Visual3DModelMaterial_Color(new Color("#A0A0A5FF")));// stoneB
		materialsList->appendDirect(new Visual3DModelMaterial_Color(new Color("#888888FF")));// stoneC
		materialsList->appendDirect(new Visual3DModelMaterial_Color(new Color("#777777FF")));// stoneD
		materialsList->appendDirect(new Visual3DModelMaterial_Color(new Color("#AF7C45FF")));// dirtA
	}

	return materialsList;
}

EntityTemplate* TC_GroundOrWall::getOrCreateTemplate(
	UseType useType, PhysicsType physicsType, Vector3Int& tileVoxelsGridSize,
	unsigned int voxelAndTileSizeCM, float heightScale)
{
	sp<Array1D<std::shared_ptr<base::IWrappedValue>>> configValues = new Array1D<std::shared_ptr<base::IWrappedValue>>(5);
	configValues->setDirect(0, base::IWrappedValue::new_int(useType));
	configValues->setDirect(1, base::IWrappedValue::new_int(physicsType));
	configValues->setDirect(2, base::IWrappedValue::new_Vector3Int(tileVoxelsGridSize));
	configValues->setDirect(3, base::IWrappedValue::new_uint(voxelAndTileSizeCM));
	configValues->setDirect(4, base::IWrappedValue::new_float(heightScale));

	return GetOrCreateTemplateFromConfigParams(configValues);
}

EntityTemplate* TC_GroundOrWall::GetOrCreateTemplateFromConfigParams(sp<Array1D<std::shared_ptr<base::IWrappedValue>>>& configValues) {
	int useType = configValues->getReference(0)->getDirectAs_int();
	int physicsType = configValues->getReference(1)->getDirectAs_int();
	Vector3Int tileVoxelsGridSize{ configValues->getReference(2)->getReferenceAs_Vector3Int() };
	unsigned int voxelAndTileSizeCM = configValues->getReference(3)->getDirectAs_uint();
	float heightScale = configValues->getReference(4)->getDirectAs_float();

	if (!tileVoxelsGridSize.equals(1, 1, 1) && !tileVoxelsGridSize.equals(1, 1, 2)) {
		// Note. Currently only 1 voxel per tile is implemented. Implement any tile voxels grid size when needed
		/// (probably for walls when dynamic textures are implemented).
		throw LogicException(LOC);
	}

	std::string smartId = BaseId + "#useType#_#physicsType#_#tileVoxelsGSizeX#_#tileVoxelsGSizeY#_#tileVoxelsGSizeZ#_#voxelAndTileSizeCM#_#heightScale#";

	StringUtil::replace(smartId, "#useType#", std::to_string(useType));
	StringUtil::replace(smartId, "#physicsType#", std::to_string(physicsType));

	StringUtil::replace(smartId, "#tileVoxelsGSizeX#", std::to_string(tileVoxelsGridSize.x));
	StringUtil::replace(smartId, "#tileVoxelsGSizeY#", std::to_string(tileVoxelsGridSize.y));
	StringUtil::replace(smartId, "#tileVoxelsGSizeZ#", std::to_string(tileVoxelsGridSize.z));

	StringUtil::replace(smartId, "#voxelAndTileSizeCM#", std::to_string(voxelAndTileSizeCM));

	StringUtil::replace(smartId, "#heightScale#", StringUtil::floatToFixedPrecisionString(heightScale, Math::FLOAT_PRECISSION_DECIMALS_COUNT));

	EntityTemplate* t = EntityTemplate::fromId(Namespace::NAMESPACE, smartId);
	if (t != nullptr) {
		return t;
	}

	t = EntityTemplate::newEntry(Namespace::NAMESPACE, smartId, BaseId, configValues);

	t->tagsList.appendReference(rpg3D::EntityTags::PLATFORM);
	t->tagsList.appendReference(rpg3D::EntityTags::STRUCTURE);

	t->setModuleTemplate(new Visual3DTemplate(
		new Visual3DCreateConfig(
			(new ArrayList<ArrayList<Drawable3DCreateConfigBase*>*>())
			->appendDirect_chain((new ArrayList<Drawable3DCreateConfigBase*>())
				->appendDirect_chain(new Drawable3DCreateConfig_Group(
					"_ROOT_",
					nullptr,
					nullptr,
					nullptr,
					nullptr
				))
				->appendDirect_chain(buildBase(voxelAndTileSizeCM, tileVoxelsGridSize, heightScale, useType))
			)
		),
		false,//requiresHalfGridOffsetX
		false//requiresHalfGridOffsetY
	));

	if (physicsType != None) {
		physics3D::PhysicsShapeConfig* pShapeConfig = nullptr;
		if (physicsType == Floor_TiledGrid2D) {
			pShapeConfig = new physics3D::PhysicsShapeConfigTiledFloorGridXY(
				0.0f/*collisionMargin*/,
				Vector2(
					tileVoxelsGridSize.x * (voxelAndTileSizeCM / 100.0f/*size_m*/),
					tileVoxelsGridSize.y * (voxelAndTileSizeCM / 100.0f/*size_m*/)
				)/*tileSize*/
			);
		} else if (physicsType == Wall_TiledLinear) {
			pShapeConfig = new physics3D::PhysicsShapeConfigTiledWallLinearX(
				rpg3D::PhysicsUtil::DEFAULT_COLLISSION_MARGIN/*collisionMargin*/,
				Vector3(
					tileVoxelsGridSize.x * (voxelAndTileSizeCM / 100.0f/*size_m*/),
					tileVoxelsGridSize.y * (voxelAndTileSizeCM / 100.0f/*size_m*/),
					tileVoxelsGridSize.z * (voxelAndTileSizeCM / 100.0f/*size_m*/)
				)/*tileSize*/
			);
		} else if (physicsType == Wall_TiledGrid2D) {
			pShapeConfig = new physics3D::PhysicsShapeConfigTiledWallGridXY(
				rpg3D::PhysicsUtil::DEFAULT_COLLISSION_MARGIN/*collisionMargin*/,
				Vector3(
					tileVoxelsGridSize.x * (voxelAndTileSizeCM / 100.0f/*size_m*/),
					tileVoxelsGridSize.y * (voxelAndTileSizeCM / 100.0f/*size_m*/),
					tileVoxelsGridSize.z * (voxelAndTileSizeCM / 100.0f/*size_m*/)
				)/*tileSize*/
			);
		} else {
			throw LogicException(LOC);
		}

		rpg3D::Physics3DTemplate* physics3D;
		t->setModuleTemplate(physics3D = new rpg3D::Physics3DTemplate(
			rpg3D::PhysicsCollissionConfig::Environment,
			nullptr/*pActorControllerCfg*/
		));
		physics3D->pBodiesConfigsList.append_emplace_chain(
			rpg3D::PBodyConfig::UssageBitGroups::Movement | rpg3D::PBodyConfig::UssageBitGroups::CardinalRotation | rpg3D::PBodyConfig::UssageBitGroups::VisualsUpdate,
			rpg3D::PBodyConfig::DynamicsType::Static/*dynamicsType*/,
			nullptr/*physicsBodyOffsetPos*/, nullptr/*physicsBodyOffsetRot*/,
			// Keep convex hull because this may use a composed physics shape in the future.
			// Collision margin is 0 by default for btBvhTriangleMeshShape that will be created. No collision margin helps with the edge so it is not inset.
			pShapeConfig,
			0.0f/*mass*/, 0.8f/*friction*/, 0.0f/*rollingFriction*/,
			Vector3{ 0, 0, 0 }/*enabledRotationAxes*/,
			0.6f/*linearDamping*/, 0.0f/*angularDamping*/,
			nullptr/*customGravity*/
		);
	}

	return t;
}

Drawable3DCreateConfig_VoxelGrid* TC_GroundOrWall::buildBase(unsigned int voxelAndTileSizeCM, Vector3Int& tileVoxelsGridSize, float heightScale, int/*UseType*/ useType) {
	Vector3* voxelGridOffset = nullptr;
	if (useType == Floor) {
		voxelGridOffset = new Vector3(0.5f, 0.5f, 0.5f - tileVoxelsGridSize.z * 1.0f);
	} else if (useType == Wall) {
		voxelGridOffset = new Vector3(0.5f, 0.5f, 0.5f);
	} else {
		throw LogicException(LOC);
	}

	return new Drawable3DCreateConfig_VoxelGrid(
		"_VoxelGridGroup_",//id
		new std::string("_ROOT_"),//parentId

		nullptr,// pos
		nullptr,//rot
		new Vector3(1.0f, 1.0f, heightScale),// scale

		voxelAndTileSizeCM / 100.0f/*voxelSizeM*/,
		voxelGridOffset,

		nullptr, nullptr,
		nullptr, &DATA_KEY_materials
    );
}
