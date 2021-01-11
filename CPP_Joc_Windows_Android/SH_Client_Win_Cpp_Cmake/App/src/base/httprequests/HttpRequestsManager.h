#pragma once

#include "HttpRequestsManagerList.h"

namespace base {
class HttpRequestsManager : public HttpRequestsManagerList {
private: typedef HttpRequestsManagerList super;
public:
    HttpRequestsManager(HttpRequestsManager const&) = delete;
    HttpRequestsManager(HttpRequestsManager &&) = default;
    HttpRequestsManager& operator=(HttpRequestsManager const&) = delete;
    HttpRequestsManager& operator=(HttpRequestsManager &&) = default;

    explicit HttpRequestsManager(IApp* app, base::Net::INativeHttpHandler* deviceHttpHandler);

    virtual ~HttpRequestsManager();
};
};
