#pragma once

#include <base/gh.h>
#include "A_Base.h"
#include <base/list/ArrayList.h>
#include <memory>

class IEventListener;
namespace base {
	class LoopEvent;
};

namespace base {
namespace audio {
class Audio : public A_Base {priv typedef A_Base super;pub dCtor(Audio);
	// This is used for sounds play overlapCooldownS protection.
	// Initial value is -1.0f;
	prot float soundOverlapProtectionTimerS = 0.0f;

	priv ArrayList<std::shared_ptr<IEventListener>> localListenersList{};

    pub explicit Audio(IApp* app, base::audio::INativeAudio* nativeAudio);
	prot void create() override;

	priv void onAppLoopEvent_general(IEventListener& eventListener, base::LoopEvent& event);

	pub int getRate() override;

	pub bool play(ISound* sound, float volume, float overlapCooldownS = 0.0f) override;

	prot void disposeMain() override;
	pub ~Audio() override;
};
};
};
