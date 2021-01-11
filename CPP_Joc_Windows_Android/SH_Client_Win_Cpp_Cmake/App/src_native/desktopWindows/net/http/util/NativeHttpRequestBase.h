#pragma once

#include <base/gh.h>
#include <base/net/http/util/INativeHttpRequest.h>

class IApp;

namespace desktopWindows {
namespace Net {
class NativeHttpRequestBase : virtual public base::Net::INativeHttpRequest {pub dCtor(NativeHttpRequestBase);
    pub std::string httpMethod;
    pub std::string baseUrl;
    pub std::string urlParams;

    prot IApp* app = nullptr;

    pub bool isRequested_start = false;
    pub bool isRequested_cancel = false;

    pub int errorCode = ErrorCode::None;

    pub int responseHttpStatusCode = -1;

    pub std::shared_ptr<std::string> responseHeader = nullptr;
    pub std::shared_ptr<std::string> responseBody = nullptr;

    pub explicit NativeHttpRequestBase(std::string& httpMethod, std::string& baseUrl, std::string& urlParams, IApp* app);

    pub void start() override;
    pub void cancel() override;

    pub int getErrorCode() override;

    pub int getResponseHttpStatusCode() override;

    pub virtual std::shared_ptr<std::string> getResponseHeader() override;
    pub virtual std::shared_ptr<std::string> getResponseBody() override;

    pub ~NativeHttpRequestBase() override;
};
};
};
