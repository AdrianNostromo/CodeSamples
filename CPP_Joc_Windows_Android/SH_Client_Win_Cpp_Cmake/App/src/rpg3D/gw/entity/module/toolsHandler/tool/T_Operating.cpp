#include "T_Operating.h"
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <base/log/GlobalAccessLogHandler.h>

using namespace rpg3D;

T_Operating::T_Operating(ToolConfigBase* config)
	: super(config)
{
	//void
}

bool T_Operating::getIsOperating(int cursorIndex) {
    if (isOperating && this->operating_cursorIndex == cursorIndex) {
        return true;
    }

    return false;
}

bool T_Operating::getIsOperatingAny() {
    return isOperating;
}

int T_Operating::getOperating_cursorIndex() {
    if (!isOperating) {
        // T_Operating is inactive.
        throw LogicException(LOC);
    }

    return operating_cursorIndex;
}

void T_Operating::setIsOperating(bool isOperating, int cursorIndex) {
    if (!isOperating) {
        // Stopping requested.
        if (!this->isOperating) {
            // Already stopped.
            throw LogicException(LOC);
        }
        if (this->operating_cursorIndex != cursorIndex) {
            // Wrong stop cursor index.
            throw LogicException(LOC);
        }

        this->operating_cursorIndex = -1;
        this->isOperating = false;
    } else {
        // Enable requested.
        if (this->isOperating || cursorIndex < 0) {
            // Already active.
            throw LogicException(LOC);
        }

        this->operating_cursorIndex = cursorIndex;
        this->isOperating = true;
    }

    onIsOperatingChanged(isOperating);
}

void T_Operating::onIsOperatingChanged(bool isOperating) {
    EVENT_tool_onIsOperatingChanged->voidData = static_cast<ITool*>(this);
    entity->dispatchEvent(*EVENT_tool_onIsOperatingChanged);

    if (config->isDirectional) {
        if (!getIsOperatingAny()) {
            EVENT_directionalTool_isOperating_disabled->voidData = static_cast<ITool*>(this);
            entity->dispatchEvent(*EVENT_directionalTool_isOperating_disabled);
        } else {
            EVENT_directionalTool_isOperating_enabled->voidData = static_cast<ITool*>(this);
            entity->dispatchEvent(*EVENT_directionalTool_isOperating_enabled);
        }
    }
}

void T_Operating::onEntityChangePre() {
    super::onEntityChangePre();
    
    if (entity != nullptr) {
        if (getIsOperatingAny()) {
            setIsOperating(false, getOperating_cursorIndex());
        }
    }
}

void T_Operating::onSelectedAndGWCapabilitiesEnabledStateChanged(bool isSelectedAndGWCapabilitiesExist, worldGame3D::IGameWorldGameParts* gwGamePartsOptional) {
    super::onSelectedAndGWCapabilitiesEnabledStateChanged(isSelectedAndGWCapabilitiesExist, gwGamePartsOptional);

    if (!isSelectedAndGWCapabilitiesExist) {
        if (getIsOperatingAny()) {
            setIsOperating(false, getOperating_cursorIndex());
        }
    }
}

T_Operating::~T_Operating() {
	//void
}
