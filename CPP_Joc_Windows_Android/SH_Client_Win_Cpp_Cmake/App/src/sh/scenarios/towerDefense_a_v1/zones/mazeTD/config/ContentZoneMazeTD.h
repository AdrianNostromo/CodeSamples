#pragma once

#include <base/gh.h>
#include <rpg3D/gw/contentZones/zone/ContentZone.h>
#include <base/list/Array3D.h>
#include <base/list/ArrayList.h>
#include <memory>

class Visual3DModelMaterial;
class EntityTemplate;

namespace towerDefense_a_v1 {
namespace MazeTD {
class ContentZoneMazeTD : public rpg3D::ContentZone {priv typedef rpg3D::ContentZone super;pub dCtor(ContentZoneMazeTD);
	pub static std::string NAME;
	pub static std::shared_ptr<worldGame3D::ContentZone> FactoryFunction();

	pub explicit ContentZoneMazeTD();

	pub int getMainUIViewID() final;
	pub rpg3D::MainInGameUIView* getNewMainUIViewUninitialised(base::IGameWorld* gw) final;

	prot void onZoneActivated(std::shared_ptr<worldGame3D::ZoneStartExtraData>& zoneStartExtraData, base::IGameWorld* gw) override;
	prot void onZoneDeactivate_pre(base::IGameWorld* gw) override;

	priv void createGroundPatch(
		float x, float y, float z,
		Array3D<signed char>* voxelsGrid,
		ArrayList<Visual3DModelMaterial*>* materialsList,
		base::IGameWorld* gw, float gridCellSize);
	void createConfigEntity(
		float x, float y, float z,
		float rotZ,
		EntityTemplate* entityTemplate,
		std::shared_ptr<std::string> instanceId,
		base::IGameWorld* gw,
		int* teamId);

    pub ~ContentZoneMazeTD() override;
};
};
};
