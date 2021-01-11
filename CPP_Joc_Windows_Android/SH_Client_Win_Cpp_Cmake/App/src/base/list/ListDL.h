#pragma once

#include <base/gh.h>
#include <base/exceptions/OutOfRangeException.h>
#include <base/exceptions/LogicException.h>
#include <base/list/IListEntry.h>
#include <base/list/IListEntryWrapper.h>
#include <base/MM.h>

template <typename T>
class ListDL {pub dCtor(ListDL);
	pub class Entry : virtual public IListEntry {pub dCtor(Entry);
		// The entry can be externally managed (eg. Ho control when the memory is freed as is for playing_sounds).
		pub bool isEntryExternallyManuallyDeleted;

		pub ListDL* list = nullptr;

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

	priv const char* fileName;
	// This is negated to show that container mode is used.
	priv int lineIndexInvertible;

	priv Entry* _first = nullptr;
	priv Entry* _last = nullptr;

    priv int _count = 0;

	// Enumeration start;
	// The idea with this is to allow for multiple entries removal,adding from a entry callback function call. Also removes the need for a mark_for_removal system.
	// This allows for manual enumarions.
	pub bool isEnumeratingInProgress = false;
	pub typename ListDL<T>::Entry* nextEnumeratingEntry = nullptr;

	pub bool isEnumeratingInProgressB = false;
	pub typename ListDL<T>::Entry* nextEnumeratingEntryB = nullptr;
	// Enumeration end;

	pub explicit ListDL(const char* fileName=nullptr, size_t lineIndex=0);

	pub Entry* getFirst();
	// The parameters isRemoveOperation and isInsertOperation are used by the ListDL;
	prot void setFirst(Entry* entry, bool isRemoveOperation, bool isInsertOperation);

	pub Entry* getLast();
	// The parameters isRemoveOperation and isInsertOperation are used by the ListDL;
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

	pub template <typename... Ts>
		Entry* insertBeforeEmplace(Entry* existingEntry, Ts&&... args);

	pub Entry* insertAfterDirect(Entry* existingEntry, T data);
	pub Entry* insertAfterReference(Entry* existingEntry, T& data);
	
	pub template <typename... Ts>
	Entry* insertAfterEmplace(Entry* existingEntry, Ts&&... args);

	pub Entry* prependDirect(T data);
	pub Entry* prependReference(T& data);
	
	pub template <typename... Ts>
	Entry* prependEmplace(Ts&&... args);

	pub Entry* appendDirect(T data);
	pub Entry* appendReference(T& data);

	pub template <typename... Ts>
	Entry* appendEmplace(Ts&&... args);

	pub void removeEntry(Entry* entry);
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

