#pragma once

#include <base/gh.h>
#include <base/net/http/INativeHttpHandler.h>
#include <base/object/ManagedObject.h>

namespace desktopWindows {
namespace Net {
class NativeHttpHandler : public base::ManagedObject, virtual public base::Net::INativeHttpHandler {priv typedef base::ManagedObject super;pub dCtor(NativeHttpHandler);
    prot IApp* app = nullptr;

    pub explicit NativeHttpHandler();

    pub void init(IApp* app) final;

    pub std::shared_ptr<base::Net::INativeHttpRequest> createRequest(std::string& httpMethod, std::string& baseUrl, std::string& urlParams) override;

    pub ~NativeHttpHandler() override;
};
};
};
