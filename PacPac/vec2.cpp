#include "vec2.h"
#include "Game.h"
using namespace std;

vec2::vec2() : vec2(0,0) {}
vec2::vec2(int x, int y) : x(x), y(y) { }

vec2 vec2::operator+(const vec2& other) {
	return vec2(x + other.x, y + other.y);
}

vec2 vec2::operator-(const vec2& other) {
	return vec2( x - other.x, y - other.y );
}

vec2 vec2::operator*(int i) {
	return vec2( x * i, y * i );
}

// This is used so vectors can use "map"
bool operator<(const vec2& one, const vec2& other)
{
//https://stackoverflow.com/questions/43154990/is-this-method-bad-for-using-2d-coordinates-as-keys-in-stdunordered-map
	int h0 = (one.x << 16) | one.y;
	int h1 = (other.x << 16) | other.y;
	return h0 < h1;
}

bool operator==(const vec2& one, const vec2& other) {
	return (one.x == other.x && one.y == other.y);
}

bool operator!=(const vec2& one, const vec2& other) {
	return (one.x != other.x || one.y != other.y);
}

int vec2::SqrDistance(const vec2& point0, const vec2& point1)
{
	int p0 = point0.x - point1.x;
	int p1 = point0.y - point1.y;
	int sqr = p0 * p0 + p1 * p1;
	return sqr;
}
