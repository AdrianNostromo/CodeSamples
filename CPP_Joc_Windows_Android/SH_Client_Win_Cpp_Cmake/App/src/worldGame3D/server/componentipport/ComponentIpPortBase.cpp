#include "ComponentIpPortBase.h"

using namespace worldGame3D;

//Request - START
ComponentIpPortBase::Request::Request(std::string& serverComponentId, IComponentIpPort::IRequestListener* listener)
    : serverComponentId(serverComponentId), listener(listener)
{
    //void
}

IComponentIpPort::IRequestListener* ComponentIpPortBase::Request::getListener() {
    return listener;
}

void ComponentIpPortBase::Request::setListener(IComponentIpPort::IRequestListener* listener) {
    this->listener = listener;
}

void ComponentIpPortBase::Request::setHandler(IHandler* handler) {
    this->handler = handler;
}

void ComponentIpPortBase::Request::cancel() {
    if(!isRequested_cancel) {
        isRequested_cancel = true;

        if(failCode == FailCodes::None) {
            failCode = FailCodes::Canceled;
        }

        if(handler != nullptr) {
            handler->onRequestCancelRequested(this);
        }

        if(httpRequest != nullptr) {
            httpRequest->cancel();
        }
    }
}

std::string& ComponentIpPortBase::Request::getServerComponentId() {
    return serverComponentId;
}

void ComponentIpPortBase::Request::onSuccess(std::shared_ptr<base::IHttpRequestsManager::IRequest> target) {
    if(handler != nullptr) {
        handler->onRequestHttpFinished(this);
    }
}

void ComponentIpPortBase::Request::onFail(std::shared_ptr<base::IHttpRequestsManager::IRequest> target) {
    if(failCode == 0) {
        failCode = 2;
    }
    if(handler != nullptr) {
        handler->onRequestHttpFinished(this);
    }
}

ComponentIpPortBase::Request::~Request() {
    //void
}
//Request - END

ComponentIpPortBase::ComponentIpPortBase(IServerPartHandler* handler, IApp* app)
    : super(handler, app)
{
    //void
}

ComponentIpPortBase::~ComponentIpPortBase() {
    //void
}
