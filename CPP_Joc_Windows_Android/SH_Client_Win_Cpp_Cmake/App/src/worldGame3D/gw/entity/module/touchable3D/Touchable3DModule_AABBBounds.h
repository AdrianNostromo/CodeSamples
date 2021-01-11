#pragma once

#include <base/gh.h>
#include "Touchable3DModuleBase.h"
#include <graphics/visual3d/drawable/IDrawable3D.h>
#include <base/math/collision/BoundingBox.h>
#include <base/list/Array3D.h>
#include <base/visual3d/material/Visual3DModelMaterial.h>

namespace worldGame3D { class IVisual3DModule; }

class Touchable3DModule_AABBBounds : public Touchable3DModuleBase {priv typedef Touchable3DModuleBase super;pub dCtor(Touchable3DModule_AABBBounds);
    prot class VERT_INDEX {
        pub static const int xyz = 0;
        pub static const int xYz = 1;
        pub static const int Xyz = 2;
        pub static const int XYz = 3;

        pub static const int xyZ = 4;
        pub static const int xYZ = 5;
        pub static const int XyZ = 6;
        pub static const int XYZ = 7;
    };

    priv static const bool IS_DEBUG_TOUCHABLE_ENTITY_BoundsAABB;

    priv bool isDirty_localCornerVerts = true;
    priv Vector3 cache_localCornerVerts_min{};
    priv Vector3 cache_localCornerVerts_max{};
    priv Vector3 localCornerVerts[8];

    priv Vector3 worldCornerVerts[8];
    priv bool isDirty_worldVerts = true;

    priv BoundingBox touchBoundsAABB{};
    priv bool isDirty_boundsAABB = true;

    priv worldGame3D::IVisual3DModule* visual3D = nullptr;

    priv Vector3 tVect{};
    priv Matrix4 tMat{};
    priv Matrix4 tMatB{};

    priv IDrawable3D* debugInstanceAABB = nullptr;
    priv Vector3 debugVoxelGridOffset{0.0f, 0.0f, 0.0f};
    priv Array3D<signed char>* debugVoxelGrid = nullptr;
    priv ArrayList<Visual3DModelMaterial*>* debugMaterialsList = nullptr;

    pub explicit Touchable3DModule_AABBBounds(
        IWorldEntity* entity,
        Touchable3DTemplate* t
    );
    prot void createMain() override;
    prot void createBMain() override;

    pub BoundingBox* getTouchBoundsAABB() final;
    
    pub Vector3* peekLocalBounds_min();
    pub Vector3* peekLocalBounds_max();

    pub void setLocalCornerVerts(Vector3& boundsMin, Vector3& boundsMax);

    prot Vector3* getWorldCornerVerts();
    prot void bakeWorldCornerVerts();

    priv void prepareLocalCornerVerts();
    priv void bakeTouchBoundsAABB();

    priv void onEvent(IEventListener& eventListener, base::EntityEvent& event);

    priv void disposeDebugModelBoundsAABB();

    pub ~Touchable3DModule_AABBBounds() override;
};
