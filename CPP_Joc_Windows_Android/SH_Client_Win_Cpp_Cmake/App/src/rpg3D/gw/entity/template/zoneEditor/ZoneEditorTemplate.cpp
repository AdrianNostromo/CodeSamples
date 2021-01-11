#include "ZoneEditorTemplate.h"
#include <rpg3D/gw/entity/module/zoneEditor/ZoneEditorModule.h>

using namespace rpg3D;

ZoneEditorTemplate::ZoneEditorTemplate()
	: super("zoneEditor", rpg3D::zoneEditorModule::IZoneEditorModule::TYPE)
{
	//void
}

IEntityModule* ZoneEditorTemplate::createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) {
	ZoneEditorTemplate* t = dynamic_cast<ZoneEditorTemplate*>(_t);
	if (!t) {
		throw LogicException(LOC);
	}

	ZoneEditorModule* mod = new ZoneEditorModule(entity, t);

	return mod;
}

ZoneEditorTemplate::~ZoneEditorTemplate() {
	//void
}
