#include "HttpRequestsManager.h"

using namespace base;

HttpRequestsManager::HttpRequestsManager(IApp* app, base::Net::INativeHttpHandler* deviceHttpHandler)
    : super(app, deviceHttpHandler)
{
    //void
}

HttpRequestsManager::~HttpRequestsManager() {
    //void
}
