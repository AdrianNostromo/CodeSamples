#pragma once

namespace base {
class IManagedListener {
public:
	virtual bool getIsListening() = 0;
	virtual void setIsListening(bool isListening) = 0;

	virtual ~IManagedListener() = default;
};
};
