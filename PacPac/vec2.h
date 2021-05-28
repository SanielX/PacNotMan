#pragma once

struct vec2
{
	vec2();
	vec2(int x, int y);

	int x, y;

	vec2 operator+(const vec2& other);
	vec2 operator-(const vec2& other);
	vec2 operator*(int i);

	friend bool operator==(const vec2& one, const vec2& other);
	friend bool operator!=(const vec2& one, const vec2& other);
	// Used to make "map" container work
	friend bool operator<(const vec2& one, const vec2& other);

	// Squared distance between 2 points
	static int SqrDistance(const vec2& point0, const vec2& point1);
};