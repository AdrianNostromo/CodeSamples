#pragma once

#include <base/gh.h>
#include <base/component/ComponentsHandler.h>
#include "IAudio.h"
#include "native/INativeAudio.h"
#include <base/component/IComponent_AppComponent.h>

class IApp;

namespace base {
namespace audio {
class A_Base : public base::ManagedObject, virtual public IAudio, public virtual IComponent_AppComponent {priv typedef base::ManagedObject super;pub dCtor(A_Base);
    prot IApp* app;
    prot base::audio::INativeAudio* nativeAudio;

    pub explicit A_Base(IApp* app, base::audio::INativeAudio* nativeAudio);

    pub void loopTick();

    pub ~A_Base() override;
};
};
};
