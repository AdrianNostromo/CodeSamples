#pragma once

#include <base/gh.h>
#include <base/math/Math.h>
#include <base/exceptions/LogicException.h>
#include <cstring>
#include <base/exceptions/OutOfRangeException.h>
#include <base/object/Object.h>
#include <base/math/Bounds3DInt.h>
#include <base/MM.h>

template <typename T>
class Array3D final/*Note1001. calls_the_reserved_dispose*/ : public base::Object {priv typedef base::Object super;pub dCtor(Array3D);
	pub Vector3Int count{};

    pub T* data = nullptr;

    pub explicit Array3D(int lengthA, int lengthB, int lengthC);
	pub explicit Array3D(int lengthA, int lengthB, int lengthC, T* data);
	pub explicit Array3D(int lengthA, int lengthB, int lengthC, bool invertA, bool invertB, bool invertC, T* data);

	pub void zeroSetData();
	pub void memSetData(int val);

	pub int getLengthA();
	pub int getLengthB();
	pub int getLengthC();
	
	pub Vector3Int* getCount();

	pub int getCountA();
	pub int getCountB();
	pub int getCountC();

	pub void resizeNoElementsMove(int lengthA, int lengthB, int lengthC);
	pub void resizeMarginOffsets(Bounds3DInt& newBounds);
	pub inline T getDirect(int gA, int gB, int gC);
	pub inline T& getReference(int gA, int gB, int gC);
	pub inline T* getPointer(int gA, int gB, int gC);
	pub inline T* getPointer(int gA, int gB, int gC, T* fallbackValue);
	pub void set(int gA, int gB, int gC, T& val);
	pub void set(Array3D<T>& src);
	pub void set(Vector3Int& offset, Array3D<T>* src, Vector3Int& srcOffset, Vector3Int& copyCount);

	pub static inline void copy(
		T* srcData, Vector3Int& srcCount, Vector3Int& srcOffset,
		T* destData, Vector3Int& destCount, Vector3Int& destOffset,
		Vector3Int& count);
	
	pub static inline void move(
		T* srcData, Vector3Int& srcCount, Vector3Int& srcOffset,
		T* destData, Vector3Int& destCount, Vector3Int& destOffset,
		Vector3Int& count);

	pub void setDirect(int gA, int gB, int gC, T val);
	pub void invertA();
	pub void invertB();
	pub void invertC();

	priv inline int gridPosToDataIndex(int gA, int gB, int gC);

    pub ~Array3D();
};

template <typename T>
Array3D<T>::Array3D(int lengthA, int lengthB, int lengthC)
        : Array3D<T>(lengthA, lengthB, lengthC, new T[lengthA * lengthB * lengthC]())
{
    //void
}

template <typename T>
Array3D<T>::Array3D(int lengthA, int lengthB, int lengthC, T* data)
	: super(),
	count(lengthA, lengthB, lengthC), data(data)
{
    //void
}

template <typename T>
Array3D<T>::Array3D(int lengthA, int lengthB, int lengthC, bool _invertA, bool _invertB, bool _invertC, T* data)
        : super(),
		count(lengthA, lengthB, lengthC), data(data)
{
    if(_invertA) {
        invertA();
    }
    if(_invertB) {
        invertB();
    }
    if(_invertC) {
        invertC();
    }
}

template <typename T>
void Array3D<T>::zeroSetData() {
	memset(data, 0, count.x * count.y * count.z * sizeof(T));
}

template <typename T>
void Array3D<T>::memSetData(int val) {
	memset(data, val, count.x * count.y * count.z * sizeof(T));
}

template <typename T>
int Array3D<T>::getLengthA() {
	return count.x;
}

template <typename T>
int Array3D<T>::getLengthB() {
	return count.y;
}

template <typename T>
int Array3D<T>::getLengthC() {
	return count.z;
}

template <typename T>
Vector3Int* Array3D<T>::getCount() {
	return &count;
}

template <typename T>
int Array3D<T>::getCountA() {
	return count.x;
}

template <typename T>
int Array3D<T>::getCountB() {
	return count.y;
}

template <typename T>
int Array3D<T>::getCountC() {
	return count.z;
}

template <typename T>
inline T* Array3D<T>::getPointer(int gA, int gB, int gC) {
	if (gA < 0 || gA >= count.x || gB < 0 || gB >= count.y || gC < 0 || gC >= count.z) {
		throw LogicException(LOC);
	}

	int index = gridPosToDataIndex(gA, gB, gC);

	return &data[index];
}

template <typename T>
inline T Array3D<T>::getDirect(int gA, int gB, int gC) {
	if (gA < 0 || gA >= count.x || gB < 0 || gB >= count.y || gC < 0 || gC >= count.z) {
		throw LogicException(LOC);
	}

	int index = gridPosToDataIndex(gA, gB, gC);

	return data[index];
}

