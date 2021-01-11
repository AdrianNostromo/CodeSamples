#include "NA_StreamHandler.h"
#include <base/app/event/AppEvent.h>
#include <base/app/IApp.h>
#include <base/app/IApp.h>
#include <base/threadsManager/IThreadsManager.h>
#include <base/threadsManager/managedThread/IManagedThread.h>

using namespace base::audio;

NA_StreamHandler::NA_StreamHandler()
        : super()
{
    //void
}

void NA_StreamHandler::createMain() {
    super::createMain();

    managedThread = app->getThreadsManager()->newThread("NativeAudio_StreamHandler"/*name*/, std::bind(&NA_StreamHandler::tickStream, this, std::placeholders::_1));
}

void NA_StreamHandler::init(IApp* app) {
    super::init(app);

    cache_isAppActive = app->getIsAppActive();
    app->addEventListener(
        IApp::Event_isAppActiveChanged->type,
        std::bind(&NA_StreamHandler::onAppEvent, this, std::placeholders::_1, std::placeholders::_2),
        autoListenersList
    );
}

int NA_StreamHandler::getStatus() {
	return status;
}

void NA_StreamHandler::onAppEvent(IEventListener& eventListener, base::AppEvent& event) {
    if (event.type == IApp::Event_isAppActiveChanged->type) {
        cache_isAppActive = app->getIsAppActive();
    } else {
        throw LogicException(LOC);
    }
}

void NA_StreamHandler::onStreamError() {
    isStreamError = true;
}

unsigned int NA_StreamHandler::tickStream(base::IManagedThread* managedThread) {
    if(isStreamError) {
        stopStream();
        status = Status::OFFLINE;
        isStreamError = false;
    }
    if (cache_isAppActive) {
        if (status != Status::ONLINE) {
            status = Status::STARTING;

            bool b = startOrRestartStream();
            if(b) {
                status = Status::ONLINE;
            }else {
                status = Status::OFFLINE;
            }
        }
    }else {
        if (status != Status::OFFLINE) {
            status = Status::CLOSING;

            bool b = stopStream();

            if(b) {
                status = Status::OFFLINE;
            }else {
                throw LogicException(LOC);
            }
        }
    }

    return 100;
}

void NA_StreamHandler::disposeMain() {
    if (managedThread != nullptr) {
        managedThread->removeManaged();
        managedThread = nullptr;
    }

    stopStream();

    super::disposeMain();
}

NA_StreamHandler::~NA_StreamHandler() {
    //void
}
