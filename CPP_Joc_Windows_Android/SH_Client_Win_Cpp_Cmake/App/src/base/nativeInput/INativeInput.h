#pragma once

#include "IHapticFeedbackManager.h"
#include <base/input/util/IInputProcessor.h>
#include <base/input/util/Touch.h>

class INativeInput : virtual public base::IHapticFeedbackManager {
public:
    virtual IInputProcessor* getInputProcessor() = 0;
    virtual void setInputProcessor(IInputProcessor* rawInputProcessor) = 0;

    virtual base::Touch::CursorMode* getCursorMode() = 0;
    virtual bool getIsCursorModeSupported(base::Touch::CursorMode* cursorMode) = 0;
    virtual void setCursorMode(base::Touch::CursorMode* cursorMode) = 0;

    virtual bool getIsSensorActive(base::Touch::SensorType* sensorId) = 0;
    virtual bool getIsSensorSupported(base::Touch::SensorType* sensorId) = 0;
    virtual void setIsSensorActiveIncremental(base::Touch::SensorType* sensorId, bool isActive) = 0;

    virtual bool getIsHapticFeedbackSupported() = 0;

    ~INativeInput() override = default;
};