	pub ~ListDL();
};

template <typename T>
ListDL<T>::Entry::Entry(bool isEntryExternallyManuallyDeleted, T& data) noexcept
	: isEntryExternallyManuallyDeleted(isEntryExternallyManuallyDeleted), data(data)
{
	//void
}

template <typename T>
template <typename... Ts>
ListDL<T>::Entry::Entry(bool isEntryExternallyManuallyDeleted, Ts&&... args) noexcept
	: isEntryExternallyManuallyDeleted(isEntryExternallyManuallyDeleted), data(std::forward<Ts>(args)...)
{
	//void
}

template <typename T>
void ListDL<T>::Entry::remove() {
	if(list == nullptr) {
		throw LogicException(LOC);
	}

	list->removeEntry(this);
}

template <typename T>
ListDL<T>::Entry::~Entry() noexcept {
	//void
}

template <typename T>
ListDL<T>::ListDL(const char* fileName, size_t lineIndex) {
	if (fileName != nullptr) {
		this->fileName = fileName;
		this->lineIndexInvertible = -lineIndex;
	} else {
		this->fileName = __FILE__;
		this->lineIndexInvertible = __LINE__;
	}
}

template <typename T>
typename ListDL<T>::Entry* ListDL<T>::getFirst() {
	return _first;
}

template <typename T>
void ListDL<T>::setFirst(Entry* entry, bool isRemoveOperation, bool isInsertOperation) {
	this->_first = entry;
}

template <typename T>
typename ListDL<T>::Entry* ListDL<T>::getLast() {
	return _last;
}

template <typename T>
void ListDL<T>::setLast(Entry* entry, bool isRemoveOperation, bool isInsertOperation) {
	this->_last = entry;

	if (isEnumeratingInProgress && nextEnumeratingEntry == nullptr && isInsertOperation) {
		nextEnumeratingEntry = this->getLast();
	}
	if (isEnumeratingInProgressB && nextEnumeratingEntryB == nullptr && isInsertOperation) {
		nextEnumeratingEntryB = this->getLast();
	}
}

template <typename T>
typename ListDL<T>::Entry* ListDL<T>::getEntryPrev(Entry* entry) {
	return entry->prev;
}

template <typename T>
typename ListDL<T>::Entry* ListDL<T>::getEntryNext(Entry* entry) {
	return entry->next;
}

template <typename T>
int ListDL<T>::count() {
	return _count;
}

template <typename T>
typename ListDL<T>::Entry* ListDL<T>::insertEntryBefore(Entry* existingEntry, Entry* entry) {
	if (entry == nullptr || entry->list) {
		throw LogicException(LOC);
	}

	if (existingEntry == nullptr && _first != nullptr) {
		// existingEntry can be nullptr only on a empty list.
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

	return entry;
}

template <typename T>
typename ListDL<T>::Entry* ListDL<T>::insertEntryAfter(Entry* existingEntry, Entry* entry) {
	if (entry == nullptr || entry->list) {
		throw LogicException(LOC);
	}

	if (existingEntry == nullptr && _last != nullptr) {
		// existingEntry can be nullptr only on a empty list.
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

	return entry;
}

template <typename T>
typename ListDL<T>::Entry* ListDL<T>::prependEntry(Entry* entry) {
	return insertEntryBefore(_first, entry);
}

template <typename T>
typename ListDL<T>::Entry* ListDL<T>::appendEntry(Entry* entry) {
	return insertEntryAfter(_last, entry);
}

template <typename T>
typename ListDL<T>::Entry* ListDL<T>::insertBeforeDirect(Entry* existingEntry, T data) {
	return insertBeforeReference(existingEntry, data);
}

template <typename T>
typename ListDL<T>::Entry* ListDL<T>::insertBeforeReference(Entry* existingEntry, T& data) {
	return insertEntryBefore(existingEntry, new(fileName, lineIndexInvertible) Entry(false, data));
}

template <typename T>
template <typename... Ts>
typename ListDL<T>::Entry* ListDL<T>::insertBeforeEmplace(Entry* existingEntry, Ts&&... args) {
	return insertEntryBefore(existingEntry, new(fileName, lineIndexInvertible) Entry(false, std::forward<Ts>(args)...));
}

template <typename T>
typename ListDL<T>::Entry* ListDL<T>::insertAfterDirect(Entry* existingEntry, T data) {
	return insertAfterReference(existingEntry, data);
}

template <typename T>
typename ListDL<T>::Entry* ListDL<T>::insertAfterReference(Entry* existingEntry, T& data) {
	return insertEntryAfter(existingEntry, new(fileName, lineIndexInvertible) Entry(false, data));
}

template <typename T>
template <typename... Ts>
typename ListDL<T>::Entry* ListDL<T>::insertAfterEmplace(Entry* existingEntry, Ts&&... args) {
	return insertEntryAfter(existingEntry, new(fileName, lineIndexInvertible) Entry(false, std::forward<Ts>(args)...));
}

template <typename T>
typename ListDL<T>::Entry* ListDL<T>::prependDirect(T data) {
	return prependReference(data);
}

template <typename T>
typename ListDL<T>::Entry* ListDL<T>::prependReference(T& data) {
	return insertBeforeReference(_first, data);
}

template <typename T>
template <typename... Ts>
typename ListDL<T>::Entry* ListDL<T>::prependEmplace(Ts&&... args) {
	return insertEntryBefore(_first, new(fileName, lineIndexInvertible) Entry(false, std::forward<Ts>(args)...));
}

template <typename T>
typename ListDL<T>::Entry* ListDL<T>::appendDirect(T data) {
	return appendReference(data);
}

template <typename T>
typename ListDL<T>::Entry* ListDL<T>::appendReference(T& data) {
	return insertAfterReference(_last, data);
}

template <typename T>
template <typename... Ts>
typename ListDL<T>::Entry* ListDL<T>::appendEmplace(Ts&&... args) {
	return insertEntryAfter(_last, new(fileName, lineIndexInvertible) Entry(false, std::forward<Ts>(args)...));
}

template <typename T>
void ListDL<T>::removeEntry(Entry* entry) {
	if (entry->list != this) {
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
}

template <typename T>
void ListDL<T>::onEntryRemovePre(Entry* entry) {
	if (isEnumeratingInProgress && entry == nextEnumeratingEntry) {
		nextEnumeratingEntry = entry->next;
	}
	if (isEnumeratingInProgressB && entry == nextEnumeratingEntryB) {
		nextEnumeratingEntryB = entry->next;
	}
}

template <typename T>
template <typename F>
void ListDL<T>::enumerateReference(F& cb) {
	startManualEnumeration();
	
	typename ListDL<T>::Entry* listEntry;
	bool stopEnumeration = false;
	while ((listEntry = nextEnumeratingEntry) != nullptr) {
		nextEnumeratingEntry = nextEnumeratingEntry->next;

		cb(listEntry, listEntry->data, stopEnumeration);
		if (stopEnumeration) {
			break;
		}
	}

	endManualEnumeration();
}

template <typename T>
template <typename F>
void ListDL<T>::enumerateDirect(F cb) {
	enumerateReference(cb);
}

template <typename T>
void ListDL<T>::startManualEnumeration() {
	if (isEnumeratingInProgress) {
		throw LogicException(LOC);
	}

	nextEnumeratingEntry = this->getFirst();
	isEnumeratingInProgress = true;
}

template <typename T>
void ListDL<T>::endManualEnumeration() {
	if (!isEnumeratingInProgress) {
		throw LogicException(LOC);
	}

	nextEnumeratingEntry = nullptr;
	isEnumeratingInProgress = false;
}

template <typename T>
void ListDL<T>::startManualEnumerationB() {
	if (isEnumeratingInProgressB) {
		throw LogicException(LOC);
	}

	nextEnumeratingEntryB = nullptr;
	isEnumeratingInProgressB = true;
}

template <typename T>
void ListDL<T>::endManualEnumerationB() {
	if (!isEnumeratingInProgressB) {
		throw LogicException(LOC);
	}

	nextEnumeratingEntryB = nullptr;
	isEnumeratingInProgressB = false;
}

template <typename T>
void ListDL<T>::clear() {
	while (_first != nullptr) {
		removeEntry(_first);
	}
}

template <typename T>
ListDL<T>::~ListDL() {
	clear();
}
