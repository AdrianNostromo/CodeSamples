#pragma once

#include <functional>
#include <memory>
#include <base/buffer/StreamBuffer.h>

namespace base {
namespace Net {
class INativeSocket {
public:
    virtual void setCb_onData(std::function<void(
        base::Net::INativeSocket* target,
        base::Buffer& data)> cb_onData) = 0;
    virtual void setCb_onLogicError(std::function<void(
        base::Net::INativeSocket* target)> cb_onLogicError) = 0;
    virtual void setCb_onNetError(std::function<void(
        base::Net::INativeSocket* target)> cb_onNetError) = 0;

    virtual void setIsInputEnabled(bool isInputEnabled) = 0;

    virtual void write(base::Buffer data) = 0;

    virtual void reservedDisposeMain() = 0;
    virtual ~INativeSocket() = default;
};
};
};
