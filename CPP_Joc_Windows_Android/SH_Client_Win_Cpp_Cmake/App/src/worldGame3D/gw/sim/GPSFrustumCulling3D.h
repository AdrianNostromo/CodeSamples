#pragma once

#include <worldGame3D/gw/entity/module/frustumcullable3d/util/IFrustumCullable3DHandler.h>
#include <worldGame3D/gw/entity/module/frustumcullable3d/FrustumCullable3DModule.h>
#include "GPSCameraFollower.h"

namespace base {
class GPSFrustumCulling3D : public GPSCameraFollower, virtual public IFrustumCullable3DHandler {priv typedef GPSCameraFollower super;pub dCtor(GPSFrustumCulling3D);
	priv static const bool IS_DEBUG_FRUSTUM_CULLING_3D;

	priv ArrayList<IFrustumCullable3DModule*> frustumCullables3DList{};

	priv ArrayList<IFrustumCullable3DModule*> invalidFrustumCullables3DList{};

	priv int visibleCullables = 0;
	priv int hiddenCullables = 0;

	priv int lastCheckedCameraUpdateCounter = -1;

	pub explicit GPSFrustumCulling3D(IGameWorld* gw);
	prot void createBMain() override;

	pub void onFrustumCullable3DInvalid(IFrustumCullable3DModule* target) override;

	priv void onGWEvent_gameLoop_frustumCulling3DUpdate(IEventListener& eventListener, base::GWEvent& event);

	priv void onEvent(IEventListener& eventListener, GWEvent& event);

	priv void onEntityAdded(IWorldEntity* entity);
	priv void onEntityRemoved_Pre(IWorldEntity* entity);

	priv void addCullable3D(IFrustumCullable3DModule* frustumCullable3D);
	priv void removeCullable3D(IFrustumCullable3DModule* frustumCullable3D);
	priv void updateCullables(ArrayList<IFrustumCullable3DModule*>* cullablesList);
	priv void logCullables();

    pub ~GPSFrustumCulling3D() override;

};
};
