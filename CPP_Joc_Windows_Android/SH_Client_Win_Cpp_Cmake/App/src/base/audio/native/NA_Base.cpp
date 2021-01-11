#include "NA_Base.h"
#include <base/math/Math.h>
#include <base/MM.h>

using namespace base::audio;

const int NA_Base::WaveFormatType::PCM = 1;
const int NA_Base::WaveFormatType::FLOAT = 2;

NA_Base::NA_Base()
	:super()
{
	//void
}

void NA_Base::init(IApp* app) {
	this->app = app;
}

void NA_Base::createMain() {
	super::createMain();

	//void;
}

void NA_Base::playSound(ISound* sound, float volume) {
	std::shared_ptr<PlayingSound> playingSound = std::make_shared<PlayingSound>(
		sound->getData(),
		volume);

	// This is done like this to make sure memory on't be freed on the audio thread.
	ListDL<std::shared_ptr<PlayingSound>>::Entry* listEntry = new ListDL<std::shared_ptr<PlayingSound>>::Entry(
		true, 
		playingSound
	);

	playingSoundsListUnmanaged.appendEntry(listEntry);
}

void NA_Base::loop_soundsRemoval() {
	ListDL<std::shared_ptr<PlayingSound>>::Entry* listEntry;
	while ((listEntry = removingPlayedSoundsListUnmanaged.getFirst()) != nullptr) {
		if (!listEntry->isEntryExternallyManuallyDeleted) {
			throw LogicException(LOC);
		}

		listEntry->remove();

		delete listEntry;
	}
}

int NA_Base::getRate() {
	return rate;
}

void NA_Base::setStats(int channelsCount, int bitsPerSample, int maxPeriodFramesCount, int rate, int waveFormatType) {
	if (channelsCount != 2) {
		// Currently only 2 channels are implemented.
		throw LogicException(LOC);
	}

	this->channelsCount = channelsCount;
	this->bitsPerSample = bitsPerSample;
	this->maxPeriodFramesCount = maxPeriodFramesCount;
	this->rate = rate;
	this->waveFormatType = waveFormatType;

	if (periodFramesList != nullptr) {
		delete[] periodFramesList;

		periodFramesList = nullptr;
	}

	periodFramesList = new base::audio::DtAudioData::Frame[maxPeriodFramesCount];
}

void NA_Base::writePeriod(void* oBuff, int framesCount) {
	// Enumerate all active Sounds and add them up in a floats buffer.
	memset(periodFramesList, 0, sizeof(base::audio::DtAudioData::Frame) * framesCount);

	playingSoundsListUnmanaged.nextEnumeratingEntry = playingSoundsListUnmanaged.getFirst();
	playingSoundsListUnmanaged.isEnumeratingInProgress = true;
	typename ListDL<std::shared_ptr<PlayingSound>>::Entry* listEntry;
	while ((listEntry = playingSoundsListUnmanaged.nextEnumeratingEntry) != nullptr) {
		playingSoundsListUnmanaged.nextEnumeratingEntry = playingSoundsListUnmanaged.nextEnumeratingEntry->next;

		std::shared_ptr<PlayingSound>& playingSound = listEntry->data;

		// Write frames count to the buffer.
		int framesToCopy = Math::min(framesCount, playingSound->data->framesCount - playingSound->frameIndex);
		for (int i = 0; i < framesToCopy; i++) {
			int srcOffset = playingSound->frameIndex + i;

			periodFramesList[i].sample_left += playingSound->data->framesList[srcOffset].sample_left * playingSound->volume;
			periodFramesList[i].sample_right += playingSound->data->framesList[srcOffset].sample_right * playingSound->volume;
		}
		playingSound->frameIndex += framesToCopy;

		if (playingSound->frameIndex >= playingSound->data->framesCount) {
			// Sound finished playing, move the list entry to the removing entries list.
			// This movement will avoid a memory free on the audio thread.

			if (!listEntry->isEntryExternallyManuallyDeleted) {
				throw LogicException(LOC);
			}

			listEntry->remove();
			removingPlayedSoundsListUnmanaged.appendEntry(listEntry);
		}
	}
	playingSoundsListUnmanaged.isEnumeratingInProgress = false;

	// Convert and clip the buffer to the destination.

	if (waveFormatType == WaveFormatType::PCM && channelsCount == 2 && bitsPerSample == 16) {
		unsigned char* cOBuff = reinterpret_cast<unsigned char*>(oBuff);

		for (int i = 0; i < framesCount; i++) {
			float valLeft = Math::clip(periodFramesList[i].sample_left, -1.0f, 1.0f);
			float valRight = Math::clip(periodFramesList[i].sample_right, -1.0f, 1.0f);

			short v_left = (short)((valLeft / 2.0f) * 65535.0f);
			short v_right = (short)((valRight / 2.0f) * 65535.0f);

			cOBuff[i * 4 + 0] = static_cast<unsigned char>(v_left & 0xff);
			cOBuff[i * 4 + 1] = static_cast<unsigned char>(v_left >> 8);

			cOBuff[i * 4 + 2] = static_cast<unsigned char>(v_right & 0xff);
			cOBuff[i * 4 + 3] = static_cast<unsigned char>(v_right >> 8);
		}
	} else if (waveFormatType == WaveFormatType::FLOAT && channelsCount == 2 && bitsPerSample == 32) {
		float* cOBuff = reinterpret_cast<float*>(oBuff);

		for (int i = 0; i < framesCount; i++) {
			float valLeft = Math::clip(periodFramesList[i].sample_left, -1.0f, 1.0f);
			float valRight = Math::clip(periodFramesList[i].sample_right, -1.0f, 1.0f);

			cOBuff[i * 2 + 0] = valLeft;
			cOBuff[i * 2 + 1] = valRight;
		}
	} else {
		// Unsupported audio format.
		// Implement when needed.
		throw LogicException(LOC);
	}
}

void NA_Base::disposeMain() {
	ListDL<std::shared_ptr<PlayingSound>>::Entry* listEntry;
	while ((listEntry = playingSoundsListUnmanaged.getFirst()) != nullptr) {
		if (!listEntry->isEntryExternallyManuallyDeleted) {
			throw LogicException(LOC);
		}

		listEntry->remove();

		delete listEntry;
	}
	while ((listEntry = removingPlayedSoundsListUnmanaged.getFirst()) != nullptr) {
		if (!listEntry->isEntryExternallyManuallyDeleted) {
			throw LogicException(LOC);
		}

		listEntry->remove();

		delete listEntry;
	}

	super::disposeMain();
}

NA_Base::~NA_Base() {
	//void
	
}
