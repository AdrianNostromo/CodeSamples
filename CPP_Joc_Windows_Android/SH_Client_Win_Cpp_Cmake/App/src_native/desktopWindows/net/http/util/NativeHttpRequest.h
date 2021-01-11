#pragma once

#include <base/gh.h>
#include <string>
#include <memory>
#include "NativeHttpRequestBase.h"

namespace base {
    class IManagedThread;
};

namespace desktopWindows {
namespace Net {
class NativeHttpRequest : public NativeHttpRequestBase {priv typedef NativeHttpRequestBase super;pub dCtor(NativeHttpRequest);
    priv class Status {
        pub static const int Prestart = 0;
        pub static const int InProgress = 1;
        pub static const int Finished = 2;
    };
    
    priv int status = Status::Prestart;

    priv int socketFd = -1;
    priv char* responseCString = nullptr;

    priv base::IManagedThread* managedThread = nullptr;

    priv float totalStartWaitTimeS = 0.0f;

    pub explicit NativeHttpRequest(std::string& httpMethod, std::string& baseUrl, std::string& urlParams, IApp* app);

    pub bool getIsFinished() override;

    pub void start() override;

    pub void handlerErrorOccured(int localErrorCode) override;

    pub unsigned int logicLoop(base::IManagedThread* managedThread);

    priv void connectToServer(char* szServerName, int portNum);
    priv void prepareRequest(std::string& requestString,
        std::string& server, std::string& filepath, std::string& filename);
    priv void sendRequest(std::string& requestString);
    priv void receiveResponse(char** responseCString, int& responseLen);
    priv void parseResponseParts(char* responseCString, int responseSize);
    priv int getHeaderLength(char* content);
    priv void mParseUrl(std::string url, std::string& serverName, std::string& filepath, std::string& filename, int& port);

    priv void cleanup();
    priv void cleanupSocket();

    pub ~NativeHttpRequest() override;
};
};
};
