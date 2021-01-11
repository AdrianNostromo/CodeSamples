#pragma once

#include <base/gh.h>
#include <memory>
#include <base/object/IWrappedValue.h>
#include <base/event/dispatcher/EventDispatcher.h>
#include <base/container/flags/event/FlagsEvent.h>

namespace base {
class FlagEntry {pub dCtor(FlagEntry);
    pub static base::FlagsEvent* FlagsEvent_flagChanged;

	pub std::string name;

	// Entry doesn't exist if this value is nullptr or this Entry doesn't exist for that value.
    priv std::shared_ptr<base::IWrappedValue> value = nullptr;

    pub base::EventDispatcher<base::FlagsEvent> eventsDispatcher{};

    pub explicit FlagEntry(std::string& name);

    pub std::shared_ptr<base::IWrappedValue> getValue();
    pub void setValue(std::shared_ptr<base::IWrappedValue> value);

    pub base::EventDispatcher<base::FlagsEvent>& getEventsDispatcher();

    pub virtual ~FlagEntry();
};
};
