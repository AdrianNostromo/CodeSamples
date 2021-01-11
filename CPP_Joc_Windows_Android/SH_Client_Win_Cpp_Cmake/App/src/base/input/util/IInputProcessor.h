#pragma once

#include <memory>

class IEventListener;
namespace base {
    class SensorEvent;
}

class IInputProcessor {
public:
    virtual void onSensorEvent(std::shared_ptr<base::SensorEvent>& sensorEvent) = 0;

    virtual ~IInputProcessor() = default;

};
