#pragma once

#include <base/gh.h>
#include <base/exceptions/OutOfRangeException.h>
#include <base/exceptions/LogicException.h>
#include <base/list/IListEntry.h>
#include <base/list/IListEntryWrapper.h>
#include <mutex>
#include <base/MM.h>

template <typename T>
class ListDL_ThreadSafe {pub dCtor(ListDL_ThreadSafe);
	pub class Entry : virtual public IListEntry {pub dCtor(Entry);
		// The entry can be externally managed (eg. Ho control when the memory is freed as is for playing_sounds).
		pub bool isEntryExternallyManuallyDeleted;

		pub ListDL_ThreadSafe* list = nullptr;

		pub Entry* prev = nullptr;
		pub Entry* next = nullptr;

		pub T data;
		// This is used in some cases (eg: The entity interactions uses this for a filter).
		pub void* extraData = nullptr;

		// This is used to notify a wrapper when a entry is removed.
		// This is used by the Events_Dispatchers_listeners.
		pub IListEntryWrapper* wrapper = nullptr;

		pub explicit Entry(bool isEntryExternallyManuallyDeleted, T& data) noexcept;
		
		pub template <typename... Ts>
		explicit Entry(bool isEntryExternallyManuallyDeleted, Ts&&... args) noexcept;

		pub void remove() override;

		pub ~Entry() noexcept override;
	};

	priv std::mutex* resourceLock;

	priv Entry* _first = nullptr;
	priv Entry* _last = nullptr;

    priv int _count = 0;

	// Enumeration start;
	// The idea with this is to allow for multiple entries removal,adding from a entry callback function call. Also removes the need for a mark_for_removal system.
	// This allows for manual enumarions.
	pub bool isEnumeratingInProgress = false;
	pub typename ListDL_ThreadSafe<T>::Entry* nextEnumeratingEntry = nullptr;

	pub bool isEnumeratingInProgressB = false;
	pub typename ListDL_ThreadSafe<T>::Entry* nextEnumeratingEntryB = nullptr;
	// Enumeration end;

	pub explicit ListDL_ThreadSafe();

	pub Entry* getFirst();
	// The parameters isRemoveOperation and isInsertOperation are used by the ListDL_ThreadSafe;
	prot void setFirst(Entry* entry, bool isRemoveOperation, bool isInsertOperation);

	pub Entry* getLast();
	// The parameters isRemoveOperation and isInsertOperation are used by the ListDL_ThreadSafe;
	prot void setLast(Entry* entry, bool isRemoveOperation, bool isInsertOperation);

	// These are used by subclass ListDL_ThreadSafe to be handled by a lock.
	pub Entry* getEntryPrev(Entry* entry);
	pub Entry* getEntryNext(Entry* entry);

	pub int count();

	// existingEntry can be nullptr only on a empty list.
	pub Entry* insertEntryBefore(Entry* existingEntry, Entry* entry);
	// existingEntry can be nullptr only on a empty list.
	pub Entry* insertEntryAfter(Entry* existingEntry, Entry* entry);
	pub Entry* prependEntry(Entry* entry);
	pub Entry* appendEntry(Entry* entry);

	pub Entry* insertBeforeDirect(Entry* existingEntry, T data);
	pub Entry* insertBeforeReference(Entry* existingEntry, T& data);

	pub Entry* insertAfterDirect(Entry* existingEntry, T data);
	pub Entry* insertAfterReference(Entry* existingEntry, T& data);

	pub Entry* prependDirect(T data);
	pub Entry* prependReference(T& data);

	pub Entry* appendDirect(T data);
	pub Entry* appendReference(T& data);

	pub template <typename... Ts>
	Entry* appendEmplace(Ts&&... args);

	pub bool removeEntry(Entry* entry);
	prot void onEntryRemovePre(Entry* entry);

	pub void clear();

	// Enumeration start;
	pub template<typename F>
	void enumerateReference(F& cb);
	pub template<typename F>
	void enumerateDirect(F cb);

	pub void startManualEnumeration();
	pub void endManualEnumeration();

