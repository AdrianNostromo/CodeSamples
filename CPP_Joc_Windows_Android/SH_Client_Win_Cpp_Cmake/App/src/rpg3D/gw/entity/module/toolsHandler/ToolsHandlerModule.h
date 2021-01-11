#pragma once

#include <memory>
#include <base/list/ArrayList.h>
#include "THM_Activation.h"

namespace rpg3D {
class ToolsHandlerModule : public THM_Activation {
private: typedef THM_Activation super;
public:
	ToolsHandlerModule(ToolsHandlerModule const&) = delete;
    ToolsHandlerModule(ToolsHandlerModule &&) = default;
    ToolsHandlerModule& operator=(ToolsHandlerModule const&) = delete;
    ToolsHandlerModule& operator=(ToolsHandlerModule &&) = default;

    explicit ToolsHandlerModule(
	    IWorldEntity* entity,
	    ToolsHandlerTemplate* t);

	~ToolsHandlerModule() override;
};
};
