#pragma once

#include <base/gh.h>
#include "NA_Base.h"

namespace base {
    class AppEvent;
    class IManagedThread;
}

namespace base {
namespace audio {
class NA_StreamHandler : public NA_Base {priv typedef NA_Base super;pub dCtor(NA_StreamHandler);
    prot class Status {
    public:
        static const int OFFLINE = 1;
        static const int STARTING = 2;
        static const int CLOSING = 3;
        static const int ONLINE = 4;
        static const int _ERROR = 5;
    };

    priv int status = Status::OFFLINE;
    priv bool isStreamError = false;

    priv bool cache_isAppActive = false;

    priv base::IManagedThread* managedThread = nullptr;

    pub explicit NA_StreamHandler();
    prot void createMain() override;
    pub void init(IApp* app) override;

	prot int getStatus();

    priv void onAppEvent(IEventListener& eventListener, base::AppEvent& event);

    prot virtual bool startOrRestartStream() = 0;
    prot virtual bool stopStream() = 0;
    prot void onStreamError();

    pub virtual unsigned int tickStream(base::IManagedThread* managedThread);

    prot void disposeMain() override;
    pub ~NA_StreamHandler() override;
};
};
};
