#pragma once

#include <functional>
#include <memory>
#include <base/buffer/StreamBuffer.h>
#include <base/list/ArrayList.h>

namespace base {
namespace Net {
class ISocket {
public:
    virtual void write(base::Buffer data) = 0;

    virtual bool getIsAnyError() = 0;
    virtual bool getIsLogicError() = 0;
    virtual bool getIsNetError() = 0;

    virtual ArrayList<std::shared_ptr<base::StreamBuffer>>* getReceivedNetBuffersList() = 0;
    virtual void lockReceiveNetBuffersList() = 0;
    virtual void unlockReceiveNetBuffersList() = 0;

    virtual void reservedDisposeMain() = 0;

    virtual ~ISocket() = default;
};
};
};
