#pragma once

#include <base/gh.h>
#include <base/list/ArrayList.h>
#include <base/map/Map1D.h>
#include <string>
#include <memory>
#include <base/object/IWrappedValue.h>
#include <base/event/dispatcher/EventDispatcher.h>
#include <base/container/flags/event/FlagsEvent.h>

namespace base {
    class TestFlagEntry;
};

namespace base {
class ManagedTestFlags {pub dCtor(ManagedTestFlags);
	priv ArrayList<std::shared_ptr<TestFlagEntry>> entriesList{};
    priv Map1D<std::string, std::shared_ptr<TestFlagEntry>> entriesMap{};

    pub explicit ManagedTestFlags();

    pub int count();

    pub void clear();

    pub bool usesFlag(std::string& flagName);

	pub TestFlagEntry* getOrCreateEntry(std::string& name);

    pub void setFlag(std::string& name, std::shared_ptr<base::IWrappedValue> value, bool alsoPassOnNoValue, bool entryValueMustBeSet);
    pub void setFlag(std::string& name, bool value, bool alsoPassOnNoValue, bool entryValueMustBeSet);

    pub void clearFlag(std::string& name);
    
    pub ArrayList<std::shared_ptr<TestFlagEntry>>& peekEntriesList();

    pub virtual ~ManagedTestFlags();
};
};
