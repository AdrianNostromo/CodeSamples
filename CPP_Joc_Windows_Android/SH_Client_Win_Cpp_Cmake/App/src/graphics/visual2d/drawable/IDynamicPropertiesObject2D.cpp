#include "IDynamicPropertiesObject2D.h"
#include <graphics/visual/Color.h>

bool IDynamicPropertiesObject2D::CheckedSet(float& dest, float src) {
	if (src != dest) {
		dest = src;

		return true;
	}

	return false;
}

bool IDynamicPropertiesObject2D::CheckedSet(int& dest, int src) {
	if (src != dest) {
		dest = src;

		return true;
	}

	return false;
}

bool IDynamicPropertiesObject2D::CheckedSet(bool& dest, bool src) {
	if (src != dest) {
		dest = src;

		return true;
	}

	return false;
}

bool IDynamicPropertiesObject2D::CheckedSet(Color* dest, Color* src) {
	if (!dest->equals(src)) {
		dest->set(*src);

		return true;
	}

	return false;
}

bool IDynamicPropertiesObject2D::CheckedSet(std::string** dest, std::string* src) {
	// Do a simple check for string **.
	if (*dest != src) {
		*dest = src;

		return true;
	}

	return false;
}
