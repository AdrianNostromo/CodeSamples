#include "IEntsGroup.h"
#include <worldGame3D/gw/entityGroups/event/EntGroupsEvent.h>

using namespace worldGame3D;

EntGroupsEvent* IEntsGroup::EGV_EntityAdded = new EntGroupsEvent("EGV_EntityAdded");
EntGroupsEvent* IEntsGroup::EGV_EntityRemovePre = new EntGroupsEvent("EGV_EntityRemovePre");
EntGroupsEvent* IEntsGroup::EGV_EntityRemoved = new EntGroupsEvent("EGV_EntityRemoved");
