#include "CfgSound.h"
#include <base/math/Math.h>
#include <base/MM.h>

using namespace base::audio;

Map1D<std::string, CfgSound*> CfgSound::entriesMap{};

CfgSound::CfgSound(std::string& id) {
	managedPath = ManagedAssetPath::stringPathToManagedSoundPath(id);
}

CfgSound* CfgSound::newEntry(std::string id) {
	// Check if the scenario already exists.
	if (entriesMap.containsKey(id)) {
		throw LogicException(LOC);
	}

	CfgSound* newTemplate = *entriesMap.putDirect(
		id,
		new CfgSound(
			id
		)
	);

	return newTemplate;
}

CfgSound* CfgSound::fromId(std::string& id) {
	return entriesMap.getDirect(id);
}

std::shared_ptr<base::audio::DtAudioData> CfgSound::rasterize(int rate) {
	// Compute the total frames count.
	int framesCount = source->computeFramesCount(rate);

	// Create the frames buffer.
	DtAudioData::Frame* framesList = new DtAudioData::Frame[framesCount];
	
	// Do the rasterizing.
	source->rasterize(rate, framesList, framesCount);

	std::shared_ptr<base::audio::DtAudioData> soundData = std::make_shared<base::audio::DtAudioData>(
		framesList, framesCount, framesCount / (float)rate
	);

	return soundData;
}

CfgSound::~CfgSound() {
	//void
}
