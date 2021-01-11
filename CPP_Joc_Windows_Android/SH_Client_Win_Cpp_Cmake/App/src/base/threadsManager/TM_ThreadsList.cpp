#include "TM_ThreadsList.h"
#include <base/log/GlobalAccessLogHandler.h>
#include <base/app/config/AppConfig.h>
#include <base/log/GlobalAccessLogHandler.h>

using namespace base;

const bool TM_ThreadsList::IS_DEBUG_LOCAL = true;

TM_ThreadsList::TM_ThreadsList()
	: super()
{
	//void
}

IManagedThread* TM_ThreadsList::newThread(std::string name, std::function<unsigned int(IManagedThread* managedThread)> cbThreadTick) {
	ManagedThread* managedThread = getNewManagedThreadClass(name, cbThreadTick);
	
	managedThreadsList.appendDirect(managedThread);
	if (AppConfig::IS_DEBUG && IS_DEBUG_LOCAL) {
		GlobalAccessLogHandler::devLogHandler->post_debug("ThreadsManager. Managed_thread created with name:%s, new count: %i", name.c_str(), managedThreadsList.count());
	}

	statistics_managedThreadsCount++;

	if (nativeThreadsStatus == Active) {
		managedThread->startNativeThread();
	}

	return managedThread;
}

void TM_ThreadsList::manageThreadsLoop() {
	// Monitor for stopped and exited threads and remove them.
	if (nativeThreadsStatus == Stopping) {
		bool isStopFinished = true;

		ListDL<ManagedThread*>::Entry* listEntry;
		managedThreadsList.startManualEnumeration();
		managedThreadsList.nextEnumeratingEntry = managedThreadsList.getFirst();
		while ((listEntry = managedThreadsList.nextEnumeratingEntry) != nullptr) {
			managedThreadsList.nextEnumeratingEntry = managedThreadsList.nextEnumeratingEntry->next;

			ManagedThread* managedThread = listEntry->data;

			if (managedThread->isThreadExited) {
				if (managedThread->isRemoveRequested) {
					// Requested to remove thread has exited, remove it from the list.
					if (AppConfig::IS_DEBUG && IS_DEBUG_LOCAL) {
						GlobalAccessLogHandler::devLogHandler->post_debug("ThreadsManager. Managed_thread removed with name:%s, new count: %i", managedThread->name.c_str(), (managedThreadsList.count() - 1));
					}

					managedThread->reservedDisposeMain();
					listEntry->remove();
				}
			} else {
				isStopFinished = false;
			}
		}
		managedThreadsList.endManualEnumeration();

		if (isStopFinished) {
			setNativeThreadsStatus(Off);
		}
	}else if (isAnyRemoveThreadsExited) {
		isAnyRemoveThreadsExited = false;

		ListDL<ManagedThread*>::Entry* listEntry;
		managedThreadsList.startManualEnumeration();
		managedThreadsList.nextEnumeratingEntry = managedThreadsList.getFirst();
		while ((listEntry = managedThreadsList.nextEnumeratingEntry) != nullptr) {
			managedThreadsList.nextEnumeratingEntry = managedThreadsList.nextEnumeratingEntry->next;

			ManagedThread* managedThread = listEntry->data;

			if (managedThread->isThreadExited && managedThread->isRemoveRequested) {
				// Requested to remove thread has exited, remove it from the list.
				if (AppConfig::IS_DEBUG && IS_DEBUG_LOCAL) {
					GlobalAccessLogHandler::devLogHandler->post_debug("ThreadsManager. Managed_thread removed with name:%s, new count: %i", managedThread->name.c_str(), (managedThreadsList.count() - 1));
				}

				managedThread->reservedDisposeMain();
				listEntry->remove();
			}
		}
		managedThreadsList.endManualEnumeration();
	}
}

void TM_ThreadsList::rt_onManagedThreadExited(ManagedThread* managedThread) {
	statistics_managedThreadsCount--;

	if (managedThread->isRemoveRequested) {
		isAnyRemoveThreadsExited = true;
	}
}

void TM_ThreadsList::rt_onNativeThreadStarted(ManagedThread* managedThread) {
	statistics_nativeThreadsCount++;

	GlobalAccessLogHandler::devLogHandler->post_debug("ThreadsManager. Native_thread started.");
}

void TM_ThreadsList::rt_onNativeThreadExited(ManagedThread* managedThread) {
	statistics_nativeThreadsCount--;

	GlobalAccessLogHandler::devLogHandler->post_debug("ThreadsManager. Native_thread exited.");
}

