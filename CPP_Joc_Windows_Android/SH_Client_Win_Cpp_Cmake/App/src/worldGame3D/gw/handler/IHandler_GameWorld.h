#pragma once

#include <base/rendering/IRenderingHandler.h>
#include <string>
#include <memory>

class IAppAssets;
class IAppMetrics;
class IAppUtils;
class IApp;
namespace base {
	class GWEventDataBase;
}

class IHandler_GameWorld {
public:
    virtual IRenderingHandler* getRenderingHandler() = 0;
/*asdAA
    ConnectedDeviceInformation getDeviceInformation();
    ClientSessionInformation getSessionInformation();

    void onNetError();
    void onLogicError();
    void onClientSessionNotValid();

    void onMatchJoin_fail_serverFull();
    void onMatchDisconnected_controlTransferredReconnectOccurred();

    ILogHandler getLogger();*/
		
    virtual IAppAssets* getAppAssets() = 0;
    virtual IAppMetrics* getAppMetrics() = 0;
    virtual IAppUtils* getAppUtils() = 0;
    virtual IApp* getApp() = 0;

    virtual void loopFinished() = 0;

    virtual void onGWEvent(std::string id, std::shared_ptr<base::GWEventDataBase> data) = 0;

    virtual ~IHandler_GameWorld() {}
};
