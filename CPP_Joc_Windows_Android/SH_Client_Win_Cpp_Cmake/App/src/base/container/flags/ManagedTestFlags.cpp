#include "ManagedTestFlags.h"
#include <base/exceptions/LogicException.h>
#include <base/object/WrappedValue.h>
#include <base/container/flags/TestFlagEntry.h>

using namespace base;

ManagedTestFlags::ManagedTestFlags() {
    //void
}

int ManagedTestFlags::count() {
	return entriesList.count();
}

void ManagedTestFlags::clear() {
	entriesMap.clear();
	entriesList.clear();
}

bool ManagedTestFlags::usesFlag(std::string& flagName) {
	for (int i = 0; i < entriesList.count(); i++) {
		std::shared_ptr<base::TestFlagEntry> testFlagEntry = entriesList.getDirect(i);

		if (testFlagEntry->name == flagName) {
			return true;
		}
	}

	return false;
}

TestFlagEntry* ManagedTestFlags::getOrCreateEntry(std::string& name) {
	std::shared_ptr<TestFlagEntry> entry = entriesMap.getDirect(name, nullptr);
	if (entry == nullptr) {
		entry = std::make_shared<TestFlagEntry>(name);
		entriesList.appendReference(entry);
		entriesMap.putDirect(name, entry);
	}

	return entry.get();
}

void ManagedTestFlags::setFlag(std::string& name, std::shared_ptr<base::IWrappedValue> value, bool alsoPassOnNoValue, bool entryValueMustBeSet) {
	TestFlagEntry* flagEntry = getOrCreateEntry(name);

	// Note. Don't check the type to be the same.

	flagEntry->value = value;
	flagEntry->alsoPassOnNoValue = alsoPassOnNoValue;
	flagEntry->entryValueMustBeSet = entryValueMustBeSet;
}

void ManagedTestFlags::setFlag(std::string& name, bool value, bool alsoPassOnNoValue, bool entryValueMustBeSet) {
	setFlag(name, base::IWrappedValue::new_bool(value), alsoPassOnNoValue, entryValueMustBeSet);
}

void ManagedTestFlags::clearFlag(std::string& name) {
	std::shared_ptr<TestFlagEntry> entry = entriesMap.getDirect(name, nullptr);

	if(entry != nullptr) {
		if(!entriesMap.removeOptional(name)) {
			throw LogicException(LOC);
		}

		if(!entriesList.remove(entry)) {
			throw LogicException(LOC);
		}
	}
}

ArrayList<std::shared_ptr<TestFlagEntry>>& ManagedTestFlags::peekEntriesList() {
	return entriesList;
}

ManagedTestFlags::~ManagedTestFlags() {
    //void
}
