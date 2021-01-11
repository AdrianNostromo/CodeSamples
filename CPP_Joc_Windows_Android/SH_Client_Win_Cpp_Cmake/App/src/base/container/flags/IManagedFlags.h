#pragma once

#include <base/gh.h>
#include <string>
#include <memory>
#include <base/object/IWrappedValue.h>
#include <base/list/ArrayList.h>

namespace base {
    class FlagEntry;
    class TestFlagEntry;
    class ManagedTestFlags;
};

namespace base {
class IManagedFlags {
    pub virtual int count() = 0;

    pub virtual void clear() = 0;

    pub virtual FlagEntry* getOrCreateEntry(std::string& name) = 0;

    pub virtual bool test(std::string& name, std::shared_ptr<base::IWrappedValue>& value, bool alsoPassOnNoValue, bool entryValueMustBeSet = false) = 0;
    pub virtual bool test(TestFlagEntry& testFlagEntry) = 0;
    pub virtual bool test(ManagedTestFlags& managedTestFlags) = 0;

    pub virtual void setFlag(std::string& name, std::shared_ptr<base::IWrappedValue> value) = 0;
    pub virtual void setFlag(std::string& name, bool value) = 0;
    pub virtual void setFlags(IManagedFlags& managedFlags) = 0;

    pub virtual void clearFlag(std::string& name) = 0;

    pub virtual ArrayList<std::shared_ptr<FlagEntry>>& peekEntriesList() = 0;

    pub virtual ~IManagedFlags() = default;
};
};
