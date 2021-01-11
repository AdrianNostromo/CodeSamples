#include "IHttpRequestsManager.h"

using namespace base;

IHttpRequestsManager::CbRequestListener::CbRequestListener() {
    //void
}

void IHttpRequestsManager::CbRequestListener::onSuccess(std::shared_ptr<IRequest> target) {
    if(cb_onSuccess) {
        cb_onSuccess(target);
    }
}

void IHttpRequestsManager::CbRequestListener::onFail(std::shared_ptr<IRequest> target) {
    if(cb_onFail) {
        cb_onFail(target);
    }
}

IHttpRequestsManager::CbRequestListener::~CbRequestListener() {
    //void
}
