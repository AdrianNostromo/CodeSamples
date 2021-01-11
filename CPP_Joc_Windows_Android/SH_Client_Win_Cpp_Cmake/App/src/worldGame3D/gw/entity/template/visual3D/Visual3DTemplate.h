#pragma once

#include <base/gh.h>
#include <string>
#include <worldGame3D/gw/entity/template/BasicModuleTemplate.h>
#include <worldGame3D/gw/entity/module/EntityModule.h>
#include <base/visual3d/util/Visual3DCreateConfig.h>

class Visual3DTemplate : public BasicModuleTemplate {priv typedef BasicModuleTemplate super;pub dCtor(Visual3DTemplate);
    pub Visual3DCreateConfig* createConfig;

    //asdz;// These values will need to the correctly set on the entity templates. Do this when the ground will have blocks with different shades of green and colors because it will be easier.
    // These values are used ot make sure the model is positioned on on exact world voxel position.
    //asdz;// error occurs with this logic if the used entity voxel size is not 0.1f (eg: The player has a smaller voxel size);
    pub bool requiresHalfGridOffsetX;
    pub bool requiresHalfGridOffsetY;

    pub explicit Visual3DTemplate(
        Visual3DCreateConfig* createConfig,

        bool requiresHalfGridOffsetX,
        bool requiresHalfGridOffsetY
    );
    
    pub IEntityModule* createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) override;
    
    pub ~Visual3DTemplate() override;
};
