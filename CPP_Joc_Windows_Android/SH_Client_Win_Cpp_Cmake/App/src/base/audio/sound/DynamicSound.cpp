#include "DynamicSound.h"

using namespace base::audio;

DynamicSound::DynamicSound(IAssetsManager* assetsManager)
	: assetsManager(assetsManager)
{
	//void
}

bool DynamicSound::play(float volume, float overlapCooldownS) {
	if (sound != nullptr) {
		return sound->play(volume, overlapCooldownS);
	}

	return false;
}

std::string* DynamicSound::getSound_id() {
	return sound_id;
}

void DynamicSound::setSound_id(std::string* sound_id) {
	this->sound_id = sound_id;

	if (this->sound_id != nullptr) {
		sound = assetsManager->getSound(*this->sound_id);
		if (sound == nullptr) {
			throw LogicException(LOC);
		}
	}else {
		sound = nullptr;
	}
}

DynamicSound::~DynamicSound() {
	//void
}
