#include <base/math/Math.h>
#include <base/ease/Ease.h>
#include <base/screen/ScreenUtil.h>
#include "GPSCameraFollower.h"
#include <graphics/environment/environment3D/cinematic/ICinematicController.h>
#include <worldGame3D/gw/main/IGamePartMain.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>

using namespace base;

GPSCameraFollower::GPSCameraFollower(IGameWorld* gw)
    : super(gw)
{
    screenListener.cb_onActiveScreenResolutionChange = std::bind(&GPSCameraFollower::onActiveScreenResolutionChange, this, std::placeholders::_1, std::placeholders::_2);

	gw->addEventListener(
		base::IGameWorld::GWEvent_gameLoop_camera->type,
		std::bind(&GPSCameraFollower::onGWEvent_gameLoop_camera, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
}

void GPSCameraFollower::createMain() {
    super::createMain();

    base::ScreenUtil::screen->addListener(&screenListener, 4);
}

void GPSCameraFollower::onActiveScreenResolutionChange(int resolutionX, int resolutionY) {
    isDirty_cameraConfig = true;
    isDirty_cameraSync = true;
}

int GPSCameraFollower::getCameraMode() {
    return cameraModeBitGroup;
}

void GPSCameraFollower::setCameraMode(int cameraModeBitGroup, bool animateTransition) {
    if(this->cameraModeBitGroup == cameraModeBitGroup) {
        return ;
    }

    this->cameraModeBitGroup = cameraModeBitGroup;

    if(this->cameraModeBitGroup == GameCameraModeBitGroups::followerFirstPersonTargetRotation && this->cameraFollowTarget) {
        cameraFollowTarget_lookAt = this->cameraFollowTarget->getComponentAs<ILookAtModule*>(true/*mustExist*/);
    }

    isDirty_cameraSync = true;

    if(animateTransition) {
        animateTransition_oldPos.set(
            gw->getGPMain()->getCC()->getCameraPosition()
        );
        animateTransition_oldDir.set(
            gw->getGPMain()->getCC()->getCameraDirection()
        );
        animateTransition_cp = 0.0f;
    }else {
        animateTransition_cp = 1.0f;
    }

    gw->dispatchEvent(*GWEvent_onCameraModeChanged);
}

void GPSCameraFollower::setCameraFollowTarget(IWorldEntity* cameraFollowTarget) {
    this->cameraFollowTarget = cameraFollowTarget;

    if(this->cameraModeBitGroup == GameCameraModeBitGroups::followerFirstPersonTargetRotation && this->cameraFollowTarget) {
        cameraFollowTarget_lookAt = this->cameraFollowTarget->getComponentAs<ILookAtModule*>(true/*mustExist*/);
    }

    isDirty_cameraSync = true;
}

void GPSCameraFollower::configCamera_followerFirstPersonCamera(
    Vector3& offsetDir, float offsetDist)
{
    followerFirstPerson.cfg_posOffset_dir.set(offsetDir)->nor();
    followerFirstPerson.cfg_posOffset_dist = offsetDist;

    isDirty_cameraConfig = true;
    isDirty_cameraSync = true;
}

void GPSCameraFollower::configCamera_followerThirdPersonCamera(
    Vector3& offsetDir,
    float posOffset_dist_min, float posOffset_dist_max,
    float minOffsetAtWindowDiagonalInches, float maxOffsetAtWindowDiagonalInches,
    Vector3& focusPointOffset)
{
    followerThirdPerson.cfg_posOffset_dir.set(offsetDir)->nor();

    followerThirdPerson.cfg_posOffset_dist_min = posOffset_dist_min;
    followerThirdPerson.cfg_posOffset_dist_max = posOffset_dist_max;

    followerThirdPerson.cfg_minOffsetAtWindowDiagonalInches = minOffsetAtWindowDiagonalInches;
    followerThirdPerson.cfg_maxOffsetAtWindowDiagonalInches = maxOffsetAtWindowDiagonalInches;

    followerThirdPerson.cfg_focusPointOffset.set(focusPointOffset);

    isDirty_cameraConfig = true;
    isDirty_cameraSync = true;
}

void GPSCameraFollower::onGWEvent_gameLoop_camera(IEventListener& eventListener, base::GWEvent& event) {
    if(isDirty_cameraConfig) {
        if(cameraModeBitGroup == GameCameraModeBitGroups::followerFirstPersonTargetRotation) {
            followerFirstPerson.baked_posOffset
                .set(followerFirstPerson.cfg_posOffset_dir)
                ->mul(followerFirstPerson.cfg_posOffset_dist);
        }else if(cameraModeBitGroup == GameCameraModeBitGroups::followerThirdPersonFixedGlobalRotation) {
        	// Use only the height because the projection_camera_matrix filed of view used is for the y axis.
            float windowSmallestSide = base::ScreenUtil::screen->getWindowSizeInches()->y;
            float distMultiplier = (windowSmallestSide - followerThirdPerson.cfg_minOffsetAtWindowDiagonalInches) / (followerThirdPerson.cfg_maxOffsetAtWindowDiagonalInches - followerThirdPerson.cfg_minOffsetAtWindowDiagonalInches);
            distMultiplier = Math::max(0.0f, Math::min(1.0f, distMultiplier));

	        float newBakedDist = followerThirdPerson.cfg_posOffset_dist_min + (followerThirdPerson.cfg_posOffset_dist_max - followerThirdPerson.cfg_posOffset_dist_min) * distMultiplier;

	        //std::cout << "Test (Camera 3d, window resize handling). Window diagonal inches: " << windowSmallestSide << ", " << distMultiplier << ", " << newBakedDist << std::endl;

            followerThirdPerson.baked_posOffset
                .set(followerThirdPerson.cfg_posOffset_dir)
                ->mul(newBakedDist);
            followerThirdPerson.baked_direction.set(followerThirdPerson.baked_posOffset)
                ->mul(-1.0f)
                ->add(followerThirdPerson.cfg_focusPointOffset)
                ->nor();
        }else {
            throw LogicException(LOC);
        }
    }

    if(animateTransition_cp < 1.0f) {
        animateTransition_cp = Math::min(1.0f, animateTransition_cp + event.deltaS * (1.0f / animateTransitionDurationS));

        isDirty_cameraSync = true;
    }

    if(!isDirty_cameraSync && cameraFollowTarget != nullptr) {
        Vector3* camTargetPos = cameraFollowTarget->getPos();

        //asdA9;// for first person, also check the rotZ;
        if(!lastCameraFollowTargetPos.equals(*camTargetPos)) {
            isDirty_cameraSync = true;
        }else if(cameraModeBitGroup == GameCameraModeBitGroups::followerFirstPersonTargetRotation && !lastCameraFollowTarget_lookAtDirection.equals(cameraFollowTarget_lookAt->peekLookAtDirection())) {
            isDirty_cameraSync = true;
        }
    }

    if(isDirty_cameraSync) {
        if(cameraFollowTarget) {
            syncCamera();
        }

        isDirty_cameraSync = false;
    }
}

void GPSCameraFollower::syncCamera() {
    Vector3* camTargetPos = cameraFollowTarget->getPos();

    Vector3 newCamPos{};
    Vector3 newCamDir{};

    if(cameraModeBitGroup == GameCameraModeBitGroups::followerFirstPersonTargetRotation) {
        newCamPos.set(
            camTargetPos->x + followerFirstPerson.baked_posOffset.x,
            camTargetPos->y + followerFirstPerson.baked_posOffset.y,
            camTargetPos->z + followerFirstPerson.baked_posOffset.z
        );

        t_v1.set(cameraFollowTarget_lookAt->peekLookAtDirection());
        newCamDir.set(
            t_v1.x,
            t_v1.y,
            t_v1.z
        );
        gw->getGPMain()->getCC()->setWorldFocusPoint(
            camTargetPos->x,
            camTargetPos->y,
            camTargetPos->z
        );

        lastCameraFollowTarget_lookAtDirection.set(t_v1);
    }else if(cameraModeBitGroup == GameCameraModeBitGroups::followerThirdPersonFixedGlobalRotation) {
        t_v1.set(*camTargetPos);
        t_v1.add(followerThirdPerson.baked_posOffset);

        newCamPos.set(
            t_v1.x,
            t_v1.y,
            t_v1.z
        );
        newCamDir.set(followerThirdPerson.baked_direction);
        gw->getGPMain()->getCC()->setWorldFocusPoint(
            camTargetPos->x,
            camTargetPos->y,
            camTargetPos->z
        );
    }else {
        throw LogicException(LOC);
    }

    if(animateTransition_cp < 1.0f) {
        // Handle animation;
        // Ease the value.
        float n = Ease::computeEaseValue(Ease::easeOutCubic, animateTransition_cp);

        newCamPos.set(
            animateTransition_oldPos.x + (newCamPos.x - animateTransition_oldPos.x) * n,
            animateTransition_oldPos.y + (newCamPos.y - animateTransition_oldPos.y) * n,
            animateTransition_oldPos.z + (newCamPos.z - animateTransition_oldPos.z) * n
        );

        newCamDir.set(
            animateTransition_oldDir.x + (newCamDir.x - animateTransition_oldDir.x) * n,
            animateTransition_oldDir.y + (newCamDir.y - animateTransition_oldDir.y) * n,
            animateTransition_oldDir.z + (newCamDir.z - animateTransition_oldDir.z) * n
        );
    }

    gw->getGPMain()->getCC()->setCameraPosition(
        newCamPos.x,
        newCamPos.y,
        newCamPos.z
    );
    gw->getGPMain()->getCC()->setCameraDirection(
        newCamDir.x,
        newCamDir.y,
        newCamDir.z
    );

    lastCameraFollowTargetPos.set(camTargetPos);
}

void GPSCameraFollower::disposeMain() {
    if(screenListener.getIsListening()) {
        base::ScreenUtil::screen->removeListener(&screenListener, 4);
    }

    super::disposeMain();
}

GPSCameraFollower::~GPSCameraFollower() {
    //void
}
