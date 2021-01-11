#include "Audio.h"
#include <base/app/IApp.h>
#include <base/appLoop/ILoopDistribution.h>
#include <base/appLoop/event/LoopEvent.h>

using namespace base::audio;

Audio::Audio(IApp* app, base::audio::INativeAudio* nativeAudio)
	: super(app, nativeAudio)
{
	//void
}

void Audio::create() {
	super::create();
	
	app->getLoopDistribution()->addEventListener(
		base::ILoopDistribution::AppLoopEvent_general->type,
		std::bind(&Audio::onAppLoopEvent_general, this, std::placeholders::_1, std::placeholders::_2),
		localListenersList
	);
}

void Audio::onAppLoopEvent_general(IEventListener& eventListener, base::LoopEvent& event) {
	soundOverlapProtectionTimerS += event.deltaS;
}

int Audio::getRate() {
	return nativeAudio->getRate();
}

bool Audio::play(ISound* sound, float volume, float overlapCooldownS) {
	if (overlapCooldownS >= 0.0f) {
		if (sound->getLastPlayTimerSRef() >= 0/*initial_play_occured*/ && sound->getLastPlayTimerSRef() + overlapCooldownS > soundOverlapProtectionTimerS) {
			// Sound overlap avoided.
			return false;
		}
	}

	sound->getLastPlayTimerSRef() = soundOverlapProtectionTimerS;
	nativeAudio->playSound(sound, volume);

	return true;
}

void Audio::disposeMain() {
	localListenersList.clear();

	super::disposeMain();
}

Audio::~Audio() {
	//void
}
