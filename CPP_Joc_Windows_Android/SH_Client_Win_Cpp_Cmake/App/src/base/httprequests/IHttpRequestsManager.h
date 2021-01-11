#pragma once

#include <memory>
#include <string>
#include <functional>

namespace base {
class IHttpRequestsManager {
public:
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
        virtual IRequestListener* getListener() = 0;

        virtual bool getIsCanceled() = 0;
        virtual void cancel() = 0;

        virtual bool getIsFinished() = 0;
        virtual int getErrorCode() = 0;
        virtual int getResponseHttpStatusCode() = 0;
        virtual std::shared_ptr<std::string> getResponseBody() = 0;

        virtual ~IRequest() = default;
    };
public:
    virtual std::shared_ptr<IRequest> startNewRequest(std::string& httpMethod, std::string& baseUrl, std::string& urlParams, IRequestListener* listener) = 0;

	virtual ~IHttpRequestsManager() = default;
};
};
