#include "WorldEntity.h"

WorldEntity::WorldEntity(EntityTemplate* t, int localEntityId, std::shared_ptr<ExtraData> extraData)
    : super(t, localEntityId, extraData)
{
    //void
}

WorldEntity::~WorldEntity() {
    //void
}
