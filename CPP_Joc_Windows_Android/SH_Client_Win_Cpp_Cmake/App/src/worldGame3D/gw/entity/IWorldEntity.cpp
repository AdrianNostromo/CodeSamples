#include "IWorldEntity.h"
#include <worldGame3D/gw/entity/util/EntityRemoveReason.h>

EntityRemoveReason* const IWorldEntity::ENT_REM_REASON_PURPOSE_FINISHED = new EntityRemoveReason("ENT_REM_REASON_PURPOSE_FINISHED", true, true);

base::EntityEvent* IWorldEntity::EVENT_syncPos = new base::EntityEvent(false, "EVENT_syncPos");
base::EntityEvent* IWorldEntity::EVENT_posManuallyChanged = new base::EntityEvent(false, "EVENT_posManuallyChanged");
base::EntityEvent* IWorldEntity::EVENT_syncRot = new base::EntityEvent(false, "EVENT_syncRot");
base::EntityEvent* IWorldEntity::EVENT_rotManuallyChanged = new base::EntityEvent(false, "EVENT_rotManuallyChanged");
base::EntityEvent* IWorldEntity::EVENT_syncCardinalRotation = new base::EntityEvent(false, "EVENT_syncCardinalRotation");
base::EntityEvent* IWorldEntity::EVENT_syncScale = new base::EntityEvent(false, "EVENT_syncScale");
base::EntityEvent* IWorldEntity::EVENT_onRemoved_pre = new base::EntityEvent(false, "EVENT_onRemoved_pre");
