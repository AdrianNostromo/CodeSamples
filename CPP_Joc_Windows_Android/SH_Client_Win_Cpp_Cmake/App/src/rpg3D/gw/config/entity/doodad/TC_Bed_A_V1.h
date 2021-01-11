#pragma once

#include <base/gh.h>
#include <string>
#include <memory>
#include <base/sp.h>
#include <base/list/Array1D.h>

class EntityTemplate;
class Drawable3DCreateConfig_VoxelGrid;
namespace base {
	class IWrappedValue;
};

namespace rpg3D {
class TC_Bed_A_V1 {
	priv static bool IsStaticsInit;

	pub static std::string BaseId;

	pub static EntityTemplate* GetOrCreateTemplate(unsigned int voxelSizeCM);
	pub static EntityTemplate* GetOrCreateTemplateFromConfigParams(sp<Array1D<std::shared_ptr<base::IWrappedValue>>>& configValues);

	priv static Drawable3DCreateConfig_VoxelGrid* BuildBase(unsigned int voxelSizeCM);

};
};
