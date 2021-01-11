#pragma once

#include <base/gh.h>
#include <rpg3D/gw/contentZones/zone/ContentZone.h>
#include <base/list/Array3D.h>
#include <base/list/ArrayList.h>

class Visual3DModelMaterial;

namespace spaceShipMissions_a_v1 {
namespace Main {
class ContentZoneMain : public rpg3D::ContentZone {priv typedef rpg3D::ContentZone super;pub dCtor(ContentZoneMain);
	pub static std::string NAME;
	pub static std::shared_ptr<worldGame3D::ContentZone> FactoryFunction();

    pub explicit ContentZoneMain();
	
	prot void onZoneActivated(std::shared_ptr<worldGame3D::ZoneStartExtraData>& zoneStartExtraData, base::IGameWorld* gw) override;

	priv void createGroundPatch(
		float x, float y, float z,
		Array3D<signed char>* voxelsGrid,
		ArrayList<Visual3DModelMaterial*>* materialsList,
		base::IGameWorld* gw);

    pub ~ContentZoneMain() override;
};
};
};
