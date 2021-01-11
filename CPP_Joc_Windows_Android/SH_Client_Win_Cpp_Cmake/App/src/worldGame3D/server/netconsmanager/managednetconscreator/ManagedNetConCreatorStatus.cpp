#include "ManagedNetConCreatorStatus.h"

using namespace worldGame3D;

//asdA3;
//private enum Status {
//    Uninitialised,
//    Failed,
//    AwaitingDisposal,
//
//    GetServerComponentIpPort,
//    AccountConnection,
//    CreateNetConSocket,
//    ValidateNetCon,
//    Success
//}
//
//private Status status = Status.Uninitialised;
//private boolean isStatusInit = false;
//
//private FailCode failCode = null;
//
//private float totalDurationS = 0f;

ManagedNetConCreatorStatus::ManagedNetConCreatorStatus(IServerPartHandler* server, std::string& serverComponentId, IListener* listener)
    : super(server, serverComponentId, listener)
{
    //void
}

//asdA3;
////asdAAA; Use this.
//public final boolean isStateAwaitingDisposal() {
//    if(status == Status.AwaitingDisposal) {
//        return true;
//    }
//
//    return false;
//}
//
//private final void setStatus(final Status status) {
//    this.status = status;
//    isStatusInit = false;
//}
//
//@Override
//public void appLoop_General(float deltaS, int deltaMS) {
//    super.appLoop_General(deltaS, deltaMS);
//
//    totalDurationS += deltaS;
//    if (status != Status.AwaitingDisposal && status != Status.Failed && totalDurationS >= 5f) {
//        failCode = FailCode.DurationLimitExceeded;
//        setStatus(Status.Failed);
//    }
//
//    switch (status) {
//        case Uninitialised:
//            if(!isStatusInit) {
//                isStatusInit = true;
//
//                setStatus(Status.AccountConnection);
//            }
//
//            //void
//
//            break;
//        case AccountConnection:
//            if(!isStatusInit) {
//                isStatusInit = true;
//
//                if(!server.isSessionConnectedOrConnecting()) {
//                    server.startAccountSessionIfNeeded();
//                }
//            }
//
//            if(!server.isSessionConnectedOrConnecting()) {
//                // Connect failed.
//
//                failCode = FailCode.AccountConnect;
//                setStatus(Status.Failed);
//            }else if(server.isSessionConnected()) {
//                setStatus(Status.GetServerComponentIpPort);
//            }
//
//            break;
//        case GetServerComponentIpPort:
//            if(!isStatusInit) {
//                isStatusInit = true;
//
//                fetchComponentIpPort();
//            }
//
//            switch (getCompIpPort_result()) {
//                case WaitingForResponse:
//                    //void
//                    break;
//                case Success:
//                    setStatus(Status.CreateNetConSocket);
//                    break;
//                case Fail:
//                    failCode = FailCode.ComponentIpPortGetFailed;
//                    setStatus(Status.Failed);
//                    break;
//                default:
//                    throw new LogicError("Err.1.");
//            }
//
//            //void
//
//            break;
//        case CreateNetConSocket:
//            if(!isStatusInit) {
//                isStatusInit = true;
//
//                // Create a binary connection to the server and pool the data using the active session.
//                createNewSocket(getCompIpPort_data().ip, getCompIpPort_data().port);
//            }
//
//            switch (getSocketCreate_statusCode()) {
//                case WaitingForCreation:
//                    //void
//                    break;
//                case Success:
//                    setStatus(Status.ValidateNetCon);
//                    break;
//                case Fail:
//                    failCode = FailCode.SocketCreation;
//                    setStatus(Status.Failed);
//                    break;
//                default:
//                    throw new LogicError("Err.1.");
//            }
//
//            break;
//        case ValidateNetCon:
//            if(!isStatusInit) {
//                isStatusInit = true;
//
//                GeneralTcpConnection con = new GeneralTcpConnection(getSocketCreate_socket());
//
//                validateClientSession(
//                    con,
//                    server.getDeviceInformation().deviceId,
//
//                    server.getSessionInformation().sessionId,
//                    server.getSessionInformation().sessionKey
//                );
//            }
//
//            switch (getValidator_statusCode()) {
//                case WaitingForValidation:
//                    //void
//                    break;
//                case Success:
//                    setStatus(Status.Success);
//                    break;
//                case Fail:
//                    failCode = FailCode.Validation;
//                    setStatus(Status.Failed);
//                    break;
//                default:
//                    throw new LogicError("Err.1.");
//            }
//
//            break;
//        case Success:
//            if(!isStatusInit) {
//                isStatusInit = true;
//
//                // Dispatch success event.
//                onSuccess(getValidator_validatedNetCon());
//
//                setStatus(Status.AwaitingDisposal);
//            }
//
//            //void
//
//            break;
//        case Failed:
//            if(!isStatusInit) {
//                isStatusInit = true;
//
//                // Dispatch success event.
//                onFailed(failCode);
//
//                setStatus(Status.AwaitingDisposal);
//            }
//
//            //void
//
//            break;
//        case AwaitingDisposal:
//            if(!isStatusInit) {
//                isStatusInit = true;
//
//                //void
//            }
//
//            //void
//
//            break;
//        default:
//            throw new LogicError("Err.1.");
//
//    }
//}

ManagedNetConCreatorStatus::~ManagedNetConCreatorStatus() {
    //void
}
