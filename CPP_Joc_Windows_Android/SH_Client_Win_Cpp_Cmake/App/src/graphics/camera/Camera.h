#pragma once

#include <base/gh.h>
#include <base/math/Vector3.h>
#include <base/math/Matrix4.h>
#include <base/math/Quaternion.h>
#include <graphics/util/Frustum.h>
#include <base/math/collision/Ray.h>

class Camera {pub dCtor(Camera);
	prot float viewportScaleZoom = 1.0f;

    prot float bakedViewportWidth = 0;
    prot float bakedViewportHeight = 0;

    prot float near = 1;
    prot float far = 100;

    /** the position of the camera **/
    prot Vector3 position{};
    /** the unit length direction vector of the camera **/
    prot Vector3 direction{ 0, 0, -1 };
    /** the unit length up vector of the camera **/
    prot Vector3 up{ 0, 1, 0 };

    /** the projection matrix **/
    prot Matrix4 projection{};
    /** the view matrix **/
    prot Matrix4 view{};
    /** the combined projection and view matrix **/
    prot Matrix4 combined{};
    /** the inverse combined projection and view matrix **/
    prot Matrix4 invProjectionView{};

    /** the frustum **/
    prot Frustum frustum{};

    prot bool isDirty = true;
    prot int updateIndex = 0;

	priv float unscaledViewportWidth = 0;
    priv float unscaledViewportHeight = 0;

    priv Ray ray{Vector3(), Vector3()};

    pub explicit Camera();

    // This is used mostly for testing.
    prot virtual void invalidate();

    pub void setViewportSize(float viewportWidth, float viewportHeight);

    pub float getViewportScaleZoom();
    pub virtual void setViewportScaleZoom(float viewportScaleZoom);

    // NOTE; The near value must be larger than 0.0f or rendering errors will occur.
    pub void setDistanceRange(float near, float far);

    pub void setUp(const Vector3& vector);

    pub Vector3 const* getPosition();
    pub void setPosition(float x, float y, float z);

    pub Vector3 const* getDirection();
    pub void setDirection(float x, float y, float z);

    pub Frustum const* getFrustum();

    /** Recalculates the projection and view matrix of this camera and the {@link Frustum} planes. Use this after you've manipulated
	 * any of the attributes of the camera. */
    pub bool updateIfNeeded();

    pub Vector3* unproject (Vector3& screenCoords);
    pub Vector3* unproject(Vector3& screenCoords, float viewportX, float viewportY, float viewportWidth, float viewportHeight);

    pub Ray& getPickRay (
            float screenX, float screenY,
            float viewportX, float viewportY,
            float viewportWidth, float viewportHeight);
    pub Ray& getPickRay (float screenX, float screenY, bool invertAxisOriginY);

    pub Matrix4 const* getCombined();

    pub int getUpdateIndex();

    prot virtual void updateMain();

    priv void computeBakedViewportSize();

    pub virtual ~Camera();
};
