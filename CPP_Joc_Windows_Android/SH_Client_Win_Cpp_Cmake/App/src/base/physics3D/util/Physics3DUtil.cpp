#include "Physics3DUtil.h"
#include <base/physics3D/config/shape/PhysicsShapeConfigBox.h>
#include <base/physics3D/config/shape/PhysicsShapeConfigSphere.h>
#include <base/physics3D/config/shape/PhysicsShapeConfigCylinder.h>
#include <base/physics3D/config/shape/PhysicsShapeConfigCapsule.h>
#include <base/physics3D/config/shape/PhysicsShapeConfigMultiSphere.h>
#include <base/physics3D/config/shape/PhysicsShapeConfigConvexHull.h>
#include <base/physics3D/config/shape/PhysicsShapeConfigTiledWallLinearX.h>
#include <base/physics3D/config/shape/PhysicsShapeConfigTiledFloorGridXY.h>
#include <base/physics3D/config/shape/PhysicsShapeConfigTiledWallGridXY.h>
#include <base/physics3D/config/shape/PSC_BvhTriangleMeshShape.h>
#include <LinearMath/btVector3.h>
#include <btBulletDynamicsCommon.h>
#include <graphics/visual3d/drawable/voxelsGrid/tiling/TilingInformation_LinearX.h>
#include <graphics/visual3d/drawable/voxelsGrid/tiling/TilingInformation_GridXY.h>
#include <base/physics3D/config/body/PBodyConfig.h>
#include <base/MM.h>
#include <base/memory/IMemoryManager.h>
#include <graphics/visual3d/util/VoxelSides.h>

using namespace physics3D;

