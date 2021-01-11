#pragma once

#include <base/gh.h>
#include <base/list/ArrayList.h>
#include <base/map/Map1D.h>
#include <string>
#include <memory>
#include <base/object/IWrappedValue.h>
#include <base/event/dispatcher/EventDispatcher.h>
#include <base/container/flags/event/FlagsEvent.h>
#include <base/container/flags/IManagedFlags.h>

namespace base {
    class FlagEntry;
};

namespace base {
class ManagedFlags : public virtual IManagedFlags {pub dCtor(ManagedFlags);
	priv ArrayList<std::shared_ptr<FlagEntry>> entriesList{};
    priv Map1D<std::string, std::shared_ptr<FlagEntry>> entriesMap{};

    pub explicit ManagedFlags();

    pub int count() final;

    pub void clear() final;

	pub FlagEntry* getOrCreateEntry(std::string& name) final;

    pub bool test(std::string& name, std::shared_ptr<base::IWrappedValue>& value, bool alsoPassOnNoValue, bool entryValueMustBeSet = false) final;
    pub bool test(TestFlagEntry& testFlagEntry) final;
    pub bool test(ManagedTestFlags& managedTestFlags) final;

    pub void setFlag(std::string& name, std::shared_ptr<base::IWrappedValue> value) final;
    pub void setFlag(std::string& name, bool value) final;
    pub void setFlags(IManagedFlags& managedFlags) final;

    pub void clearFlag(std::string& name) final;

    pub ArrayList<std::shared_ptr<FlagEntry>>& peekEntriesList() final;

    pub virtual ~ManagedFlags();
};
};
