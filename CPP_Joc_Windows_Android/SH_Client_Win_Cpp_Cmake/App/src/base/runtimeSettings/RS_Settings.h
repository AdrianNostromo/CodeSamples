#pragma once

#include <base/gh.h>
#include "RS_Base.h"
#include <base/map/Map1D.h>
#include <memory>
#include <base/event/dispatcher/EventDispatcher.h>

namespace base {
class RS_Settings : public RS_Base {priv typedef RS_Base super;pub dCtor(RS_Settings);
    priv class SettingEntry {pub dCtor(SettingEntry);
        // Can be nullptr if only listeners exist on an uninitialised setting entry.
        pub std::shared_ptr<IWrappedValue> wValue;

        pub EventDispatcher<EventRuntimeSetting> dispatcher{};

        pub explicit SettingEntry()
            : wValue(nullptr)
        {};

        pub ~SettingEntry() {
            dispatcher.reservedDispose();
        };

    };

    priv Map1D<std::string, std::shared_ptr<SettingEntry>> settingsMap{};

    pub explicit RS_Settings();

    priv std::shared_ptr<SettingEntry> getOrCreateSettingEntry(std::string& settingId);

    pub bool getSettingBool(std::string& settingId, bool notFoundValue) final;
    pub void setSetting(std::string& settingId, bool value) final;
    pub bool clearSettingIfExists(std::string& settingId) final;

    pub std::shared_ptr<IEventListener> addListener(
        std::string& settingId,
        std::function<void(IEventListener& eventListener, EventRuntimeSetting& event)> cb) final;

    pub ~RS_Settings() override;
};
};
