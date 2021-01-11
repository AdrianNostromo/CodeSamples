#include "ManagedNetConCreatorIpPort.h"

using namespace worldGame3D;

//asdA3;
//protected enum ComponentIpPortResult {
//    WaitingForResponse,
//    Success,
//    Fail
//}
//
//private final IComponentIpPort.Listener componentIpPortListener = new IComponentIpPort.Listener() {
//    @Override
//    public void onSuccess(final IpPort serverComponentIpPort) {
//        compIpPort_data = serverComponentIpPort;
//
//        compIpPort_result = ComponentIpPortResult.Success;
//    }
//
//    @Override
//    public void onFail() {
//        compIpPort_result = ComponentIpPortResult.Fail;
//    }
//};
//
//private ComponentIpPortResult compIpPort_result;
//private IpPort compIpPort_data;

ManagedNetConCreatorIpPort::ManagedNetConCreatorIpPort(IServerPartHandler* server, std::string& serverComponentId, IListener* listener)
    : super(server, serverComponentId, listener)
{
    //void
}

//asdA3
//protected final ComponentIpPortResult getCompIpPort_result() {
//    return compIpPort_result;
//}
//
//protected final IpPort getCompIpPort_data() {
//    return compIpPort_data;
//}
//
//protected final void fetchComponentIpPort() {
//    compIpPort_result = ComponentIpPortResult.WaitingForResponse;
//
//    server.getComponentIpPort().poolServerComponentIpPort(
//        serverComponentId,
//        componentIpPortListener
//    );
//}
//
//}

ManagedNetConCreatorIpPort::~ManagedNetConCreatorIpPort() {
    //void
}
