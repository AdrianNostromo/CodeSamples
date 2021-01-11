#pragma once

#include <base/gh.h>
#include "NA_StreamHandler.h"

namespace base {
namespace audio {
class NativeAudio : public NA_StreamHandler {priv typedef NA_StreamHandler super;pub dCtor(NativeAudio);
    pub explicit NativeAudio();

    pub ~NativeAudio() override;
};
};
};