template <typename T>
inline T& Array3D<T>::getReference(int gA, int gB, int gC) {
	if (gA < 0 || gA >= count.x || gB < 0 || gB >= count.y || gC < 0 || gC >= count.z) {
		throw LogicException(LOC);
	}

	int index = gridPosToDataIndex(gA, gB, gC);

	return data[index];
}

template <typename T>
inline T* Array3D<T>::getPointer(int gA, int gB, int gC, T* fallbackValue) {
	if (gA < 0 || gA >= count.x || gB < 0 || gB >= count.y || gC < 0 || gC >= count.z) {
		return fallbackValue;
	}

	int index = gridPosToDataIndex(gA, gB, gC);

	return &data[index];
}

template <typename T>
void Array3D<T>::resizeNoElementsMove(int lengthA, int lengthB, int lengthC) {
	if (data) {
		delete[] data;

		data = nullptr;

		this->count.set(
			0, 
			0, 
			0
		);
	}

	if (lengthA > 0 && lengthB > 0 && lengthC > 0) {
		data = new T[lengthA * lengthB * lengthC]();

		this->count.set(
			lengthA,
			lengthB,
			lengthC
		);
	}
}

template <typename T>
void Array3D<T>::resizeMarginOffsets(Bounds3DInt& newBounds) {
	Bounds3DInt currentBounds{
		0, count.x,
		0, count.y,
		0, count.z
	};
	Bounds3DInt intersectionBounds{ currentBounds };
	intersectionBounds.intersect(newBounds);

	Vector3Int copySrcOffset{ intersectionBounds.min };

	Vector3Int copyDestOffset{
		intersectionBounds.min.x - newBounds.min.x,
		intersectionBounds.min.y - newBounds.min.y,
		intersectionBounds.min.z - newBounds.min.z
	};
	Vector3Int copyCount{
		intersectionBounds.getLenX(),
		intersectionBounds.getLenY(),
		intersectionBounds.getLenZ()
	};

	Vector3Int newCount{
		newBounds.getLenX(),
		newBounds.getLenY(),
		newBounds.getLenZ()
	};

	T* newData = new T[newCount.x * newCount.y * newCount.z]();
	// Copy the intersection region.
	move(
		data, count, copySrcOffset,
		newData, newCount, copyDestOffset,
		copyCount
	);

	delete[] data;
	data = nullptr;

	data = newData;
	count.set(newCount);
}

template <typename T>
void Array3D<T>::set(int gA, int gB, int gC, T& val) {
	if (gA < 0 || gA >= count.x || gB < 0 || gB >= count.y || gC < 0 || gC >= count.z) {
		throw OutOfRangeException();
	}

	int index = gridPosToDataIndex(gA, gB, gC);

	data[index] = val;
}

template <typename T>
void Array3D<T>::set(Array3D<T>& src) {
	if (count.x != src.count.x || count.y != src.count.y || count.z != src.count.z) {
		resizeNoElementsMove(src.count.x, src.count.y, src.count.z);
	}

	// Copy all data using memcpy.
	memcpy(data, src.data, sizeof(T) * count.x * count.y * count.z);
}

template <typename T>
void Array3D<T>::set(Vector3Int& offset, Array3D<T>* src, Vector3Int& srcOffset, Vector3Int& copyCount) {
	// Hard check this and src bounds.
	copy(
		src->data, src->count, srcOffset,
		data, count, offset,
		copyCount
	);
}

template <typename T>
inline void Array3D<T>::copy(
	T* srcData, Vector3Int& srcCount, Vector3Int& srcOffset,
	T* destData, Vector3Int& destCount, Vector3Int& destOffset,
	Vector3Int& copyCount)
{
	if (copyCount.x < 0 || copyCount.y < 0 || copyCount.z < 0) {
		throw LogicException(LOC);
	}
	if (srcOffset.x < 0 || srcOffset.y < 0 || srcOffset.z < 0
		|| srcOffset.x + copyCount.x > srcCount.x || srcOffset.y + copyCount.y > srcCount.y || srcOffset.z + copyCount.z > srcCount.z)
	{
		throw LogicException(LOC);
	}
	if (destOffset.x < 0 || destOffset.y < 0 || destOffset.z < 0
		|| destOffset.x + copyCount.x > destCount.x || destOffset.y + copyCount.y > destCount.y || destOffset.z + copyCount.z > destCount.z)
	{
		throw LogicException(LOC);
	}

	for (int gA = 0; gA < copyCount.x; gA++) {
		for (int gB = 0; gB < copyCount.y; gB++) {
			for (int gC = 0; gC < copyCount.z; gC++) {
				int srcG_a = srcOffset.x + gA;
				int srcG_b = srcOffset.y + gB;
				int srcG_c = srcOffset.z + gC;

				int destG_a = destOffset.x + gA;
				int destG_b = destOffset.y + gB;
				int destG_c = destOffset.z + gC;

				int srcIndex = srcG_c * (srcCount.x * srcCount.y) + srcG_b * srcCount.x + srcG_a;
				int destIndex = destG_c * (destCount.x * destCount.y) + destG_b * destCount.x + destG_a;

				// Use thge copy operator, don't do move.
				destData[destIndex] = srcData[srcIndex];
			}
		}
	}
}

