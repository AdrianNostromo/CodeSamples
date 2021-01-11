#include <worldGame3D/gw/entity/module/voxelisedStructure/VoxelisedStructureModule.h>
#include "VoxelisedStructureTemplate.h"

VoxelisedStructureTemplate::VoxelisedStructureTemplate(
    std::string voxelisedConstructLayerId)
    : super("voxelisedStructure", IVoxelisedStructureModule::TYPE),
      voxelisedConstructLayerId(voxelisedConstructLayerId)
{
    //void
}

IEntityModule* VoxelisedStructureTemplate::createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) {
    VoxelisedStructureTemplate* t = dynamic_cast<VoxelisedStructureTemplate*>(_t);
    if(!t) {
        throw LogicException(LOC);
    }

    VoxelisedStructureModule* mod = new VoxelisedStructureModule(entity, t);

    return mod;
}

VoxelisedStructureTemplate::~VoxelisedStructureTemplate() {
    //void
}
