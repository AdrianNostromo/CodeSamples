#pragma once

#include <base/gh.h>
#include "GPLCC_PersistentEntities.h"
#include <base/fileSystem/fileHandler/async/util/AsyncFHEvent.h>
#include <memory>
#include <string>
#include <rpg3D/gw/util/ParseData_VoxlesGrid3DGeometry.h>

namespace startarea_a_v1 {
class GPLCC_PersistentEntitiesLoader : public GPLCC_PersistentEntities {priv typedef GPLCC_PersistentEntities super;pub dCtor(GPLCC_PersistentEntitiesLoader);
	priv class LoadPhase {
		pub static const int Idle;
		pub static const int StartRequested;
		pub static const int LoadEntitiesList;
		pub static const int LoadEntitiesTemplates;
		pub static const int CreateEntities;
		pub static const int Finished;
	};

	priv class ParseData_EntitiesList {
		pub class CfgEntity {
			pub std::string instanceId;
			pub float x;
			pub float y;
			pub float z;
			pub float angZDeg;
			pub std::string haulerEntityInstanceId;

			pub std::shared_ptr<rpg3D::ParseData_VoxlesGrid3DGeometry> entityTemplate{};

			pub IWorldEntity* tmpCreatedEntity = nullptr;
		};
		
		pub int persistentEntitiesIdCounter = -1;
		pub int entitiesCount = -1;

		pub ArrayList<CfgEntity> entitiesList{};

		pub int loadingTemplate_entityIndex = -1;
	};

	priv int loadPhase = LoadPhase::Idle;
	priv bool isLoadPhaseInit = false;

	// These callbacks will be auto disconnected
	priv ArrayList<std::shared_ptr<IEventListener>> fhListenersList{};

	priv std::shared_ptr<std::string> data_waitingFileRead = nullptr;

	priv ParseData_EntitiesList parseData_EntitiesList{};

    pub explicit GPLCC_PersistentEntitiesLoader(base::IGameWorld* gw);

	pub void loadAllPersistentEntities() override;

	priv void onGWEvent_gameLoop_contentCreation(IEventListener& eventListener, base::GWEvent& event);
	priv int tickLoadPhase();

	priv bool initLoadPhase_LoadEntitiesList();
	priv bool tickLoadPhase_LoadEntitiesList();
	priv static void parseData_EntitiesList_persistentEntitiesIdCounter(ParseData_EntitiesList* parseData_EntitiesList, std::string& line);
	priv static void parseData_EntitiesList_entitiesCount(ParseData_EntitiesList* parseData_EntitiesList, std::string& line);
	priv static void parseData_EntitiesList_entity(ParseData_EntitiesList* parseData_EntitiesList, std::string& line);

	priv void initLoadPhase_LoadEntitiesTemplates();
	priv bool tickLoadPhase_LoadEntitiesTemplates();
	
	priv void initLoadPhase_CreateEntities();

	prot virtual void onPersistentEntitiesLoadFinished();

	priv void onAsyncFHEvent(IEventListener& eventListener, base::AsyncFHEvent& event);

	priv bool readCachedStringFileAsync(std::string& cacheId);

    pub ~GPLCC_PersistentEntitiesLoader() override;
};
};
