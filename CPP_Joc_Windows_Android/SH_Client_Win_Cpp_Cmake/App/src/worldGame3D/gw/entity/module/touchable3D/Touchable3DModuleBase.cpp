#include "Touchable3DModuleBase.h"

Touchable3DModuleBase::Touchable3DModuleBase(
    IWorldEntity* entity,
    Touchable3DTemplate* t
)
    : super(t->moduleName, entity),
    t(t)
{
    //void
}

Touchable3DModuleBase::~Touchable3DModuleBase() {
    //void
}
