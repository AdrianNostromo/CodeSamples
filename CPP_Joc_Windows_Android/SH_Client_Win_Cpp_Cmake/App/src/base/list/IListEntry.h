#pragma once

class IListEntry {
public:
	virtual void remove() = 0;

	virtual ~IListEntry() noexcept = default;
};
