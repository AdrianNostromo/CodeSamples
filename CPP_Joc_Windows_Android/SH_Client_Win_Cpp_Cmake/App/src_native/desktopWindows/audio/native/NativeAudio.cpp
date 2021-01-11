#include "NativeAudio.h"
#include <base/exceptions/LogicException.h>
#include <iostream>
#include <Audioclient.h>
#include <audiopolicy.h>
#include <mmdeviceapi.h>
#include <ksmedia.h>
#include <stdio.h>
#include <cmath>
#include <limits>
#include <thread>
#include <avrt.h>

// REFERENCE_TIME time units per second and per millisecond
#define REFTIMES_PER_SEC  10000000
#define REFTIMES_PER_MILLISEC  10000

#define SAFE_RELEASE(punk)  \
              if ((punk) != NULL)  \
                { (punk)->Release(); (punk) = NULL; }

#define TWO_PI (3.14159265359*2)

using namespace desktopWindows::audio;

// Note. These variables are randomly here because glfw will cause errors if the windows includes are moved to the header file.
const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
const IID IID_IAudioClient = __uuidof(IAudioClient);
const IID IID_IAudioRenderClient = __uuidof(IAudioRenderClient);

NativeAudio::NativeAudio()
	: super()
{
	//void
}

unsigned int NativeAudio::tickStream(base::IManagedThread* managedThread) {
	super::tickStream(managedThread);

	if (getStatus() != Status::ONLINE) {
		return 50;// sleepDurationMs
	}

	HRESULT hr = WaitForSingleObject(hEvent, INFINITE);
	if (FAILED(hr)) {
		onStreamError();
	}

	UINT32 numFramesPadding = 0;
	hr = pAudioClient->GetCurrentPadding(&numFramesPadding);
	if (FAILED(hr)) {
		onStreamError();
	}

	int numFramesAvailable = bufferFrameCount - numFramesPadding;

	// The buffer contains the offset already.
	BYTE* pData = nullptr;

	hr = pRenderClient->GetBuffer(numFramesAvailable, &pData);
	if (FAILED(hr)) {
		onStreamError();
	}

	writePeriod(pData, numFramesAvailable);

	hr = pRenderClient->ReleaseBuffer(numFramesAvailable, 0);
	if (FAILED(hr)) {
		onStreamError();
	}

	return 0;// sleepDurationMs
}

