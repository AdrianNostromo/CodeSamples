#include <base/net/http/util/HttpMethods.h>
#include <base/app/config/AppConfig.h>
#include "ComponentIpPortActiveRequests.h"
#include <assert.h>
#include <base/util/json11/json11.h>
#include <memory>
#include <base/exceptions/LogicException.h>
#include <base/app/IApp.h>
#include <base/app/IApp.h>
#include <base/appLoop/ILoopDistribution.h>
#include <base/appLoop/event/LoopEvent.h>

using namespace worldGame3D;

ComponentIpPortActiveRequests::ComponentIpPortActiveRequests(IServerPartHandler* handler, IApp* app)
    : super(handler, app)
{
    //void
}

void ComponentIpPortActiveRequests::create() {
    super::create();

    app->getLoopDistribution()->addEventListener(
        base::ILoopDistribution::AppLoopEvent_inputDispatch->type,
        std::bind(&ComponentIpPortActiveRequests::onLoopEvent_appLoop_general, this, std::placeholders::_1, std::placeholders::_2),
        localAutoListenersList
    );
}

std::shared_ptr<IComponentIpPort::IRequest> ComponentIpPortActiveRequests::poolServerComponentIpPort(
    std::string& serverComponentId,
    IRequestListener* listener
) {
    std::shared_ptr<Request> newRequest = std::make_shared<Request>(serverComponentId, listener);
    newRequestsList.appendDirect(newRequest);

    return newRequest;
}

void ComponentIpPortActiveRequests::onLoopEvent_appLoop_general(IEventListener& eventListener, base::LoopEvent& event) {
    while(newRequestsList.size() > 0) {
        std::shared_ptr<Request> entry = newRequestsList.removeAndGetDirect(0);

        if (entry->getFailCode() != 0) {
            // Move to finished.
            entry->setHandler(nullptr);

            finishedRequestsList.appendDirect(entry);
        } else {
            // Activate request.
            entry->setHandler(this);

            // Make the http request.
            std::string baseUrl = std::string("http://") + AppConfig::SERVER_ENTRY_POINT_IP_PORT.ip + ":" + std::to_string(AppConfig::SERVER_ENTRY_POINT_IP_PORT.port);
            std::string urlParams = std::string("client_type_name=") + AppConfig::CLIENT_TYPE + "&request_type=component_ip_port&server_component_name=" + entry->getServerComponentId();
            entry->httpRequest = app->getHttpRequests()->startNewRequest(
                base::Net::HttpMethods::GET,
                baseUrl,
                urlParams,
                entry.get()
            );

            activeRequestList.appendDirect(entry);
        }
    }

    while(finishedRequestsList.size() > 0) {
        std::shared_ptr<Request> entry = finishedRequestsList.removeAndGetDirect(0);

        int responseHttpStatusCode = 0;
        std::shared_ptr<std::string> responseBody;
        if (entry->getFailCode() == FailCodes::None) {
            // Parse the http response and check for errors.
            responseHttpStatusCode = entry->httpRequest->getResponseHttpStatusCode();
            responseBody = entry->httpRequest->getResponseBody();
        }
        if (entry->getFailCode() == FailCodes::None) {
            if (responseHttpStatusCode != 200) {
                entry->setFailCode(FailCodes::DataPoolFailed);
            }
        }

        json11::Json jsonObject;
        if (entry->getFailCode() == FailCodes::None) {
            std::string err;
            jsonObject = json11::Json::parse(*responseBody, err);
            if (!err.empty()) {
                entry->setFailCode(FailCodes::DataPoolFailed);
            }
        }

        if (entry->getFailCode() == FailCodes::None) {
            std::string ip = jsonObject["ip"].string_value();
            int port = jsonObject["port"].int_value();

            base::Net::IpPort ipPort{ip, port};
            entry->responseIpPort = std::make_shared<base::Net::IpPort>(ip, port);
        }

        if (entry->getListener() != nullptr) {
            if (entry->getFailCode() == FailCodes::None) {
                entry->getListener()->onSuccess(entry);
            } else {
                entry->getListener()->onFail(entry);
            }
        }
    }
}

void ComponentIpPortActiveRequests::onRequestHttpFinished(Request* target) {
    // Move the request to the finished requests list.
    moveActiveRequestPointerToFinishedList(target);
}

void ComponentIpPortActiveRequests::onRequestCancelRequested(Request* target) {
    // The request is in the active requests list.

    // Move the request to the finished list.
    moveActiveRequestPointerToFinishedList(target);
}

void ComponentIpPortActiveRequests::moveActiveRequestPointerToFinishedList(Request* target) {
    for (int i = 0; i < activeRequestList.size(); i++) {
        std::shared_ptr<Request> entry = activeRequestList.getDirect(i);

        if(entry.get() == target) {
            if(activeRequestList.removeAndGetDirect(i) != entry) {
                // Thread synchronization error.
                throw LogicException(LOC);
            }

            entry->setIsFinished(true);
            finishedRequestsList.appendDirect(entry);

            return ;
        }
    }

    throw LogicException(LOC);
}

ComponentIpPortActiveRequests::~ComponentIpPortActiveRequests() {
    //void
}
