#pragma once

#include <base/gh.h>
#include "IContentZone.h"
#include <memory>
#include <base/list/ArrayList.h>

class IEventListener;
namespace base {
	class IGameWorld;
};

namespace worldGame3D {
class CZ_Base : virtual public IContentZone {pub dCtor(CZ_Base);
	priv std::string id;

	// This is used by some events processing.
	prot base::IGameWorld* gw = nullptr;

	priv bool isActive = false;
	priv std::shared_ptr<worldGame3D::ZoneStartExtraData> zoneStartExtraData = nullptr;

	prot ArrayList<std::shared_ptr<IEventListener>> autoListenersList{};

    pub explicit CZ_Base(std::string& id);
	
	pub std::string& getId() final;

	pub bool getIsActive() final;
	pub std::shared_ptr<worldGame3D::ZoneStartExtraData> getZoneStartExtraData() final;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="zoneStartExtraData"></param>
	/// <param name="gw"></param>
	/// <returns>Is activation finished</returns>
	pub void preActivate(
		std::shared_ptr<worldGame3D::ZoneStartExtraData>& zoneStartExtraData,
		base::IGameWorld* gw) final;
	pub bool finishActivation() final;
	pub void deactivate(base::IGameWorld* gw) final;

	/// <summary>
	/// This may be called even if load finished.
	/// </summary>
	/// <returns>true if assets finished loading.</returns>
	prot virtual bool loadActivationAssets(base::IGameWorld* gw);
	prot virtual void unloadActivationAssets();

	prot virtual void onZoneActivatedPre(std::shared_ptr<worldGame3D::ZoneStartExtraData>& zoneStartExtraData, base::IGameWorld* gw);
	prot virtual void onZoneActivated(std::shared_ptr<worldGame3D::ZoneStartExtraData>& zoneStartExtraData, base::IGameWorld* gw);
	// This is used to position the player.
	prot virtual void onZoneActivatedPositionPlayer(std::shared_ptr<worldGame3D::ZoneStartExtraData>& zoneStartExtraData, base::IGameWorld* gw);
	prot virtual void onZoneActivatedB(std::shared_ptr<worldGame3D::ZoneStartExtraData>& zoneStartExtraData, base::IGameWorld* gw);

	prot virtual void onZoneDeactivate_pre(base::IGameWorld* gw);

    pub ~CZ_Base() override;
};
};
