#pragma once

namespace worldGame3D {
class INetConsManager {
public:
    //void

    virtual ~INetConsManager() = default;
};
};

//asdA3;
//public interface INetConsManager {
//
//    enum FailCode {
//        NetError
//    }
//
//    interface Listener {
//        void onSuccess(final GeneralTcpConnection netCon);
//        void onFailed(final FailCode failCode);
//    }
//
//    void newValidatedServerCon(
//        final String serverComponentType,
//        final Listener listener
//    );
//
//}
