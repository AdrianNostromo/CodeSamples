#pragma once

#include <base/gh.h>
#include "CMBase.h"

namespace base {
	class GWEvent;
};

namespace rpg3D {
class CMLogicBlocks : public CMBase {priv typedef CMBase super;pub dCtor(CMLogicBlocks);
    prot ArrayList<ILogicBlock*> logicBlocksList{};
	prot Map1D<int, void*> logicBlocksMap{};

	priv worldGame3D::IGameWorldGameParts* gwGamePartsOptional = nullptr;

	pub explicit CMLogicBlocks(
	    IWorldEntity* entity,
	    ControllerTemplate* t);
	prot void connectToGameParts(worldGame3D::IGameWorldGameParts* gwGameParts) override;
	prot void disconnectFromGamePartsPre() override;

	pub void* getLogicBlockMustExist(int id) final;
	pub void* getLogicBlockOptional(int id) final;

	pub void registerLogicBlock(ILogicBlock* logicBlock, int id, void* logicBlockInterfaceRawPtr) final;
	pub void setSingleActiveGroupLogicBlock(int groupID, int lbID) final;

	priv void onGWEvent_gameLoop_aiLogic(IEventListener& eventListener, base::GWEvent& event);

	prot void disposePre() override;
	prot void disposeMain() override;
	pub ~CMLogicBlocks() override;
};
};
