#pragma once

#include <functional>
#include <memory>
#include <string>

namespace base {
namespace Net {
class INativeHttpRequest {
public:
    class ErrorCode {
    public:
        static const int None = 0;
        static const int LogicError = 1;
        static const int NetError = 2;
        static const int BadResponse = 3;
        static const int Canceled = 4;
    };
public:
    virtual void start() = 0;
    virtual void cancel() = 0;

    virtual bool getIsFinished() = 0;
    virtual int getErrorCode() = 0;
    // This is used to handle the timeout fail mode.
    // localErrorCode is "<0";
    virtual void handlerErrorOccured(int localErrorCode) = 0;

    virtual int getResponseHttpStatusCode() = 0;

    virtual std::shared_ptr<std::string> getResponseHeader() = 0;
    virtual std::shared_ptr<std::string> getResponseBody() = 0;

    virtual ~INativeHttpRequest() = default;
};
};
};
