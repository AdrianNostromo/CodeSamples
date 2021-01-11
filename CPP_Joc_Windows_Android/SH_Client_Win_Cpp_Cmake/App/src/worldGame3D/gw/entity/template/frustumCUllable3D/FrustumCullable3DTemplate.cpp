#include "FrustumCullable3DTemplate.h"
#include <worldGame3D/gw/entity/module/frustumcullable3d/FrustumCullable3DModule.h>

const int FrustumCullable3DTemplate::SHAPE_SPHERE = 1;

FrustumCullable3DTemplate::FrustumCullable3DTemplate(
    const int shape)
    : super("frustumCullable3D", IFrustumCullable3DModule::TYPE),
      shape(shape)
{
    //void
}

IEntityModule* FrustumCullable3DTemplate::createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) {
    FrustumCullable3DTemplate* t = dynamic_cast<FrustumCullable3DTemplate*>(_t);
    if(!t) {
        throw LogicException(LOC);
    }

    FrustumCullable3DModule* mod = new FrustumCullable3DModule(entity, t);

    return mod;
}

FrustumCullable3DTemplate* FrustumCullable3DTemplate::newOfsetedZSphere(float offsetZ, float radiusM) {
    FrustumCullable3DTemplate* t = new FrustumCullable3DTemplate(SHAPE_SPHERE);

    t->sphere_offsetZ = offsetZ;
    t->sphere_radiusM = radiusM;

    return t;
}

FrustumCullable3DTemplate::~FrustumCullable3DTemplate() {
    //void
}
