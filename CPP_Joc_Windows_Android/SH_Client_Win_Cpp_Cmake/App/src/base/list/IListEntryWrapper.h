#pragma once

class IListEntryWrapper {
public:
	// On this call, only remove the list entry, don't do much logic.
	virtual void onListEntryRemoved() = 0;

	virtual ~IListEntryWrapper() = default;
};
