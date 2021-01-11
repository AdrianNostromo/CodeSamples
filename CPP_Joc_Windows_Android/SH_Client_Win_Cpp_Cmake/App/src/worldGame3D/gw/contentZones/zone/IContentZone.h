#pragma once

#include <base/gh.h>
#include <string>
#include <memory>

namespace base {
	class IGameWorld;
	class GWEvent;
};
namespace worldGame3D {
	class ZoneStartExtraData;
};

namespace worldGame3D {
class IContentZone {
	pub virtual std::string& getId() = 0;

	pub static base::GWEvent* GWEvent_onZoneActivated;
	pub static base::GWEvent* GWEvent_onZoneDeactivatePre;

	pub virtual bool getIsActive() = 0;
	pub virtual std::shared_ptr<worldGame3D::ZoneStartExtraData> getZoneStartExtraData() = 0;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="zoneStartExtraData"></param>
	/// <param name="gw"></param>
	/// <returns>Is activation finished</returns>
	pub virtual void preActivate(
		std::shared_ptr<worldGame3D::ZoneStartExtraData>& zoneStartExtraData,
		base::IGameWorld* gw) = 0;
	pub virtual bool finishActivation() = 0;
	pub virtual void deactivate(base::IGameWorld* gw) = 0;

    pub virtual ~IContentZone() = default;
};
};
