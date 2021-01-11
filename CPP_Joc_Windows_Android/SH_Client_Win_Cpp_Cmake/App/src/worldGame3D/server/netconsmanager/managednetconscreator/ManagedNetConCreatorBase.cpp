#include "ManagedNetConCreatorBase.h"

using namespace worldGame3D;

ManagedNetConCreatorBase::ManagedNetConCreatorBase(IServerPartHandler* server, std::string& serverComponentId, IListener* listener)
    : super(),
      server(server), serverComponentId(serverComponentId), listener(listener)
{
    //void
}

//asdA3;
//@Override
//public <T extends Object> T getUserData() {
//    return (T)userData;
//}
//
//@Override
//public void setUserData(final Object userData) {
//    this.userData = userData;
//}
//
//protected final void onSuccess(final GeneralTcpConnection netCon) {
//    if(getIsDisposed()) {
//        return ;
//    }
//
//    listener.onSuccess(this, netCon);
//}
//
//protected final void onFailed(FailCode failCode) {
//    if(getIsDisposed()) {
//        return ;
//    }
//
//    listener.onFailed(this, failCode);
//}

ManagedNetConCreatorBase::~ManagedNetConCreatorBase() {
    //void
}
