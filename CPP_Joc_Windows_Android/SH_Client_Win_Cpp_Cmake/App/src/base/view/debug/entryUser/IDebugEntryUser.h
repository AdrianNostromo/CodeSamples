#pragma once

#include <base/gh.h>
#include <string>
#include <memory>
#include <functional>

namespace base {
    class IWrappedValue;
};

namespace base {
class IDebugEntryUser {
    pub virtual bool getIsConnected() = 0;
    pub virtual void remove() = 0;

    pub virtual void setLinesCount(int linesCount) = 0;

    pub virtual void setPlaceholderedText(std::string placeholderedText) = 0;
    pub virtual void appendTrackedDataValue(std::shared_ptr<IWrappedValue> trackedWrappedValue) = 0;

    pub virtual void invalidateData() = 0;

    pub virtual void setToggleSwitchIsOn(bool isOn) = 0;
    pub virtual void setCb_onToggleSwitchStateChanged(
        std::function<void(
            IDebugEntryUser* debugEntryUser, bool newState)> cb_onToggleSwitchStateChanged
    ) = 0;
    pub virtual void setCb_onBtnCursorDown(
        std::function<void(
            IDebugEntryUser* debugEntryUser)> cb_onBtnCursorDown
    ) = 0;

    pub virtual ~IDebugEntryUser() = default;
};
};