TM_ThreadsList::NativeThreadsStatus TM_ThreadsList::getNativeThreadsStatus() {
	return nativeThreadsStatus;
}

void TM_ThreadsList::setIsNatveThreadsPrefferedEnabled(bool isNatveThreadsPrefferedEnabled) {
	if (this->isNatveThreadsPrefferedEnabled == isNatveThreadsPrefferedEnabled) {
		return;
	}
	this->isNatveThreadsPrefferedEnabled = isNatveThreadsPrefferedEnabled;

	if (!isNatveThreadsPrefferedEnabled && nativeThreadsStatus == Active) {
		// Start all threads stopping.
		setNativeThreadsStatus(Stopping);
	} else if (isNatveThreadsPrefferedEnabled && nativeThreadsStatus == Off) {
		// Instantly start all native threads.
		setNativeThreadsStatus(Active);
	}
}

void TM_ThreadsList::setNativeThreadsStatus(NativeThreadsStatus nativeThreadsStatus) {
	if (this->nativeThreadsStatus == nativeThreadsStatus) {
		return;
	}

	if (nativeThreadsStatus == Off) {
		if (AppConfig::IS_DEBUG && IS_DEBUG_LOCAL) {
			GlobalAccessLogHandler::devLogHandler->post_debug("ThreadsManager. Status changed: Off");
		}

		if (this->nativeThreadsStatus != Stopping) {
			throw LogicException(LOC);
		}

		this->nativeThreadsStatus = Off;

		if (isNatveThreadsPrefferedEnabled) {
			setNativeThreadsStatus(Active);
		}
	} else if (nativeThreadsStatus == Stopping) {
		if (AppConfig::IS_DEBUG && IS_DEBUG_LOCAL) {
			GlobalAccessLogHandler::devLogHandler->post_debug("ThreadsManager. Status changed: Stopping");
		}

		if (this->nativeThreadsStatus != Active) {
			throw LogicException(LOC);
		}

		//
		ListDL<ManagedThread*>::Entry* listEntry;
		managedThreadsList.startManualEnumeration();
		managedThreadsList.nextEnumeratingEntry = managedThreadsList.getFirst();
		while ((listEntry = managedThreadsList.nextEnumeratingEntry) != nullptr) {
			managedThreadsList.nextEnumeratingEntry = managedThreadsList.nextEnumeratingEntry->next;

			ManagedThread* managedThread = listEntry->data;

			managedThread->isNativeThreadExitRequested = true;
		}
		managedThreadsList.endManualEnumeration();
		
		this->nativeThreadsStatus = Stopping;
	} else if (nativeThreadsStatus == Active) {
		if (AppConfig::IS_DEBUG && IS_DEBUG_LOCAL) {
			GlobalAccessLogHandler::devLogHandler->post_debug("ThreadsManager. Status changed: Active");
		}

		if (this->nativeThreadsStatus != Off) {
			throw LogicException(LOC);
		}

		// Activate all native threads that are off.
		ListDL<ManagedThread*>::Entry* listEntry;
		managedThreadsList.startManualEnumeration();
		managedThreadsList.nextEnumeratingEntry = managedThreadsList.getFirst();
		while ((listEntry = managedThreadsList.nextEnumeratingEntry) != nullptr) {
			managedThreadsList.nextEnumeratingEntry = managedThreadsList.nextEnumeratingEntry->next;

			ManagedThread* managedThread = listEntry->data;

			if (!managedThread->isRemoveRequested) {
				managedThread->startNativeThread();
			}
		}
		managedThreadsList.endManualEnumeration();

		this->nativeThreadsStatus = Active;

		if (!isNatveThreadsPrefferedEnabled) {
			setNativeThreadsStatus(Stopping);
		}
	} else {
		throw LogicException(LOC);
	}
}

int TM_ThreadsList::getStatistics_managedThreadsCount() {
	return statistics_managedThreadsCount;
}

int TM_ThreadsList::getStatistics_nativeThreadsCount() {
	return statistics_nativeThreadsCount;
}

void TM_ThreadsList::disposeMain() {
	int i = managedThreadsList.count();
	if (i > 0) {
		// Threads must be removed by the creators code before this can be disposed.
		throw LogicException(LOC);
	}

	super::disposeMain();
}

TM_ThreadsList::~TM_ThreadsList() {
	//void
}
