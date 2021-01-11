#pragma once

#include "IDataBlocksTcpCon.h"
#include "ICryptoTcpCon.h"

namespace base {
namespace Net {
class IGeneralTcpCon : virtual public ICryptoTcpCon {
public:
    //void

    virtual ~IGeneralTcpCon() = default;
};
};
};
