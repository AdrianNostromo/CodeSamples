#pragma once

#include <base/gh.h>
#include <base/object/Object.h>
#include "ILogicBlock.h"
#include <base/list/ArrayList.h>
#include <memory>
#include <base/event/listener/IEventListener.h>

class IWorldEntity;
namespace rpg3D {
class IControllerModule;
}

namespace rpg3D {
class LogicBlock : public base::Object, virtual public ILogicBlock {priv typedef base::Object super;pub dCtor(LogicBlock);
	prot IWorldEntity* entity = nullptr;
	prot IWorldEntity* e = nullptr;
	//asd_1;// remove after compilation if not needed;
	prot rpg3D::IControllerModule* controller = nullptr;

	// <=0.0f; Max framerate.
	priv float tickCooldownS;
	priv float bufferredLogicTickDeltaS;

	priv bool isActive = false;
	// This is used to switch zone speciffic game_parts that are used by this logic block when the active zone changes.
	priv bool isZoneGamePartsReady = false;

	// This contains event listeners from this module and are auto disposed on disposePre;
	prot ArrayList<std::shared_ptr<IEventListener>> autoListenersList_whileActive{};

    pub explicit LogicBlock(
		float tickCooldownMS
	);

	pub void setEntity(IWorldEntity* entity, rpg3D::IControllerModule* controller) final;
	prot virtual void onEntityChanged();

	pub void logicTickIfNeeded(float deltaS) final;
	pub virtual void logicTick(float deltaS);

	pub bool getIsActive() final;
	pub void setIsActive(bool isActive, worldGame3D::IGameWorldGameParts* gwGameParts=nullptr) final;
	prot virtual void onIsActiveChanged(bool isActive_new, worldGame3D::IGameWorldGameParts* gwGameParts);

	pub bool getIsZoneGamePartsReady();

	pub void connectToGameParts(worldGame3D::IGameWorldGameParts* gwGameParts) override;
	pub void disconnectFromGamePartsPre() override;
	prot virtual void disconnectedFromGameParts();

	pub void reservedDisposePre() final;
	prot virtual void disposePre();
	pub ~LogicBlock() override;
};
};
