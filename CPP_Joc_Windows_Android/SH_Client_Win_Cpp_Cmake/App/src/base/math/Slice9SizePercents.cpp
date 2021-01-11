#include "Slice9SizePercents.h"

Slice9SizePercents::Slice9SizePercents()
	: x{ 0.0f, 0.0f, 0.0f }, y{ 0.0f, 0.0f, 0.0f }
{
	//void
}

Slice9SizePercents::Slice9SizePercents(Slice9SizePercents const& src)
	: x{ src.x[0], src.x[1], src.x[2] }, y{ src.y[0], src.y[1], src.y[2] }
{
	//void
}

Slice9SizePercents::Slice9SizePercents(
	float x0, float x1, float x2,
	float y0, float y1, float y2)
	: x{ x0, x1, x2 }, y{ y0, y1, y2 }
{
	//void
}

Slice9SizePercents* Slice9SizePercents::set(
	float x0, float x1, float x2,
	float y0, float y1, float y2)
{
	this->x[0] = x0;
	this->x[1] = x1;
	this->x[2] = x2;
	
	this->y[0] = y0;
	this->y[1] = y1;
	this->y[2] = y2;

	return this;
}

Slice9SizePercents* Slice9SizePercents::set(const Slice9SizePercents& src) {
	this->x[0] = src.x[0];
	this->x[1] = src.x[1];
	this->x[2] = src.x[2];

	this->y[0] = src.y[0];
	this->y[1] = src.y[1];
	this->y[2] = src.y[2];

	return this;
}

bool Slice9SizePercents::equals(const Slice9SizePercents& other) {
	if (this->x[0] != other.x[0] || this->x[1] != other.x[1] || this->x[2] != other.x[2]
		|| this->y[0] != other.y[0] || this->y[1] != other.y[1] || this->y[2] != other.y[2])
	{
		return false;
	}

	return true;
}

bool Slice9SizePercents::equals(const Slice9SizePercents* other) {
	if (other == nullptr 
		|| this->x[0] != other->x[0] || this->x[1] != other->x[1] || this->x[2] != other->x[2]
		|| this->y[0] != other->y[0] || this->y[1] != other->y[1] || this->y[2] != other->y[2])
	{
		return false;
	}

	return true;
}
