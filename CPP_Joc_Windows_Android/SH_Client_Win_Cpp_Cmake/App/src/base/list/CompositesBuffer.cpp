#include "CompositesBuffer.h"
#include <base/math/Math.h>
#include <base/exceptions/LogicException.h>
#include <cstring>

CompositesBuffer::CompositesBuffer(unsigned int _stride, int _minCapacity, int initialCapacity)
	: super(),
	_stride(_stride), _minCapacity(_minCapacity)
{
	if (_minCapacity > 0 || initialCapacity > 0) {
		reserve(Math::max(_minCapacity, initialCapacity));
	}
}

unsigned int CompositesBuffer::stride() {
	return _stride;
}

unsigned int CompositesBuffer::count() {
	return _count;
}

void CompositesBuffer::setCount(int newCount) {
	if (_count == newCount) {
		return;
	}

	if (newCount > _capacity) {
		reserve(newCount);
	}

	_count = newCount;
}

Bounds1DInt& CompositesBuffer::getUnusedCenterRegionRef() {
	return unusedCenterRegion;
}

unsigned int CompositesBuffer::byteCount() {
	return _count * _stride;
}

unsigned int CompositesBuffer::capacity() {
	return _capacity;
}

unsigned int CompositesBuffer::insertCapacity() {
	return _capacity - _count;
}

unsigned int CompositesBuffer::byteCapacity() {
	return _capacity * _stride;
}

bool CompositesBuffer::getIsLockedSize() {
	return isLockedSize;
}

void CompositesBuffer::lockSize() {
	if (isLockedSize) {
		// Already locked.
		throw LogicException(LOC);
	}

	isLockedSize = true;
}

void* CompositesBuffer::getData(unsigned int offsetEntriesCount, unsigned int validationAccessedEntriesCount) {
	if (offsetEntriesCount + validationAccessedEntriesCount > _count) {
		throw LogicException(LOC);
	}

	if (offsetEntriesCount == 0) {
		return data;
	}

	return static_cast<char*>(data) + (offsetEntriesCount * _stride);
}

char* CompositesBuffer::getDataBlockAsCharP() {
	return static_cast<char*>(data);
}

void CompositesBuffer::reserve(int reserveCapacity) {
	if (reserveCapacity > _capacity) {
		if (isLockedSize) {
			// Size is locked, not allowed.
			throw LogicException(LOC);
		}

		data = realloc(data, reserveCapacity * _stride);
		//memset(data, 0, reserveCapacity * _stride);

		_capacity = reserveCapacity;
	}
}

void CompositesBuffer::reserveInsertCapacity(int reserveInsertCapacity) {
	reserve(_count + reserveInsertCapacity);
}

void* CompositesBuffer::appendUninitialised(int elementsCount) {
	reserveInsertCapacity(elementsCount);
	
	void* ptr = static_cast<char*>(data) + _count * _stride;

	_count += elementsCount;
	
	return ptr;
}

void CompositesBuffer::appendUShort(unsigned short val) {
	if (_stride != sizeof(unsigned short)) {
		throw LogicException(LOC);
	}

	reserveInsertCapacity(1);

	void* ptr = static_cast<char*>(data) + _count * _stride;
	*((unsigned short*)ptr) = val;

	_count++;
}

void CompositesBuffer::append(void* srcData, int srcCount, unsigned int validationStride) {
	if (_stride != validationStride) {
		throw LogicException(LOC);
	}

	reserveInsertCapacity(srcCount);

	if (srcData != nullptr) {
		void* destData = static_cast<char*>(data) + _count * _stride;
		memcpy(destData, srcData, srcCount * _stride);
	}

	_count += srcCount;
}

void CompositesBuffer::appendAsUShortWithOffset(void* srcData, int srcCount, unsigned int validationStride, unsigned short offset) {
	if (_stride != validationStride) {
		throw LogicException(LOC);
	}

	reserveInsertCapacity(srcCount);

	unsigned short* destBuff = static_cast<unsigned short*>(data);
	unsigned short* srcBuff = static_cast<unsigned short*>(srcData);

	for (int i = 0; i < srcCount; i++) {
		destBuff[_count + i] = offset + srcBuff[i];
	}
	_count += srcCount;
}

//void* CompositesBuffer::get(int index) {
//	throw LogicException(LOC);
//	//asd;
//}
//
//void* CompositesBuffer::operator[](int index) {
//	throw LogicException(LOC);
//	//asd;
//}
//
//void* CompositesBuffer::insert(int index) {
//	throw LogicException(LOC);
//	//asd;
//}
//
//void* CompositesBuffer::append() {
//	throw LogicException(LOC);
//	//asd;
//}
//
//bool CompositesBuffer::remove(int index) {
//	throw LogicException(LOC);
//	//asd;
//}
//
//void CompositesBuffer::swap(int indexA, int indexB) {
//	throw LogicException(LOC);
//	//asd;
//}

void CompositesBuffer::clear() {
	//asd_r;// maybe lower the size if the size is not locked. Other logic may be required, implement whn remove is implemented.

	_count = 0;
}

void CompositesBuffer::disposeMain() {
	if (data != nullptr) {
		free(data);
		data = nullptr;
	}

	super::disposeMain();
}

CompositesBuffer::~CompositesBuffer() {
	reservedDisposeIfNeeded();// Note1001
}
