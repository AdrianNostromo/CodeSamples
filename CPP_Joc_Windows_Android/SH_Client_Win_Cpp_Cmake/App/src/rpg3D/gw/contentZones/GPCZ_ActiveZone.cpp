#include "GPCZ_ActiveZone.h"
#include <rpg3D/gw/contentZones/zone/ContentZone.h>
#include <rpg3D/gw/ui/IGamePartUserInterface.h>
#include <worldGame3D/gw/IGameWorld.h>

using namespace rpg3D;

GPCZ_ActiveZone::GPCZ_ActiveZone(base::IGameWorld* gw)
	: super(gw)
{
	//void
}

void GPCZ_ActiveZone::handleMainUIViewSwitching(std::shared_ptr<worldGame3D::ContentZone> activeZone) {
	int newMainUIVIewID;
	rpg3D::ContentZone* cActiveZone = nullptr;
	if (activeZone != nullptr) {
		cActiveZone = dynamic_cast<rpg3D::ContentZone*>(activeZone.get());
		if (cActiveZone == nullptr) {
			throw LogicException(LOC);
		}

		newMainUIVIewID = cActiveZone->getMainUIViewID();
	} else {
		newMainUIVIewID = -1;
	}

	if (activeMainUIVIewID != newMainUIVIewID) {
		activeMainUIVIewID = newMainUIVIewID;

		rpg3D::IGamePartUserInterface* gpUI = gw->getComponentAs<rpg3D::IGamePartUserInterface*>(true/*mustExist*/);

		if (activeMainUIVIewID >= 0) {
			gpUI->setNewMainUIViewUninitialised(cActiveZone->getNewMainUIViewUninitialised(gw));
		} else {
			gpUI->setNewMainUIViewUninitialised(nullptr);
		}
	}
}

GPCZ_ActiveZone::~GPCZ_ActiveZone() {
	//void
}
