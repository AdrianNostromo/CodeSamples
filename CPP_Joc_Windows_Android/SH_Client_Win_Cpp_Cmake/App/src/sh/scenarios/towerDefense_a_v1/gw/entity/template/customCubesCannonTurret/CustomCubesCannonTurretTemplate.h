#pragma once

#include <base/gh.h>
#include <base/math/Vector3.h>
#include <worldGame3D/gw/entity/template/BasicModuleTemplate.h>
#include <string>
#include <base/list/Array1D.h>

namespace towerDefense_a_v1 {
class CustomCubesCannonTurretTemplate : public BasicModuleTemplate {priv typedef BasicModuleTemplate super;pub dCtor(CustomCubesCannonTurretTemplate);
	pub float turretTurnSpeedZD;
	pub float barrelTurnSpeedYD;

	pub std::string visualId_groundPad;
	pub std::string visualId_turret;
	pub std::string visualId_barrel;
	pub std::string visualId_projectile;

	pub float barrelPosZ_normal;
	pub float barrelPosZ_loading;

	pub float barrelRotY_loading;

	pub Array1D<float>* turretReloadAnglesZ;

	pub std::string* activationSoundConfig_id;
	pub float activationSoundConfig_volume;

    pub CustomCubesCannonTurretTemplate(
		float turretTurnSpeedZD, float barrelTurnSpeedYD,
		std::string visualId_groundPad,
		std::string visualId_turret,
		std::string visualId_barrel,
		std::string visualId_projectile,

		float barrelPosZ_normal, float barrelPosZ_loading, float barrelRotY_loading,
		Array1D<float>* turretReloadAnglesZ,

		std::string* activationSoundConfig_id, float activationSoundConfig_volume);
	
	pub IEntityModule* createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) override;

	pub ~CustomCubesCannonTurretTemplate() override;
};
};