	pub void startManualEnumerationB();
	pub void endManualEnumerationB();
	// Enumeration end;

	pub ~ListDL_ThreadSafe();
};

template <typename T>
ListDL_ThreadSafe<T>::Entry::Entry(bool isEntryExternallyManuallyDeleted, T& data) noexcept
	: isEntryExternallyManuallyDeleted(isEntryExternallyManuallyDeleted), data(data)
{
	//void
}

template <typename T>
template <typename... Ts>
ListDL_ThreadSafe<T>::Entry::Entry(bool isEntryExternallyManuallyDeleted, Ts&&... args) noexcept
	: isEntryExternallyManuallyDeleted(isEntryExternallyManuallyDeleted), data(std::forward<Ts>(args)...)
{
	//void
}

template <typename T>
void ListDL_ThreadSafe<T>::Entry::remove() {
	if(list == nullptr) {
		throw LogicException(LOC);
	}

	list->removeEntry(this);
}

template <typename T>
ListDL_ThreadSafe<T>::Entry::~Entry() noexcept {
	//void
}

template <typename T>
ListDL_ThreadSafe<T>::ListDL_ThreadSafe() {
	resourceLock = new std::mutex();
}

template <typename T>
typename ListDL_ThreadSafe<T>::Entry* ListDL_ThreadSafe<T>::getFirst() {
	// Lock here in case there is a active 
	resourceLock->lock();
	
	typename ListDL_ThreadSafe<T>::Entry* ret = _first;

	resourceLock->unlock();
	
	return ret;
}

template <typename T>
void ListDL_ThreadSafe<T>::setFirst(Entry* entry, bool isRemoveOperation, bool isInsertOperation) {
	if (!isRemoveOperation && !isInsertOperation) {
		resourceLock->lock();
	}

	this->_first = entry;

	if (!isRemoveOperation && !isInsertOperation) {
		resourceLock->unlock();
	}
}

template <typename T>
typename ListDL_ThreadSafe<T>::Entry* ListDL_ThreadSafe<T>::getLast() {
	resourceLock->lock();

	typename ListDL_ThreadSafe<T>::Entry* ret = _last;

	resourceLock->unlock();

	return ret;
}

template <typename T>
void ListDL_ThreadSafe<T>::setLast(Entry* entry, bool isRemoveOperation, bool isInsertOperation) {
	if (!isRemoveOperation && !isInsertOperation) {
		resourceLock->lock();
	}

	this->_last = entry;

	if (isEnumeratingInProgress && nextEnumeratingEntry == nullptr && isInsertOperation) {
		nextEnumeratingEntry = this->getLast();
	}
	if (isEnumeratingInProgressB && nextEnumeratingEntryB == nullptr && isInsertOperation) {
		nextEnumeratingEntryB = this->getLast();
	}

	if (!isRemoveOperation && !isInsertOperation) {
		resourceLock->unlock();
	}
}

template <typename T>
typename ListDL_ThreadSafe<T>::Entry* ListDL_ThreadSafe<T>::getEntryPrev(Entry* entry) {
	resourceLock->lock();
	typename ListDL_ThreadSafe<T>::Entry* ret = entry->prev;
	resourceLock->unlock();

	return ret;
}

template <typename T>
typename ListDL_ThreadSafe<T>::Entry* ListDL_ThreadSafe<T>::getEntryNext(Entry* entry) {
	resourceLock->lock();
	typename ListDL_ThreadSafe<T>::Entry* ret = entry->next;
	resourceLock->unlock();

	return ret;
}

template <typename T>
int ListDL_ThreadSafe<T>::count() {
	resourceLock->lock();
	int ret = _count;
	resourceLock->unlock();

	return ret;
}

