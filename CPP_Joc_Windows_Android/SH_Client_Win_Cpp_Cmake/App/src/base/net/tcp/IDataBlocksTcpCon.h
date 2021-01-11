#pragma once

#include "IDataBlocksTcpCon.h"
#include <base/net/buffer/NetMsgsBlock.h>
#include <memory>
#include <base/buffer/StreamBuffer.h>

namespace base {
namespace Net {
class IDataBlocksTcpCon {
public:
    virtual std::shared_ptr<base::StreamBuffer> popNextMsg() = 0;

    virtual void sendBlock(std::shared_ptr<NetMsgsBlock> blockDataNetBuffer) = 0;

    virtual ~IDataBlocksTcpCon() = default;
};
};
};
