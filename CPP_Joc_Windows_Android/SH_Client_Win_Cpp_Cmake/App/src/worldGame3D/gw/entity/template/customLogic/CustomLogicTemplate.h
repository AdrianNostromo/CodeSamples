#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/template/BasicModuleTemplate.h>
#include <functional>

namespace base {
	class CustomLogicModule;
}

namespace base {
class CustomLogicTemplate : public BasicModuleTemplate {priv typedef BasicModuleTemplate super;pub dCtor(CustomLogicTemplate);
	pub std::function<CustomLogicModule* (IWorldEntity* entity, CustomLogicTemplate* t)> moduleFactory;

	pub explicit CustomLogicTemplate(
		std::string moduleName, int moduleId,
		std::function<CustomLogicModule* (IWorldEntity* entity, CustomLogicTemplate* t)> moduleFactory);
    
	pub IEntityModule* createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) override;
    
	pub ~CustomLogicTemplate() override;
};
};
