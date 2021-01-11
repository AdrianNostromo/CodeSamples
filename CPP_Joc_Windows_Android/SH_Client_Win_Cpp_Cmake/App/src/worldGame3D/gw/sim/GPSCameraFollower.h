#pragma once

#include <worldGame3D/gw/util/GameCameraModeBitGroups.h>
#include <worldGame3D/gw/entity/module/lookAt/LookAtModule.h>
#include "GPSimBase.h"
#include <base/screen/IScreen.h>

namespace base {
class GPSCameraFollower : public GPSimBase {priv typedef GPSimBase super;pub dCtor(GPSCameraFollower);
    priv class CfgFollowerThirdPersonCamera {
		pub Vector3 cfg_posOffset_dir{};

		pub float cfg_posOffset_dist_min = 0.0f;
		pub float cfg_posOffset_dist_max = 0.0f;

		pub float cfg_minOffsetAtWindowDiagonalInches = 0.0f;
		pub float cfg_maxOffsetAtWindowDiagonalInches = 0.0f;

		pub Vector3 cfg_focusPointOffset{};

		pub Vector3 baked_posOffset{};
		pub Vector3 baked_direction{};
    };
    priv class CfgFollowerFirstPersonCamera {
		pub Vector3 cfg_posOffset_dir{};
		pub float cfg_posOffset_dist = 0.0f;

		pub Vector3 baked_posOffset{};
    };
	
	priv CfgFollowerThirdPersonCamera followerThirdPerson{};
	priv CfgFollowerFirstPersonCamera followerFirstPerson{};

	priv IWorldEntity* cameraFollowTarget = nullptr;
    // This is used to control the camera direction.
	priv ILookAtModule* cameraFollowTarget_lookAt = nullptr;

    // This is used for first person and third person camera modes.
	priv Vector3 lastCameraFollowTargetPos{0.0f, 0.0f, 0.0f};
    // This is used for first person camera mode.
	priv Vector3 lastCameraFollowTarget_lookAtDirection{};

	priv bool isDirty_cameraConfig = false;
	priv bool isDirty_cameraSync = false;

	priv Vector3 t_v1{};

	priv int cameraModeBitGroup = GameCameraModeBitGroups::followerFirstPersonTargetRotation;

	priv float animateTransitionDurationS = 0.5f;

	priv float animateTransition_cp = 1.0f;
	priv Vector3 animateTransition_oldPos{};
	priv Vector3 animateTransition_oldDir{};

	priv base::IScreen::CbListener screenListener{"GPSCameraFollower"};

	pub explicit GPSCameraFollower(IGameWorld* gw);

	pub int getCameraMode() override;
	pub void setCameraMode(int cameraModeBitGroup, bool animateTransition) override;

	pub void configCamera_followerFirstPersonCamera(
        Vector3& offsetDir, float offsetDist) override;
	pub void configCamera_followerThirdPersonCamera(
        Vector3& offsetDir,
        float posOffset_dist_min, float posOffset_dist_max,
        float minOffsetAtWindowDiagonalInches, float maxOffsetAtWindowDiagonalInches,
        Vector3& focusPointOffset) override;
	pub void setCameraFollowTarget(IWorldEntity* cameraFollowTarget);

	priv void onGWEvent_gameLoop_camera(IEventListener& eventListener, base::GWEvent& event);

    pub ~GPSCameraFollower() override;
protected:
    void createMain() override;

    void disposeMain() override;
private:
    void onActiveScreenResolutionChange(int resolutionX, int resolutionY);

    void syncCamera();

};
};
