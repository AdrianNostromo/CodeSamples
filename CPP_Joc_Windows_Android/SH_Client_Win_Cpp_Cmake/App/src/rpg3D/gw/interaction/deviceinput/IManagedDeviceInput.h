#pragma once

#include <base/gh.h>
#include <functional>
#include <worldGame3D/gw/interaction/deviceinput/IManagedDeviceInput.h>
#include <base/math/Vector2.h>
#include <base/math/Vector3.h>
#include <base/input/sensor/event/SensorEvent.h>
#include <memory>
#include <base/input/util/Touch.h>

namespace base {
	class IBlockingInputLayer;
};

namespace rpg3D {
class IManagedDeviceInput : virtual public base::IManagedDeviceInput {
	pub class CustomSensorTypes {
		pub static base::Touch::SensorType* LocalisedScreenCursor;
		pub static base::Touch::SensorType* DirectionalMovement;
		pub static base::Touch::SensorType* SensitizedLockedCursor;
		pub static base::Touch::SensorType* ToolExtraActivation;
	};

	pub virtual void setCb_onEntityControlInputs(std::function<void(
		std::shared_ptr<base::SensorEvent>& sensorEvent)> cb_onEntityControlInputs) = 0;

	pub virtual void clearInput() = 0;

	pub virtual bool getIsCursorBlockedModeSupported() = 0;
	pub virtual void setIsCursorBlockedMode(bool isCursorBlockedMode) = 0;

	pub virtual bool getIsSensorSupported(base::Touch::SensorType* sensorId) = 0;
	pub virtual void setIsSensorEnabled(base::Touch::SensorType* sensorId, bool isEnabled) = 0;

	pub virtual base::IBlockingInputLayer* getBlockingInputLayer() = 0;

	pub ~IManagedDeviceInput() override = default;
};
};
