#include "VoxelizedEditorTemplate.h"
#include <rpg3D/gw/entity/module/voxelizedEditor/VoxelizedEditorModule.h>

using namespace rpg3D;

VoxelizedEditorTemplate::VoxelizedEditorTemplate()
	: super("voxelizedEditor", rpg3D::entityEditorModule::IVoxelizedEditorModule::TYPE)
{
	//void
}

IEntityModule* VoxelizedEditorTemplate::createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) {
	VoxelizedEditorTemplate* t = dynamic_cast<VoxelizedEditorTemplate*>(_t);
	if (!t) {
		throw LogicException(LOC);
	}

	VoxelizedEditorModule* mod = new VoxelizedEditorModule(entity, t);

	return mod;
}

VoxelizedEditorTemplate::~VoxelizedEditorTemplate() {
	//void
}
