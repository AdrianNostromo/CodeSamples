#pragma once

#include <base/component/IComponent_ServerComponent.h>
#include <worldGame3D/server/util/ServerPart.h>
#include "IComponentIpPort.h"
#include <worldGame3D/server/IServerPartHandler.h>
#include <base/net/http/util/INativeHttpRequest.h>
#include <base/httprequests/IHttpRequestsManager.h>

namespace worldGame3D {
class ComponentIpPortBase : public ServerPart, virtual public IComponentIpPort, public virtual IComponent_ServerComponent {
private: typedef ServerPart super;
public:
    class Request : virtual public IRequest, virtual public base::IHttpRequestsManager::IRequestListener {
    public:
        class IHandler {
        public:
            virtual void onRequestHttpFinished(Request* target) = 0;
            virtual void onRequestCancelRequested(Request* target) = 0;

            virtual ~IHandler() = default;
        };
    public:
        std::shared_ptr<base::IHttpRequestsManager::IRequest> httpRequest = nullptr;

        std::shared_ptr<base::Net::IpPort> responseIpPort;

    private:
        std::string serverComponentId;
        IComponentIpPort::IRequestListener* listener;

        IHandler* handler = nullptr;

        bool isRequested_cancel = false;

        bool isFinished = false;
        int failCode = 0;

    public:
        Request(Request const&) = delete;
        Request(Request &&) = default;
        Request& operator=(Request const&) = delete;
        Request& operator=(Request &&) = default;

        explicit Request(std::string& serverComponentId, IComponentIpPort::IRequestListener* listener);

        IComponentIpPort::IRequestListener* getListener();
        void setListener(IComponentIpPort::IRequestListener* listener) override;

        void setHandler(IHandler* handler);

        void setIsFinished(bool isFinished) { this->isFinished = isFinished; };
        bool getIsFinished() override { return isFinished; };

        int getFailCode() override { return failCode; };
        void setFailCode(int failCode) { this->failCode = failCode; };

        void cancel() override;

        std::string& getServerComponentId() override;
        std::shared_ptr<base::Net::IpPort> getResponseIpPort() override { return responseIpPort; };

        // These are for the http request;
        void onSuccess(std::shared_ptr<base::IHttpRequestsManager::IRequest> target) override;
        void onFail(std::shared_ptr<base::IHttpRequestsManager::IRequest> target) override;

        ~Request() override;
    };
public:
    ComponentIpPortBase(ComponentIpPortBase const&) = delete;
    ComponentIpPortBase(ComponentIpPortBase &&) = default;
    ComponentIpPortBase& operator=(ComponentIpPortBase const&) = delete;
    ComponentIpPortBase& operator=(ComponentIpPortBase &&) = default;

    explicit ComponentIpPortBase(IServerPartHandler* handler, IApp* app);

    virtual ~ComponentIpPortBase();
};
};
