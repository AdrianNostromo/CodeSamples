#pragma once

#include <assert.h>
#include <base/exceptions/LogicException.h>

/**
 * @deprecated Use ListDL instead.
 */
// Pointers Doubly Linked List.
// pdll::List<ClassName> listExample;
namespace pdll
{
	template <typename T>
	class Entry;

	template <typename T>
	class List {
	public:
		Entry<T>* first = nullptr;
		Entry<T>* last = nullptr;
		int size = 0;

		List(List const&) = delete;
		List(List &&) = default;
		List& operator=(List const&) = delete;
		List& operator=(List &&) = default;
		explicit List();

		Entry<T>* insertBefore(Entry<T>* existingEntry, Entry<T>* entry);
		Entry<T>* insertBefore(Entry<T>* entry);
		Entry<T>* insertBefore(Entry<T>* existingEntry, T* data);
		Entry<T>* insertBefore(T* data);

		Entry<T>* insertAfter(Entry<T>* existingEntry, Entry<T>* entry);
		Entry<T>* insertAfter(Entry<T>* entry);
		Entry<T>* insertAfter(Entry<T>* existingEntry, T* data);
		Entry<T>* insertAfter(T* data);

		bool remove(Entry<T>* entry);
		void clear();

		~List();
	};

	template <typename T>
	class Entry {
	public:
		bool isManagedByList;

		T * data;

		Entry<T>* prev = nullptr;
		Entry<T>* next = nullptr;

		List<T>* list = nullptr;

		Entry(Entry const&) = delete;
		Entry(Entry &&) = default;
		Entry& operator=(Entry const&) = delete;
		Entry& operator=(Entry &&) = default;
		explicit Entry(bool isManagedByList, T* data);
	};

	// List

	template <typename T>
	List<T>::List() {
		//void
	}

	template <typename T>
	Entry<T>* List<T>::insertBefore(Entry<T>* existingEntry, Entry<T>* entry) {
		if (entry->list) {
            throw LogicException(LOC);
		}

		if (!existingEntry) {
			if (first) {
				return insertBefore(first, entry);
			}

			// Insert at start of empty list.
			entry->prev = nullptr;
			entry->next = nullptr;
			
			first = entry;
			last = entry;
		} else {
			// Insert before existingEntry.
			if (existingEntry->prev) {
				existingEntry->prev->next = entry;
				entry->prev = existingEntry->prev;
			}

			entry->next = existingEntry;
			existingEntry->prev = entry;

			if (first == existingEntry) {
				first = entry;
			}
		}

		entry->list = this;

        size++;

		return entry;
	}

	template <typename T>
	Entry<T>* List<T>::insertBefore(Entry<T>* entry) {
		return insertBefore(nullptr, entry);
	}

	template <typename T>
	Entry<T>* List<T>::insertBefore(Entry<T>* existingEntry, T* data) {
		Entry<T>* entry = new Entry<T>(true, data);

		return insertBefore(existingEntry, entry);
	}

	template <typename T>
	Entry<T>* List<T>::insertBefore(T* data) {
		return insertBefore(nullptr, data);
	}

	template <typename T>
	Entry<T>* List<T>::insertAfter(Entry<T>* existingEntry, Entry<T>* entry) {
		if (entry->list) {
            throw LogicException(LOC);
		}

		if (!existingEntry) {
			if (last) {
				return insertAfter(last, entry);
			}

			// Insert at start of empty list.
			entry->prev = nullptr;
			entry->next = nullptr;

			first = entry;
			last = entry;
		} else {
			if (existingEntry->next) {
				existingEntry->next->prev = entry;
				entry->next = existingEntry->next;
			}

			entry->prev = existingEntry;
			existingEntry->next = entry;

			if (last == existingEntry) {
				last = entry;
			}
		}

		entry->list = this;

        size++;

		return entry;
	}

	template <typename T>
	Entry<T>* List<T>::insertAfter(Entry<T>* entry) {
		return insertAfter(nullptr, entry);
	}
	
	template <typename T>
	Entry<T>* List<T>::insertAfter(Entry<T>* existingEntry, T* data) {
		Entry<T>* entry = new Entry<T>(true, data);

		return insertAfter(existingEntry, entry);
	}

	template <typename T>
	Entry<T>* List<T>::insertAfter(T* data) {
		return insertAfter(nullptr, data);
	}

	template <typename T>
	bool List<T>::remove(Entry<T>* entry) {
		if (entry->list != this) {
            throw LogicException(LOC);
		}

		if (first == entry) {
			first = entry->next;
		}
		if (last == entry) {
			last = entry->prev;
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

		if(entry->isManagedByList) {
			delete entry;
		}

        size--;
		
		return true;
	}

	template <typename T>
	void List<T>::clear() {
		while (first) {
			remove(first);
		}
	}

	template <typename T>
	List<T>::~List() {
		clear();
	}

	// Entry

	template <typename T>
	Entry<T>::Entry(bool _isManagedByList, T* _data)
		: isManagedByList(_isManagedByList), data(_data)
	{
		//void
	}

}
