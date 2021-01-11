#pragma once

#include <base/gh.h>
#include <memory>
#include <base/list/Array2D.h>
#include <base/math/Vector2.h>
#include <base/sp.h>

class btRigidBody;
class btCollisionShape;
class btTriangleMesh;
class btMotionState;
class btVector3;
namespace graphics {
    class TilingInformation;
};
namespace physics3D {
    class PhysicsShapeConfig;
};
namespace rpg3D {
    class PBodyConfig;
};

namespace physics3D {
class Physics3DUtil {
    pub static btRigidBody* BuildPBody(
        rpg3D::PBodyConfig* pBodyConfig, float scaledMass, btMotionState* pMotionState, btCollisionShape* pShape, const btVector3& localInertia);
    
    pub static btCollisionShape* BuildShape(
        physics3D::PhysicsShapeConfig* pShapeConfig, std::shared_ptr<graphics::TilingInformation> tilingInformation=nullptr);

    // Returns isTriangulationRequired;
    priv static bool BuildTriangulationDistancesGrid(Array2D<signed char>& tilingGrid, Array2D<signed char>& oDistancesGridX);
    priv static void TriangulateDistancesGrid(
        Array2D<signed char>& distancesGridX, Vector3& tileSize, sp<Vector3> geometryGridOffset, float collisionMargin,
        int enabledSidesTriangulationBitMask, 
        btTriangleMesh* oTriangleMeshTerrain);

};
};
