#include "IComponentIpPort.h"

using namespace worldGame3D;

IComponentIpPort::CbRequestListener::CbRequestListener() {
    //void
}

void IComponentIpPort::CbRequestListener::onSuccess(std::shared_ptr<IRequest> target) {
    if(cb_onSuccess) {
        cb_onSuccess(target);
    }
}

void IComponentIpPort::CbRequestListener::onFail(std::shared_ptr<IRequest> target) {
    if(cb_onFail) {
        cb_onFail(target);
    }
}

IComponentIpPort::CbRequestListener::~CbRequestListener() {
    //void
}