bool NativeAudio::startOrRestartStream() {
	if (pAudioClient != nullptr) {
		bool b = stopStream();
		if (!b) {
			return b;
		}
	}

	HRESULT hr;
	REFERENCE_TIME hnsRequestedDuration = REFTIMES_PER_SEC;

	DWORD flags = 0;

	// This does some init of something not connected to audio.
	// https://docs.microsoft.com/en-us/windows/win32/api/combaseapi/nf-combaseapi-coinitializeex
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	hr = CoCreateInstance(
		CLSID_MMDeviceEnumerator, NULL,
		CLSCTX_ALL, IID_IMMDeviceEnumerator,
		(void**)& pEnumerator);
	if (FAILED(hr)) {
		return false;
	}

	hr = pEnumerator->GetDefaultAudioEndpoint(
		eRender, eConsole, &pDevice);
	if (FAILED(hr)) {
		return false;
	}

	hr = pDevice->Activate(
		IID_IAudioClient, CLSCTX_ALL,
		NULL, (void**)& pAudioClient);
	if (FAILED(hr)) {
		return false;
	}

	REFERENCE_TIME defaultDevicePeriod;
	REFERENCE_TIME minimumDevicePeriod;
	hr = pAudioClient->GetDevicePeriod(&defaultDevicePeriod, &minimumDevicePeriod);
	if (FAILED(hr)) {
		return false;
	}
	//The GetMixFormat method retrieves the stream format that the audio engine uses for its internal processing of shared-mode streams.
	WAVEFORMATEX* waveFormat = nullptr;
	hr = pAudioClient->GetMixFormat(&waveFormat);
	if (FAILED(hr)) {
		return false;
	}
	int localisedWaveFormatType = -1;
	if (waveFormat->wFormatTag == WAVE_FORMAT_EXTENSIBLE) {
		WAVEFORMATEXTENSIBLE* extensibleWaveFormat = reinterpret_cast<WAVEFORMATEXTENSIBLE*>(waveFormat);
		if (extensibleWaveFormat->SubFormat == KSDATAFORMAT_SUBTYPE_IEEE_FLOAT) {
			localisedWaveFormatType = WaveFormatType::FLOAT;
		} else if (extensibleWaveFormat->SubFormat == KSDATAFORMAT_SUBTYPE_PCM) {
			localisedWaveFormatType = WaveFormatType::PCM;
		} else {
			// Unsupported wave format.
			return false;
		}
	}

	WAVEFORMATEX* closestWaveFormat = nullptr;
	hr = pAudioClient->IsFormatSupported(
		AUDCLNT_SHAREMODE_SHARED,
		waveFormat,
		&closestWaveFormat
	);
	if (hr != S_OK) {
		// Format supported. This is just some extra checks.
		return false;
	}

	int waveFormat_nChannels = waveFormat->nChannels;
	int waveFormat_wBitsPerSample = waveFormat->wBitsPerSample;

	hr = pAudioClient->Initialize(
		AUDCLNT_SHAREMODE_SHARED,
		AUDCLNT_STREAMFLAGS_EVENTCALLBACK,
		//This is minimumDevicePeriod for exclusive and 0 for shared.
		// This will be cliped to the lowest corect value automatically.
		0,//hnsBufferDuration
		//This is minimumDevicePeriod for exclusive and 0 for shared.
		0,//hnsPeriodicity
		waveFormat,
		nullptr);
	if (FAILED(hr)) {
		return false;
	}

	float devicePeriodS = (float)defaultDevicePeriod / (float)REFTIMES_PER_SEC;
	float devicePeriodFrames = devicePeriodS * (float)waveFormat->nSamplesPerSec;

	// Get the actual size of the allocated buffer.
	hr = pAudioClient->GetBufferSize(&bufferFrameCount);
	if (FAILED(hr)) {
		return false;
	}

	int FrameSize_bytes = bufferFrameCount * waveFormat->nChannels * waveFormat->wBitsPerSample / 8;

	hr = pAudioClient->GetService(
		IID_IAudioRenderClient,
		(void**)& pRenderClient);
	if (FAILED(hr)) {
		return false;
	}

	hEvent = CreateEvent(nullptr, false, false, nullptr);
	if (hEvent == INVALID_HANDLE_VALUE) {
		printf("CreateEvent failed\n");

		return false;
	}

	hr = pAudioClient->SetEventHandle(hEvent);
	if (FAILED(hr)) {
		if (hr == E_INVALIDARG) {
			int a = 1;
		}
		return false;
	}

	// Set initial data to empty. Not sure if needed.
	BYTE* pData = nullptr;
	hr = pRenderClient->GetBuffer(bufferFrameCount, &pData);
	if (FAILED(hr)) {
		return false;
	}

	memset(pData, 0, FrameSize_bytes);

	hr = pRenderClient->ReleaseBuffer(bufferFrameCount, flags);
	if (FAILED(hr)) {
		return false;
	}

	// increase thread priority for optimal av performance
	DWORD taskIndex = 0;
	hTask = AvSetMmThreadCharacteristics(TEXT("Pro Audio"), &taskIndex);
	if (hTask == NULL) {
		hr = E_FAIL;
		if (FAILED(hr)) {
			return false;
		}
	}

	hr = pAudioClient->Start();
	if (FAILED(hr)) {
		return false;
	}

	setStats(
		waveFormat->nChannels,
		waveFormat->wBitsPerSample,
		bufferFrameCount,
		waveFormat->nSamplesPerSec,
		localisedWaveFormatType
	);

	return true;
}

bool NativeAudio::stopStream() {
	if (pAudioClient == nullptr) {
		return true;
	}

	HRESULT hr = pAudioClient->Stop();
	if (FAILED(hr)) {
		return false;
	}

	SAFE_RELEASE(pEnumerator)
	SAFE_RELEASE(pDevice)
	SAFE_RELEASE(pAudioClient)
	SAFE_RELEASE(pRenderClient)

	if (hEvent != NULL) {
		CloseHandle(hEvent);
	}

	if (hTask != NULL) {
		AvRevertMmThreadCharacteristics(hTask);
	}

	pEnumerator = nullptr;
	pDevice = nullptr;
	pAudioClient = nullptr;
	pRenderClient = nullptr;

	return true;
}

NativeAudio::~NativeAudio() {
	//void
}
