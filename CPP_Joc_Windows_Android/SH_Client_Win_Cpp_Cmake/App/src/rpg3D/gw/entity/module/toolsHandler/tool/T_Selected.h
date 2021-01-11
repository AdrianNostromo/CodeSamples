#pragma once

#include <base/gh.h>
#include "T_Filters.h"

class IWorldEntity;
class IListEntry;
namespace rpg3D {
	class IToolsHandlerModule;
	class ToolSlotConfig;
};

namespace rpg3D {
class T_Selected : public T_Filters {priv typedef T_Filters super;pub dCtor(T_Selected);
	prot IWorldEntity* entity = nullptr;
	prot rpg3D::IToolsHandlerModule* toolsHandler = nullptr;

	priv bool isToolSelected = false;
	priv int selectedToolSlotIndex = -1;
	priv ToolSlotConfig* selectedToolSlot = nullptr;

	pub IListEntry* listEntry_selectedToolsList = nullptr;
	pub IListEntry* listEntry_selectedToolsList_userSelectable = nullptr;

	pub explicit T_Selected(ToolConfigBase* config);

	pub virtual IWorldEntity* getEntity();
	pub void setEntity(IWorldEntity* entity);

	prot virtual void onEntityChangePre();
	prot virtual void onEntityChanged();

	pub void selectedStateChanged(bool isToolSelected, int toolSlotIndex, ToolSlotConfig* toolSlot, worldGame3D::IGameWorldGameParts* gwGamePartsOptional);
	prot virtual void _onSelectedStateChanged(bool isToolSelected, worldGame3D::IGameWorldGameParts* gwGamePartsOptional);

	pub bool getIsToolSelected();
	pub ToolSlotConfig* getSelectedToolSlot();
	pub int getSelectedToolSlotIndex();

	prot ToolSlotConfig* peekSelectedToolSlot();

    pub ~T_Selected() override;
};
};