template <typename T>
typename ListDL_ThreadSafe<T>::Entry* ListDL_ThreadSafe<T>::insertEntryBefore(Entry* existingEntry, Entry* entry) {
	resourceLock->lock();

	if (entry->list) {
		resourceLock->unlock();
		throw LogicException(LOC);
	}

	if (existingEntry == nullptr && _first != nullptr) {
		// existingEntry can be nullptr only on a empty list.
		resourceLock->unlock();
		throw LogicException(LOC);
	}

	if (!existingEntry) {
		// Insert at start of empty list.
		entry->prev = nullptr;
		entry->next = nullptr;

		setFirst(entry, false/*isRemoveOperation*/, true/*isInsertOperation*/);
		setLast(entry, false/*isRemoveOperation*/, true/*isInsertOperation*/);
	} else {
		// Insert before existingEntry.
		if (existingEntry->prev) {
			existingEntry->prev->next = entry;
			entry->prev = existingEntry->prev;
		}

		entry->next = existingEntry;
		existingEntry->prev = entry;

		if (_first == existingEntry) {
			setFirst(entry, false/*isRemoveOperation*/, true/*isInsertOperation*/);
		}
	}

	entry->list = this;

	_count++;

	resourceLock->unlock();
	return entry;
}

template <typename T>
typename ListDL_ThreadSafe<T>::Entry* ListDL_ThreadSafe<T>::insertEntryAfter(Entry* existingEntry, Entry* entry) {
	resourceLock->lock();

	if (entry->list) {
		resourceLock->unlock();
		throw LogicException(LOC);
	}

	if (existingEntry == nullptr && _last != nullptr) {
		// existingEntry can be nullptr only on a empty list.
		resourceLock->unlock();
		throw LogicException(LOC);
	}

	if (!existingEntry) {
		// Insert at start of empty list.
		entry->prev = nullptr;
		entry->next = nullptr;

		setFirst(entry, false/*isRemoveOperation*/, true/*isInsertOperation*/);
		setLast(entry, false/*isRemoveOperation*/, true/*isInsertOperation*/);
	} else {
		if (existingEntry->next) {
			existingEntry->next->prev = entry;
			entry->next = existingEntry->next;
		}

		entry->prev = existingEntry;
		existingEntry->next = entry;

		if (_last == existingEntry) {
			setLast(entry, false/*isRemoveOperation*/, true/*isInsertOperation*/);
		}
	}

	entry->list = this;

	_count++;

	resourceLock->unlock();
	return entry;
}

template <typename T>
typename ListDL_ThreadSafe<T>::Entry* ListDL_ThreadSafe<T>::prependEntry(Entry* entry) {
	return insertEntryBefore(_first, entry);
}

template <typename T>
typename ListDL_ThreadSafe<T>::Entry* ListDL_ThreadSafe<T>::appendEntry(Entry* entry) {
	return insertEntryAfter(_last, entry);
}

template <typename T>
typename ListDL_ThreadSafe<T>::Entry* ListDL_ThreadSafe<T>::insertBeforeDirect(Entry* existingEntry, T data) {
	return insertBeforeReference(existingEntry, data);
}

template <typename T>
typename ListDL_ThreadSafe<T>::Entry* ListDL_ThreadSafe<T>::insertBeforeReference(Entry* existingEntry, T& data) {
	return insertEntryBefore(existingEntry, new Entry(false, data));
}

template <typename T>
typename ListDL_ThreadSafe<T>::Entry* ListDL_ThreadSafe<T>::insertAfterDirect(Entry* existingEntry, T data) {
	return insertAfterReference(existingEntry, data);
}

template <typename T>
typename ListDL_ThreadSafe<T>::Entry* ListDL_ThreadSafe<T>::insertAfterReference(Entry* existingEntry, T& data) {
	return insertEntryAfter(existingEntry, new Entry(false, data));
}

template <typename T>
typename ListDL_ThreadSafe<T>::Entry* ListDL_ThreadSafe<T>::prependDirect(T data) {
	return prependReference(data);
}

template <typename T>
typename ListDL_ThreadSafe<T>::Entry* ListDL_ThreadSafe<T>::prependReference(T& data) {
	return insertBeforeReference(_first, data);
}

template <typename T>
typename ListDL_ThreadSafe<T>::Entry* ListDL_ThreadSafe<T>::appendDirect(T data) {
	return appendReference(data);
}

template <typename T>
typename ListDL_ThreadSafe<T>::Entry* ListDL_ThreadSafe<T>::appendReference(T& data) {
	return insertAfterReference(_last, data);
}

