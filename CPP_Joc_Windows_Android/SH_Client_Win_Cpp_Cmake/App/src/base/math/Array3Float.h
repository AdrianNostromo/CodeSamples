#pragma once

#include <base/gh.h>

class Array3Float {
	pub float a = 0.0f;
	pub float b = 0.0f;
	pub float c = 0.0f;

	// Note. The copy constructor is initialised manually below.
	//Array3Float(Array3Float const& src) = delete;
	pub Array3Float(Array3Float &&) = default;
	pub Array3Float& operator=(Array3Float const&) = delete;
	pub Array3Float& operator=(Array3Float &&) = default;

	pub explicit Array3Float();
	// This the same as the copy constructor.
	pub explicit Array3Float(Array3Float const& src);
	pub explicit Array3Float(float a, float b, float c);

	pub Array3Float* set(float a, float b, float c);
	pub Array3Float* set(const Array3Float& vector);

};
