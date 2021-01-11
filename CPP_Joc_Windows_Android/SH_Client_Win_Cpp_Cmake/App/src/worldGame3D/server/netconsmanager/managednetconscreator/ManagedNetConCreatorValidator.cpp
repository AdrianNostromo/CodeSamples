#include "ManagedNetConCreatorValidator.h"
#include <base/MM.h>
#include <base/memory/IMemoryManager.h>

using namespace worldGame3D;

//asdA3;
//protected enum ValidatorStatusCode {
//    WaitingForValidation,
//    Success,
//    Fail
//}
//
//private RegisteredNetClientSessionValidator.Listener validatorListener = newt RegisteredNetClientSessionValidator.Listener() {
//    @Override
//    public void onNetClientSessionValidation_ok(RegisteredNetClientSessionValidatorBase target) {
//        GeneralTcpConnection con = clientSessionValidator.extractNetCon();
//
//        validator_validatedNetCon = con;
//        validator_statusCode = ValidatorStatusCode.Success;
//    }
//
//    @Override
//    public void onNetClientSessionValidation_notValid(RegisteredNetClientSessionValidatorBase target) {
//        disposeClientSessionValidator();
//
//        validator_statusCode = ValidatorStatusCode.Fail;
//    }
//
//    @Override
//    public void onNetClientSessionValidation_netError(RegisteredNetClientSessionValidatorBase target) {
//        disposeClientSessionValidator();
//
//        validator_statusCode = ValidatorStatusCode.Fail;
//    }
//
//    @Override
//    public void onNetClientSessionValidation_logicError(RegisteredNetClientSessionValidatorBase target) {
//        disposeClientSessionValidator();
//
//        throw new LogicError("Err.1.");
//    }
//};
//
//private RegisteredNetClientSessionValidator clientSessionValidator = null;
//
//private ValidatorStatusCode validator_statusCode;
//private GeneralTcpConnection validator_validatedNetCon;

ManagedNetConCreatorValidator::ManagedNetConCreatorValidator(IServerPartHandler* server, std::string& serverComponentId, IListener* listener)
    : super(server, serverComponentId, listener)
{
    //void
}

//asdA3;
//protected final ValidatorStatusCode getValidator_statusCode() {
//    return validator_statusCode;
//}
//
//protected final GeneralTcpConnection getValidator_validatedNetCon() {
//    return validator_validatedNetCon;
//}
//
//protected final void validateClientSession(GeneralTcpConnection con, final int deviceId, final int sessionId, final String sessionKey) {
//    if(clientSessionValidator != null) {
//        throw new LogicError("MatchFinderNetClientSessionValidator.validateClientSession.1.");
//    }
//
//    validator_statusCode = ValidatorStatusCode.WaitingForValidation;
//
//    if(server.getDeviceInformation() == null || server.getSessionInformation() == null) {
//        validator_statusCode = ValidatorStatusCode.Fail;
//    }else {
//        addComponent(clientSessionValidator = newt RegisteredNetClientSessionValidator(
//            validatorListener,
//            new SessionValidationData(
//                deviceId,
//
//                sessionId,
//                sessionKey
//            ),
//            con,
//            server.getLogger()
//        ));
//    }
//}
//
//protected final void disposeClientSessionValidator() {
//    if(clientSessionValidator != null) {
//        removeComponent(clientSessionValidator, true/*doDispose*/);
//        clientSessionValidator = null;
//    }
//}
//
//@Override
//public void dispose() {
//    if(clientSessionValidator != null) {
//        disposeClientSessionValidator();
//    }
//
//    super.dispose();
//}

ManagedNetConCreatorValidator::~ManagedNetConCreatorValidator() {
    //void
}
