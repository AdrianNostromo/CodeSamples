#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/template/BasicModuleTemplate.h>

class FrustumCullable3DTemplate : public BasicModuleTemplate {priv typedef BasicModuleTemplate super;
    pub static const int SHAPE_SPHERE;

    pub const int shape;

    pub float sphere_offsetZ;
    pub float sphere_radiusM;

    pub FrustumCullable3DTemplate(
        const int shape
    );
    pub IEntityModule* createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) override;
    pub static FrustumCullable3DTemplate* newOfsetedZSphere(float offsetZ, float radiusM);
    
    pub ~FrustumCullable3DTemplate() override;
};
