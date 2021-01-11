#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/template/BasicModuleTemplate.h>

namespace rpg3D {
class Grid2DContentTemplate : public BasicModuleTemplate {priv typedef BasicModuleTemplate super;pub dCtor(Grid2DContentTemplate);
	pub bool isGridOrganisingGPMandatory;

	pub explicit Grid2DContentTemplate(
		bool isGridOrganisingGPMandatory);

	pub IEntityModule* createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) override;

	pub ~Grid2DContentTemplate() override;
};
};
