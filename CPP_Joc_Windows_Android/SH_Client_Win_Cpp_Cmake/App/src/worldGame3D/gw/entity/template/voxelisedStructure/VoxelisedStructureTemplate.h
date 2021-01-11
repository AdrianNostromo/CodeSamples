#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/template/BasicModuleTemplate.h>
#include <string>

class VoxelisedStructureTemplate : public BasicModuleTemplate {priv typedef BasicModuleTemplate super;pub dCtor(VoxelisedStructureTemplate);
    pub std::string voxelisedConstructLayerId;

    pub explicit VoxelisedStructureTemplate(
        std::string voxelisedConstructLayerId
    );
    
    pub IEntityModule* createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) override;
    
    pub ~VoxelisedStructureTemplate() override;
};
