#pragma once

#include "IManagedListener.h"

namespace base {
class ManagedListener : virtual public IManagedListener {
private:
	bool isListening = false;

public:
	ManagedListener(ManagedListener const&) = delete;
	ManagedListener(ManagedListener &&) = default;
	ManagedListener& operator=(ManagedListener const&) = delete;
	ManagedListener& operator=(ManagedListener &&) = default;

	explicit ManagedListener();

	bool getIsListening() override;
	void setIsListening(bool isListening) override;

	~ManagedListener() override;
};
};
