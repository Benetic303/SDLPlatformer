#pragma once

#include <iostream>

struct Vector2f {
	Vector2f()
		:x(0.0f), y(0.0f)
	{}

	Vector2f(float p_x, float p_y) 
		:x(p_x), y(p_y)
	{}
	void print() {
		std::cout << x << ", " << y << std::endl;
	}

	// Comparison operator for std::map
	bool operator<(const Vector2f& other) const {
		return (x < other.x) || (x == other.x && y < other.y);
	}


	float x, y;

};
 