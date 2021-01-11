#pragma once

#include <base/gh.h>
#include "HttpRequestsManagerBase.h"
#include <base/net/http/util/INativeHttpRequest.h>
#include <memory>
#include <base/list/ArrayList.h>

class IEventListener;
namespace base {
    class LoopEvent;
};

namespace base {
class HttpRequestsManagerList : public HttpRequestsManagerBase {priv typedef HttpRequestsManagerBase super;pub dCtor(HttpRequestsManagerList);
    prot ArrayList<std::shared_ptr<Request>> activeRequestsList{};

    priv static const bool IS_DEBUG = false;

    priv static const float MAX_REQUEST_LIFE_S;

    priv ArrayList<std::shared_ptr<IEventListener>> localAutoListenersList{};

    pub explicit HttpRequestsManagerList(IApp* app, base::Net::INativeHttpHandler* deviceHttpHandler);
    prot void create() override;

    pub std::shared_ptr<IRequest> startNewRequest(std::string& httpMethod, std::string& baseUrl, std::string& urlParams, IRequestListener* listener) override;

    prot void onLoopEvent_appLoop_general(IEventListener& eventListener, base::LoopEvent& event);

    priv void removeRequest(std::shared_ptr<Request> request);

    pub ~HttpRequestsManagerList() override;
};
};
