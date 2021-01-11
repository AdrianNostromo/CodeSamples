#pragma once

#include <memory>

namespace base {
class IServer {
public:
    virtual void reservedCreate() = 0;

    virtual void reservedDisposeMain() = 0;

    virtual ~IServer() = default;
};
};
