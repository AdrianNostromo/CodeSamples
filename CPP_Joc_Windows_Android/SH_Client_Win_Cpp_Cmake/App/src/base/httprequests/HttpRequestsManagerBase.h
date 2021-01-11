#pragma once

#include <base/gh.h>
#include <base/component/ComponentsHandler.h>
#include "IHttpRequestsManager.h"
#include <memory>
#include <base/net/http/util/INativeHttpRequest.h>
#include <base/net/http/INativeHttpHandler.h>
#include <base/component/IComponent_AppComponent.h>

namespace base {
class HttpRequestsManagerBase : public base::ManagedObject, virtual public IHttpRequestsManager, public virtual IComponent_AppComponent {priv typedef base::ManagedObject super;pub dCtor(HttpRequestsManagerBase);
    pub class Request : virtual public IRequest {pub dCtor(Request);
        pub float requestLifeS = 0.0f;

        pub std::shared_ptr<base::Net::INativeHttpRequest> nativeRequest;

        priv IRequestListener* listener;

        priv bool isCanceled = false;

        pub explicit Request(std::shared_ptr<base::Net::INativeHttpRequest> nativeRequest, IRequestListener* listener);

        pub void start();

        pub IRequestListener* getListener() override;

        pub bool getIsCanceled() override;
        pub void cancel() override;

        pub bool getIsFinished() override;
        pub int getErrorCode() override;
        pub int getResponseHttpStatusCode() override;
        pub std::shared_ptr<std::string> getResponseBody() override;

        pub ~Request() override;
    };

    prot IApp* app;
    prot base::Net::INativeHttpHandler* deviceHttpHandler;

    pub explicit HttpRequestsManagerBase(IApp* app, base::Net::INativeHttpHandler* deviceHttpHandler);

    pub ~HttpRequestsManagerBase() override;
};
};
