#pragma once

#include <base/gh.h>

class IListEntry;

class IEventListener {
	pub virtual bool isConnected() = 0;
	pub virtual void disconnect() = 0;

	pub virtual ~IEventListener() = default;
};
