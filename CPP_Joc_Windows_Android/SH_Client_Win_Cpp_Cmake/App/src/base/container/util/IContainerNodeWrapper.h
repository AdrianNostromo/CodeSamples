#pragma once

class IContainerNodeWrapper {
public:
	// On this call, only remove the list entry, don't do much logic.
	virtual void onContainerNodeRemovePre() = 0;

	virtual ~IContainerNodeWrapper() = default;
};
