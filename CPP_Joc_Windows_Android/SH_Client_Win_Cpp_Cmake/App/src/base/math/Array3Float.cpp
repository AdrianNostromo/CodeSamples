#include "Array3Float.h"

Array3Float::Array3Float() {
    set(0.0f, 0.0f, 0.0f);
}

Array3Float::Array3Float(Array3Float const& src) {
	set(src.a, src.b, src.c);
}

Array3Float::Array3Float(float a, float b, float c) {
	set(a, b, c);
}

Array3Float* Array3Float::set(float a, float b, float c) {
	this->a = a;
	this->b = b;
	this->c = c;

	return this;
}

Array3Float* Array3Float::set(const Array3Float& vector) {
	return this->set(vector.a, vector.b, vector.c);
}
