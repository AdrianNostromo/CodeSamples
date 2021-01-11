#include "ThreadsManagerNative.h"
#include "managedThread/ManagedThreadNative.h"

using namespace desktopWindows;

ThreadsManagerNative::ThreadsManagerNative()
	: super()
{
	//void
}

base::ManagedThread* ThreadsManagerNative::getNewManagedThreadClass(std::string& name, std::function<unsigned int(base::IManagedThread* managedThread)> cbThreadTick) {
	ManagedThreadNative* managedThread = new ManagedThreadNative(this, name, cbThreadTick);

	return managedThread;
}

ThreadsManagerNative::~ThreadsManagerNative() {
	//void
}
