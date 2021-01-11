#include <base/math/Math.h>
#include <worldGame3D/gw/entity/template/visual3D/Visual3DTemplate.h>
#include "Touchable3DModule_AABBBounds.h"
#include <worldGame3D/gw/entity/module/visual3D/Visual3DModule.h>
#include <base/app/config/AppConfig.h>
#include <base/visual3d/Visual3DUtil.h>
#include <base/visual3d/material/Visual3DModelMaterial_Color.h>
#include <base/exceptions/LogicException.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <graphics/visual3d/drawable/VoxelGridDrawable3D.h>

const bool Touchable3DModule_AABBBounds::IS_DEBUG_TOUCHABLE_ENTITY_BoundsAABB = false;

Touchable3DModule_AABBBounds::Touchable3DModule_AABBBounds(
    IWorldEntity* entity,
    Touchable3DTemplate* t
)
    : super(entity, t)
{
    for(int i=0;i<8;i++) {
        localCornerVerts[i] = Vector3(0.0f, 0.0f, 0.0f);
        worldCornerVerts[i] = Vector3(0.0f, 0.0f, 0.0f);
    }
	setLocalCornerVerts(t->mouseboxMin, t->mouseboxMax);

    e->addEventListener(
        IWorldEntity::EVENT_syncPos->type,
        std::bind(&Touchable3DModule_AABBBounds::onEvent, this, std::placeholders::_1, std::placeholders::_2),
        autoListenersList
    );
}

void Touchable3DModule_AABBBounds::createMain() {
    super::createMain();

    if(AppConfig::IS_DEBUG && IS_DEBUG_TOUCHABLE_ENTITY_BoundsAABB) {
        debugVoxelGrid = new Array3D<signed char>(1, 1, 1);
        debugVoxelGrid->setDirect(0, 0, 0, 0);

        debugMaterialsList = new ArrayList<Visual3DModelMaterial*>();
        debugMaterialsList->appendDirect(new Visual3DModelMaterial_Color(
            new Color("#00FF00FF")
        ));
    }
}

void Touchable3DModule_AABBBounds::createBMain() {
    super::createBMain();

    visual3D = entity->getComponentAs<worldGame3D::IVisual3DModule*>(false/*mustExist*/);
}

void Touchable3DModule_AABBBounds::setLocalCornerVerts(Vector3& boundsMin, Vector3& boundsMax) {
	isDirty_localCornerVerts = true;
	cache_localCornerVerts_min.set(boundsMin);
	cache_localCornerVerts_max.set(boundsMax);
}

void Touchable3DModule_AABBBounds::prepareLocalCornerVerts() {
	if (!isDirty_localCornerVerts) {
		return;
	}
	isDirty_localCornerVerts = false;

    localCornerVerts[VERT_INDEX::xyz].set(
        Math::min(cache_localCornerVerts_min.x, cache_localCornerVerts_max.x),
        Math::min(cache_localCornerVerts_min.y, cache_localCornerVerts_max.y),
        Math::min(cache_localCornerVerts_min.z, cache_localCornerVerts_max.z)
    );

    localCornerVerts[VERT_INDEX::xYz].set(
        Math::min(cache_localCornerVerts_min.x, cache_localCornerVerts_max.x),
        Math::max(cache_localCornerVerts_min.y, cache_localCornerVerts_max.y),
        Math::min(cache_localCornerVerts_min.z, cache_localCornerVerts_max.z)
    );

    localCornerVerts[VERT_INDEX::Xyz].set(
        Math::max(cache_localCornerVerts_min.x, cache_localCornerVerts_max.x),
        Math::min(cache_localCornerVerts_min.y, cache_localCornerVerts_max.y),
        Math::min(cache_localCornerVerts_min.z, cache_localCornerVerts_max.z)
    );

    localCornerVerts[VERT_INDEX::XYz].set(
        Math::max(cache_localCornerVerts_min.x, cache_localCornerVerts_max.x),
        Math::max(cache_localCornerVerts_min.y, cache_localCornerVerts_max.y),
        Math::min(cache_localCornerVerts_min.z, cache_localCornerVerts_max.z)
    );

    localCornerVerts[VERT_INDEX::xyZ].set(
        Math::min(cache_localCornerVerts_min.x, cache_localCornerVerts_max.x),
        Math::min(cache_localCornerVerts_min.y, cache_localCornerVerts_max.y),
        Math::max(cache_localCornerVerts_min.z, cache_localCornerVerts_max.z)
    );

    localCornerVerts[VERT_INDEX::xYZ].set(
        Math::min(cache_localCornerVerts_min.x, cache_localCornerVerts_max.x),
        Math::max(cache_localCornerVerts_min.y, cache_localCornerVerts_max.y),
        Math::max(cache_localCornerVerts_min.z, cache_localCornerVerts_max.z)
    );

    localCornerVerts[VERT_INDEX::XyZ].set(
        Math::max(cache_localCornerVerts_min.x, cache_localCornerVerts_max.x),
        Math::min(cache_localCornerVerts_min.y, cache_localCornerVerts_max.y),
        Math::max(cache_localCornerVerts_min.z, cache_localCornerVerts_max.z)
    );

    localCornerVerts[VERT_INDEX::XYZ].set(
        Math::max(cache_localCornerVerts_min.x, cache_localCornerVerts_max.x),
        Math::max(cache_localCornerVerts_min.y, cache_localCornerVerts_max.y),
        Math::max(cache_localCornerVerts_min.z, cache_localCornerVerts_max.z)
    );

    // The entity scale is applied to the world_verts.
}

