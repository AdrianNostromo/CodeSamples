#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/contentZones/zone/ContentZone.h>

namespace rpg3D {
	class MainInGameUIView;
	class ContentConfigsProvider;
};

namespace rpg3D {
class CZ_Base : public worldGame3D::ContentZone {priv typedef worldGame3D::ContentZone super;pub dCtor(CZ_Base);
	prot ContentConfigsProvider* contentConfigsProvider = nullptr;

    pub explicit CZ_Base(std::string& id);

	pub virtual int getMainUIViewID();
	pub virtual rpg3D::MainInGameUIView* getNewMainUIViewUninitialised(base::IGameWorld* gw);

	prot void onZoneActivatedPre(std::shared_ptr<worldGame3D::ZoneStartExtraData>& zoneStartExtraData, base::IGameWorld* gw) override;

    pub ~CZ_Base() override;
};
};
