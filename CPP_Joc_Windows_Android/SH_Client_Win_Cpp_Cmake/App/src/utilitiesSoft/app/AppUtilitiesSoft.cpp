#include "AppUtilitiesSoft.h"
#include <base/util/DeviceInfo.h>
#include <base/app/config/AppConfig.h>

using namespace utilitiesSoft;

AppUtilitiesSoft::AppUtilitiesSoft(
    int deviceType,
    INativeInput* nativeInputConnection,
    base::IProfileStorage* profileStorage,
    base::Net::INativeHttpHandler* nativeHttpHandler,
    base::Net::INativeSocketsHandler* nativeSocketsHandler,
	base::IFileSystem* fileSystem, IGL* gl,
    base::INativeConsole* nativeConsole,
    base::audio::INativeAudio* nativeAudio,
    base::ThreadsManager* threadsManager)
    : super(
		deviceType,
		nativeInputConnection,
        profileStorage,
        nativeHttpHandler,
        nativeSocketsHandler,
        fileSystem, gl,
        nativeConsole,
        nativeAudio,
        threadsManager)
{
    //void
}

AppUtilitiesSoft::~AppUtilitiesSoft() {
    //void
}
