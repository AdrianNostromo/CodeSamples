#pragma once

#include <base/gh.h>
#include <string>
#include <memory>
#include <functional>
#include <base/object/IWrappedValue.h>
#include <base/event/listener/EventListener.h>

class IEventListener;

namespace base {
class IRuntimeSettings {
    pub class EventRuntimeSetting {pub dCtor(EventRuntimeSetting);
        pub std::string settingId;
        // Can be nullptr if setting not initialised or when a existing entry with a non-nullptr wValue was cleared.
        pub std::shared_ptr<IWrappedValue> newWValue;

        pub explicit EventRuntimeSetting(std::string& settingId, std::shared_ptr<IWrappedValue> newWValue)
            : settingId(settingId), newWValue(newWValue)
        {};

        pub virtual ~EventRuntimeSetting()
        {};
    };
    
    // Global access version. Use the variable from app where possible.
    pub static IRuntimeSettings* runtimeSettings;

    pub virtual bool getSettingBool(std::string& settingId, bool notFoundValue) = 0;
    pub virtual void setSetting(std::string& settingId, bool value) = 0;
    // Returns true if entry existed, false if entry not found.
    pub virtual bool clearSettingIfExists(std::string& settingId) = 0;

    pub virtual std::shared_ptr<IEventListener> addListener(
        std::string& settingId,
        std::function<void(IEventListener& eventListener, EventRuntimeSetting& event)> cb) = 0;

    pub virtual ~IRuntimeSettings() = default;
};
};
