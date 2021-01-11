#include "CMLogicBlocks.h"
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <rpg3D/gw/entity/module/controller/logicBlock/ILogicBlock.h>
#include <worldGame3D/gw/entity/event/IPluggableEventsDispatcher.h>

using namespace rpg3D;

CMLogicBlocks::CMLogicBlocks(
	IWorldEntity* entity,
	ControllerTemplate* t)
	: super(entity, t)
{
	e->getGWEvents()->addEventListener(
		base::IGameWorld::GWEvent_gameLoop_aiLogic->type,
		std::bind(&CMLogicBlocks::onGWEvent_gameLoop_aiLogic, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
}

void CMLogicBlocks::connectToGameParts(worldGame3D::IGameWorldGameParts* gwGameParts) {
	super::connectToGameParts(gwGameParts);

	this->gwGamePartsOptional = gwGameParts;

	for (int i = 0; i < logicBlocksList.count(); i++) {
		ILogicBlock* lBlock = logicBlocksList.getDirect(i);

		if (lBlock->getIsActive()) {
			lBlock->connectToGameParts(gwGameParts);
		}
	}
}

void CMLogicBlocks::disconnectFromGamePartsPre() {
	this->gwGamePartsOptional = nullptr;

	for (int i = 0; i < logicBlocksList.count(); i++) {
		ILogicBlock* lBlock = logicBlocksList.getDirect(i);

		// This always occurs to handle optimisations cleanup.
		lBlock->disconnectFromGamePartsPre();
	}

	super::disconnectFromGamePartsPre();
}

void* CMLogicBlocks::getLogicBlockMustExist(int id) {
	if (id < 0) {
		throw LogicException(LOC);
	}

	void* lBlock = logicBlocksMap.getDirect(id, nullptr);
	if (lBlock == nullptr) {
		throw NotFoundException();
	}

	return lBlock;
}

void* CMLogicBlocks::getLogicBlockOptional(int id) {
	if (id < 0) {
		throw LogicException(LOC);
	}

	void* lBlock = logicBlocksMap.getDirect(id, nullptr);

	return lBlock;
}

void CMLogicBlocks::registerLogicBlock(ILogicBlock* logicBlock, int id, void* logicBlockInterfaceRawPtr) {
	if (logicBlockInterfaceRawPtr == nullptr || id < 0) {
		throw LogicException(LOC);
	}

	if (logicBlocksMap.containsKey(id)) {
		throw LogicException(LOC);
	}

	logicBlocksMap.putDirect(id, logicBlockInterfaceRawPtr);

	this->logicBlocksList.appendReference(logicBlock);

	logicBlock->setEntity(e, this);
}

void CMLogicBlocks::setSingleActiveGroupLogicBlock(int groupID, int lbID) {
	// Enumerate all lb-s from the group and a activate a single one.
	for (int i = 0; i < logicBlocksList.count(); i++) {
		ILogicBlock* lBlock = logicBlocksList.getDirect(i);

		if (lBlock->getGroupID() == groupID) {
			if (lBlock->getId() != lbID) {
				// Should be innactive.
				if (lBlock->getIsActive()) {
					lBlock->setIsActive(false);
				}
			}
		}
	}

	// Use a separate loop for activation to neverr have 2 lb-s active of a certain groupId.
	bool isTargetLBFound = false;
	for (int i = 0; i < logicBlocksList.count(); i++) {
		ILogicBlock* lBlock = logicBlocksList.getDirect(i);

		if (lBlock->getGroupID() == groupID) {
			if (lBlock->getId() == lbID) {
				// Should be active.
				if (isTargetLBFound) {
					// Logic block with the same id is already active.
					throw LogicException(LOC);
				}
				isTargetLBFound = true;

				if (!lBlock->getIsActive()) {
					lBlock->setIsActive(true, gwGamePartsOptional);
				}
			}
		}
	}

	if (lbID >= 0 && !isTargetLBFound) {
		// Logic block not found but requested.
		throw LogicException(LOC);
	}
}

void CMLogicBlocks::onGWEvent_gameLoop_aiLogic(IEventListener& eventListener, base::GWEvent& event) {
	for (int i = 0; i < logicBlocksList.count(); i++) {
		ILogicBlock* lBlock = logicBlocksList.getDirect(i);

		lBlock->logicTickIfNeeded(event.deltaS);
	}
}

void CMLogicBlocks::disposePre() {
	for (int i = 0; i < logicBlocksList.count(); i++) {
		ILogicBlock* lBlock = logicBlocksList.getDirect(i);

		if (lBlock->getIsActive()) {
			lBlock->setIsActive(false);
		}

		lBlock->setEntity(nullptr, nullptr);

		lBlock->reservedDisposePre();
	}

	super::disposePre();
}

void CMLogicBlocks::disposeMain() {
	logicBlocksMap.clear();

	for (int i = 0; i < logicBlocksList.count(); i++) {
		ILogicBlock* lBlock = logicBlocksList.getDirect(i);

		lBlock->reservedDisposeMain();
		delete lBlock;
	}
	logicBlocksList.clear();

	super::disposeMain();
}

CMLogicBlocks::~CMLogicBlocks() {
	//void
}
