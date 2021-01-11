#include "NetConsManagerCreator.h"

using namespace worldGame3D;

//asdA3;
//private ArrayList<ManagedNetConCreator> netConCreatorsList = new ArrayList<ManagedNetConCreator>();
//
//private final IManagedNetConsCreator.Listener conCreatorListener = new IManagedNetConsCreator.Listener() {
//    @Override
//    public void onSuccess(IManagedNetConsCreator target, GeneralTcpConnection netCon) {
//        onConCreateSuccess(target, netCon);
//    }
//
//    @Override
//    public void onFailed(IManagedNetConsCreator target, IManagedNetConsCreator.FailCode failCode) {
//        onConCreateFailed(target, failCode);
//    }
//};

NetConsManagerCreator::NetConsManagerCreator(IServerPartHandler* server)
    : super(server)
{
    //void
}

//asdA3;
//@Override
//public void newValidatedServerCon(final String serverComponentType, final Listener listener) {
//    ManagedNetConCreator managedCreator = new ManagedNetConCreator(
//        server,
//        serverComponentType,
//        conCreatorListener
//    );
//    managedCreator.setUserData(listener);
//
//    addComponent(managedCreator);
//}
//
//protected void onConCreateSuccess(IManagedNetConsCreator target, GeneralTcpConnection netCon) {
//    Listener listener = target.getUserData();
//
//    listener.onSuccess(netCon);
//}
//
//protected void onConCreateFailed(IManagedNetConsCreator target, IManagedNetConsCreator.FailCode failCode) {
//    Listener listener = target.getUserData();
//
//    FailCode localisedFailCode;
//
//    switch (failCode) {
//        case NetError:
//        case AccountConnect:
//        case DurationLimitExceeded:
//        case ComponentIpPortGetFailed:
//        case SocketCreation:
//        case Validation:
//            localisedFailCode = FailCode.NetError;
//            break;
//        default:
//            throw new LogicError("Err.1.");
//    }
//
//    listener.onFailed(localisedFailCode);
//}

NetConsManagerCreator::~NetConsManagerCreator() {
    //void
}
