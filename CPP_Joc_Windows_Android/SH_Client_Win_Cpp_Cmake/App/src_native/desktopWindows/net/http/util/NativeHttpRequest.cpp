#include "NativeHttpRequest.h"
#include <assert.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <base/exceptions/LogicException.h>
#include <base/net/http/util/HttpMethods.h>
#include <functional>
#include <chrono>
#include <thread>
#include <base/app/IApp.h>
#include <base/threadsManager/IThreadsManager.h>
#include <base/threadsManager/managedThread/IManagedThread.h>

using namespace desktopWindows::Net;

NativeHttpRequest::NativeHttpRequest(std::string& httpMethod, std::string& baseUrl, std::string& urlParams, IApp* app)
    : super(httpMethod, baseUrl, urlParams, app)
{
    //void
}

bool NativeHttpRequest::getIsFinished() {
    if(status == Status::Finished) {
        return true;
    }

    return false;
}

void NativeHttpRequest::start() {
    super::start();

    if (status != Status::Prestart) {
        throw LogicException(LOC);
    }

    managedThread = app->getThreadsManager()->newThread("NativeHttpRequest", std::bind(&NativeHttpRequest::logicLoop, this, std::placeholders::_1));
}

void NativeHttpRequest::handlerErrorOccured(int localErrorCode) {
    errorCode = localErrorCode;

    // Close the socket in case it is blocking the thread.
    cleanupSocket();
}

unsigned int NativeHttpRequest::logicLoop(base::IManagedThread* managedThread) {
    // Wait for the start command to be received.
    if(!isRequested_start && !isRequested_cancel) {
        totalStartWaitTimeS += 0.1f;
        if(totalStartWaitTimeS >= 5.0f) {
            // Request never started. Logic error.

            managedThread->removeManaged();
            managedThread = nullptr;

            throw LogicException(LOC);
        }

        return 100;//sleepDurationMs
    }

    managedThread->removeManaged();
    managedThread = nullptr;

    status = Status::InProgress;

    if(httpMethod != base::Net::HttpMethods::GET) {
        // Currently only Get http request method is supported;
        throw LogicException(LOC);
    }

    //"http://www.foo.com/watch?param1=value1&param2=value2";
    std::string finalUrl = baseUrl + "/?" + urlParams;

    // Parse url to components.
    std::string server, filepath, filename;
    int port = -1;
    if(errorCode == ErrorCode::None && !isRequested_cancel && status == Status::InProgress) {
        mParseUrl(finalUrl, server, filepath, filename, port);
    }

    // Connect to server.
    if(errorCode == ErrorCode::None && !isRequested_cancel && status == Status::InProgress) {
        connectToServer((char*)server.c_str(), port);
    }

    // Prepare request.
    std::string requestString;
    if(errorCode == ErrorCode::None && !isRequested_cancel && status == Status::InProgress) {
        prepareRequest(requestString, server, filepath, filename);
    }

    // Send request.
    if(errorCode == ErrorCode::None && !isRequested_cancel && status == Status::InProgress) {
        sendRequest(requestString);
    }

    // Receive data if any.
    int responseSize = 0;
    if(errorCode == ErrorCode::None && !isRequested_cancel && status == Status::InProgress) {
        receiveResponse(&responseCString, responseSize);
    }

    // If data received, extract header and body.
    if(errorCode == ErrorCode::None && !isRequested_cancel && status == Status::InProgress) {
        parseResponseParts(responseCString, responseSize);
    }

    if(status != Status::InProgress) {
        throw LogicException(LOC);
    }

    cleanup();

    if (isRequested_cancel) {
        // This code overrides error codes.
        errorCode = ErrorCode::Canceled;
    }

    status = Status::Finished;

    return 0;
}

void NativeHttpRequest::connectToServer(char *szServerName, int portNum) {
    struct hostent *hp;
    unsigned int addr;
	/*asdA10;
    struct sockaddr_in server;

    socketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socketFd == -1) {
        errorCode = ErrorCode::NetError;

        return ;
    }

    if(inet_addr(szServerName) == INADDR_NONE) {
        hp=gethostbyname(szServerName);
    }else {
        addr=inet_addr(szServerName);
        hp=gethostbyaddr((char*)&addr,sizeof(addr),AF_INET);
    }

    if(hp == nullptr) {
        errorCode = ErrorCode::NetError;

        return ;
    }

    server.sin_addr.s_addr = static_cast<in_addr_t>(*((unsigned long*)hp->h_addr));
    server.sin_family = AF_INET;
    server.sin_port = htons(static_cast<uint16_t>(portNum));
    if(connect(socketFd, (struct sockaddr*)& server, sizeof(server))) {
        errorCode = ErrorCode::NetError;

        return ;
    }*/
}

void NativeHttpRequest::prepareRequest(std::string& requestString,
    std::string& server, std::string& filepath, std::string& filename)
{
    std::stringstream ss;
    ss << "GET " << filepath << " HTTP/1.0" << "\r\n";
    ss << "Host: " << server << "\r\n";
    ss << "\r\n";

    requestString = ss.str();
}

