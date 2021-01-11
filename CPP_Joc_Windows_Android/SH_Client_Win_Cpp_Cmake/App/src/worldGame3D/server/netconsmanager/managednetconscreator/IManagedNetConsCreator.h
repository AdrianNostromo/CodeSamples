#pragma once

#include <base/net/tcp/GeneralTcpConnection.h>

namespace worldGame3D {
class IManagedNetConsCreator {
public:
    enum FailCode {
        NetError,
        DurationLimitExceeded,
        AccountConnect,
        ComponentIpPortGetFailed,
        SocketCreation,
        Validation
    };
    class IListener {
    public:
        void onSuccess(IManagedNetConsCreator* target, base::Net::GeneralTcpConnection* netCon);
        void onFailed(IManagedNetConsCreator* target, FailCode* failCode);
    };
public:
    //void

    virtual ~IManagedNetConsCreator() = default;
};
};

//asdA3;
//public interface IManagedNetConsCreator {
//
//    void setUserData(Object userData);
//    <T extends Object> T getUserData();
//
//}
