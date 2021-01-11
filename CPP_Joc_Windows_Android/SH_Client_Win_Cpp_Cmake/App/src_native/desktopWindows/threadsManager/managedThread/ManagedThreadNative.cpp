#include "ManagedThreadNative.h"
#include <thread>
#include <base/exceptions/LogicException.h>

using namespace desktopWindows;

ManagedThreadNative::ManagedThreadNative(
	base::IHandler_ManagedThread* handler,
	std::string& name,
	std::function<unsigned int(base::IManagedThread* managedThread)> cbThreadTick)
	: super(handler, name, cbThreadTick)
{
	//void
}

void ManagedThreadNative::startNativeThread() {
	if (t != nullptr || isRemoveRequested) {
		// Thread already exists or want's to be removed. This should never occur.
		throw LogicException(LOC);
	}

	isNativeThreadExitRequested = false;
	isThreadExited = false;

	t = new std::thread(&ManagedThreadNative::onThreadTick, this);
	t->detach();
}

int ManagedThreadNative::onThreadTick() {
	onNativeThreadStarted();

	while (true) {
		int sleepDurationMs = super::onThreadTick();
		if (sleepDurationMs < 0) {
			break;
		}

		if (sleepDurationMs > 0) {
			std::this_thread::sleep_for(std::chrono::milliseconds(sleepDurationMs));
		}
	}

	onNativeThreadExited();

	return 0;
}

void ManagedThreadNative::onNativeThreadExited() {
	if (t == nullptr) {
		throw LogicException(LOC);
	}
	
	delete t;
	t = nullptr;

	super::onNativeThreadExited();
}

void ManagedThreadNative::disposeMain() {
	if (t != nullptr) {
		throw LogicException(LOC);
	}

	super::disposeMain();
}

ManagedThreadNative::~ManagedThreadNative() {
	//void
}
