#include <rpg3D/gw/entity/module/userControlled/UserControlledModule.h>
#include "UserControlledTemplate.h"

using namespace rpg3D;

UserControlledTemplate::UserControlledTemplate()
	: super("userControlled", IUserControlledModule::TYPE)
{
	//void
}

IEntityModule* UserControlledTemplate::createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) {
	UserControlledTemplate* t = dynamic_cast<UserControlledTemplate*>(_t);
	if(!t) {
		throw LogicException(LOC);
	}

	UserControlledModule* mod = new UserControlledModule(entity, t);

	return mod;
}

UserControlledTemplate::~UserControlledTemplate() {
	//void
}