Vector3* Touchable3DModule_AABBBounds::getWorldCornerVerts() {
    if(isDirty_worldVerts) {
        bakeWorldCornerVerts();
    }

    return worldCornerVerts;
}

void Touchable3DModule_AABBBounds::bakeWorldCornerVerts() {
    if(!isDirty_worldVerts) {
        return ;
    }

    isDirty_worldVerts = false;

	Matrix4 entityTransform{};
	if (visual3D != nullptr) {
		entityTransform.set(visual3D->peekRootDrawable_globalTransform(false));
	} else {
		entityTransform.translate(*e->getPos());
	}

	prepareLocalCornerVerts();

    if(t->shapeType == Touchable3DTemplate::ShapeType::AABB) {
        for(int i=0;i<8;i++) {
            Vector3* lVert = &localCornerVerts[i];

            tMatB.idt();
            tMatB.translate(*lVert);

            tMat.set(entityTransform);
            tMat.mul(tMatB);

            tMat.getTranslation(worldCornerVerts[i]);
        }
    }else if(t->shapeType == Touchable3DTemplate::ShapeType::FixedSizeAABB) {
        entityTransform.getTranslation(tVect);

        for(int i=0;i<8;i++) {
            Vector3* lVert = &localCornerVerts[i];

            worldCornerVerts[i].set(tVect);
            worldCornerVerts[i].add(*lVert);
        }
    }else {
        throw LogicException(LOC);
    }

}

BoundingBox* Touchable3DModule_AABBBounds::getTouchBoundsAABB() {
    if(visual3D != nullptr && !visual3D->peekRootDrawable_isGlobalTransformInit()) {
        return nullptr;
    }

    if(isDirty_boundsAABB) {
        bakeTouchBoundsAABB();

        if(AppConfig::IS_DEBUG && IS_DEBUG_TOUCHABLE_ENTITY_BoundsAABB) {
            disposeDebugModelBoundsAABB();

            Visual3DModelMaterial_Color* mat = dynamic_cast<Visual3DModelMaterial_Color*>(debugMaterialsList->getDirect(0));
            mat->diffuseColor->set(
                Math::Rand::nextFloat(),
                Math::Rand::nextFloat(),
                Math::Rand::nextFloat(),
                1.0f
            );

            debugInstanceAABB = Visual3DUtil::newVoxelGrid(
                1.0f,
                &debugVoxelGridOffset,
                debugVoxelGrid,
                debugMaterialsList,
                nullptr/*tilingInformation*/
            );
            debugInstanceAABB->setScale(
                touchBoundsAABB.getSizeX(),
                touchBoundsAABB.getSizeY(),
                touchBoundsAABB.getSizeZ()
            );
            debugInstanceAABB->setPosition(
                touchBoundsAABB.getCenterX(),
                touchBoundsAABB.getCenterY(),
                touchBoundsAABB.getCenterZ()
            );

            if(!visual3D->getVisualsParent()) {
                throw LogicException(LOC);
            }
            visual3D->getVisualsParent()->addChild(debugInstanceAABB);
        }
    }

    return &touchBoundsAABB;
}

Vector3* Touchable3DModule_AABBBounds::peekLocalBounds_min() {
	prepareLocalCornerVerts();

	return &localCornerVerts[VERT_INDEX::xyz];
}

Vector3* Touchable3DModule_AABBBounds::peekLocalBounds_max() {
	prepareLocalCornerVerts();

	return &localCornerVerts[VERT_INDEX::XYZ];
}

void Touchable3DModule_AABBBounds::bakeTouchBoundsAABB() {
    if(!isDirty_boundsAABB) {
        return ;
    }

    getWorldCornerVerts();

    isDirty_boundsAABB = false;

    touchBoundsAABB.set(worldCornerVerts);
}

void Touchable3DModule_AABBBounds::onEvent(IEventListener& eventListener, base::EntityEvent& event) {
    if (event.type == IWorldEntity::EVENT_syncPos->type || event.type == IWorldEntity::EVENT_syncRot->type || event.type == IWorldEntity::EVENT_syncScale->type) {
        isDirty_worldVerts = true;
        isDirty_boundsAABB = true;
    }
	
	else {
        throw LogicException(LOC);
    }
}

void Touchable3DModule_AABBBounds::disposeDebugModelBoundsAABB() {
    if(debugInstanceAABB) {
        if(debugInstanceAABB->getParent()) {
            debugInstanceAABB->getParent()->removeChild(debugInstanceAABB);
        }

        delete debugInstanceAABB;

        debugInstanceAABB = nullptr;
    }
}

Touchable3DModule_AABBBounds::~Touchable3DModule_AABBBounds() {
    if(AppConfig::IS_DEBUG && IS_DEBUG_TOUCHABLE_ENTITY_BoundsAABB) {
        disposeDebugModelBoundsAABB();
    }
}
