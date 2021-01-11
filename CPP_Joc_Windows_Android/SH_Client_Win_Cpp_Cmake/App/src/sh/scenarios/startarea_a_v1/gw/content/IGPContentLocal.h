#pragma once

#include <worldGame3D/gw/content/IGPContent.h>
#include <worldGame3D/gw/event/GWEvent.h>

namespace startarea_a_v1 {
class IGPContentLocal : virtual public base::IGPContent {
public:
	static base::GWEvent* GWEvent_onActiveStarshipChanged;

public:
    virtual IWorldEntity* getActiveStarship() = 0;

    ~IGPContentLocal() override = default;
};
};
