#pragma once

#include <base/gh.h>
#include "IDebugEntryUser.h"
#include <string>
#include <base/list/ArrayList.h>
#include <functional>
#include <base/object/IWrappedValue.h>
#include <functional>

namespace base {
    class IDebugEntry;
};

namespace base {
class DebugEntryUser : public virtual IDebugEntryUser {pub dCtor(DebugEntryUser);
    pub IDebugEntry* debugEntry;

    pub std::string placeholderedText = "";

    pub ArrayList<std::shared_ptr<base::IWrappedValue>> wrappedValuesList{};

    pub std::function<void(
        IDebugEntryUser* debugEntryUser, bool newState)> cb_onToggleSwitchStateChanged = nullptr;
    pub std::function<void(
        IDebugEntryUser* debugEntryUser)> cb_onBtnCursorDown = nullptr;

    pub explicit DebugEntryUser(IDebugEntry* debugEntry);

    pub bool getIsConnected() final;
    pub void remove() final;

    pub void setLinesCount(int linesCount) final;

    pub void setPlaceholderedText(std::string placeholderedText) final;
    pub void appendTrackedDataValue(std::shared_ptr<base::IWrappedValue> trackedWrappedValue) final;
    pub void invalidateData() final;

    pub void setToggleSwitchIsOn(bool isOn) final;
    pub void setCb_onToggleSwitchStateChanged(
        std::function<void(
            IDebugEntryUser* debugEntryUser, bool newState)> cb_onToggleSwitchStateChanged
    ) final;
    pub void setCb_onBtnCursorDown(
        std::function<void(
            IDebugEntryUser* debugEntryUser)> cb_onBtnCursorDown
    ) final;

    pub std::string getText();

    pub ~DebugEntryUser() override;
};
};
