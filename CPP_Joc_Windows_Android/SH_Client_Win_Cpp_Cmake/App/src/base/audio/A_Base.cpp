#include "A_Base.h"

using namespace base::audio;

A_Base::A_Base(IApp* app, base::audio::INativeAudio* nativeAudio)
	: super(),
	app(app),
	nativeAudio(nativeAudio)
{
	//void
}

void A_Base::loopTick() {
	nativeAudio->loop_soundsRemoval();
}

A_Base::~A_Base() {
	//void
}
