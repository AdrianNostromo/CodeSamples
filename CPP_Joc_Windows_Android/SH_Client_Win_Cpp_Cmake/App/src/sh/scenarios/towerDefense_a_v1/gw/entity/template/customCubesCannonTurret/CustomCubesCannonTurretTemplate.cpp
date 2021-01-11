#include "CustomCubesCannonTurretTemplate.h"
#include "../../module/customCubesCannonTurret/CustomCubesCannonTurretModule.h"
#include <base/math/Math.h>
#include <base/audio/sound/ISound.h>
#include <worldGame3D/gw/IEntityModuleCreator.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <base/app/IAppAssets.h>
#include <base/assets/IAssetsManager.h>

using namespace towerDefense_a_v1;

CustomCubesCannonTurretTemplate::CustomCubesCannonTurretTemplate(
	float turretTurnSpeedZD, float barrelTurnSpeedYD,
	std::string visualId_groundPad,
	std::string visualId_turret,
	std::string visualId_barrel,
	std::string visualId_projectile,

	float barrelPosZ_normal, float barrelPosZ_loading, float barrelRotY_loading,
	Array1D<float>* turretReloadAnglesZ,

	std::string* activationSoundConfig_id, float activationSoundConfig_volume)
	: super("customCubesCannonTurret", ICustomCubesCannonTurretModule::TYPE),
	turretTurnSpeedZD(turretTurnSpeedZD), barrelTurnSpeedYD(barrelTurnSpeedYD),
	visualId_groundPad(visualId_groundPad),
	visualId_turret(visualId_turret),
	visualId_barrel(visualId_barrel),
	visualId_projectile(visualId_projectile),

	barrelPosZ_normal(barrelPosZ_normal), barrelPosZ_loading(barrelPosZ_loading), barrelRotY_loading(barrelRotY_loading),
	turretReloadAnglesZ(turretReloadAnglesZ),

	activationSoundConfig_id(activationSoundConfig_id), activationSoundConfig_volume(activationSoundConfig_volume)
{
	//void
}

IEntityModule* CustomCubesCannonTurretTemplate::createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) {
    CustomCubesCannonTurretTemplate* t = dynamic_cast<CustomCubesCannonTurretTemplate*>(_t);
    if(!t) {
        throw LogicException(LOC);
    }

	base::audio::ISound* activationSound;
	if (activationSoundConfig_id != nullptr) {
		activationSound = moduleCreator->getGW()->getAppAssets()->getEmbeddedAssetsManager()->getSound(*activationSoundConfig_id);
		if (activationSound == nullptr) {
			throw LogicException(LOC);
		}
	} else {
		activationSound = nullptr;
	}

	CustomCubesCannonTurretModule* mod = new CustomCubesCannonTurretModule(
		entity, t, 
		activationSound
	);

    return mod;
}

CustomCubesCannonTurretTemplate::~CustomCubesCannonTurretTemplate() {
    //void
}
