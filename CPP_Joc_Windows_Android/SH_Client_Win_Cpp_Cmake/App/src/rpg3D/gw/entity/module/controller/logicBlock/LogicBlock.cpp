#include "LogicBlock.h"

using namespace rpg3D;

LogicBlock::LogicBlock(
	float tickCooldownS)
	: super(),
	tickCooldownS(tickCooldownS), bufferredLogicTickDeltaS(tickCooldownS)
{
	//void
}

void LogicBlock::setEntity(IWorldEntity* entity, rpg3D::IControllerModule* controller) {
	if (this->isActive) {
		// Not allowed to change the entity while active.
		throw LogicException(LOC);
	}
	if (this->entity == entity) {
		throw LogicException(LOC);
	}
	if (this->entity != nullptr && entity != nullptr) {
		// Must remove from previous entity before setting to the new one.
		throw LogicException(LOC);
	}

	this->entity = entity;
	this->e = entity;
	this->controller = controller;

	onEntityChanged();
}

void LogicBlock::onEntityChanged() {
	//void
}

bool LogicBlock::getIsActive() {
	return isActive;
}

void LogicBlock::setIsActive(bool isActive, worldGame3D::IGameWorldGameParts* gwGameParts) {
	if (this->isActive == isActive) {
		return;
	}

	this->isActive = isActive;
	this->isZoneGamePartsReady = gwGameParts != nullptr ? true : false;

	onIsActiveChanged(this->isActive, gwGameParts);
}

void LogicBlock::onIsActiveChanged(bool isActive_new, worldGame3D::IGameWorldGameParts* gwGameParts) {
	if (!isActive) {
		autoListenersList_whileActive.clear();
	}
}

bool LogicBlock::getIsZoneGamePartsReady() {
	return isZoneGamePartsReady;
}

void LogicBlock::connectToGameParts(worldGame3D::IGameWorldGameParts* gwGameParts) {
	this->isZoneGamePartsReady = true;
}

void LogicBlock::disconnectFromGamePartsPre() {
	this->isZoneGamePartsReady = false;

	disconnectedFromGameParts();
}

void LogicBlock::disconnectedFromGameParts() {
	//void
}

void LogicBlock::logicTickIfNeeded(float deltaS) {
	bufferredLogicTickDeltaS += deltaS;

	if (tickCooldownS < 0.0f || bufferredLogicTickDeltaS >= tickCooldownS) {
		logicTick(bufferredLogicTickDeltaS);

		bufferredLogicTickDeltaS = 0.0f;
	}
}

void LogicBlock::logicTick(float deltaS) {
	//void
}

void LogicBlock::reservedDisposePre() {
	disposePre();
}

void LogicBlock::disposePre() {
	if (isActive) {
		// The logic block must be deactivated before it is disposed_pre.
		throw LogicException(LOC);
	}
}

LogicBlock::~LogicBlock() {
	//void
}
