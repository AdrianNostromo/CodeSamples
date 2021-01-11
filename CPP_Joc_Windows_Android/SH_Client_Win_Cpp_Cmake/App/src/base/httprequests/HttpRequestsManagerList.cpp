#include "HttpRequestsManagerList.h"
#include <base/exceptions/LogicException.h>
#include <assert.h>
#include <base/log/ILogHandler.h>
#include <base/app/config/AppConfig.h>
#include <base/log/GlobalAccessLogHandler.h>
#include <base/app/IApp.h>
#include <base/appLoop/ILoopDistribution.h>
#include <base/appLoop/event/LoopEvent.h>

using namespace base;

const float HttpRequestsManagerList::MAX_REQUEST_LIFE_S = 15.0f;

HttpRequestsManagerList::HttpRequestsManagerList(IApp* app, base::Net::INativeHttpHandler* deviceHttpHandler)
    : super(app, deviceHttpHandler)
{
    //void
}

void HttpRequestsManagerList::create() {
    super::create();

    app->getLoopDistribution()->addEventListener(
        base::ILoopDistribution::AppLoopEvent_inputDispatch->type,
        std::bind(&HttpRequestsManagerList::onLoopEvent_appLoop_general, this, std::placeholders::_1, std::placeholders::_2),
        localAutoListenersList
    );
}

std::shared_ptr<IHttpRequestsManager::IRequest> HttpRequestsManagerList::startNewRequest(std::string& httpMethod, std::string& baseUrl, std::string& urlParams, IRequestListener* listener) {
    if(AppConfig::IS_DEBUG && IS_DEBUG) {
        GlobalAccessLogHandler::devLogHandler->postLog("Http request. Start.");
    }

    std::shared_ptr<Request> httpRequest = std::make_shared<Request>(deviceHttpHandler->createRequest(
        httpMethod, baseUrl, urlParams
    ), listener);
    activeRequestsList.appendDirect(httpRequest);
    httpRequest->start();

    return static_cast<std::shared_ptr<IHttpRequestsManager::IRequest>>(httpRequest);
}

void HttpRequestsManagerList::onLoopEvent_appLoop_general(IEventListener& eventListener, base::LoopEvent& event) {
    for(int i=activeRequestsList.size() - 1;i>=0;i--) {
        std::shared_ptr<Request> entry = activeRequestsList.getDirect(i);
        if(entry != nullptr) {
            entry->requestLifeS += event.deltaS;

            if(entry->requestLifeS >= MAX_REQUEST_LIFE_S) {
                if(AppConfig::IS_DEBUG && IS_DEBUG) {
                    GlobalAccessLogHandler::devLogHandler->postLog("Http request. Timed_out.");
                }
                
                entry->nativeRequest->handlerErrorOccured(-1);
            }

            if(entry->nativeRequest->getIsFinished()) {
                if(entry->nativeRequest->getErrorCode() == 0) {
                    if(!entry->getIsCanceled() && entry->getListener() != nullptr) {
                        entry->getListener()->onSuccess(entry);
                    }

                    if(AppConfig::IS_DEBUG && IS_DEBUG) {
                        GlobalAccessLogHandler::devLogHandler->postLog("Http request. Success.");
                    }
                }else {
                    if(!entry->getIsCanceled() && entry->getListener() != nullptr) {
                        entry->getListener()->onFail(entry);
                    }

                    if(AppConfig::IS_DEBUG && IS_DEBUG) {
                        GlobalAccessLogHandler::devLogHandler->postLog("Http request. Failed.");
                    }
                }
                removeRequest(entry);
            }
        }

    }
}

void HttpRequestsManagerList::removeRequest(std::shared_ptr<Request> request) {
    int i = activeRequestsList.indexOf(request);
    if(i < 0) {
        throw LogicException(LOC);
    }

    if(!activeRequestsList.remove(request)) {
        throw LogicException(LOC);
    }
}

HttpRequestsManagerList::~HttpRequestsManagerList() {
    //void
}
