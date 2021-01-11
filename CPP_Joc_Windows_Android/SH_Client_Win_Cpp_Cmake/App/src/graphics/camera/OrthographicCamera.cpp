#include "OrthographicCamera.h"

OrthographicCamera::OrthographicCamera(bool yDown)
	: super()
{
	this->near = 0;

	if (yDown) {
		up.set(0, -1, 0);
		direction.set(0, 0, 1);
	} else {
		up.set(0, 1, 0);
		direction.set(0, 0, -1);
	}
}

void OrthographicCamera::updateMain() {
	super::updateMain();

	projection.setToOrtho(
		-bakedViewportWidth / 2,
		bakedViewportWidth / 2,
		-bakedViewportHeight / 2,
		bakedViewportHeight / 2, near, far
	);
	
	tmp.set(position)->add(direction);
	view.setToLookAt(position, tmp, up);
	
	combined.set(projection);
	Matrix4::Mul(combined.val, view.val);

    // Update frustum;
    invProjectionView.set(combined);
    Matrix4::Inv(invProjectionView.val);
    frustum.update(invProjectionView);
}

OrthographicCamera::~OrthographicCamera() {
    //void
}
