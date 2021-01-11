#pragma once

#include <base/gh.h>
#include "GPPLH_Platforms.h"
#include <memory>
#include <string>
#include <base/math/Vector3.h>

class EntityTemplate;
class IWorldEntity;
namespace base {
	class GWEvent;
	class EntityEvent;
};
namespace rpg3D {
	class IStackableModule;
};

namespace towerDefense_a_v1 {
namespace PathTD {
class GPPLH_PlayLoop : public GPPLH_Platforms {priv typedef GPPLH_Platforms super;pub dCtor(GPPLH_PlayLoop);
	priv class Status {
		pub static const int ZERO;
		pub static const int preStartDelay;
		pub static const int waitingForNextWaveStartRequest;
		pub static const int spawningWaveEnemies;
		pub static const int waitingForWaveEnd;
		pub static const int postWaveDelay;
	};
	priv int status = Status::ZERO;
	priv bool isStatusInit = false;

	priv Vector3 enemiesSpawnLocation{};
	priv float enemiesSpawnRotZ = 0.0f;
	priv bool isEnemySpawnLocationInit = false;

	// General cooldown.
	priv float cooldownS;
	priv int enemiesToSpawn;
	priv int mobsActiveoOnWorld;

	priv IWorldEntity* turretSpawnPlatform_1 = nullptr;
	priv rpg3D::IStackableModule* turretSpawnPlatform_1_stackable = nullptr;
	priv IWorldEntity* turretSpawnPlatform_2 = nullptr;
	priv rpg3D::IStackableModule* turretSpawnPlatform_2_stackable = nullptr;

	priv bool isNextWaveStartRequested = false;

    pub explicit GPPLH_PlayLoop(base::IGameWorld* gw);
	prot void createBMain() override;

	priv void uiView_onStartNextWaveRequest(IMainInGameUIView* target);

	priv void onGWEvent_gameLoop_contentCreation(IEventListener& eventListener, base::GWEvent& event);
	priv int tickStatus(float deltaS);

	priv IWorldEntity* spawnMob(
		float x, float y, float z,
		float rotZ,
		EntityTemplate* entityTemplate,
		std::shared_ptr<std::string> instanceId,
		base::IGameWorld* gw);

	priv void handleTurretsShopPlatforms();

	priv void onSpawnedWorldMobEntityEvent(IEventListener& eventListener, base::EntityEvent& event);

    pub ~GPPLH_PlayLoop() override;
};
};
};
