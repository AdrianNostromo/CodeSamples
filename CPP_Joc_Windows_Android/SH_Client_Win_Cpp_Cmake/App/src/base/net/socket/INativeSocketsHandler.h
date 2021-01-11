#pragma once

#include <memory>
#include <base/net/socket/util/INativeSocketCreator.h>
#include <base/object/IManagedObject.h>

class IApp;

namespace base {
namespace Net {
class INativeSocketsHandler : virtual public base::IManagedObject {
public:
    virtual void init(IApp* app) = 0;
    
    virtual std::shared_ptr<base::Net::INativeSocketCreator> newNativeSocketCreator(std::string& ip, int port) = 0;

    virtual ~INativeSocketsHandler() = default;
};
};
};
