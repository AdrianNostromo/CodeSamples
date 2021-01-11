#pragma once

#include <base/view/IUIView.h>

namespace sh {
class IStatusView : virtual public IUIView {
public:
    class StatusType {
    public:
        static const int None = 0;

        static const int General_ok = 1;

        static const int ConnectToServer_inProgress = 2;
        static const int ConnectToServer_error = 3;
        static const int CheckInternet_inProgress = 4;
        static const int CheckInternet_error = 5;
    };
public:
    virtual void activateStatus(int statusType, float durationS) = 0;

    virtual void setIsInternet(bool isInternet) = 0;
    virtual void setIsConnectedToServer(bool isConnectedToServer) = 0;

    ~IStatusView() override = default;
};
};
