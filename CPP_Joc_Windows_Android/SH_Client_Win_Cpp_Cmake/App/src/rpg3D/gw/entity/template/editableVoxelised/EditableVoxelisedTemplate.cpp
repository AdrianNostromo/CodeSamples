#include "EditableVoxelisedTemplate.h"
#include <rpg3D/gw/entity/module/editableVoxelised/EditableVoxelisedModule.h>

using namespace rpg3D;

EditableVoxelisedTemplate::EditableVoxelisedTemplate(bool isEditorStructureEnabled)
	: super("editableVoxelised", IEditableVoxelisedModule::TYPE),
	isEditorStructureEnabled(isEditorStructureEnabled)
{
	//void
}

IEntityModule* EditableVoxelisedTemplate::createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) {
	EditableVoxelisedTemplate* t = dynamic_cast<EditableVoxelisedTemplate*>(_t);
	if (!t) {
		throw LogicException(LOC);
	}

	EditableVoxelisedModule* mod = new EditableVoxelisedModule(entity, t);

	return mod;
}

EditableVoxelisedTemplate::~EditableVoxelisedTemplate() {
	//void
}
