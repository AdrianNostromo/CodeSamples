#include <sh/scenarios/battlezones_a_v1/gw/config/entity/platform/PlatformTemplate_ZoneTeleporter_Wood.h>
#include "ZoneTemplate.h"

using namespace battlezones_a_v1;

Map1D<std::string, ZoneTemplate*> ZoneTemplate::entriesMap{};

ZoneTemplate::GroundPatchSpawnInfo::GroundPatchSpawnInfo(std::string& scenarioNamespace, std::string& id, Vector3& pos, Quaternion& rot, Array3D<signed char>* voxelsGrid, ArrayList<Visual3DModelMaterial*>* materialsList)
	: scenarioNamespace(scenarioNamespace), id(id), pos(pos), rot(rot), voxelsGrid(voxelsGrid), materialsList(materialsList)
{
	//void
}

ZoneTemplate::GroundPatchSpawnInfo::~GroundPatchSpawnInfo() {
	//void
}

ZoneTemplate::EntitySpawnInfo::EntitySpawnInfo(std::string& scenarioNamespace, std::string& id, Vector3& pos, Quaternion& rot, std::shared_ptr<std::string> instanceId)
	: scenarioNamespace(scenarioNamespace), id(id), pos(pos), rot(rot), instanceId(instanceId)
{
	//void
}

ZoneTemplate::EntitySpawnInfo::~EntitySpawnInfo() {
	//void
}

ZoneTemplate::ZoneTemplate(std::string& id)
	: id(id)
{
	entriesMap.putDirect(id, this);
}

ZoneTemplate::EntitySpawnInfo* ZoneTemplate::getEnvironmentEntityWithInstanceId(std::shared_ptr<std::string>& instanceId) {
	for(int i=0;i<environmentEntitiesList.count();i++) {
		EntitySpawnInfo* entitySpawnInfo = environmentEntitiesList.getPointer(i);

		if(entitySpawnInfo->instanceId == instanceId) {
			return entitySpawnInfo;
		}
	}

	return nullptr;
}

ZoneTemplate::~ZoneTemplate() {
	//void
}
