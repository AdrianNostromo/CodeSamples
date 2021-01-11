#include "ManagedFlags.h"
#include <base/exceptions/LogicException.h>
#include <base/object/WrappedValue.h>
#include <base/container/flags/FlagEntry.h>
#include <base/container/flags/TestFlagEntry.h>
#include <base/container/flags/ManagedTestFlags.h>

using namespace base;

ManagedFlags::ManagedFlags() {
    //void
}

int ManagedFlags::count() {
	return entriesList.count();
}

void ManagedFlags::clear() {
	entriesMap.clear();
	entriesList.clear();
}

FlagEntry* ManagedFlags::getOrCreateEntry(std::string& name) {
	std::shared_ptr<FlagEntry> entry = entriesMap.getDirect(name, nullptr);
	if (entry == nullptr) {
		entry = std::make_shared<FlagEntry>(name);
		entriesList.appendReference(entry);
		entriesMap.putDirect(name, entry);
	}

	return entry.get();
}

bool ManagedFlags::test(std::string& name, std::shared_ptr<base::IWrappedValue>& value, bool alsoPassOnNoValue, bool entryValueMustBeSet) {
	std::shared_ptr<FlagEntry> entry = entriesMap.getDirect(name, nullptr);

	if (entry == nullptr || entry->getValue() == nullptr) {
		if (entryValueMustBeSet) {
			// The flag must be set on this pattern. The tester requires it to exist with a valid value.
			// This is correct behaviour and logic works as intended when this is false.
			throw LogicException(LOC);
		}

		if (alsoPassOnNoValue) {
			return true;
		}

		return false;
	}else if (!entry->getValue()->equals(value)) {
		return false;
	}

	return true;
}

bool ManagedFlags::test(TestFlagEntry& testFlagEntry) {
	return test(
		testFlagEntry.name,
		testFlagEntry.value,
		testFlagEntry.alsoPassOnNoValue,
		testFlagEntry.entryValueMustBeSet
	);
}

bool ManagedFlags::test(ManagedTestFlags& managedTestFlags) {
	ArrayList<std::shared_ptr<TestFlagEntry>>& testFlagsList = managedTestFlags.peekEntriesList();

	for(int i=0;i< testFlagsList.count();i++) {
		std::shared_ptr<TestFlagEntry> testFlagEntry = testFlagsList.getDirect(i);

		bool b = test(
			testFlagEntry->name,
			testFlagEntry->value,
			testFlagEntry->alsoPassOnNoValue,
			testFlagEntry->entryValueMustBeSet
		);
		if(!b) {
			return false;
		}
	}

	return true;
}

void ManagedFlags::setFlag(std::string& name, std::shared_ptr<base::IWrappedValue> value) {
	FlagEntry* flagEntry = getOrCreateEntry(name);

	// Note. Don't check the type to be the same.

	flagEntry->setValue(value);
}

void ManagedFlags::setFlag(std::string& name, bool value) {
	setFlag(name, base::IWrappedValue::new_bool(value));
}

void ManagedFlags::setFlags(IManagedFlags& managedFlags) {
	ArrayList<std::shared_ptr<FlagEntry>>& testFlagsList = managedFlags.peekEntriesList();

	for (int i = 0; i < testFlagsList.count(); i++) {
		std::shared_ptr<FlagEntry> flagEntry = testFlagsList.getDirect(i);

		setFlag(
			flagEntry->name,
			flagEntry->getValue()
		);
	}
}

void ManagedFlags::clearFlag(std::string& name) {
	std::shared_ptr<FlagEntry> entry = entriesMap.getDirect(name, nullptr);

	if(entry != nullptr) {
		if(!entriesMap.removeOptional(name)) {
			throw LogicException(LOC);
		}

		if(!entriesList.remove(entry)) {
			throw LogicException(LOC);
		}
	}
}

ArrayList<std::shared_ptr<FlagEntry>>& ManagedFlags::peekEntriesList() {
	return entriesList;
}

ManagedFlags::~ManagedFlags() {
    //void
}
