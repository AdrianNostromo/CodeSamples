#include "MDIN_Move_UIDirectionalPad.h"
#include <rpg3D/gw/ui/IGamePartUserInterface.h>

using namespace rpg3D;

MDIN_Move_UIDirectionalPad::MDIN_Move_UIDirectionalPad(base::IGameWorld* gw)
	: super(gw)
{
	uiDpadControlsListener.cb_onSetMoveDirection = std::bind(&MDIN_Move_UIDirectionalPad::uiDPad_onSetMoveDirection, this, std::placeholders::_1, std::placeholders::_2);
}

void MDIN_Move_UIDirectionalPad::createBMain() {
	super::createBMain();

	gw->getComponentAs<IGamePartUserInterface*>(true/*mustExist*/)->setMainUiDPadListener(&uiDpadControlsListener);
}

void MDIN_Move_UIDirectionalPad::uiDPad_onSetMoveDirection(float moveDirVectorX, float moveDirVectorY) {
	if(uiDPadDirVector.equals(moveDirVectorX, moveDirVectorY)) {
		return ;
	}

	uiDPadDirVector.set(moveDirVectorX, moveDirVectorY);

	onMoveInput();
}

void MDIN_Move_UIDirectionalPad::disposePre() {
	gw->getComponentAs<IGamePartUserInterface*>(true/*mustExist*/)->setMainUiDPadListener(nullptr);

	super::disposePre();
}

MDIN_Move_UIDirectionalPad::~MDIN_Move_UIDirectionalPad() {
	//void
}
