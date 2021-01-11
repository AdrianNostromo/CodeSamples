#include "ManagedNetConCreatorSocket.h"

using namespace worldGame3D;

//asdA3;
//protected enum SocketCreateStatusCode {
//    WaitingForCreation,
//    Success,
//    Fail
//}
//
//private SocketCreator socketCreator = null;
//
//private SocketCreateStatusCode socketCreate_statusCode;
//private Socket socketCreate_socket = null;

ManagedNetConCreatorSocket::ManagedNetConCreatorSocket(IServerPartHandler* server, std::string& serverComponentId, IListener* listener)
    : super(server, serverComponentId, listener)
{
    //void
}

//asdA3;
//protected final SocketCreateStatusCode getSocketCreate_statusCode() {
//    return socketCreate_statusCode;
//}
//
//protected final Socket getSocketCreate_socket() {
//    return socketCreate_socket;
//}
//
//protected final void createNewSocket(String ip, int port) {
//    if(socketCreator != null) {
//        throw new LogicError("ClientAccountCreatorNetConnector.createNewSocket.1.");
//    }
//
//    socketCreator = new SocketCreator().connect(ip, port);
//
//    socketCreate_statusCode = SocketCreateStatusCode.WaitingForCreation;
//}
//
//@Override
//public void appLoop_General(float deltaS, int deltaMS) {
//    if(socketCreator != null) {
//        if(socketCreator.getResult() != SocketCreator.RESULT.IN_PROGRESS) {
//            if(socketCreator.getResult() == SocketCreator.RESULT.SUCCESS) {
//                onSocketCreate_result(SocketCreateStatusCode.Success, socketCreator.extractSocket());
//            }else if(socketCreator.getResult() == SocketCreator.RESULT.CANCELLED) {
//                throw new LogicError("Err.1.");
//            }else if(socketCreator.getResult() == SocketCreator.RESULT.LOGIC_ERROR) {
//                throw new LogicError("Err.1.");
//            }else if(socketCreator.getResult() == SocketCreator.RESULT.NET_ERROR) {
//                onSocketCreate_result(SocketCreateStatusCode.Fail, null);
//            }else {
//                throw new LogicError("Err.1.");
//            }
//
//            socketCreator = null;
//        }
//    }
//}
//
//private void onSocketCreate_result(final SocketCreateStatusCode statusCode, final Socket socket) {
//    socketCreate_statusCode = statusCode;
//    socketCreate_socket = socket;
//}
//
//@Override
//public void dispose() {
//    if(socketCreator != null) {
//        socketCreator.cancel();
//
//        socketCreator = null;
//    }
//
//    super.dispose();
//}

ManagedNetConCreatorSocket::~ManagedNetConCreatorSocket() {
    //void
}
