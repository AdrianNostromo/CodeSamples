#pragma once

#include <base/gh.h>
#include <base/net/socket/INativeSocketsHandler.h>
#include <base/object/ManagedObject.h>

namespace desktopWindows {
namespace Net {
class NativeSocketsHandler : public base::ManagedObject, virtual public base::Net::INativeSocketsHandler {priv typedef base::ManagedObject super;pub dCtor(NativeSocketsHandler);
    prot IApp* app = nullptr;

    pub explicit NativeSocketsHandler();
    pub void init(IApp* app) final;

    pub std::shared_ptr<base::Net::INativeSocketCreator> newNativeSocketCreator(std::string& ip, int port) override;

    pub ~NativeSocketsHandler() override;
};
};
};
