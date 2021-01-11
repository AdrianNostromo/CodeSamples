#include "AttributesList.h"
#include "Attribute.h"
#include <graphics/shader/program/ShaderProgram.h>
#include <base/opengl/util/GLUtil.h>
#include <graphics/material/Material.h>
#include <graphics/visual/Texture.h>
#include <graphics/visual/TextureArray.h>
#include <graphics/camera/OrthographicCamera.h>
#include <base/MM.h>

using namespace graphics;

AttributesList::AttributesList()
	: super()
{
	//void
}

long AttributesList::getBitMask() {
	return bitMask;
}

ArrayList<Attribute*>* AttributesList::peekList() {
	return &list;
}

int AttributesList::count() {
	return list.count();
}

void AttributesList::onEnableAtribute(Attribute* attribute) {
	this->bitMask |= attribute->typeBitGroup;
}

void AttributesList::onDisableAtribute(Attribute* attribute) {
	this->bitMask &= ~attribute->typeBitGroup;
}

int AttributesList::BinarySearchAttribute(ArrayList<Attribute*>& list, int l, int r, long typeBitGroup) {
	if (r >= l) {
		int mid = l + (r - l) / 2;

		// If the element is present at the middle 
		// itself 
		if (list.getDirect(mid)->typeBitGroup == typeBitGroup)
			return mid;

		// If element is smaller than mid, then 
		// it can only be present in left subarray 
		if (list.getDirect(mid)->typeBitGroup > typeBitGroup)
			return BinarySearchAttribute(list, l, mid - 1, typeBitGroup);

		// Else the element can only be present 
		// in right subarray 
		return BinarySearchAttribute(list, mid + 1, r, typeBitGroup);
	}

	// We reach here when element is not 
	// present in array 
	return -1;
}

Attribute* AttributesList::getOptional(long typeBitGroup) {
	int indexOfAttribute = BinarySearchAttribute(list, 0, list.count() - 1, typeBitGroup);
	if (indexOfAttribute < 0) {
		return nullptr;
	}

	Attribute* attribute = list.getDirect(indexOfAttribute);

	return attribute;
}

void AttributesList::set(Attribute* attribute) {
	int idx = indexOf(attribute->typeBitGroup);
	if (idx >= 0) {
		removeAndDispose(attribute->typeBitGroup);
	}

	int insertAfterIndex = -1;
	for (int i = 0; i < list.count(); i++) {
		Attribute* entry = list.getDirect(i);

		if (entry->typeBitGroup == attribute->typeBitGroup) {
			// Currenctly only 1 attribute of a type is allowed to be set.
			throw LogicException(LOC);
		}

		if (entry->typeBitGroup > attribute->typeBitGroup) {
			break;
		}

		insertAfterIndex = i;
	}
	list.insertDirect(insertAfterIndex + 1, attribute);

	onEnableAtribute(attribute);
}

void AttributesList::removeAndDispose(long typeBitMask) {
	for (int i = list.count() - 1; i >= 0; i--) {
		Attribute* attribute = list.getDirect(i);

		if ((attribute->typeBitGroup & typeBitMask) == attribute->typeBitGroup) {
			onDisableAtribute(attribute);
			list.remove(i);

			delete attribute;

			// Remove all entryes that pass the mask.
			//return;
		}
	}
}

bool AttributesList::hasAll(const long typeBitMask) {
	return (this->bitMask & typeBitMask) == typeBitMask;
}

int AttributesList::indexOf(const long typeBitGroup) {
	if (hasAll(typeBitGroup)) {
		for (int i = 0; i < list.count(); i++) {
			if (list.getDirect(i)->typeBitGroup == typeBitGroup) {
				return i;
			}
		}
	}

	return -1;
}

AttributesList* AttributesList::clone(AttributesList* o) {
	if (o == nullptr) {
		o = newt AttributesList();
	}

	for (int i = 0; i < list.count(); i++) {
		Attribute* attribute = list.getDirect(i);

		Attribute* cloneAttribute = attribute->clone();
		o->list.appendDirect(cloneAttribute);
	}

	o->bitMask = bitMask;

	return o;
}

void AttributesList::dispose() {
	removeAndDispose(-1);

	super::dispose();
}

AttributesList::~AttributesList() {
	reservedDisposeIfNeeded();// Note1001
}
