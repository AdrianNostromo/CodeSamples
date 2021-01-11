#include <base/screen/ScreenUtil.h>
#include "Camera.h"
#include <base/exceptions/LogicException.h>
#include <base/math/Math.h>

Camera::Camera() {
	//void
}

void Camera::invalidate() {
    isDirty = true;
}

void Camera::setViewportSize(float viewportWidth, float viewportHeight) {
    if(this->unscaledViewportWidth == viewportWidth && this->unscaledViewportHeight == viewportHeight) {
        return ;
    }

    this->unscaledViewportWidth = viewportWidth;
    this->unscaledViewportHeight = viewportHeight;

    computeBakedViewportSize();

    invalidate();
}

float Camera::getViewportScaleZoom() {
    return viewportScaleZoom;
}

void Camera::setViewportScaleZoom(float viewportScaleZoom) {
    if(this->viewportScaleZoom == viewportScaleZoom) {
        return ;
    }

    this->viewportScaleZoom = viewportScaleZoom;

    computeBakedViewportSize();

    invalidate();
}

void Camera::computeBakedViewportSize() {
    bakedViewportWidth = unscaledViewportWidth * viewportScaleZoom;
    bakedViewportHeight = unscaledViewportHeight * viewportScaleZoom;
}

void Camera::setDistanceRange(float near, float far) {
    if(this->near == near && this->far == far) {
        return ;
    }

    this->near = near;
    this->far = far;

    invalidate();
}

void Camera::setUp(const Vector3& vector) {
    if(up.equals(vector)) {
        return ;
    }

    up.set(vector);

    invalidate();
}

Vector3 const* Camera::getPosition() {
    return &position;
}

void Camera::setPosition(float x, float y, float z) {
    // Need to use rounding error check because the physics always has movements.
    if(position.equals(x, y, z, Math::FLOAT_ROUNDING_ERROR)) {
        return ;
    }

    position.set(x, y, z);

    invalidate();
}

Vector3 const* Camera::getDirection() {
    return &direction;
}

void Camera::setDirection(float x, float y, float z) {
    if(direction.equals(x, y, z, Math::FLOAT_ROUNDING_ERROR)) {
        return ;
    }

    direction.set(x, y, z);
    direction.nor();

    invalidate();
}

Frustum const* Camera::getFrustum() {
	return &frustum;
}

bool Camera::updateIfNeeded() {
    if(isDirty) {
		isDirty = false;

        updateMain();

        return true;
    }

    return false;
}

void Camera::updateMain() {
	updateIndex++;
}

/** Function to translate a point given in screen coordinates to world space. It's the same as GLU gluUnProject but does not
     * rely on OpenGL. The viewport is assumed to span the whole screen and is fetched from {@link Graphics#getWidth()} and
     * {@link Graphics#getHeight()}. The x- and y-coordinate of vec are assumed to be in screen coordinates (origin is the top left
     * corner, y pointing down, x pointing to the right) as reported by the touch methods in {@link Input}. A z-coordinate of 0
     * will return a point on the near plane, a z-coordinate of 1 will return a point on the far plane.
     * @param screenCoords the point in screen coordinates
     * @return the mutated and unprojected screenCoords {@link Vector3} */
Vector3* Camera::unproject(Vector3& screenCoords) {
    return unproject(screenCoords, 0, 0, base::ScreenUtil::screen->getWindowResolutionX(), base::ScreenUtil::screen->getWindowResolutionY());
}

/** Function to translate a point given in screen coordinates to world space. It's the same as GLU gluUnProject, but does not
	 * rely on OpenGL. The x- and y-coordinate of vec are assumed to be in screen coordinates (origin is the top left corner, y
	 * pointing down, x pointing to the right) as reported by the touch methods in {@link Input}. A z-coordinate of 0 will return a
	 * point on the near plane, a z-coordinate of 1 will return a point on the far plane. This method allows you to specify the
	 * viewport position and dimensions in the coordinate system expected by {@link GL20#glViewport(int, int, int, int)}, with the
	 * origin in the bottom left corner of the screen.
	 * @param screenCoords the point in screen coordinates (origin top left)
	 * @param viewportX the coordinate of the bottom left corner of the viewport in glViewport coordinates.
	 * @param viewportY the coordinate of the bottom left corner of the viewport in glViewport coordinates.
	 * @param viewportWidth the width of the viewport in pixels
	 * @param viewportHeight the height of the viewport in pixels
	 * @return the mutated and unprojected screenCoords {@link Vector3} */
Vector3* Camera::unproject(
        Vector3& screenCoords,
        float viewportX, float viewportY,
        float viewportWidth, float viewportHeight)
{
    float x = screenCoords.x, y = screenCoords.y;
    x = x - viewportX;
    y = base::ScreenUtil::screen->getWindowResolutionY() - y - 1;
    y = y - viewportY;
    screenCoords.x = (2 * x) / viewportWidth - 1;
    screenCoords.y = (2 * y) / viewportHeight - 1;
    screenCoords.z = 2 * screenCoords.z - 1;
    screenCoords.prj(invProjectionView);
    return &screenCoords;
}

/** Creates a picking {@link Ray} from the coordinates given in screen coordinates. It is assumed that the viewport spans the
 * whole screen. The screen coordinates origin is assumed to be in the yMax xMin corner, its y-axis pointing down, the x-axis
 * pointing to the xMax. The returned instance is not a new instance but an internal member only accessible via this function.
 * @param viewportX the coordinate of the yMin xMin corner of the viewport in glViewport coordinates.
 * @param viewportY the coordinate of the yMin xMin corner of the viewport in glViewport coordinates.
 * @param viewportWidth the width of the viewport in pixels
 * @param viewportHeight the height of the viewport in pixels
 * @return the picking Ray. */
Ray& Camera::getPickRay (
        float screenX, float screenY,
        float viewportX, float viewportY,
        float viewportWidth, float viewportHeight)
{
    unproject(
            *ray.origin.set(screenX, screenY, 0),
            viewportX, viewportY,
            viewportWidth, viewportHeight
    );
    unproject(
            *ray.direction.set(screenX, screenY, 1),
            viewportX, viewportY,
            viewportWidth, viewportHeight
    );
    ray.direction.sub(ray.origin)->nor();
    return ray;
}

/** Creates a picking {@link Ray} from the coordinates given in screen coordinates. It is assumed that the viewport spans the
 * whole screen. The screen coordinates origin is assumed to be in the yMax xMin corner, its y-axis pointing down, the x-axis
 * pointing to the xMax. The returned instance is not a new instance but an internal member only accessible via this function.
 * @return the picking Ray. */
Ray& Camera::getPickRay (float screenX, float screenY, bool invertAxisOriginY) {
    return getPickRay(screenX, (!invertAxisOriginY) ? screenY : base::ScreenUtil::screen->getWindowResolutionY() - screenY, 0, 0, base::ScreenUtil::screen->getWindowResolutionX(), base::ScreenUtil::screen->getWindowResolutionY());
}

Matrix4 const* Camera::getCombined() {
    return &combined;
}

int Camera::getUpdateIndex() {
	return updateIndex;
}

Camera::~Camera() {
	//void
}
