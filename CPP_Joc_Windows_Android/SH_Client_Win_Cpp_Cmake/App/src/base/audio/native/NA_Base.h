#pragma once

#include <base/gh.h>
#include <base/audio/native/INativeAudio.h>
#include <base/audio/util/PlayingSound.h>
#include <base/list/ListDL.h>
#include <base/object/ManagedObject.h>
#include <base/list/ArrayList.h>
#include <base/MM.h>

class IApp;
class IEventListener;

namespace base {
namespace audio {
class NA_Base : public base::ManagedObject, virtual public INativeAudio {priv typedef base::ManagedObject super;pub dCtor(NA_Base);
	prot class WaveFormatType {
		pub static const int PCM;
		pub static const int FLOAT;
	};

	prot IApp* app = nullptr;

	// This contains event listeners from this module and are auto disposed on disposePre;
	prot ArrayList<std::shared_ptr<IEventListener>> autoListenersList{};

	// The entries must be manually created and removed.
	// This is required for sounds removal to occur without the audio thread releasing memory which might cause underruns to occur.
	// Entries from this list are moved by the audio thread to the removing... list and are removed by the application thread.
	prot ListDL<std::shared_ptr<PlayingSound>> playingSoundsListUnmanaged{LOC};
	// The entries must be manually created and removed.
	prot ListDL<std::shared_ptr<PlayingSound>> removingPlayedSoundsListUnmanaged{LOC};

	priv int channelsCount = -1;
	priv int bitsPerSample = -1;
	priv int maxPeriodFramesCount = -1;
	priv int rate = -1;
	priv int waveFormatType = -1;

	// This is always using float and converts to the target type as needed.
	// This is used to allow for volume clipping.
	priv base::audio::DtAudioData::Frame* periodFramesList = nullptr;
	
    pub explicit NA_Base();
	pub void init(IApp* app) override;
	prot void createMain() override;

	pub void playSound(ISound* sound, float volume) override;

	pub void loop_soundsRemoval() override;

	pub int getRate() override;

	prot void setStats(int channelsCount, int bitsPerSample, int maxPeriodFramesCount, int rate, int waveFormatType);
	prot void writePeriod(void* oBuff, int framesCount);

	prot void disposeMain() override;
    pub ~NA_Base() override;
};
};
};
