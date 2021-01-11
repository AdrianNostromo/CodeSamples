#include <worldGame3D/gw/entity/template/BasicModuleTemplate.h>

#include <worldGame3D/gw/entity/module/EntityModule.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/IEntityModuleCreator.h>

BasicModuleTemplate::BasicModuleTemplate(std::string moduleName, int moduleId)
    : moduleName(std::move(moduleName)), moduleId(moduleId)
{
    if(this->moduleName.size() <= 0 || this->moduleId < 0) {
        assert(false);
    }
}

IEntityModule* BasicModuleTemplate::createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) {
    return nullptr;
}

BasicModuleTemplate::~BasicModuleTemplate() {
    //void
}
