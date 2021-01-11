#include <worldGame3D/gw/entity/template/visual3D/Visual3DTemplate.h>
#include "MEMBase.h"
#include <worldGame3D/gw/entity/module/visual3D/Visual3DModule.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>

MEMBase::MEMBase(
    IWorldEntity* entity,
    rpg3D::MovingEntityTemplate* t
)
    : super(t->moduleName, entity),
    t(t)
{
    //void
}

void MEMBase::createBMain() {
    super::createBMain();

    //void
}

void MEMBase::disposePre() {
    //void

    super::disposePre();
}

MEMBase::~MEMBase() {
    //void
}
