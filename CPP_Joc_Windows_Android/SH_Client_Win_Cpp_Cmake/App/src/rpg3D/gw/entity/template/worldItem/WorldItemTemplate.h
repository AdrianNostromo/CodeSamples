#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/template/BasicModuleTemplate.h>

namespace rpg3D {
class WorldItemTemplate : public BasicModuleTemplate {priv typedef BasicModuleTemplate super;pub dCtor(WorldItemTemplate);
	pub std::string* impactSoundConfig_id;
	pub float impactSoundConfig_volume;
	
	pub WorldItemTemplate(
		std::string* impactSoundConfig_id, float impactSoundConfig_volume);
    
	pub IEntityModule* createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) override;
	
	pub ~WorldItemTemplate() override;
};
};
