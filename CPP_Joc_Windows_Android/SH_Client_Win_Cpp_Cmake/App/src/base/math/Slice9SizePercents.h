#pragma once

#include <base/gh.h>
#include <base/exceptions/LogicException.h>

// This is used to configure 9_slice scaling sprite texRegion regions.
class Slice9SizePercents {
	// Keep these priv and use updat efunctions because the total sizes are computed on changes.
	pub float x[3];
	pub float y[3];

	// Note. The copy constructor is initialised manually below.
	//Slice9SizePercents(Slice9SizePercents const& src) = delete;
	pub Slice9SizePercents(Slice9SizePercents &&) = default;
	pub Slice9SizePercents& operator=(Slice9SizePercents const&) = delete;
	pub Slice9SizePercents& operator=(Slice9SizePercents &&) = default;

	pub explicit Slice9SizePercents();
	// This the same as the copy constructor.
	pub explicit Slice9SizePercents(Slice9SizePercents const& src);
	pub explicit Slice9SizePercents(
		float x0, float x1, float x2,
		float y0, float y1, float y2);

	pub Slice9SizePercents* set(
		float x0, float x1, float x2,
		float y0, float y1, float y2);
	pub Slice9SizePercents* set(const Slice9SizePercents& src);

	pub bool equals(const Slice9SizePercents& other);
	pub bool equals(const Slice9SizePercents* other);

};
