#pragma once

#include <string>

class Color {
public:
	float r;
	float g;
	float b;
	float a;

public:
	//Color(Color const&) = delete;
	Color(Color &&) = default;
	Color& operator=(Color const&) = delete;
	Color& operator=(Color &&) = default;

	explicit Color();
    explicit Color(const Color&);
	explicit Color(float r, float g, float b, float a);
	explicit Color(std::string hex);

	Color* set(float r, float g, float b, float a);
	Color* set(Color& color);
	Color* set(std::string hex);

	Color* mul(Color& color);

	Color* clamp();

	bool equals(Color& target);
	bool equals(Color* target);

	// #FF0044FF
	std::string serialize();
};
