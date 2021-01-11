#include "Color.h"
#include <iostream>
#include <sstream>
#include <base/math/Math.h>
#include <base/exceptions/DataFormatException.h>

Color::Color() {
	set(0.0f, 0.0f, 0.0f, 0.0f);
}

Color::Color(const Color& color) {
    set(color.r, color.g, color.b, color.a);
}

Color::Color(float r, float g, float b, float a) {
	set(r, g, b, a);
}

Color::Color(std::string hex) {
    set(hex);
}

Color* Color::set(float r, float g, float b, float a) {
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;

	return this;
}

Color* Color::set(Color& color) {
	this->r = color.r;
	this->g = color.g;
	this->b = color.b;
	this->a = color.a;

	return this;
}

Color* Color::set(std::string hex) {
    
    if(hex.at(0) == '#') {
        hex = hex.erase(0, 1);
    }
	if (hex.length() != 8) {
		throw DataFormatException();
	}

	int intR;
	int intG;
	int intB;
	int intA;
	std::istringstream(hex.substr(0,2)) >> std::hex >> intR;
    std::istringstream(hex.substr(2,2)) >> std::hex >> intG;
    std::istringstream(hex.substr(4,2)) >> std::hex >> intB;
    std::istringstream(hex.substr(6,2)) >> std::hex >> intA;

    set(
        intR / 255.0f,
        intG / 255.0f,
        intB / 255.0f,
        intA / 255.0f
    );

	return this;
}

Color* Color::mul(Color& color) {
	this->r *= color.r;
	this->g *= color.g;
	this->b *= color.b;
	this->a *= color.a;

	return clamp();
}

Color* Color::clamp() {
	if (r < 0) {
		r = 0;
	} else if (r > 1) {
		r = 1;
	}

	if (g < 0) {
		g = 0;
	} else if (g > 1) {
		g = 1;
	}

	if (b < 0) {
		b = 0;
	} else if (b > 1) {
		b = 1;
	}

	if (a < 0) {
		a = 0;
	} else if (a > 1) {
		a = 1;
	}

	return this;
}

bool Color::equals(Color& target) {
	return equals(&target);
}

bool Color::equals(Color* target) {
	if (target == nullptr || target->r != r || target->g != g || target->b != b || target->a != a) {
		return false;
	}

	return true;
}

std::string Color::serialize() {
	std::string colNameR = Math::toString_1Byte_Base16(Math::round(r * 255.0f));
	std::string colNameG = Math::toString_1Byte_Base16(Math::round(g * 255.0f));
	std::string colNameB = Math::toString_1Byte_Base16(Math::round(b * 255.0f));
	std::string colNameA = Math::toString_1Byte_Base16(Math::round(a * 255.0f));

	std::string colName = "#" + colNameR + colNameG + colNameB + colNameA;

	return colName;
}
