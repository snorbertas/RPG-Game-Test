#pragma once

// Simple 2D box
class CollisionBox {
public:
	CollisionBox() {}
	CollisionBox(int X, int Y, int W = 0, int H = 0) : x(X), y(Y), w(W), h(H) {}

public:
	bool Empty() const {
		return w == 0 || h == 0;
	}

public:
	double x = 0;
	double y = 0;
	int w = 0;
	int h = 0;

public:
	bool operator==(const CollisionBox& box) const {
		return x == box.x && y == box.y;
	}
	bool operator<(const CollisionBox& box) const {
		if (x != box.x)
			return x < box.x;
		if (y != box.y)
			return y < box.y;
		return false;
	}
};