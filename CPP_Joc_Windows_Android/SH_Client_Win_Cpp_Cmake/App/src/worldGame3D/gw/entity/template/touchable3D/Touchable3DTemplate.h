#pragma once

#include <base/gh.h>
#include <base/math/Vector3.h>
#include <worldGame3D/gw/entity/template/BasicModuleTemplate.h>

class Touchable3DTemplate : public BasicModuleTemplate {priv typedef BasicModuleTemplate super;pub dCtor(Touchable3DTemplate);
    pub class ShapeType {
        pub static const int AABB = 1;
        pub static const int FixedSizeAABB = 2;
        //asdB;// implement cuboid shape that can be rotated with the player.
    };

    pub const int shapeType;

    pub Vector3 mouseboxMin;
    pub Vector3 mouseboxMax;

    pub explicit Touchable3DTemplate(
        const int shapeType,
        Vector3 mouseboxMin,
        Vector3 mouseboxMax
    );
    
    pub IEntityModule* createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) override;
    
    pub ~Touchable3DTemplate() override;
};
