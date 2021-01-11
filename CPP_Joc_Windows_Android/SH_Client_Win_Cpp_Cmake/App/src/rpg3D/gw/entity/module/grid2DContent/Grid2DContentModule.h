#pragma once

#include <base/gh.h>
#include "G2DCM_Base.h"
#include <base/math/Vector2Int.h>

namespace base {
	class EntityEvent;
};
namespace rpg3D {
	class IGPGrid2DContent;
};

namespace rpg3D {
class Grid2DContentModule : public G2DCM_Base {priv typedef G2DCM_Base super;pub dCtor(Grid2DContentModule);
	prot IGPGrid2DContent* gpGrid2DContent = nullptr;
	priv IListEntry* gridListEntry = nullptr;
	
	priv IListEntry* inGCell_listEntry = nullptr;
	priv Vector2Int gPos{};
	priv Bounds2D gCellBounds{};

	priv ArrayList<std::shared_ptr<IEventListener>> localAutoListenersList{};

    pub explicit Grid2DContentModule(
	    IWorldEntity* entity,
	    Grid2DContentTemplate* t);
	prot void createBMain() override;
	prot void connectToGameParts(worldGame3D::IGameWorldGameParts* gwGameParts) override;
	prot void disconnectFromGamePartsPre() override;

	pub IListEntry*& getGridListEntryReference() final;
	
	pub Vector2Int& getGPosRef(bool mustBeInAGCell = true) final;
	pub Bounds2D& getGCellBoundsRef(bool mustBeInAGCell = true) final;
	pub IListEntry*& getInGCell_listEntryRef() final;

	priv void onEvent(IEventListener& eventListener, base::EntityEvent& event);

	prot void disposePre() override;
	pub ~Grid2DContentModule() override;
};
};
