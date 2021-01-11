#pragma once

#include "INativeSocket.h"
#include <memory>

namespace base {
namespace Net {
class INativeSocketCreator {
public:
    class ErrorCode {
    public:
        static const int None = 0;
        static const int LogicError = 1;
        static const int NetError = 2;
        static const int Canceled = 3;
    };
public:
    virtual void start() = 0;
    virtual void cancel() = 0;

    virtual bool getIsFinished() = 0;
    virtual int getErrorCode() = 0;

    virtual std::shared_ptr<base::Net::INativeSocket> extractNativeSocket() = 0;

    virtual ~INativeSocketCreator() = default;
};
};
};
