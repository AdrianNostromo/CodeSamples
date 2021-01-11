#include "ManagedThread.h"
#include <base/exceptions/LogicException.h>
#include "IHandler_ManagedThread.h"

using namespace base;

const bool ManagedThread::IS_DEBUG_LOCAL = true;

ManagedThread::ManagedThread(
	IHandler_ManagedThread* handler,
	std::string& name,
	std::function<unsigned int(base::IManagedThread* managedThread)> cbThreadTick)
	: super(),
	handler(handler),
	name(name),
	cbThreadTick(cbThreadTick)
{
	//void
}

void ManagedThread::removeManaged() {
	if (cbThreadTick == nullptr) {
		throw LogicException(LOC);
	}

	isRemoveRequested = true;
	cbThreadTick = nullptr;

	if (isThreadExited) {
		handler->rt_onManagedThreadExited(this);
	}
}

int ManagedThread::onThreadTick() {
	if (isRemoveRequested || isNativeThreadExitRequested) {
		return -1;
	}
	if (!cbThreadTick) {
		return -1;
	}

	unsigned int sleepDurationMs = cbThreadTick(this);
	if (!cbThreadTick) {
		return -1;
	}
	if (isRemoveRequested || isNativeThreadExitRequested) {
		return -1;
	}

	return sleepDurationMs;
}

void ManagedThread::onNativeThreadStarted() {
	handler->rt_onNativeThreadStarted(this);
}

void ManagedThread::onNativeThreadExited() {
	isThreadExited = true;

	handler->rt_onNativeThreadExited(this);

	handler->rt_onManagedThreadExited(this);
}

ManagedThread::~ManagedThread() {
	//void
}
