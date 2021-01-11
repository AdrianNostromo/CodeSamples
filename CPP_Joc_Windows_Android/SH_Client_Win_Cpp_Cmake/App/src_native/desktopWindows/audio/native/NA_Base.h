#pragma once

#include <base/gh.h>
#include <base/audio/native/NativeAudio.h>

namespace desktopWindows {
namespace audio {
class NA_Base : public base::audio::NativeAudio {priv typedef base::audio::NativeAudio super;pub dCtor(NA_Base);
    pub explicit NA_Base();

    pub ~NA_Base() override;
};
};
};
