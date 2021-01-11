#pragma once

class IContainerNode {
public:
	virtual void remove() = 0;

	virtual ~IContainerNode() noexcept = default;
};
