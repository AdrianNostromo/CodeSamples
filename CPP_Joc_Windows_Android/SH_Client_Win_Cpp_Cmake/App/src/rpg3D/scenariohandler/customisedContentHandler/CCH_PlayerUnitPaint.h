#pragma once

#include <base/gh.h>
#include "CCH_Base.h"

class IEventListener;
namespace base {
    class AsyncFHEvent;
    class LoopEvent;
}

namespace rpg3D {
class CCH_PlayerUnitPaint : public CCH_Base {priv typedef CCH_Base super;pub dCtor(CCH_PlayerUnitPaint);
    priv std::shared_ptr<EntityMaterialsAndVoxelsGrid> playerUnitMaterialsAndVoxelsGrid = nullptr;

    priv ArrayList<std::shared_ptr<IEventListener>> fhListenersList{};

    priv bool isLoadInProgress = false;
    priv std::shared_ptr<std::string> data_waitingFileRead = nullptr;

    pub explicit CCH_PlayerUnitPaint(IApp* app);

    pub std::shared_ptr<EntityMaterialsAndVoxelsGrid> getPlayerUnitMaterialsAndVoxelsGrid() final;
    pub virtual void updatePlayerUnitMaterialsAndVoxelsGrid(VoxlesGrid3DGeometry* voxels3DGeometry, bool dispatchGWUpdateEvent) final;

    priv void onAsyncFHEvent(IEventListener& eventListener, base::AsyncFHEvent& event);
    priv void onLoopEvent_processReadDataLoop(IEventListener& eventListener, base::LoopEvent& event);

    pub ~CCH_PlayerUnitPaint() override;
};
};
