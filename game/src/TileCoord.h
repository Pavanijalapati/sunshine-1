#pragma once
#include <algorithm>
#include "Math.h"

struct TileCoord
{
	int x = 0;
	int y = 0;

	TileCoord() {}
	TileCoord(int ax, int ay) : x(ax), y(ay) {}
	TileCoord(float ax, float ay) : x(ax), y(ay) {}
	TileCoord(Vector2 position) : x(position.x), y(position.y) {}
	Vector2 toVec2() const { return Vector2{ (float)x,(float)y }; }

	void operator+=(const TileCoord& r) { x += r.x; y += r.y; }
	void operator-=(const TileCoord& r) { x -= r.x; y -= r.y; }
};

bool operator!=(const TileCoord& l, const TileCoord& r) { return l.x != r.x || l.y != r.y; }
bool operator==(const TileCoord& l, const TileCoord& r) { return l.x == r.x && l.y == r.y; }

TileCoord operator+(const TileCoord& l, const TileCoord& r) { return { l.x + r.x, l.y + r.y }; }
TileCoord operator-(const TileCoord& l, const TileCoord& r) { return { l.x - r.x, l.y - r.y }; }

TileCoord operator*(const TileCoord& l, const int& r) { return { l.x * r, l.y * r }; }
TileCoord operator/(const TileCoord& l, const int& r) { return { l.x / r, l.y / r }; }

template <> struct std::hash<TileCoord>
{
	size_t operator()(const TileCoord& pos) const // Hash function which produces a unique number (hash) for each unique TileCoord.
	{
		const static long int halfWidth = sqrt(numeric_limits<int>::max());
		const static long int width = halfWidth * 2;
		long long int x = pos.x;
		long long int y = pos.y;
		size_t hash = ((y + halfWidth) * width) + (x + halfWidth);
		//size_t hash = ((x + y) * (x + y + 1) / 2) + y;
		return hash;
	}
};


