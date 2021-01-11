#pragma once

#include <base/input/IInputHandler.h>

class IAppInput {
public:
    virtual IInputHandler* getInputHandler() = 0;

    virtual ~IAppInput() = default;
};
