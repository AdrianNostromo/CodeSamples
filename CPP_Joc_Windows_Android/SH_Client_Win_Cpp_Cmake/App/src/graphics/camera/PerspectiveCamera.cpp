#include <base/math/Math.h>
#include "PerspectiveCamera.h"

PerspectiveCamera::PerspectiveCamera(float fieldOfViewY)
    : fieldOfViewY(fieldOfViewY)
{
    //void
}

void PerspectiveCamera::setFieldOfViewY(float fieldOfViewY) {
	if(this->fieldOfViewY == fieldOfViewY) {
		return ;
	}

	this->fieldOfViewY = fieldOfViewY;

    invalidate();
}

void PerspectiveCamera::updateMain() {
    super::updateMain();

    float aspect = bakedViewportWidth / bakedViewportHeight;

    projection.setToProjection(Math::abs(near), Math::abs(far), fieldOfViewY * viewportScaleZoom, aspect);

    tmp.set(position)->add(direction);
    view.setToLookAt(position, tmp, up);
    combined.set(projection);
    Matrix4::Mul(combined.val, view.val);

    // Update frustum;
    invProjectionView.set(combined);
    Matrix4::Inv(invProjectionView.val);
    frustum.update(invProjectionView);
}

PerspectiveCamera::~PerspectiveCamera() {
    //void
}
