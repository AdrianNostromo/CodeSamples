#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/template/BasicModuleTemplate.h>
#include <rpg3D/gw/entity/template/toolshandler/util/ToolSlotConfig.h>
#include <base/list/Array1D.h>

namespace rpg3D {
class ToolsHandlerTemplate : public BasicModuleTemplate {priv typedef BasicModuleTemplate super;pub dCtor(ToolsHandlerTemplate);
	pub Array1D<ToolSlotConfig*>* toolSlotsList;

    pub explicit ToolsHandlerTemplate(Array1D<ToolSlotConfig*>* toolSlotsList);

	pub IEntityModule* createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) override;

	pub ~ToolsHandlerTemplate() override;
};
};