void NativeHttpRequest::sendRequest(std::string& requestString) {
	/*asdA10;
    send(socketFd, requestString.c_str(), requestString.length(), 0);*/
}

void NativeHttpRequest::receiveResponse(char** responseCString, int& responseLen) {
    const int bufSize = 512;
    char readBuffer[bufSize];
    char* tmpResult = nullptr;
    long totalBytesRead;
    long partReadSize;

	/*asdA10;
    totalBytesRead = 0;
    while(true) {
        memset(readBuffer, 0, bufSize);
        partReadSize = recv(socketFd, readBuffer, bufSize, 0);

        if (partReadSize <= 0) {
            break;
        }

        tmpResult = (char*)realloc(tmpResult, static_cast<size_t>(partReadSize + totalBytesRead));

        memcpy(tmpResult+totalBytesRead, readBuffer, static_cast<size_t>(partReadSize));
        totalBytesRead += partReadSize;
    }

    if(totalBytesRead == 0) {
        responseLen = 0;
        *responseCString = nullptr;

        return ;
    }

    responseLen = static_cast<int>(totalBytesRead);
    *responseCString = tmpResult;*/
}

void NativeHttpRequest::parseResponseParts(char* responseCString, int responseSize) {
    int headerLen = getHeaderLength(responseCString);
    int bodyLen = responseSize - headerLen;

    char* bodyCString = new char[bodyLen + 1];
    memcpy(bodyCString, responseCString+headerLen, static_cast<size_t>(bodyLen));
    bodyCString[bodyLen] = 0x0;

    responseBody = std::make_shared<std::string>(bodyCString);
    delete[] bodyCString;

    char* headerCString = new char[headerLen + 1];
    strncpy(headerCString, responseCString, static_cast<size_t>(headerLen));
    headerCString[headerLen] = 0x0;

    responseHeader = std::make_shared<std::string>(headerCString);
    delete[] headerCString;

    //"HTTP/1.1 200 OK";

    // Extract the number from the first space to the second one.
    // Limit the number to max 5 digits numbers.
    std::size_t rcIndexStart = responseHeader->find(' ');
    if(rcIndexStart == std::string::npos) {
        errorCode = ErrorCode::BadResponse;

        return ;
    }
    rcIndexStart++;

    std::size_t rcIndexEnd = responseHeader->find(' ', rcIndexStart);
    if(rcIndexEnd == std::string::npos) {
        errorCode = ErrorCode::BadResponse;

        return ;
    }

    std::string s = responseHeader->substr(rcIndexStart, rcIndexEnd - rcIndexStart);
    responseHttpStatusCode = std::stoi(s);

    if(responseHttpStatusCode < 100 || responseHttpStatusCode > 599) {
        // Status code is not valid.
        errorCode = ErrorCode::BadResponse;
    }
}

int NativeHttpRequest::getHeaderLength(char *content) {
    const char *srchStr1 = "\r\n\r\n", *srchStr2 = "\n\r\n\r";
    char *findPos;
    int ofset = -1;

    findPos = strstr(content, srchStr1);
    if (findPos != nullptr) {
        ofset = static_cast<int>(findPos - content);
        ofset += strlen(srchStr1);
    }else {
        findPos = strstr(content, srchStr2);
        if (findPos != nullptr)
        {
            ofset = static_cast<int>(findPos - content);
            ofset += strlen(srchStr2);
        }
    }

    return ofset;
}

void NativeHttpRequest::mParseUrl(std::string url, std::string& serverName, std::string& filepath, std::string& filename, int& port) {
    std::string::size_type n;

    if (url.substr(0,7) == "http://") {
        url.erase(0, 7);
    }

    if (url.substr(0,8) == "https://") {
        url.erase(0, 8);
    }

    std::string tSerName;
    n = url.find('/');
    if (n != std::string::npos) {
        tSerName = url.substr(0,n);
        filepath = url.substr(n);
        n = filepath.rfind('/');
        filename = filepath.substr(n+1);
    }else {
        tSerName = url;
        filepath = "/";
        filename = "";
    }

    std::size_t portOffset = tSerName.find(':');
    if (portOffset == std::string::npos) {
        errorCode = ErrorCode::LogicError;

        return ;
    }

    serverName = tSerName.substr(0, portOffset);
    std::string portString = tSerName.substr(portOffset + 1, std::string::npos);
    port = std::stoi(portString);
}

void NativeHttpRequest::cleanup() {
    cleanupSocket();
}

void NativeHttpRequest::cleanupSocket() {
	/*asdA10;
    if(socketFd >= 0) {
        shutdown(socketFd, SHUT_RDWR);
        close(socketFd);

        socketFd = -1;
    }*/
}

NativeHttpRequest::~NativeHttpRequest() {
    if(responseCString) {
        delete[] responseCString;

        responseCString = nullptr;
    }
}
