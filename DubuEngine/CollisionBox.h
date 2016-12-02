#pragma once

// Simple 2d box
class CollisionBox {
public:
	CollisionBox() {}
	CollisionBox(int X, int Y, int W = 0, int H = 0) : x(X), y(Y), w(W), h(H) {}

	double x = 0;
	double y = 0;
	int w = 0;
	int h = 0;
};