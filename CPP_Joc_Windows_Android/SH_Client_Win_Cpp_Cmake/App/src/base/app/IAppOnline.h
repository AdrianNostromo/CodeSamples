#pragma once

#include <base/httprequests/IHttpRequestsManager.h>

class IAppOnline {
public:
    virtual base::IHttpRequestsManager* getHttpRequests() = 0;

    virtual ~IAppOnline() = default;
};
