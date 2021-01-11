#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/event/GWEvent.h>
#include <memory>

class IWorldEntity;
namespace base {
	class WrappedWorldEntity;
};
namespace worldGame3D {
	class IEntsGroup;
};

namespace base {
class IGPContent {
	pub static GWEvent* GWEvent_onPlayerEntitySelected;
	pub static GWEvent* GWEvent_onPlayerEntityDeselectedPre;
	pub static GWEvent* GWEvent_onSelectedPlayerEntityChanged;
	
	pub virtual IWorldEntity* getSelectedPlayerEntity(bool mustExist) = 0;
	// The entity must be managed with the above functions.
	pub virtual void setSelectManagedPlayerEntity(IWorldEntity* entity) = 0;
	pub virtual void setSelectManagedPlayerEntity(std::shared_ptr<base::WrappedWorldEntity> wEntity, bool handleBadEntity) = 0;

	pub virtual worldGame3D::IEntsGroup* getPlayerEntitiesGroup() = 0;

	pub virtual bool getIsSelectorEnabled() = 0;
	pub virtual void setIsSelectorEnabled(bool isSelectorEnabled) = 0;

	pub virtual ~IGPContent() = default;
};
};
