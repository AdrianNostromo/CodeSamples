#pragma once

#include <base/gh.h>
#include "NA_Base.h"
#include <string>

typedef void* HANDLE;

class IAudioClient;
class IAudioRenderClient;
class IMMDeviceEnumerator;
class IMMDevice;

namespace desktopWindows {
namespace audio {
class NativeAudio : public NA_Base {priv typedef NA_Base super;pub dCtor(NativeAudio);
	priv HANDLE hEvent = nullptr;
	priv HANDLE hTask = nullptr;

	priv IAudioClient* pAudioClient = nullptr;
	priv IAudioRenderClient* pRenderClient = nullptr;
	priv unsigned int bufferFrameCount;
	priv IMMDeviceEnumerator* pEnumerator = nullptr;
	priv IMMDevice* pDevice = nullptr;

    pub explicit NativeAudio();

	prot unsigned int tickStream(base::IManagedThread* managedThread) override;

	prot bool startOrRestartStream() override;
	prot bool stopStream() override;

	pub ~NativeAudio() override;
};
};
};