template <typename T>
template <typename... Ts>
typename ListDL_ThreadSafe<T>::Entry* ListDL_ThreadSafe<T>::appendEmplace(Ts&&... args) {
	return insertEntryAfter(_last, new Entry(false, std::forward<Ts>(args)...));
}

template <typename T>
bool ListDL_ThreadSafe<T>::removeEntry(Entry* entry) {
	resourceLock->lock();

	if (entry->list != this) {
		resourceLock->unlock();
		throw LogicException(LOC);
	}

	onEntryRemovePre(entry);

	if(entry->wrapper) {
		entry->wrapper->onListEntryRemoved();
	}

	if (_first == entry) {
		setFirst(entry->next, true/*isRemoveOperation*/, false/*isInsertOperation*/);
	}
	if (_last == entry) {
		setLast(entry->prev, true/*isRemoveOperation*/, false/*isInsertOperation*/);
	}

	if (entry->prev) {
		entry->prev->next = entry->next;
	}
	if (entry->next) {
		entry->next->prev = entry->prev;
	}

	entry->prev = nullptr;
	entry->next = nullptr;

	entry->list = nullptr;

	if(!entry->isEntryExternallyManuallyDeleted) {
		delete entry;
	}

	_count--;

	resourceLock->unlock();
	return true;
}

template <typename T>
void ListDL_ThreadSafe<T>::onEntryRemovePre(Entry* entry) {
	if (isEnumeratingInProgress && entry == nextEnumeratingEntry) {
		nextEnumeratingEntry = entry->next;
	}
	if (isEnumeratingInProgressB && entry == nextEnumeratingEntryB) {
		nextEnumeratingEntryB = entry->next;
	}
}

template <typename T>
template <typename F>
void ListDL_ThreadSafe<T>::enumerateReference(F& cb) {
	if (isEnumeratingInProgress) {
		throw LogicException(LOC);
	}

	nextEnumeratingEntry = this->getFirst();
	isEnumeratingInProgress = true;

	typename ListDL_ThreadSafe<T>::Entry* listEntry;
	bool stopEnumeration = false;
	while ((listEntry = nextEnumeratingEntry) != nullptr) {
		nextEnumeratingEntry = nextEnumeratingEntry->next;

		cb(listEntry, listEntry->data, stopEnumeration);
		if (stopEnumeration) {
			break;
		}
	}

	nextEnumeratingEntry = nullptr;
	isEnumeratingInProgress = false;
}

template <typename T>
template <typename F>
void ListDL_ThreadSafe<T>::enumerateDirect(F cb) {
	enumerateReference(cb);
}

template <typename T>
void ListDL_ThreadSafe<T>::startManualEnumeration() {
	if (isEnumeratingInProgress) {
		throw LogicException(LOC);
	}

	nextEnumeratingEntry = nullptr;
	isEnumeratingInProgress = true;
}

template <typename T>
void ListDL_ThreadSafe<T>::endManualEnumeration() {
	if (!isEnumeratingInProgress) {
		throw LogicException(LOC);
	}

	nextEnumeratingEntry = nullptr;
	isEnumeratingInProgress = false;
}

template <typename T>
void ListDL_ThreadSafe<T>::startManualEnumerationB() {
	if (isEnumeratingInProgressB) {
		throw LogicException(LOC);
	}

	nextEnumeratingEntryB = nullptr;
	isEnumeratingInProgressB = true;
}

template <typename T>
void ListDL_ThreadSafe<T>::endManualEnumerationB() {
	if (!isEnumeratingInProgressB) {
		throw LogicException(LOC);
	}

	nextEnumeratingEntryB = nullptr;
	isEnumeratingInProgressB = false;
}

template <typename T>
void ListDL_ThreadSafe<T>::clear() {
	while (_first != nullptr) {
		removeEntry(_first);
	}
}

template <typename T>
ListDL_ThreadSafe<T>::~ListDL_ThreadSafe() {
	clear();

	if (resourceLock != nullptr) {
		delete resourceLock;
	}
}
