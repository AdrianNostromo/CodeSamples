#pragma once

#include "ICinematicController.h"
#include <string>
#include <base/math/Vector3.h>

namespace graphics {
	class IEnvironment3D;
};

namespace graphics {
class CinematicController : virtual public ICinematicController {
public:
	// This is changed by World3DCinematic
	bool isActive = false;

    std::shared_ptr<CinematicController> prev = nullptr;
    std::shared_ptr<CinematicController> post = nullptr;

private:
	std::string location;
	IEnvironment3D* environment3D;

	Vector3 dt_worldFocusPoint;
	float dt_camera3DZoom;
	Vector3 dt_cameraPosition;
	Vector3 dt_cameraDirection;
	Vector3 dt_sunLightDirection;

public:
	CinematicController(std::string& location, IEnvironment3D* environment3D);

	std::string getLocation() { return location; }

	std::shared_ptr<CinematicController> peekPrev() override { return prev; }

	Vector3* getWorldFocusPoint() override { return &dt_worldFocusPoint; }
	void setWorldFocusPoint(float x, float y, float z) override;

	float getCamera3DZoom() override { return dt_camera3DZoom; }
	void setCamera3DZoom(float cameraZoom) override;

	Vector3* getCameraPosition() override { return &dt_cameraPosition; }
	void setCameraPosition(float dirX, float dirY, float dirZ) override;

	Vector3* getCameraDirection() override { return &dt_cameraDirection; }
	void setCameraDirection(float dirX, float dirY, float dirZ) override;

	Vector3* getSunLightDirection() override { return &dt_sunLightDirection; }
	void setSunLightDirection(float dirX, float dirY, float dirZ) override;

	Camera* peekCamera() override;

	int setIsActive(bool isActive);

	~CinematicController() override;
private:
	void sync_worldFocusPoint();
	void sync_camera3DZoom();
	void sync_cameraPosition();
	void sync_cameraDirection();
	void sync_sunLightDirection();

};
};
