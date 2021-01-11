#pragma once

#include <base/gh.h>
#include "V3DM_Positional.h"

class IWorldEntity;

namespace worldGame3D {
class Visual3DModule : public V3DM_Positional {priv typedef V3DM_Positional super;pub dCtor(Visual3DModule);
    pub explicit Visual3DModule(
        IWorldEntity* entity,
        Visual3DTemplate* t,
        ArrayList<IDrawableNode3D*>* drawablesList, Map1D<std::string, IDrawableNode3D*>* drawablesMap, IDrawableNode3D* rootDrawable);

	pub ~Visual3DModule() override;
};
};
