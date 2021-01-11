#pragma once

#include <base/gh.h>
#include "IInputLayer.h"
#include <base/input/util/Touch.h>

namespace base {
class IBlockingInputLayer : virtual public IInputLayer {
    pub class IHandler{
        pub virtual bool getIsCursorModeSupported(base::Touch::CursorMode* cursorMode) = 0;
        pub virtual void onCursorModeChanged(IBlockingInputLayer* target) = 0;

        pub virtual bool getIsSensorSupported(base::Touch::SensorType* sensorId) = 0;
        pub virtual void onIsSensorActiveChanged(base::Touch::SensorType* sensorId, bool isActive) = 0;

        pub virtual ~IHandler() = default;
    };

    pub virtual IHandler* getHandler() = 0;
    pub virtual void setHandler(IHandler* handler) = 0;

    pub virtual bool getIsSensorActive(base::Touch::SensorType* sensorId) = 0;
    pub virtual bool getIsSensorSupported(base::Touch::SensorType* sensorId) = 0;
    pub virtual void setIsSensorActive(base::Touch::SensorType* sensorId, bool isActive) = 0;

    pub virtual bool distributeInputCacheEntry(std::shared_ptr<base::SensorEvent>& inputCacheEntry) = 0;

    pub ~IBlockingInputLayer() override = default;

};
};
