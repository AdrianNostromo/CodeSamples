#pragma once

#include <base/gh.h>
#include <string>
#include <base/map/Map1D.h>
#include <base/map/ExtraData.h>

class IEntityModule;
class IWorldEntity;
class IEntityModuleCreator;

class BasicModuleTemplate {pub dCtor(BasicModuleTemplate);
    // This is used for serialising and parsing of tempaltes (when it will be implemented).
    pub std::string moduleName;
    pub int moduleId;

    pub explicit BasicModuleTemplate(std::string moduleName, int moduleId);

    // Extendable;
    pub virtual IEntityModule* createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator);

    pub virtual ~BasicModuleTemplate();

};
