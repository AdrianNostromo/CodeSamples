#pragma once

#include <base/gh.h>
#include <memory>
#include <string>
#include <base/net/http/util/INativeHttpRequest.h>
#include <base/object/IManagedObject.h>

class IApp;

namespace base {
namespace Net {
class INativeHttpHandler : virtual public base::IManagedObject {
    pub virtual void init(IApp* app) = 0;

    pub virtual std::shared_ptr<base::Net::INativeHttpRequest> createRequest(std::string& httpMethod, std::string& baseUrl, std::string& urlParams) = 0;

    pub virtual ~INativeHttpHandler() = default;

};
};
};
