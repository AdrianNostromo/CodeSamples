#pragma once

#include <base/gh.h>
#include "IAppUtilitiesSoft.h"
#include <base/app/AppBase.h>

namespace _calculator {
	class CalculatorView;
};

namespace utilitiesSoft {
class AppUtilitiesSoft : public AppBase, virtual public IAppUtilitiesSoft {priv typedef AppBase super;pub dCtor(AppUtilitiesSoft);
    pub explicit AppUtilitiesSoft(
        int deviceType,
        INativeInput* nativeInputConnection,
        base::IProfileStorage* profileStorage,
        base::Net::INativeHttpHandler* nativeHttpHandler,
        base::Net::INativeSocketsHandler* nativeSocketsHandler,
		base::IFileSystem* fileSystem, IGL* gl,
        base::INativeConsole* nativeConsole,
        base::audio::INativeAudio* nativeAudio,
        base::ThreadsManager* threadsManager);

    pub ~AppUtilitiesSoft() override;
};
};
