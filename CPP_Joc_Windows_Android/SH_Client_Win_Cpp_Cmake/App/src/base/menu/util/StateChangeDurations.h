#pragma once

#include <string>
#include <base/list/Array1D.h>
#include <memory>
#include "UiId.h"

class StateChangeDurations {
public:
    // String or String[]
    UiId* startComposedStateID;
    // String or String[]
    UiId* endComposedStateID;

    float durationS;

public:
    StateChangeDurations(StateChangeDurations const&) = delete;
    StateChangeDurations(StateChangeDurations &&) = default;
    StateChangeDurations& operator=(StateChangeDurations const&) = delete;
    StateChangeDurations& operator=(StateChangeDurations &&) = default;
    explicit StateChangeDurations(std::string startComposedStateID, std::string endComposedStateID, float durationS);

    virtual ~StateChangeDurations();
};
