#pragma once

// This is for more complex animations
class DirtParticle {
public:
	DirtParticle(int x, int y) : x(x), y(y) {}

	// Tick
	bool Tick() {
		// If invisibile, return false
		if (opacity < 0) return false;

		// Update
		x += x_vel;
		y += y_vel;
		opacity -= 0.05;
		size += 0.5;
		return true;
	}

	// Draw
	void Draw(Game* g);

private:
	double x, y;
	double y_vel = -3;
	double x_vel = 0;
	double opacity = 0.8;
	double size = 5.0;
};

// Tick every dirt particle in vector
void HandleDirtParticles(Game* g);

// Render every dirt particle in vector
void RenderDirtParticles(Game* g);