#include "CZ_Base.h"
#include <rpg3D/gw/view/mainui/MainInGameUIView.h>
#include <worldGame3D/gw/IGameWorld.h>
#include "entitiesConfigsProvider/ContentConfigsProvider.h"

using namespace rpg3D;

CZ_Base::CZ_Base(std::string& id)
	: super(id)
{
	//void
}

int CZ_Base::getMainUIViewID() {
	return rpg3D::IMainInGameUIView::ID;
}

rpg3D::MainInGameUIView* CZ_Base::getNewMainUIViewUninitialised(base::IGameWorld* gw) {
	rpg3D::MainInGameUIView* ret = new rpg3D::MainInGameUIView(gw->getHandler()->getApp(), gw);

	return ret;
}

void CZ_Base::onZoneActivatedPre(std::shared_ptr<worldGame3D::ZoneStartExtraData>& zoneStartExtraData, base::IGameWorld* gw) {
	super::onZoneActivatedPre(zoneStartExtraData, gw);

	if (contentConfigsProvider == nullptr) {
		contentConfigsProvider = new ContentConfigsProvider();

		//asd_01;// set default entres for tiledXY_ground, tiledXY_walls, posRot;
	}
}

CZ_Base::~CZ_Base() {
	if (contentConfigsProvider != nullptr) {
		delete contentConfigsProvider;
		contentConfigsProvider = nullptr;
	}
}