btRigidBody* Physics3DUtil::BuildPBody(
	rpg3D::PBodyConfig* pBodyConfig, float scaledMass, btMotionState* pMotionState, btCollisionShape* pShape, const btVector3& localInertia)
{
	btRigidBody::btRigidBodyConstructionInfo rbInfo(scaledMass, pMotionState, pShape, localInertia);

	btRigidBody* pBody = new btRigidBody(rbInfo);

	if (pBodyConfig->dynamicsType == rpg3D::PBodyConfig::DynamicsType::Static) {
		if (pBodyConfig->mass != 0.0f) {
			// Only dynamic objects can have mass.
			throw LogicException(LOC);
		}

		pBody->setCollisionFlags(pBody->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
	} else if (pBodyConfig->dynamicsType == rpg3D::PBodyConfig::DynamicsType::Kinematic) {
		if (pBodyConfig->mass != 0.0f) {
			// Only dynamic objects can have mass.
			throw LogicException(LOC);
		}

		pBody->setCollisionFlags(pBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	} else if (pBodyConfig->dynamicsType == rpg3D::PBodyConfig::DynamicsType::Dynamic) {
		if (pBodyConfig->mass <= 0.0f) {
			// Dynamic objects need a valid mass.
			throw LogicException(LOC);
		}

		//void
	} else {
		// Unahdled value.
		throw LogicException(LOC);
	}

	pBody->setFriction(pBodyConfig->friction);
	pBody->setRollingFriction(pBodyConfig->rollingFriction);

	pBody->setDamping(pBodyConfig->linearDamping, pBodyConfig->angularDamping);

	// Allow movement on all 3 axes.
	pBody->setLinearFactor(btVector3(1, 1, 1));
	pBody->setAngularFactor(btVector3(pBodyConfig->enabledRotationAxes.x, pBodyConfig->enabledRotationAxes.y, pBodyConfig->enabledRotationAxes.z));

	return pBody;
}

btCollisionShape* Physics3DUtil::BuildShape(physics3D::PhysicsShapeConfig* pShapeConfig, std::shared_ptr<graphics::TilingInformation> tilingInformation) {
	btCollisionShape* pShape;
	if (pShapeConfig->type == physics3D::PhysicsShapeConfigBox::TYPE) {
		physics3D::PhysicsShapeConfigBox* cPShapeConfig = pShapeConfig->getAs<physics3D::PhysicsShapeConfigBox*>(physics3D::PhysicsShapeConfigBox::TYPE);

		btVector3 halfExtents{ cPShapeConfig->halfExtents.x, cPShapeConfig->halfExtents.y, cPShapeConfig->halfExtents.z };

		btBoxShape* cPShape;
		pShape = cPShape = new btBoxShape(halfExtents);
		float ff = cPShape->getMargin();
		cPShape->setMargin(cPShapeConfig->collisionMargin);
	} else if (pShapeConfig->type == physics3D::PhysicsShapeConfigSphere::TYPE) {
		physics3D::PhysicsShapeConfigSphere* cPShapeConfig = pShapeConfig->getAs<physics3D::PhysicsShapeConfigSphere*>(physics3D::PhysicsShapeConfigSphere::TYPE);

		btSphereShape* cPShape;
		pShape = cPShape = new btSphereShape(cPShapeConfig->radius);

		// Spheres don't require collission margin, just set the final radius.
		//cPShape->setMargin(cPShapeConfig->collisionMargin);
	} else if (pShapeConfig->type == physics3D::PhysicsShapeConfigCylinder::TYPE) {
		physics3D::PhysicsShapeConfigCylinder* cPShapeConfig = pShapeConfig->getAs<physics3D::PhysicsShapeConfigCylinder*>(physics3D::PhysicsShapeConfigCylinder::TYPE);

		btVector3 halfExtents{ cPShapeConfig->halfExtents.x, cPShapeConfig->halfExtents.y, cPShapeConfig->halfExtents.z };

		btCylinderShape* cPShape;
		if (cPShapeConfig->mainAxis.equals(Vector3::X)) {
			cPShape = new btCylinderShapeX(halfExtents);
		} else if (cPShapeConfig->mainAxis.equals(Vector3::Y)) {
			// btCylinderShape is Y aligned.
			cPShape = new btCylinderShape(halfExtents);
		} else if (cPShapeConfig->mainAxis.equals(Vector3::Z)) {
			cPShape = new btCylinderShapeZ(halfExtents);
		} else {
			throw LogicException(LOC);
		}

		float ff = cPShape->getMargin();
		cPShape->setMargin(cPShapeConfig->collisionMargin);

		pShape = cPShape;
	} else if (pShapeConfig->type == physics3D::PhysicsShapeConfigCapsule::TYPE) {
		physics3D::PhysicsShapeConfigCapsule* cPShapeConfig = pShapeConfig->getAs<physics3D::PhysicsShapeConfigCapsule*>(physics3D::PhysicsShapeConfigCapsule::TYPE);

		btCapsuleShape* cPShape;
		if (cPShapeConfig->mainAxis.equals(Vector3::X)) {
			cPShape = new btCapsuleShapeX(cPShapeConfig->endDomesRadius, cPShapeConfig->cylinderPartHeight);
		} else if (cPShapeConfig->mainAxis.equals(Vector3::Y)) {
			// btCapsuleShape is Y aligned.
			cPShape = new btCapsuleShape(cPShapeConfig->endDomesRadius, cPShapeConfig->cylinderPartHeight);
		} else if (cPShapeConfig->mainAxis.equals(Vector3::Z)) {
			cPShape = new btCapsuleShapeZ(cPShapeConfig->endDomesRadius, cPShapeConfig->cylinderPartHeight);
		} else {
			throw LogicException(LOC);
		}

		// Capsules don't use margin. Their entire radius == margin;
		//cPShape->setMargin(cPShapeConfig->collisionMargin);

		pShape = cPShape;
	} else if (pShapeConfig->type == physics3D::PhysicsShapeConfigMultiSphere::TYPE) {
		physics3D::PhysicsShapeConfigMultiSphere* cPShapeConfig = pShapeConfig->getAs<physics3D::PhysicsShapeConfigMultiSphere*>(physics3D::PhysicsShapeConfigMultiSphere::TYPE);

		int spheresCount = cPShapeConfig->sphereInfos.getCount();

		btVector3* positions = new btVector3[spheresCount];
		btScalar* radii = new btScalar[spheresCount];
		for (int i = 0; i < spheresCount; i++) {
			physics3D::PhysicsShapeConfigMultiSphere::SphereInfo* sphereInfo = cPShapeConfig->sphereInfos.getPointer(i);

			positions[i] = btVector3{ sphereInfo->position.x, sphereInfo->position.y, sphereInfo->position.z };
			radii[i] = sphereInfo->radius;
		}

		pShape = new btMultiSphereShape(positions, radii, spheresCount);
	} else if (pShapeConfig->type == physics3D::PhysicsShapeConfigConvexHull::TYPE) {
		physics3D::PhysicsShapeConfigConvexHull* cPShapeConfig = pShapeConfig->getAs<physics3D::PhysicsShapeConfigConvexHull*>(physics3D::PhysicsShapeConfigConvexHull::TYPE);

		btConvexHullShape* cPShape = new btConvexHullShape(reinterpret_cast<btScalar*>(cPShapeConfig->points.data), cPShapeConfig->points.getCount(), sizeof(physics3D::PhysicsShapeConfigConvexHull::Vec3));
		float ff = cPShape->getMargin();
		cPShape->setMargin(cPShapeConfig->collisionMargin);

		pShape = cPShape;
	} else if (pShapeConfig->type == physics3D::PhysicsShapeConfigTiledWallLinearX::TYPE) {
		physics3D::PhysicsShapeConfigTiledWallLinearX* cPShapeConfig = pShapeConfig->getAs<physics3D::PhysicsShapeConfigTiledWallLinearX*>(physics3D::PhysicsShapeConfigTiledWallLinearX::TYPE);

		if (tilingInformation == nullptr || tilingInformation->getType() != graphics::TilingInformation_LinearX::TYPE) {
			throw LogicException(LOC);
		}

		graphics::TilingInformation_LinearX* cTilingInformation = tilingInformation->castAs<graphics::TilingInformation_LinearX*>(graphics::TilingInformation_LinearX::TYPE);

		float CM = cPShapeConfig->collisionMargin;

		float xx = 0.0f + CM;
		float XX = cPShapeConfig->tileSize.x * cTilingInformation->tilingLengthCount - CM;
		float yy = 0.0f + CM;
		float YY = cPShapeConfig->tileSize.y - CM;
		float zz = 0.0f + CM;
		float ZZ = cPShapeConfig->tileSize.z - CM;

		physics3D::PhysicsShapeConfigConvexHull::Vec3 verts[8]{
				{xx, yy, zz}, {xx, YY, zz}, {XX, YY, zz}, {XX, yy, zz},
				{xx, yy, ZZ}, {xx, YY, ZZ}, {XX, YY, ZZ}, {XX, yy, ZZ}
		};

		//asd_x;// use a param to control which sides have geometry built and use that to not create bottom triangles for walls.

		btConvexHullShape* cPShape = new btConvexHullShape(reinterpret_cast<btScalar*>(verts), 8, sizeof(physics3D::PhysicsShapeConfigConvexHull::Vec3));
		float ff = cPShape->getMargin();
		cPShape->setMargin(cPShapeConfig->collisionMargin);

		pShape = cPShape;
	} else if (pShapeConfig->type == physics3D::PhysicsShapeConfigTiledFloorGridXY::TYPE) {
		physics3D::PhysicsShapeConfigTiledFloorGridXY* cPShapeConfig = pShapeConfig->getAs<physics3D::PhysicsShapeConfigTiledFloorGridXY*>(physics3D::PhysicsShapeConfigTiledFloorGridXY::TYPE);

		if (tilingInformation == nullptr || tilingInformation->getType() != graphics::TilingInformation_GridXY::TYPE) {
			throw LogicException(LOC);
		}

		graphics::TilingInformation_GridXY* cTilingInformation = tilingInformation->castAs<graphics::TilingInformation_GridXY*>(graphics::TilingInformation_GridXY::TYPE);

		// Create a distancesGrid to prepare for the triangulation algo.
		// This also checks if the room is a filled rectangular room that doesn't require triangulation.
		if (cTilingInformation->tilingGrid->getLengthA() > 100 || cTilingInformation->tilingGrid->getLengthB() > 100) {
			// The grid is larger than what this algo supports  (char distances), convert distancesGrid to a short.
			throw LogicException(LOC);
		}
		Array2D<signed char> distancesGridX{ cTilingInformation->tilingGrid->getLengthA(), cTilingInformation->tilingGrid->getLengthB() };
		bool isTriangulationRequired = BuildTriangulationDistancesGrid(*cTilingInformation->tilingGrid, distancesGridX);
		if (isTriangulationRequired) {
			// Create quads from distancesGrid and convert them to triangles.
			btTriangleMesh* triangleMeshTerrain = new btTriangleMesh();

			Vector3 tileSize3D{ cPShapeConfig->tileSize.x, cPShapeConfig->tileSize.y, 0 };
			TriangulateDistancesGrid(
				distancesGridX, tileSize3D, cTilingInformation->geometryGridOffset, cPShapeConfig->collisionMargin,
				// Only draw zMax instead of zMin because the collision margin is correct like this.
				/*VoxelSides::BitGroup_xMin | VoxelSides::BitGroup_xMax | VoxelSides::BitGroup_yMin | VoxelSides::BitGroup_yMax | VoxelSides::BitGroup_zMin |*/ VoxelSides::BitGroup_zMax/*enabledSidesTriangulationBitMask*/,
				triangleMeshTerrain
			);

			btBvhTriangleMeshShape* cPShape = new btBvhTriangleMeshShape(triangleMeshTerrain, true);
			float ff = cPShape->getMargin();
			cPShape->setMargin(cPShapeConfig->collisionMargin);

			pShape = cPShape;
		} else {
			float CM = cPShapeConfig->collisionMargin;

			float xx = 0.0f + CM;
			float XX = cPShapeConfig->tileSize.x * cTilingInformation->tilingGrid->getLengthA() - CM;
			float yy = 0.0f + CM;
			float YY = cPShapeConfig->tileSize.y * cTilingInformation->tilingGrid->getLengthB() - CM;
			//float zz = not_used;
			float ZZ = 0.0f - CM;

			physics3D::PhysicsShapeConfigConvexHull::Vec3 verts[4]{
					{xx, yy, ZZ}, {xx, YY, ZZ}, {XX, YY, ZZ}, {XX, yy, ZZ}
			};
			btConvexHullShape* cPShape = new btConvexHullShape(reinterpret_cast<btScalar*>(verts), 4, sizeof(physics3D::PhysicsShapeConfigConvexHull::Vec3));
			float ff = cPShape->getMargin();
			cPShape->setMargin(cPShapeConfig->collisionMargin);

			pShape = cPShape;
		}
	} else if (pShapeConfig->type == physics3D::PhysicsShapeConfigTiledWallGridXY::TYPE) {
		physics3D::PhysicsShapeConfigTiledWallGridXY* cPShapeConfig = pShapeConfig->getAs<physics3D::PhysicsShapeConfigTiledWallGridXY*>(physics3D::PhysicsShapeConfigTiledWallGridXY::TYPE);

		if (tilingInformation == nullptr || tilingInformation->getType() != graphics::TilingInformation_GridXY::TYPE) {
			throw LogicException(LOC);
		}

		graphics::TilingInformation_GridXY* cTilingInformation = tilingInformation->castAs<graphics::TilingInformation_GridXY*>(graphics::TilingInformation_GridXY::TYPE);

		// Create a distancesGrid to prepare for the triangulation algo.
		// This also checks if the room is a filled rectangular room that doesn't require triangulation.
		if (cTilingInformation->tilingGrid->getLengthA() > 100 || cTilingInformation->tilingGrid->getLengthB() > 100) {
			// The grid is larger than what this algo supports  (char distances), convert distancesGrid to a short.
			throw LogicException(LOC);
		}
		Array2D<signed char> distancesGridX{ cTilingInformation->tilingGrid->getLengthA(), cTilingInformation->tilingGrid->getLengthB() };
		bool isTriangulationRequired = BuildTriangulationDistancesGrid(*cTilingInformation->tilingGrid, distancesGridX);
		if (isTriangulationRequired) {
			// Create quads from distancesGrid and convert them to triangles.
			btTriangleMesh* triangleMeshTerrain = new btTriangleMesh();

			TriangulateDistancesGrid(
				distancesGridX, cPShapeConfig->tileSize, cTilingInformation->geometryGridOffset, cPShapeConfig->collisionMargin,
				VoxelSides::BitGroup_xMin | VoxelSides::BitGroup_xMax | VoxelSides::BitGroup_yMin | VoxelSides::BitGroup_yMax /*| VoxelSides::BitGroup_zMin*/ | VoxelSides::BitGroup_zMax/*enabledSidesTriangulationBitMask*/,
				triangleMeshTerrain
			);

			btBvhTriangleMeshShape* cPShape = new btBvhTriangleMeshShape(triangleMeshTerrain, true);
			float ff = cPShape->getMargin();
			cPShape->setMargin(cPShapeConfig->collisionMargin);

			pShape = cPShape;
		} else {
			float CM = cPShapeConfig->collisionMargin;

			//asd_x;// use a param to control which sides have geometry built.

			float xx = 0.0f + CM;
			float XX = cPShapeConfig->tileSize.x * cTilingInformation->tilingGrid->getLengthA() - CM;
			float yy = 0.0f + CM;
			float YY = cPShapeConfig->tileSize.y * cTilingInformation->tilingGrid->getLengthB() - CM;
			float zz = 0.0f + CM;
			float ZZ = cPShapeConfig->tileSize.z - CM;

			physics3D::PhysicsShapeConfigConvexHull::Vec3 verts[8]{
				{xx, yy, zz}, {xx, YY, zz}, {XX, YY, zz}, {XX, yy, zz},
				{xx, yy, ZZ}, {xx, YY, ZZ}, {XX, YY, ZZ}, {XX, yy, ZZ}
			};

			btConvexHullShape* cPShape = new btConvexHullShape(reinterpret_cast<btScalar*>(verts), 8, sizeof(physics3D::PhysicsShapeConfigConvexHull::Vec3));
			float ff = cPShape->getMargin();
			cPShape->setMargin(cPShapeConfig->collisionMargin);

			pShape = cPShape;
		}
	} else if (pShapeConfig->type == physics3D::PSC_BvhTriangleMeshShape::TYPE) {
		physics3D::PSC_BvhTriangleMeshShape* cPShapeConfig = pShapeConfig->getAs<physics3D::PSC_BvhTriangleMeshShape*>(physics3D::PSC_BvhTriangleMeshShape::TYPE);

		btTriangleMesh* triangleMeshTerrain = new btTriangleMesh();

		int trianglesCount = cPShapeConfig->points.count() / 3;
		physics3D::PSC_BvhTriangleMeshShape::Vec3* pointsArrayBuff = cPShapeConfig->points.getData();
		for (int i = 0; i < trianglesCount; i++) {
			physics3D::PSC_BvhTriangleMeshShape::Vec3& v1 = pointsArrayBuff[(i * 3) + 0];
			physics3D::PSC_BvhTriangleMeshShape::Vec3& v2 = pointsArrayBuff[(i * 3) + 1];
			physics3D::PSC_BvhTriangleMeshShape::Vec3& v3 = pointsArrayBuff[(i * 3) + 2];
			btVector3 p1{ v1.x, v1.y, v1.z };
			btVector3 p2{ v2.x, v2.y, v2.z };
			btVector3 p3{ v3.x, v3.y, v3.z };

			triangleMeshTerrain->addTriangle(p1, p2, p3);
		}

		btBvhTriangleMeshShape* cPShape = new btBvhTriangleMeshShape(triangleMeshTerrain, true);
		float ff = cPShape->getMargin();
		cPShape->setMargin(cPShapeConfig->collisionMargin);

		pShape = cPShape;
	} else {
		throw LogicException(LOC);
	}

	return pShape;
}

bool Physics3DUtil::BuildTriangulationDistancesGrid(Array2D<signed char>& tilingGrid, Array2D<signed char>& oDistancesGridX) {
	bool isTriangulationRequired = false;

	Vector2Int gSize{ oDistancesGridX.getLengthA() , oDistancesGridX.getLengthB() };

	for (int gX = gSize.x - 1; gX >= 0; gX--) {
		for (int gY = gSize.y - 1; gY >= 0; gY--) {
			// If on_edge or tileType==-1; Set to 0 distances as needed on each axis separately.
			// Increment the righ and top values if needed.
			signed char tileTypeIndex = tilingGrid.getDirect(gX, gY);

			if (tileTypeIndex == static_cast<signed char>(-1)) {
				isTriangulationRequired = true;
			}

			if (tileTypeIndex == static_cast<signed char>(-1)/*no_tile_physics*/) {
				oDistancesGridX.setDirect(gX, gY, 0);
			} else if (gX == gSize.x - 1/*on_right_edge_and_physics_floor*/) {
				oDistancesGridX.setDirect(gX, gY, 1);
			} else {
				oDistancesGridX.setDirect(gX, gY, oDistancesGridX.getDirect(gX + 1, gY) + 1);
			}
		}
	}

	return isTriangulationRequired;
}

void Physics3DUtil::TriangulateDistancesGrid(
	Array2D<signed char>& distancesGrid, Vector3& tileSize, sp<Vector3> geometryGridOffset, float collisionMargin,
	int enabledSidesTriangulationBitMask,
	btTriangleMesh* oTriangleMeshTerrain)
{
	Vector2Int gSize{ distancesGrid.getLengthA(), distancesGrid.getLengthB() };

	// Geometry offset m;
	Vector3 go{0.0f, 0.0f, 0.0f};
	if (geometryGridOffset != nullptr) {
		// Note. Current tiling logic is for center_of_voxel origin. Physics uses minXYZ for origin and requires a offset.
		/// Note. Also physics doesn't use the offsetZ because ground and walls use the offsetZ differently.
		/// Note. When auto-physics geometry is built, this entire logic will not be needed anymore.
		go.set(
			(geometryGridOffset->x - 0.5f) * tileSize.x,
			(geometryGridOffset->y - 0.5f) * tileSize.y,
			(0.0f) * tileSize.z
		);
	}

	// Enumerate each row from bottom up.
	for (int gX = 0; gX < gSize.x; gX++) {
		int activeQuadStartGPosY = 0;
		// -1; Grab the dist when the gPos is processed.
		int activeQuadGSizeX = -1;

		for (int gY = 0; gY < gSize.y; gY++) {
			char cGDistRight = distancesGrid.getDirect(gX, gY);

			if (cGDistRight == 0) {
				activeQuadStartGPosY = gY + 1;
				activeQuadGSizeX = -1;

				continue;
			}

			if (activeQuadGSizeX == -1) {
				if (gY != activeQuadStartGPosY) {
					throw LogicException(LOC);
				}

				activeQuadGSizeX = cGDistRight;
			}

			if (gY + 1 >= gSize.y/*top_reached*/ || distancesGrid.getDirect(gX, gY + 1) != activeQuadGSizeX/*top_gPos_has_different_width*/) {
				if (cGDistRight > 0) {
					Bounds2DInt quadBounds{
						gX/*xMin*/, gX + activeQuadGSizeX/*xMax*/,
						activeQuadStartGPosY/*yMin*/, gY + 1/*yMax*/
					};

					// Quad bake required.
					btVector3 vertex_xMin_yMin_zMin(quadBounds.min.x * tileSize.x + collisionMargin + go.x, quadBounds.min.y * tileSize.y + collisionMargin + go.y, 0.0f	   + collisionMargin + go.z);
					btVector3 vertex_xMax_yMin_zMin(quadBounds.max.x * tileSize.x - collisionMargin + go.x, quadBounds.min.y * tileSize.y + collisionMargin + go.y, 0.0f	   + collisionMargin + go.z);
					btVector3 vertex_xMax_yMax_zMin(quadBounds.max.x * tileSize.x - collisionMargin + go.x, quadBounds.max.y * tileSize.y - collisionMargin + go.y, 0.0f	   + collisionMargin + go.z);
					btVector3 vertex_xMin_yMax_zMin(quadBounds.min.x * tileSize.x + collisionMargin + go.x, quadBounds.max.y * tileSize.y - collisionMargin + go.y, 0.0f	   + collisionMargin + go.z);
					btVector3 vertex_xMin_yMin_zMax(quadBounds.min.x * tileSize.x + collisionMargin + go.x, quadBounds.min.y * tileSize.y + collisionMargin + go.y, tileSize.z - collisionMargin + go.z);
					btVector3 vertex_xMax_yMin_zMax(quadBounds.max.x * tileSize.x - collisionMargin + go.x, quadBounds.min.y * tileSize.y + collisionMargin + go.y, tileSize.z - collisionMargin + go.z);
					btVector3 vertex_xMax_yMax_zMax(quadBounds.max.x * tileSize.x - collisionMargin + go.x, quadBounds.max.y * tileSize.y - collisionMargin + go.y, tileSize.z - collisionMargin + go.z);
					btVector3 vertex_xMin_yMax_zMax(quadBounds.min.x * tileSize.x + collisionMargin + go.x, quadBounds.max.y * tileSize.y - collisionMargin + go.y, tileSize.z - collisionMargin + go.z);

					// The triangles are drawn CCW while looking at a face and using the face min axis values as the origin;
					if ((enabledSidesTriangulationBitMask & VoxelSides::BitGroup_xMin) == VoxelSides::BitGroup_xMin) {
						oTriangleMeshTerrain->addTriangle(vertex_xMin_yMin_zMin, vertex_xMin_yMin_zMax, vertex_xMin_yMax_zMax);
						oTriangleMeshTerrain->addTriangle(vertex_xMin_yMin_zMin, vertex_xMin_yMax_zMax, vertex_xMin_yMax_zMin);
					}
					if ((enabledSidesTriangulationBitMask & VoxelSides::BitGroup_xMax) == VoxelSides::BitGroup_xMax) {
						oTriangleMeshTerrain->addTriangle(vertex_xMax_yMin_zMin, vertex_xMax_yMax_zMin, vertex_xMax_yMax_zMax);
						oTriangleMeshTerrain->addTriangle(vertex_xMax_yMin_zMin, vertex_xMax_yMax_zMax, vertex_xMax_yMin_zMax);
					}
					if ((enabledSidesTriangulationBitMask & VoxelSides::BitGroup_yMin) == VoxelSides::BitGroup_yMin) {
						oTriangleMeshTerrain->addTriangle(vertex_xMin_yMin_zMin, vertex_xMax_yMin_zMin, vertex_xMax_yMin_zMax);
						oTriangleMeshTerrain->addTriangle(vertex_xMin_yMin_zMin, vertex_xMax_yMin_zMax, vertex_xMin_yMin_zMax);
					}
					if ((enabledSidesTriangulationBitMask & VoxelSides::BitGroup_yMax) == VoxelSides::BitGroup_yMax) {
						oTriangleMeshTerrain->addTriangle(vertex_xMin_yMax_zMin, vertex_xMin_yMax_zMax, vertex_xMax_yMax_zMax);
						oTriangleMeshTerrain->addTriangle(vertex_xMin_yMax_zMin, vertex_xMax_yMax_zMax, vertex_xMax_yMax_zMin);
					}
					if ((enabledSidesTriangulationBitMask & VoxelSides::BitGroup_zMin) == VoxelSides::BitGroup_zMin) {
						oTriangleMeshTerrain->addTriangle(vertex_xMin_yMin_zMin, vertex_xMin_yMax_zMin, vertex_xMax_yMax_zMin);
						oTriangleMeshTerrain->addTriangle(vertex_xMin_yMin_zMin, vertex_xMax_yMax_zMin, vertex_xMax_yMin_zMin);
					}
					if ((enabledSidesTriangulationBitMask & VoxelSides::BitGroup_zMax) == VoxelSides::BitGroup_zMax) {
						oTriangleMeshTerrain->addTriangle(vertex_xMin_yMin_zMax, vertex_xMax_yMin_zMax, vertex_xMax_yMax_zMax);
						oTriangleMeshTerrain->addTriangle(vertex_xMin_yMin_zMax, vertex_xMax_yMax_zMax, vertex_xMin_yMax_zMax);
					}

					//GlobalAccessLogHandler::devLogHandler->post_debug("Physics grid shape quad bake(xMin, yMin, lenX, lenY) %i %i %i %i", quadBounds.min.x, quadBounds.min.y, quadBounds.getLenX(), quadBounds.getLenY());

					// Must set the distancesGrid to 0 for the quad gCells;
					distancesGrid.setRegionDirect(&quadBounds, 0);
				}

				activeQuadStartGPosY = gY + 1;
				activeQuadGSizeX = -1;
			}
		}
	}
}
