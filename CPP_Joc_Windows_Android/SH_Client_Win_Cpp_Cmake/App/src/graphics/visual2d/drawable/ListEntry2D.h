#pragma once

#include <base/gh.h>
#include <base/menu/AnimatedComposedStiledVisual.h>
#include <base/map/Map1D.h>

namespace base {
	class List2D;
};

namespace base {
class ListEntry2D : public AnimatedComposedStiledVisual {priv typedef AnimatedComposedStiledVisual super;pub dCtor(ListEntry2D);
	pub List2D* list;

	// This can be use to find entries with a faster comparison than using userDataMap strings;
	/// This is provided by the user and is not managed by the list.
	pub const int entryUid = -1;
	pub Map1D<std::string, std::shared_ptr<base::IWrappedValue>>* userDataMap = nullptr;

	pub bool isActive = false;

	pub std::shared_ptr<std::string> localStateFlags = nullptr;

	pub explicit ListEntry2D(
		IAppMetrics* appMetrics, IAppAssets* appAssets, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, List2D* list,
		int entryUid);

	pub Map1D<std::string, std::shared_ptr<base::IWrappedValue>>* getUserDataMap();

	pub bool getIsActive();
	pub void setIsActive(bool isActive, bool doAnimation);

	pub void show(bool doAnimation);

	pub void setLocalStateFlags(std::shared_ptr<std::string> localStateFlags);

	pub std::string computeStateFlags(std::string baseState) override;

	pub ~ListEntry2D() override;
};
};
