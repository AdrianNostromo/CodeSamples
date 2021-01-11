#pragma once

#include <base/gh.h>
#include <memory>
#include <base/object/IWrappedValue.h>
#include <base/event/dispatcher/EventDispatcher.h>
#include <base/container/flags/event/FlagsEvent.h>

namespace base {
class TestFlagEntry {pub dCtor(TestFlagEntry);
	pub std::string name;

	// Entry doesn't exist if this value is nullptr or this Entry doesn't exist for that value.
    pub std::shared_ptr<base::IWrappedValue> value = nullptr;
    // This is used to also pass when the value is not set.
    pub bool alsoPassOnNoValue = false;

    pub bool entryValueMustBeSet;
    
    pub explicit TestFlagEntry(std::string& name);

    pub virtual ~TestFlagEntry();
};
};
