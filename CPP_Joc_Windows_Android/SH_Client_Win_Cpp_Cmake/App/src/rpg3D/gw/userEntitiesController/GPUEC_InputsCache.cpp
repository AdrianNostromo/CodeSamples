#include "GPUEC_InputsCache.h"
#include <rpg3D/gw/interaction/deviceinput/IManagedDeviceInput.h>
#include <worldGame3D/gw/interaction/IGPInteraction.h>
#include <worldGame3D/gw/IGameWorld.h>

using namespace rpg3D;

GPUEC_InputsCache::GPUEC_InputsCache(base::IGameWorld* gw)
	: super(gw)
{
	//void
}

void GPUEC_InputsCache::createMain() {
	super::createMain();

	IManagedDeviceInput* input = gw->getInteractionAs<base::IGPInteraction*>()->getInputAs<IManagedDeviceInput*>();
	input->setCb_onEntityControlInputs(std::bind(&GPUEC_InputsCache::onEntityControlInputs, this, std::placeholders::_1));
}

void GPUEC_InputsCache::onEntityControlInputs(std::shared_ptr<base::SensorEvent>& sensorEvent) {
	cachedInputsList.appendReference(sensorEvent);
}

bool GPUEC_InputsCache::processCacheInput(std::shared_ptr<base::SensorEvent>& input) {
	return false;
}

void GPUEC_InputsCache::onGameLoop_localInput(float deltaS, int deltaMS, float gameTimeS) {
	super::onGameLoop_localInput(deltaS, deltaMS, gameTimeS);

	if (controledEntityTarget == nullptr) {
		cachedInputsList.clear();

		return;
	}

	isControlledEntityChangeLocked = true;

	if (cachedInputsList.count() > 0) {
		for (int i = 0; i < cachedInputsList.count(); i++) {
			std::shared_ptr<base::SensorEvent>& cachedEntry = cachedInputsList.getReference(i);

			if (!processCacheInput(cachedEntry)) {
				throw LogicException(LOC);
			}
		}

		cachedInputsList.clear();
	}

	isControlledEntityChangeLocked = false;
}

void GPUEC_InputsCache::onEntityControlTypeChanged() {
	super::onEntityControlTypeChanged();

	isDirtyControlType = true;
}

void GPUEC_InputsCache::clearInput() {
	super::clearInput();

	cachedInputsList.clear();
}

GPUEC_InputsCache::~GPUEC_InputsCache() {
	//void
}
