#pragma once

#include <base/gh.h>
#include <base/object/Object.h>
#include "IPhysicsActor.h"
#include <base/list/ArrayList.h>

class IWorldEntity;
namespace rpg3D {
	class PhysicsEntityCallbackFilter;
};

namespace rpg3D {
class PA_Base : public base::Object, public virtual IPhysicsActor {priv typedef base::Object super; pub dCtor(PA_Base);
	priv bool isDisposedPre = false;

	prot IWorldEntity* e;

	prot int physicsGroupsBitMap;
	prot int anyInteraction_maskBitMap;
	prot int hardImpact_maskBitMap;

	priv std::shared_ptr<rpg3D::WrappedPhysicsActor> wrapper;

	prot ArrayList<std::shared_ptr<PhysicsEntityCallbackFilter>> contactFilters{};

	// This pActor is used in a unprocessed contact_change counter.
	/// This is used for error detection at a more convenient location.
	priv int usedInChangeContactsCount = 0;

    pub explicit PA_Base(IWorldEntity* e, int physicsGroupsBitMap, int anyInteraction_maskBitMap, int hardImpact_maskBitMap);

	pub IWorldEntity* getEntity() override;

	pub int getPhysicsGroupsBitMap() final;
	pub int getHardImpact_maskBitMap() final;

	pub std::shared_ptr<rpg3D::WrappedPhysicsActor> getWrapper() final;

	pub IPhysicsEntityCallbackFilter* addEntityContactFilter(std::function<void(
		IPhysicsEntityCallbackFilter* filter, IWorldEntity* otherEntity, bool isContact)> cb_onContact) final;
	pub void removeEntityContactFilter(IPhysicsEntityCallbackFilter* localFilter) final;

	pub int& getUsedInChangeContactsCount() final;

	pub void reservedDisposePre();
	prot virtual void disposePre();
	prot void disposeMain() override;
    pub ~PA_Base() override;
};
};