template <typename T>
inline void Array3D<T>::move(
	T* srcData, Vector3Int& srcCount, Vector3Int& srcOffset,
	T* destData, Vector3Int& destCount, Vector3Int& destOffset,
	Vector3Int& copyCount)
{
	if (copyCount.x < 0 || copyCount.y < 0 || copyCount.z < 0) {
		throw LogicException(LOC);
	}
	if (srcOffset.x < 0 || srcOffset.y < 0 || srcOffset.z < 0
		|| srcOffset.x + copyCount.x > srcCount.x || srcOffset.y + copyCount.y > srcCount.y || srcOffset.z + copyCount.z > srcCount.z)
	{
		throw LogicException(LOC);
	}
	if (destOffset.x < 0 || destOffset.y < 0 || destOffset.z < 0
		|| destOffset.x + copyCount.x > destCount.x || destOffset.y + copyCount.y > destCount.y || destOffset.z + copyCount.z > destCount.z)
	{
		throw LogicException(LOC);
	}

	for (int gA = 0; gA < copyCount.x; gA++) {
		for (int gB = 0; gB < copyCount.y; gB++) {
			for (int gC = 0; gC < copyCount.z; gC++) {
				int srcG_a = srcOffset.x + gA;
				int srcG_b = srcOffset.y + gB;
				int srcG_c = srcOffset.z + gC;

				int destG_a = destOffset.x + gA;
				int destG_b = destOffset.y + gB;
				int destG_c = destOffset.z + gC;

				int srcIndex = srcG_c * (srcCount.x * srcCount.y) + srcG_b * srcCount.x + srcG_a;
				int destIndex = destG_c * (destCount.x * destCount.y) + destG_b * destCount.x + destG_a;

				destData[destIndex] = std::move(srcData[srcIndex]);
			}
		}
	}
}

template <typename T>
void Array3D<T>::setDirect(int gA, int gB, int gC, T val) {
	if (gA < 0 || gA >= count.x || gB < 0 || gB >= count.y || gC < 0 || gC >= count.z) {
		throw OutOfRangeException();
	}

	int index = gridPosToDataIndex(gA, gB, gC);

	data[index] = std::move(val);
}

template <typename T>
void Array3D<T>::invertA() {
	int gHalfA = static_cast<int>(Math::floor(count.x / 2.0f));

	T t;
	int indexSrc;
	int indexDst;
	// Enumerate half-ish of the grid and invert.
	for (int a = 0; a < gHalfA; a++) {
		for (int b = 0; b < count.y; b++) {
			for (int c = 0; c < count.z; c++) {
				indexSrc = gridPosToDataIndex(a, b, c);
				indexDst = gridPosToDataIndex((count.x - 1) - a, b, c);

				t = data[indexSrc];
				data[indexSrc] = data[indexDst];
				data[indexDst] = t;
			}
		}
	}
}

template <typename T>
void Array3D<T>::invertB() {
	int gHalfB = static_cast<int>(Math::floor(count.y / 2.0f));

	T t;
	int indexSrc;
	int indexDst;
	// Enumerate half-ish of the grid and invert.
	for (int a = 0; a < count.x; a++) {
		for (int b = 0; b < gHalfB; b++) {
			for (int c = 0; c < count.z; c++) {
				indexSrc = gridPosToDataIndex(a, b, c);
				indexDst = gridPosToDataIndex(a, (count.y - 1) - b, c);

				t = data[indexSrc];
				data[indexSrc] = data[indexDst];
				data[indexDst] = t;
			}
		}
	}
}

template <typename T>
void Array3D<T>::invertC() {
	int gHalfC = static_cast<int>(Math::floor(count.z / 2.0f));

	T t;
	int indexSrc;
	int indexDst;
	// Enumerate half-ish of the grid and invert.
	for (int a = 0; a < count.x; a++) {
		for (int b = 0; b < count.y; b++) {
			for (int c = 0; c < gHalfC; c++) {
				indexSrc = gridPosToDataIndex(a, b, c);

				indexDst = gridPosToDataIndex(a, b, (count.z - 1) - c);

				t = data[indexSrc];
				data[indexSrc] = data[indexDst];
				data[indexDst] = t;
			}
		}
	}
}

template <typename T>
inline int Array3D<T>::gridPosToDataIndex(int gA, int gB, int gC) {
	if (gA < 0 || gB < 0 || gC < 0 || gA >= count.x || gB >= count.y || gC >= count.z) {
		throw LogicException(LOC);
	}
	int index = gC * (count.x * count.y) + gB * count.x + gA;

	return index;
}

template <typename T>
Array3D<T>::~Array3D() {
	if (data) {
		delete[] data;

		data = nullptr;
	}

	reservedDisposeIfNeeded();// Note1001
}
