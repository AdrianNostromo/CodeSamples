#include "EVM_RotationResetter.h"
#include <base/ease/Ease.h>
#include <graphics/visual3d/drawable/IContainer3D.h>
#include <worldGame3D/gw/event/GWEvent.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/IGameWorld.h>

using namespace rpg3D;

EVM_RotationResetter::EVM_RotationResetter(
	IWorldEntity* entity,
	EditableVoxelisedTemplate* t)
	: super(
		entity,
		t)
{
	e->getGWEvents()->addEventListener(
		base::IGameWorld::GWEvent_gameLoop_simulation->type,
		std::bind(&EVM_RotationResetter::onGWEvent_gameLoop_simulation, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
}

void EVM_RotationResetter::resetEditEntityRotation() {
	if (resetRotationCP >= 1.0f) {
		resetRotationCP = 0.0f;
	}
}

void EVM_RotationResetter::onIsEditingEnabledChanged() {
	super::onIsEditingEnabledChanged();

	resetRotationCP = 1.0f;
}

void EVM_RotationResetter::onGWEvent_gameLoop_simulation(IEventListener& eventListener, base::GWEvent& event) {
	if (resetRotationCP < 1.0f && isEditingEnabled) {
		if (resetRotationCP == 0.0f) {
			startRot.set(getEditorStructure_rotator()->getRotation());
		}

		resetRotationCP = Math::min(1.0f, resetRotationCP + event.deltaS * 3.0f);
		float easedValue = Ease::computeEaseValue(Ease::easeOutSine, resetRotationCP);

		// Reset rotation.
		tmp_rot.set(startRot);
		tmp_rot.slerp(endRot, easedValue);

		getEditorStructure_rotator()->setRotation(tmp_rot);
	}
}

EVM_RotationResetter::~EVM_RotationResetter() {
	//void
}
