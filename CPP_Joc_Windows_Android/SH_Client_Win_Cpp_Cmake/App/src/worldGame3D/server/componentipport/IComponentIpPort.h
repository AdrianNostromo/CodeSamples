#pragma once

#include <string>
#include <base/net/util/IpPort.h>
#include <memory>
#include <functional>

namespace worldGame3D {
class IComponentIpPort {
public:
    class FailCodes {
    public:
        static const int None = 0;
        static const int LogicError = 1;
        static const int DataPoolFailed = 2;
        static const int Canceled = 3;
    };

    class IRequest;
    class IRequestListener {
    public:
        virtual void onSuccess(std::shared_ptr<IRequest> target) = 0;
        virtual void onFail(std::shared_ptr<IRequest> target) = 0;

        virtual ~IRequestListener() = default;
    };
    class CbRequestListener : virtual public IRequestListener {
    public:
        std::function<void(std::shared_ptr<IRequest> target)> cb_onSuccess;
        std::function<void(std::shared_ptr<IRequest> target)> cb_onFail;

    public:
        CbRequestListener(CbRequestListener const&) = delete;
        CbRequestListener(CbRequestListener &&) = default;
        CbRequestListener& operator=(CbRequestListener const&) = delete;
        CbRequestListener& operator=(CbRequestListener &&) = default;

        explicit CbRequestListener();

        void onSuccess(std::shared_ptr<IRequest> target) override;
        void onFail(std::shared_ptr<IRequest> target) override;

        ~CbRequestListener() override;
    };
    class IRequest {
    public:
        virtual void setListener(IRequestListener* listener) = 0;

        virtual void cancel() = 0;

        virtual bool getIsFinished() = 0;
        virtual int getFailCode() = 0;

        virtual std::string& getServerComponentId() = 0;
        virtual std::shared_ptr<base::Net::IpPort> getResponseIpPort() = 0;

        virtual ~IRequest() = default;
    };
public:
    virtual std::shared_ptr<IRequest> poolServerComponentIpPort(
        std::string& serverComponentId,
        IRequestListener* listener
    ) = 0;

    virtual ~IComponentIpPort() = default;
};
};
