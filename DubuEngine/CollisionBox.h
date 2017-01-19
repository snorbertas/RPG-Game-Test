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
};